#include "panel_container.h"

#include <renderer/renderer.h>
#include <glgui/menu.h>

#include "panel_console.h"
#include "panel_2D.h"
#include "panel_time.h"
#include "monitor_window.h"
#include "monitor_menu.h"

using namespace glgui;

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

	m_pViewbackButton = AddControl(new CMonitorMenu());
	m_pViewbackButton->SetMenuOpen(MENUOPEN_SIDE);
	m_pGroupsButton = AddControl(new CMenu("Groups"));
	m_pGroupsButton->SetMenuOpen(MENUOPEN_SIDE);

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

	m_pGroupsButton->ClearSubmenus();

	const auto& aGroups = Viewback()->GetGroups();

	for (auto& oGroup : aGroups)
		m_pGroupsButton->AddSubmenu(oGroup.m_sName.c_str(), this, ShowGroup);

	Layout();
}

void CPanelContainer::Layout()
{
	if (!Application()->GetRenderer())
		return;

	float flWindowWidth = (float)Application()->GetRenderer()->GetDrawableWidth();
	float flWindowHeight = (float)Application()->GetRenderer()->GetDrawableHeight();

	SetSize(flWindowWidth, flWindowHeight);

	flWindowWidth -= MonitorWindow()->ButtonPanelWidth();

	if (m_pMaximizedPanel)
	{
		m_pPanelConsole->SetIsMaximized(false);

		m_pPanel2D->SetIsMaximized(false);

		m_pPanelTime->SetIsMaximized(false);

		m_pMaximizedPanel->SetIsMaximized(true);
		m_pMaximizedPanel->SetDimensionsAnimate(FRect(MonitorWindow()->ButtonPanelWidth(), 0, flWindowWidth, flWindowHeight), 0.3);
	}
	else
	{
		m_pPanelConsole->SetIsMaximized(false);
		m_pPanelConsole->SetDimensionsAnimate(FRect(MonitorWindow()->ButtonPanelWidth(), 0, flWindowWidth / 2, flWindowHeight / 2), 0.3);

		m_pPanel2D->SetIsMaximized(false);
		m_pPanel2D->SetDimensionsAnimate(FRect(m_pPanelConsole->GetRight(), 0, flWindowWidth / 2, flWindowHeight / 2), 0.3);

		m_pPanelTime->SetIsMaximized(false);
		m_pPanelTime->SetDimensionsAnimate(FRect(MonitorWindow()->ButtonPanelWidth(), flWindowHeight / 2, flWindowWidth, flWindowHeight / 2), 0.3);
	}

	m_pViewbackButton->SetSize(MonitorWindow()->ButtonPanelWidth(), MonitorWindow()->ButtonPanelWidth());
	m_pGroupsButton->SetSize(MonitorWindow()->ButtonPanelWidth(), MonitorWindow()->ButtonPanelWidth());
	m_pGroupsButton->SetPos(0, MonitorWindow()->ButtonPanelWidth());

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

void CPanelContainer::ShowGroupCallback(const tstring& sArgs)
{
	m_pGroupsButton->CloseMenu();

	int iGroup = stoi(sArgs);

	auto& aMeta = Viewback()->GetMeta();
	for (auto& oMeta : aMeta)
		oMeta.m_bVisible = false;

	auto& aGroups = Viewback()->GetGroups();
	for (auto& i : aGroups[iGroup].m_iChannels)
		aMeta[i].m_bVisible = true;

	Layout();
}

glgui::CControl<class CPanel_Console> CPanelContainer::GetConsolePanel()
{
	return m_pPanelConsole;
}

