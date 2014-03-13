#include "panel_container.h"

#include "application.h"

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
	float flWindowWidth = (float)Application()->GetWindowWidth();
	float flWindowHeight = (float)Application()->GetWindowHeight();

	SetSize(flWindowWidth, flWindowHeight);

	if (m_pMaximizedPanel)
	{
		m_pPanelConsole->SetIsMaximized(false);
		m_pPanelConsole->SetVisible(false);

		m_pPanel2D->SetIsMaximized(false);
		m_pPanel2D->SetVisible(false);

		m_pPanelTime->SetIsMaximized(false);
		m_pPanelTime->SetVisible(false);

		m_pMaximizedPanel->SetIsMaximized(true);
		m_pMaximizedPanel->SetVisible(true);
		m_pMaximizedPanel->SetPos(0, 0);
		m_pMaximizedPanel->SetSize(flWindowWidth, flWindowHeight);
	}
	else
	{
		m_pPanelConsole->SetIsMaximized(false);
		m_pPanelConsole->SetVisible(true);
		m_pPanelConsole->SetSize(flWindowWidth / 2, flWindowHeight / 2);

		m_pPanel2D->SetIsMaximized(false);
		m_pPanel2D->SetVisible(true);
		m_pPanel2D->SetPos(flWindowWidth / 2, 0);
		m_pPanel2D->SetSize(flWindowWidth / 2, flWindowHeight / 2);

		m_pPanelTime->SetIsMaximized(false);
		m_pPanelTime->SetVisible(true);
		m_pPanelTime->SetPos(0, flWindowHeight / 2);
		m_pPanelTime->SetSize(flWindowWidth, flWindowHeight / 2);
	}

	BaseClass::Layout();
}

void CPanelContainer::SetMaximizedPanel(const glgui::CControl<CPanel_Base>& pPanel)
{
	m_pMaximizedPanel = pPanel;

	Layout();
}

glgui::CControl<class CPanel_Console> CPanelContainer::GetConsolePanel()
{
	return m_pPanelConsole;
}

