#include "panel_2d.h"

#include <renderer/renderingcontext.h>
#include <tinker/profiler.h>
#include <glgui/button.h>

#include "monitor_window.h"

using namespace glgui;

void CPanel_2D::RegistrationUpdate()
{
	m_apLabels.clear();
	m_aiDataLabels.clear();

	auto& aChannels = MonitorWindow()->GetViewback()->GetChannels();
	auto& aMeta = MonitorWindow()->GetViewback()->GetMeta();

	m_aiDataLabels.resize(aChannels.size());
	for (int& i : m_aiDataLabels)
		i = -1;

	for (size_t i = 0; i < aChannels.size(); i++)
	{
		auto& oReg = aChannels[i];
		auto& oMeta = aMeta[i];

		if (oReg.m_eDataType != VB_DATATYPE_VECTOR)
			continue;

		m_apLabels.push_back(AddControl(new CButton(oReg.m_sName)));
		m_apLabels.back()->SetTextColor(Color(oMeta.m_clrColor.x, oMeta.m_clrColor.y, oMeta.m_clrColor.z, 1.0f));
		m_apLabels.back()->SetAlign(CLabel::TA_MIDDLECENTER);
		m_apLabels.back()->SetHeight(18);
		m_apLabels.back()->SetClickedListener(this, ToggleVisible, tsprintf("%d", i));
		m_apLabels.back()->SetBorder(BT_SOME);

		m_aiDataLabels[i] = m_apLabels.size() - 1;
	}

	Layout();
}

void CPanel_2D::Layout()
{
	BaseClass::Layout();

	auto& aChannels = MonitorWindow()->GetViewback()->GetChannels();
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

	float flYPos = 10;

	for (size_t i = 0; i < aMeta.size(); i++)
	{
		if (m_aiDataLabels[i] < 0)
			continue;

		int iLabel = m_aiDataLabels[i];

		m_apLabels[iLabel]->SetVisible(false);

		if (!aChannels[i].m_bActive)
			continue;

		m_apLabels[iLabel]->SetVisible(true);
		m_apLabels[iLabel]->SetPos(10, flYPos);

		flYPos += m_apLabels[iLabel]->GetHeight() + 10;
	}
}

void CPanel_2D::Paint(float x, float y, float w, float h)
{
	TPROF("CPanel_2D::Paint()");

	BaseClass::Paint(x, y, w, h);

	float flRatio = h / w;

	CBaseControl::PaintRect(x + w / 2, y + h / 4, 1, h / 2, Color(128, 128, 128, 255));
	CBaseControl::PaintRect(x + w / 2 - (flRatio * w / 4), y + h / 2, w * flRatio / 2, 1, Color(128, 128, 128, 255));

	auto& oChannels = MonitorWindow()->GetViewback()->GetChannels();
	auto& oData = MonitorWindow()->GetViewback()->GetData();
	auto& oMeta = MonitorWindow()->GetViewback()->GetMeta();

	double flLatestData = MonitorWindow()->GetViewback()->GetLatestDataTime();

	for (size_t i = 0; i < oChannels.size(); i++)
	{
		auto& oReg = oChannels[i];
		if (oReg.m_eDataType != VB_DATATYPE_VECTOR)
			continue;

		if (oData[i].m_aVectorData.size() < 2)
			continue;

		if (!oMeta[i].m_bVisible)
			continue;

		if (!oChannels[i].m_bActive)
			continue;

		auto& aVectorData = oData[i].m_aVectorData;

		size_t iStart = FindStartTime(aVectorData, flLatestData - oMeta[i].m_flDisplayDuration);

		if (oChannels[i].m_flMax != 0)
		{
			oMeta[i].m_vecMaxValue.x = oChannels[i].m_flMax;
			oMeta[i].m_vecMaxValue.y = oChannels[i].m_flMax;
		}
		else
		{
			for (size_t j = iStart; j < aVectorData.size() - 1; j++)
			{
				if (std::fabs(aVectorData[j].data.x) > oMeta[i].m_vecMaxValue.x)
					oMeta[i].m_vecMaxValue.x = std::fabs(aVectorData[j].data.x);

				if (std::fabs(aVectorData[j].data.y) > oMeta[i].m_vecMaxValue.y)
					oMeta[i].m_vecMaxValue.y = std::fabs(aVectorData[j].data.y);
			}
		}

		float flMaxX = oMeta[i].m_vecMaxValue.x * 1.3f;
		float flMaxY = oMeta[i].m_vecMaxValue.y / flRatio * 1.3f;

		float flMax = std::max(flMaxX, flMaxY);

		CRenderingContext c(MonitorWindow()->GetRenderer(), true);

		c.UseProgram("line");
		c.SetUniform("vecColor", Vector4D(1, 1, 1, 1));
		c.SetBlend(BLEND_ALPHA);

		c.BeginRenderLines();

		Color clrColor(oMeta[i].m_clrColor.x, oMeta[i].m_clrColor.y, oMeta[i].m_clrColor.z, 0.1f);
		c.Color(clrColor);

		Vector v;
		for (size_t j = iStart; j < aVectorData.size() - 1; j++)
		{
			v.x = RemapVal(aVectorData[j].data.x, -flMax, flMax, x, x + w);
			v.y = RemapVal(aVectorData[j].data.y, -flMax * flRatio, flMax * flRatio, y, y + h);
			c.Vertex(v);

			c.Color(Color(clrColor.r(), clrColor.g(), clrColor.b(), (int)RemapVal((float)j, (float)iStart, (float)aVectorData.size() - 2, 25, 255)));

			v.x = RemapVal(aVectorData[j + 1].data.x, -flMax, flMax, x, x + w);
			v.y = RemapVal(aVectorData[j + 1].data.y, -flMax * flRatio, flMax * flRatio, y, y + h);
			c.Vertex(v);
		}

		c.EndRender();
	}
}

void CPanel_2D::ToggleVisibleCallback(const tstring& sArgs)
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

