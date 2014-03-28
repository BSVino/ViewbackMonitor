#include "panel_time.h"

#include <glgui/button.h>
#include <glgui/rootpanel.h>
#include <tinker/profiler.h>
#include <renderer/renderingcontext.h>

#include "monitor_window.h"

using namespace glgui;

CPanel_Time::CPanel_Time()
{
	m_bDragging = false;
	m_bOverrideTime = false;
	m_flShowTime = 5;
	m_flFastForwardStartTime = 0;
	m_flFastForwardFromTime = 0;
}

void CPanel_Time::RegistrationUpdate()
{
	m_apLabels.clear();
	m_aiDataLabels.clear();

	auto& aChannels = MonitorWindow()->GetViewback()->GetChannels();
	auto& aMeta = MonitorWindow()->GetViewback()->GetMeta();

	m_aiDataLabels.resize(aChannels.size());
	for (int& i : m_aiDataLabels)
		i = 0;

	float flYPos = 20;

	for (size_t i = 0; i < aChannels.size(); i++)
	{
		auto& oReg = aChannels[i];
		auto& oMeta = aMeta[i];

		if (oReg.m_eDataType != VB_DATATYPE_INT && oReg.m_eDataType != VB_DATATYPE_FLOAT)
			continue;

		m_apLabels.push_back(AddControl(new CButton(oReg.m_sFieldName)));
		m_apLabels.back()->SetPos(20, flYPos);
		m_apLabels.back()->SetTextColor(Color(oMeta.m_clrColor.x, oMeta.m_clrColor.y, oMeta.m_clrColor.z, 1.0f));
		m_apLabels.back()->SetAlign(CLabel::TA_MIDDLECENTER);
		m_apLabels.back()->SetClickedListener(this, ToggleVisible, tsprintf("%d", i));
		m_apLabels.back()->SetHeight(18);

		if (oReg.m_eDataType == VB_DATATYPE_FLOAT)
			m_apLabels.back()->SetBorder(BT_NONE);
		else
			m_apLabels.back()->SetBorder(BT_SOME);

		flYPos += m_apLabels.back()->GetHeight() + 10;

		m_aiDataLabels[i] = m_apLabels.size() - 1;
	}

	m_bDragging = false;
	m_bOverrideTime = false;
	m_flShowTime = 5;
	m_flFastForwardStartTime = 0;
	m_flFastForwardFromTime = 0;

	Layout();
}

void CPanel_Time::Layout()
{
	BaseClass::Layout();

	auto& aMeta = MonitorWindow()->GetViewback()->GetMeta();

	for (auto& pLabel : m_apLabels)
	{
		int iChannel = stoi(pLabel->GetClickedListenerArgs());

		TAssert(iChannel >= 0);
		TAssert(iChannel < (int)aMeta.size());

		if (iChannel < 0)
			continue;
		if (iChannel >= (int)aMeta.size())
			continue;

		auto& oMeta = aMeta[iChannel];

		if (oMeta.m_bVisible)
			pLabel->SetTextColor(Color(oMeta.m_clrColor.x, oMeta.m_clrColor.y, oMeta.m_clrColor.z, 1.0f));
		else
			pLabel->SetTextColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
	}
}

double CPanel_Time::GetCurrentViewTime()
{
	double flPredictedTime = MonitorWindow()->GetViewback()->PredictCurrentTime();
	double flTimeNow = flPredictedTime;

	if (m_bOverrideTime)
		flTimeNow = m_flOverrideTime;

	if (m_flFastForwardStartTime)
	{
		flTimeNow = RemapBiased(RootPanel()->GetTime(), m_flFastForwardStartTime, m_flFastForwardStartTime + 1.0, m_flFastForwardFromTime, flPredictedTime, 0.8);

		if (flTimeNow > flPredictedTime)
		{
			flTimeNow = flPredictedTime;
			m_flFastForwardStartTime = 0;
			m_flFastForwardFromTime = 0;
		}
	}

	return flTimeNow;
}

void CPanel_Time::Think()
{
	BaseClass::Think();

	MonitorWindow()->GetViewback()->SetDataClearTime(GetCurrentViewTime() - m_flShowTime - 10);
}

void CPanel_Time::Paint(float x, float y, float w, float h)
{
	TPROF("CPanel_Time::Paint()");

	if (!MonitorWindow()->GetViewback()->HasConnection())
	{
		BaseClass::Paint(x, y, w, h);
		return;
	}

	double flSecondsToShow = m_flShowTime;
	double flTimeNow = GetCurrentViewTime();

	for (int i = (int)(flTimeNow - flSecondsToShow); i < flTimeNow; i++)
	{
		float flX = (float)RemapVal((double)i, flTimeNow - flSecondsToShow, flTimeNow, (double)x, (double)x + w);

		if (flX > x + w)
			continue;

		if (flX < x - 30)
			continue;

		tstring sTime = tsprintf("%ds", i);
		CLabel::PaintText(sTime, sTime.length(), "sans-serif", 12, flX + 5, y + h - 14, Color(128, 128, 128, 255), FRect(x, y, x+w, y+h));

		if (flX < x)
			continue;

		CBaseControl::PaintRect(flX, y, 1, h, Color(128, 128, 128, 255));
	}

	auto& oChannels = MonitorWindow()->GetViewback()->GetChannels();
	auto& oData = MonitorWindow()->GetViewback()->GetData();
	auto& oMeta = MonitorWindow()->GetViewback()->GetMeta();

	for (size_t i = 0; i < oChannels.size(); i++)
	{
		auto& oReg = oChannels[i];

		if (oReg.m_eDataType != VB_DATATYPE_INT && oReg.m_eDataType != VB_DATATYPE_FLOAT)
			continue;

		if (!oMeta[i].m_bVisible)
			continue;

		size_t iStart = 0;
		size_t iEnd = 0;
		if (oReg.m_eDataType == VB_DATATYPE_INT)
		{
			if (oData[i].m_aIntData.size() < 2)
				continue;

			iStart = FindStartTime(oData[i].m_aIntData, flTimeNow - flSecondsToShow);
			iEnd = FindStartTime(oData[i].m_aIntData, flTimeNow);

			// Allow the data to go flush up to the screen edge.
			if (iEnd < oData[i].m_aIntData.size() - 1)
				iEnd++;
		}
		else if (oReg.m_eDataType == VB_DATATYPE_FLOAT)
		{
			if (oData[i].m_aFloatData.size() < 2)
				continue;

			iStart = FindStartTime(oData[i].m_aFloatData, flTimeNow - flSecondsToShow);
			iEnd = FindStartTime(oData[i].m_aFloatData, flTimeNow);

			// Allow the data to go flush up to the screen edge.
			if (iEnd < oData[i].m_aFloatData.size() - 1)
				iEnd++;
		}
		else
			continue;

		// Allow the data to go flush up to the screen edge.
		if (iStart > 0)
			iStart--;

		if (oReg.m_eDataType == VB_DATATYPE_FLOAT)
		{
			auto& aFloatData = oData[i].m_aFloatData;

			if (oChannels[i].m_flMin != 0 || oChannels[i].m_flMax != 0)
			{
				oMeta[i].m_vecMaxValue.x = oChannels[i].m_flMin;
				oMeta[i].m_vecMaxValue.y = oChannels[i].m_flMax;

				float flRange = oChannels[i].m_flMax - oChannels[i].m_flMin;

				oMeta[i].m_vecMaxValue.x -= flRange * 0.3f;
				oMeta[i].m_vecMaxValue.y += flRange * 0.3f;
			}
			else
			{
				float flMin = aFloatData[iStart].data;
				float flMax = aFloatData[iStart].data;

				for (size_t j = iStart; j <= iEnd; j++)
				{
					if (aFloatData[j].time > flTimeNow)
						break;

					if (aFloatData[j].data < flMin)
						flMin = aFloatData[j].data;

					if (aFloatData[j].data > flMax)
						flMax = aFloatData[j].data;
				}

				float flRange = flMax - flMin;

				if (flRange > 0)
				{
					flMin -= flRange * 0.3f;
					flMax += flRange * 0.3f;

					oMeta[i].m_vecMaxValue.x = flMin; // Consider x the min
					oMeta[i].m_vecMaxValue.y = flMax; // Consider y the max
				}
			}

			CRenderingContext c(MonitorWindow()->GetRenderer(), true);

			Color clrColor(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 1.0f);
			c.UseProgram("line");

			c.SetUniform("vecColor", clrColor);
			c.SetBlend(BLEND_ALPHA);

			c.BeginRenderLines();

			c.Color(Color(255, 255, 255, 255));

			Vector v;
			for (size_t j = iStart; j < iEnd; j++)
			{
				float x1, x2, y1, y2;
				x1 = (float)RemapVal(aFloatData[j].time, flTimeNow - flSecondsToShow, flTimeNow, (double)x, (double)x + w);
				x2 = (float)RemapVal(aFloatData[j + 1].time, flTimeNow - flSecondsToShow, flTimeNow, (double)x, (double)x + w);

				if (x1 < x && x2 < x)
					continue;

				y1 = RemapVal(aFloatData[j].data, oMeta[i].m_vecMaxValue.x, oMeta[i].m_vecMaxValue.y, y + h, y);
				y2 = RemapVal(aFloatData[j + 1].data, oMeta[i].m_vecMaxValue.x, oMeta[i].m_vecMaxValue.y, y + h, y);

				if (x1 < x)
				{
					y1 = RemapVal(x, x1, x2, y1, y2);
					x1 = x;
				}

				v.x = x1;
				v.y = y1;
				c.Vertex(v);

				v.x = x2;
				v.y = y2;
				c.Vertex(v);
			}

			c.EndRender();

			tstring sValue = tsprintf("%.2f", aFloatData[iEnd].data);
			float flWidth = CLabel::GetTextWidth(sValue, sValue.length(), "sans-serif", 12);
			CLabel::PaintText(sValue, sValue.length(), "sans-serif", 12, x + w - flWidth - 10, v.y - 14, Color(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 1.0f));
		}

		if (oReg.m_eDataType == VB_DATATYPE_INT)
		{
			int iLabel = m_aiDataLabels[i];
			auto& aIntData = oData[i].m_aIntData;

			CLabel* pLabel = m_apLabels[iLabel];

			float ax, ay;
			pLabel->GetPaintPos(ax, ay);

			int iValue = aIntData[iStart].data;
			double flValueTimeStart = aIntData[iStart].time;

			Color clrBox(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 1.0f);

			for (size_t j = iStart + 1; j <= iEnd; j++)
			{
				float flXStart = (float)RemapVal(flValueTimeStart, flTimeNow - flSecondsToShow, flTimeNow, (double)x, (double)x + w);
				float flXEnd = (float)RemapVal(aIntData[j].time, flTimeNow - flSecondsToShow, flTimeNow, (double)x, (double)x + w) - 2;

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

bool CPanel_Time::MousePressed(int code, int mx, int my)
{
	bool bUsed = BaseClass::MousePressed(code, mx, my);

	if (!bUsed)
	{
		m_bDragging = true;
		m_aflLastDragVelocity.clear();

		if (!m_bOverrideTime)
		{
			m_bOverrideTime = true;
			m_flOverrideTime = MonitorWindow()->GetViewback()->PredictCurrentTime();
		}
	}

	return true;
}

bool CPanel_Time::MouseReleased(int code, int mx, int my)
{
	// Our heuristic for whether the user wants to fling is, is the finger
	// speeding up or slowing down at the end of the mouse event?
	int iNegativeAccel = 0;
	for (size_t i = 1; i < m_aflLastDragVelocity.size(); i++)
	{
		if (m_aflLastDragVelocity[i] < m_aflLastDragVelocity[i - 1])
			iNegativeAccel++;
	}

	if (m_bDragging && m_bOverrideTime && iNegativeAccel > 3 && m_aflLastDragVelocity.back() < -50.0f)
	{
		m_bOverrideTime = false;
		m_flFastForwardStartTime = RootPanel()->GetTime();
		m_flFastForwardFromTime = m_flOverrideTime;
	}

	m_bDragging = false;

	return BaseClass::MouseReleased(code, mx, my);
}

void CPanel_Time::CursorMoved(int mx, int my, int dx, int dy)
{
	BaseClass::CursorMoved(mx, my, dx, dy);

	if (m_bDragging && RootPanel()->GetFrameTime())
	{
		double flNewVelocity = dx / RootPanel()->GetFrameTime();
		m_aflLastDragVelocity.push_back(flNewVelocity);

		if (m_aflLastDragVelocity.size() > 6)
			m_aflLastDragVelocity.pop_front();
	}

	if (m_bDragging && m_bOverrideTime)
	{
		m_flOverrideTime -= RemapVal((double)dx, 0.0, (double)GetWidth(), 0.0, m_flShowTime);

		if (m_flOverrideTime > MonitorWindow()->GetViewback()->PredictCurrentTime())
			m_bOverrideTime = false;
	}
}

void CPanel_Time::ToggleVisibleCallback(const tstring& sArgs)
{
	unsigned int i = stoi(sArgs);

	auto& aMeta = MonitorWindow()->GetViewback()->GetMeta();

	if (i < 0)
		return;

	if (i >= aMeta.size())
		return;

	aMeta[i].m_bVisible = !aMeta[i].m_bVisible;

	Layout();
}


