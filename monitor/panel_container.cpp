#include "panel_container.h"

#include <renderer/renderer.h>
#include <glgui/menu.h>
#include <textures/texturesheet.h>

#include "panel_console.h"
#include "panel_2D.h"
#include "panel_time.h"
#include "panel_controls.h"
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

	m_pPanelControls = AddControl(new CPanel_Controls());
	m_pPanelControls->SetBorder(CBaseControl::BT_SOME);
	m_pPanelControls->SetBackgroundColor(Color(0, 0, 0, 255));
	m_pPanelControls->SetHighlighted(true);

	m_pViewbackButton = AddControl(new CMonitorMenu(false));
	m_pViewbackButton->SetTooltip("Options");
	m_pChannelsButton = AddControl(new CPictureButton("Channels"));
	m_pChannelsButton->SetTooltip("Channels");
	m_pChannelsButton->SetClickedListener(this, Channels);
	m_pGroupsButton = AddControl(new CMenu("Groups"));
	m_pGroupsButton->SetTooltip("Groups");

	CTextureSheet oSheet("materials/buttons.txt");

	m_pChannelsButton->SetSheetTexture(oSheet.GetSheet("Channels"), oSheet.GetArea("Channels"), oSheet.GetSheetWidth("Channels"), oSheet.GetSheetHeight("Channels"));
	m_pGroupsButton->SetSheetTexture(oSheet.GetSheet("Groups"), oSheet.GetArea("Groups"), oSheet.GetSheetWidth("Groups"), oSheet.GetSheetHeight("Groups"));

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

	CChannelPanel::Get()->RegistrationUpdate();

	Layout();
}

void CPanelContainer::Layout()
{
	if (!Application()->GetRenderer())
		return;

	float flWindowWidth = (float)Application()->GetRenderer()->GetDrawableWidth();
	float flWindowHeight = (float)Application()->GetRenderer()->GetDrawableHeight();

	SetSize(flWindowWidth, flWindowHeight);

	if (MonitorWindow()->ButtonPanelSide())
		flWindowWidth -= MonitorWindow()->ButtonPanelSize();
	else
		flWindowHeight -= MonitorWindow()->ButtonPanelSize();

	if (m_pMaximizedPanel)
	{
		m_pPanelConsole->SetIsMaximized(false);
		m_pPanel2D->SetIsMaximized(false);
		m_pPanelTime->SetIsMaximized(false);
		m_pPanelControls->SetIsMaximized(false);

		m_pMaximizedPanel->SetIsMaximized(true);

		if (MonitorWindow()->ButtonPanelSide())
			m_pMaximizedPanel->SetDimensionsAnimate(FRect(MonitorWindow()->ButtonPanelSize(), 0, flWindowWidth, flWindowHeight), 0.3);
		else
			m_pMaximizedPanel->SetDimensionsAnimate(FRect(0, MonitorWindow()->ButtonPanelSize(), flWindowWidth, flWindowHeight), 0.3);
	}
	else
	{
		m_pPanelConsole->SetIsMaximized(false);
		m_pPanel2D->SetIsMaximized(false);
		m_pPanelTime->SetIsMaximized(false);
		m_pPanelControls->SetIsMaximized(false);

		if (MonitorWindow()->ButtonPanelSide())
		{
			m_pPanelConsole->SetDimensionsAnimate(FRect(MonitorWindow()->ButtonPanelSize(), 0, flWindowWidth / 2, flWindowHeight / 2), 0.3);
			m_pPanel2D->SetDimensionsAnimate(FRect(m_pPanelConsole->GetRight(), 0, flWindowWidth / 2, flWindowHeight / 2), 0.3);
			m_pPanelControls->SetDimensionsAnimate(FRect(MonitorWindow()->ButtonPanelSize(), flWindowHeight / 2, flWindowWidth / 2, flWindowHeight / 2), 0.3);
			m_pPanelTime->SetDimensionsAnimate(FRect(m_pPanelControls->GetRight(), flWindowHeight / 2, flWindowWidth / 2, flWindowHeight / 2), 0.3);
		}
		else
		{
			m_pPanelConsole->SetDimensionsAnimate(FRect(0, MonitorWindow()->ButtonPanelSize(), flWindowWidth / 2, flWindowHeight / 2), 0.3);
			m_pPanel2D->SetDimensionsAnimate(FRect(m_pPanelConsole->GetRight(), MonitorWindow()->ButtonPanelSize(), flWindowWidth / 2, flWindowHeight / 2), 0.3);
			m_pPanelControls->SetDimensionsAnimate(FRect(0, m_pPanelConsole->GetBottom(), flWindowWidth / 2, flWindowHeight / 2), 0.3);
			m_pPanelTime->SetDimensionsAnimate(FRect(m_pPanelControls->GetRight(), m_pPanelConsole->GetBottom(), flWindowWidth / 2, flWindowHeight / 2), 0.3);
		}
	}

	menuopen_t eOpen = MENUOPEN_BOTTOM;
	if (GetWidth() > GetHeight())
		eOpen = MENUOPEN_SIDE;

	m_pViewbackButton->SetMenuOpen(eOpen);
	m_pGroupsButton->SetMenuOpen(eOpen);

	tvector<CButton*> apButtons;
	apButtons.push_back(m_pViewbackButton);
	apButtons.push_back(m_pChannelsButton);
	apButtons.push_back(m_pGroupsButton);

	if (MonitorWindow()->ButtonPanelSide())
	{
		for (size_t i = 0; i < apButtons.size(); i++)
		{
			apButtons[i]->SetSize(MonitorWindow()->ButtonPanelSize(), MonitorWindow()->ButtonPanelSize());
			apButtons[i]->SetPos(0, MonitorWindow()->ButtonPanelSize() * i);
		}
	}
	else
	{
		for (size_t i = 0; i < apButtons.size(); i++)
		{
			apButtons[i]->SetSize(MonitorWindow()->ButtonPanelSize(), MonitorWindow()->ButtonPanelSize());
			apButtons[i]->SetPos(MonitorWindow()->ButtonPanelSize() * i, 0);
		}
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

void CPanelContainer::ShowGroupCallback(const tstring& sArgs)
{
	m_pGroupsButton->CloseMenu();

	int iGroup = stoi(sArgs);

	Viewback()->ActivateGroup(iGroup);

	Layout();

	CChannelPanel::Get()->Layout();
}

void CPanelContainer::ChannelsCallback(const tstring& sArgs)
{
	CChannelPanel::Create();
}

glgui::CControl<class CPanel_Console> CPanelContainer::GetConsolePanel()
{
	return m_pPanelConsole;
}

