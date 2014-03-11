#include "panel_2d.h"

#include <renderer/renderingcontext.h>
#include <tinker/profiler.h>
#include <glgui/label.h>

#include "monitor_window.h"

using namespace glgui;

void CPanel_2D::RegistrationUpdate()
{
	m_apLabels.clear();

	auto& aRegistrations = MonitorWindow()->GetViewback()->GetRegistrations();
	auto& aMeta = MonitorWindow()->GetViewback()->GetMeta();

	float flYPos = 10;

	for (size_t i = 0; i < aRegistrations.size(); i++)
	{
		auto& oReg = aRegistrations[i];
		auto& oMeta = aMeta[i];

		if (oReg.m_eDataType != VB_DATATYPE_VECTOR)
			continue;

		m_apLabels.push_back(AddControl(new CLabel(oReg.m_sFieldName)));
		m_apLabels.back()->SetPos(10, flYPos);
		m_apLabels.back()->SetTextColor(Color(oMeta.m_clrColor.x, oMeta.m_clrColor.y, oMeta.m_clrColor.z, 1.0f));
		m_apLabels.back()->SetAlign(CLabel::TA_BOTTOMLEFT);

		flYPos += m_apLabels.back()->GetHeight() + 10;
	}
}

void CPanel_2D::Paint(float x, float y, float w, float h)
{
	TPROF("CPanel_2D::Paint()");

	BaseClass::Paint(x, y, w, h);

	float flRatio = h / w;

	CBaseControl::PaintRect(x + w / 2, y + h / 4, 1, h / 2, Color(128, 128, 128, 255));
	CBaseControl::PaintRect(x + w / 2 - (flRatio * w / 4), y + h / 2, w * flRatio / 2, 1, Color(128, 128, 128, 255));

	auto& oRegistrations = MonitorWindow()->GetViewback()->GetRegistrations();
	auto& oData = MonitorWindow()->GetViewback()->GetData();
	auto& oMeta = MonitorWindow()->GetViewback()->GetMeta();

	for (size_t i = 0; i < oRegistrations.size(); i++)
	{
		auto& oReg = oRegistrations[i];
		if (oReg.m_eDataType != VB_DATATYPE_VECTOR)
			continue;

		if (oData[i].m_aVectorData.size() < 2)
			continue;

		auto& aVectorData = oData[i].m_aVectorData;

		size_t iStart = FindStartTime(aVectorData, oMeta[i].m_flDisplayDuration);

		for (size_t j = iStart; j < aVectorData.size() - 1; j++)
		{
			if (std::fabs(aVectorData[j].data.x) > oMeta[i].m_vecMaxValue.x)
				oMeta[i].m_vecMaxValue.x = std::fabs(aVectorData[j].data.x);

			if (std::fabs(aVectorData[j].data.y) > oMeta[i].m_vecMaxValue.y)
				oMeta[i].m_vecMaxValue.y = std::fabs(aVectorData[j].data.y);
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

