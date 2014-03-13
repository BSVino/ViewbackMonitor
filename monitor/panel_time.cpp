#include "panel_time.h"

#include <glgui/label.h>
#include <tinker/profiler.h>
#include <renderer/renderingcontext.h>

#include "monitor_window.h"

using namespace glgui;

void CPanel_Time::RegistrationUpdate()
{
	m_apLabels.clear();
	m_aiDataLabels.clear();

	auto& aRegistrations = MonitorWindow()->GetViewback()->GetRegistrations();
	auto& aMeta = MonitorWindow()->GetViewback()->GetMeta();

	m_aiDataLabels.resize(aRegistrations.size());
	for (int& i : m_aiDataLabels)
		i = 0;

	float flYPos = 20;

	for (size_t i = 0; i < aRegistrations.size(); i++)
	{
		auto& oReg = aRegistrations[i];
		auto& oMeta = aMeta[i];

		if (oReg.m_eDataType != VB_DATATYPE_INT && oReg.m_eDataType != VB_DATATYPE_FLOAT)
			continue;

		m_apLabels.push_back(AddControl(new CLabel(oReg.m_sFieldName)));
		m_apLabels.back()->SetPos(20, flYPos);
		m_apLabels.back()->SetTextColor(Color(oMeta.m_clrColor.x, oMeta.m_clrColor.y, oMeta.m_clrColor.z, 1.0f));
		m_apLabels.back()->SetAlign(CLabel::TA_MIDDLECENTER);

		if (oReg.m_eDataType == VB_DATATYPE_INT)
			m_apLabels.back()->SetBackgroundColor(Color(0, 0, 0, 155));

		flYPos += m_apLabels.back()->GetHeight() + 20;

		m_aiDataLabels[i] = m_apLabels.size() - 1;
	}
}


void CPanel_Time::Paint(float x, float y, float w, float h)
{
	TPROF("CPanel_Time::Paint()");

	if (!MonitorWindow()->GetViewback()->HasConnection())
	{
		BaseClass::Paint(x, y, w, h);
		return;
	}

	double flSecondsToShow = 5;
	double flNewestData = MonitorWindow()->GetViewback()->FindNewestData();

	for (int i = (int)(flNewestData - flSecondsToShow); i < flNewestData; i++)
	{
		float flX = (float)RemapVal((double)i, flNewestData - flSecondsToShow, flNewestData, (double)x, (double)x + w);

		if (flX < x - 20)
			continue;

		if (flX > x + w)
			continue;

		CBaseControl::PaintRect(flX, y, 1, h, Color(128, 128, 128, 255));

		tstring sTime = sprintf("%ds", i);
		CLabel::PaintText(sTime, sTime.length(), "sans-serif", 12, flX + 5, y + h - 14, Color(128, 128, 128, 255));
	}

	auto& oRegistrations = MonitorWindow()->GetViewback()->GetRegistrations();
	auto& oData = MonitorWindow()->GetViewback()->GetData();
	auto& oMeta = MonitorWindow()->GetViewback()->GetMeta();

	for (size_t i = 0; i < oRegistrations.size(); i++)
	{
		auto& oReg = oRegistrations[i];

		if (oReg.m_eDataType != VB_DATATYPE_INT && oReg.m_eDataType != VB_DATATYPE_FLOAT)
			continue;

		size_t iStart = 0;
		if (oReg.m_eDataType == VB_DATATYPE_INT)
		{
			if (oData[i].m_aIntData.size() < 2)
				continue;

			iStart = FindStartTime(oData[i].m_aIntData, flSecondsToShow);
		}
		else if (oReg.m_eDataType == VB_DATATYPE_FLOAT)
		{
			if (oData[i].m_aFloatData.size() < 2)
				continue;

			iStart = FindStartTime(oData[i].m_aFloatData, flSecondsToShow);
		}
		else
			continue;

		if (oReg.m_eDataType == VB_DATATYPE_FLOAT)
		{
			auto& aFloatData = oData[i].m_aFloatData;

			oMeta[i].m_vecMaxValue.x = aFloatData[iStart].data; // Consider x the min
			oMeta[i].m_vecMaxValue.y = aFloatData[iStart].data; // Consider y the max

			for (size_t j = iStart; j < aFloatData.size(); j++)
			{
				if (aFloatData[j].data < oMeta[i].m_vecMaxValue.x)
					oMeta[i].m_vecMaxValue.x = aFloatData[j].data;

				if (aFloatData[j].data > oMeta[i].m_vecMaxValue.y)
					oMeta[i].m_vecMaxValue.y = aFloatData[j].data;
			}

			float flRange = oMeta[i].m_vecMaxValue.y - oMeta[i].m_vecMaxValue.x;
			oMeta[i].m_vecMaxValue.x -= flRange * 0.3f;
			oMeta[i].m_vecMaxValue.y += flRange * 0.3f;

			CRenderingContext c(MonitorWindow()->GetRenderer(), true);

			Color clrColor(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 1.0f);
			c.UseProgram("line");

			c.SetUniform("vecColor", clrColor);
			c.SetBlend(BLEND_ALPHA);

			c.BeginRenderLines();

			c.Color(Color(255, 255, 255, 255));

			Vector v;
			for (size_t j = iStart; j < aFloatData.size() - 1; j++)
			{
				v.x = (float)RemapVal(aFloatData[j].time, flNewestData - flSecondsToShow, flNewestData, (double)x, (double)x + w);
				v.y = RemapVal(aFloatData[j].data, oMeta[i].m_vecMaxValue.x, oMeta[i].m_vecMaxValue.y, y + h, y);
				c.Vertex(v);

				v.x = (float)RemapVal(aFloatData[j+1].time, flNewestData - flSecondsToShow, flNewestData, (double)x, (double)x + w);
				v.y = RemapVal(aFloatData[j + 1].data, oMeta[i].m_vecMaxValue.x, oMeta[i].m_vecMaxValue.y, y + h, y);
				c.Vertex(v);
			}

			c.EndRender();

			tstring sValue = sprintf("%.2f", aFloatData.back().data);
			CLabel::PaintText(sValue, sValue.length(), "sans-serif", 12, v.x - 40, v.y - 14, Color(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 1.0f));
		}

		if (oReg.m_eDataType == VB_DATATYPE_INT)
		{
			int iLabel = m_aiDataLabels[i];
			auto& aIntData = oData[i].m_aIntData;

			CLabel* pLabel = m_apLabels[iLabel];

			float ax, ay;
			pLabel->GetAbsPos(ax, ay);

			int iValue = aIntData[iStart].data;
			double flValueTimeStart = aIntData[iStart].time;

			Color clrBox(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 1.0f);

			for (size_t j = iStart + 1; j < aIntData.size(); j++)
			{
				float flXStart = (float)RemapVal(flValueTimeStart, flNewestData - flSecondsToShow, flNewestData, (double)x, (double)x + w);
				float flXEnd = (float)RemapVal(aIntData[j].time, flNewestData - flSecondsToShow, flNewestData, (double)x, (double)x + w) - 2;

				if (flXStart < x)
					flXStart = x;

				PaintRect(flXStart, ay, flXEnd - flXStart, pLabel->GetHeight(), clrBox, 2);

				tstring sValue = MonitorWindow()->GetViewback()->GetLabelForValue(oReg.m_iHandle, iValue).c_str();
				CLabel::PaintText(sValue, sValue.length(), "sans-serif", 12, flXStart + 5, ay + pLabel->GetHeight() - 14, Color(0, 0, 0, 255));

				if (iValue != aIntData[j].data)
				{
					iValue = aIntData[j].data;
					flValueTimeStart = aIntData[j].time;
				}
			}
		}
	}

	BaseClass::Paint(x, y, w, h);
}



