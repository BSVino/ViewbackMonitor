#include "panel_container.h"

#include <application.h>
#include <renderer/renderer.h>

#include "panel_console.h"
#include "panel_2D.h"
#include "panel_time.h"

void CPanelContainer::Setup()
{
	m_pPanelConsole = AddControl(new CPanel_Console());
	m_pPanelConsole->SetBorder(CBaseControl::BT_SOME);
	m_pPanelConsole->SetBackgroundColor(Color(0, 0, 0, 255));
	m_pPanelConsole->SetHighlighted(true);

	m_pPanel2D = AddControl(new CPanel_2D());
	m_pPanel2D->SetBorder(CBaseControl::BT_SOME);
	m_pPanel2D->SetBackgroundColor(Color(0, 0, 0, 255));
	m_pPanel2D->SetHighlighted(false);

	m_pPanelTime = AddControl(new CPanel_Time());
	m_pPanelTime->SetBorder(CBaseControl::BT_SOME);
	m_pPanelTime->SetBackgroundColor(Color(0, 0, 0, 255));
	m_pPanelTime->SetHighlighted(true);

	Layout();
}

void CPanelContainer::RegistrationUpdate()
{
	for (auto& pChild : m_apControls)
	{
		CPanel_Base* pPanelBase = dynamic_cast<CPanel_Base*>(pChild.get());
		if (!pPanelBase)
			continue;

		pPanelBase->RegistrationUpdate();
	}
}

void CPanelContainer::Layout()
{
	if (!Application()->GetRenderer())
		return;

	float flWindowWidth = (float)Application()->GetRenderer()->GetDrawableWidth();
	float flWindowHeight = (float)Application()->GetRenderer()->GetDrawableHeight();

	SetSize(flWindowWidth, flWindowHeight);

	if (m_pMaximizedPanel)
	{
		m_pPanelConsole->SetIsMaximized(false);

		m_pPanel2D->SetIsMaximized(false);

		m_pPanelTime->SetIsMaximized(false);

		m_pMaximizedPanel->SetIsMaximized(true);
		m_pMaximizedPanel->SetDimensionsAnimate(FRect(0, 0, flWindowWidth, flWindowHeight), 0.3);
	}
	else
	{
		m_pPanelConsole->SetIsMaximized(false);
		m_pPanelConsole->SetDimensionsAnimate(FRect(0, 0, flWindowWidth / 2, flWindowHeight / 2), 0.3);

		m_pPanel2D->SetIsMaximized(false);
		m_pPanel2D->SetDimensionsAnimate(FRect(flWindowWidth / 2, 0, flWindowWidth / 2, flWindowHeight / 2), 0.3);

		m_pPanelTime->SetIsMaximized(false);
		m_pPanelTime->SetDimensionsAnimate(FRect(0, flWindowHeight / 2, flWindowWidth, flWindowHeight / 2), 0.3);
	}

	BaseClass::Layout();
}

void CPanelContainer::SetMaximizedPanel(const glgui::CControl<CPanel_Base>& pPanel)
{
	if (m_pMaximizedPanel && !pPanel)
		MoveToTop(m_pMaximizedPanel);

	if (pPanel && !m_pMaximizedPanel)
		MoveToTop(pPanel);

	m_pMaximizedPanel = pPanel;

	Layout();
}

glgui::CControl<class CPanel_Console> CPanelContainer::GetConsolePanel()
{
	return m_pPanelConsole;
}

