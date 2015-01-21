#include "monitor_menu.h"

#include <glgui/label.h>
#include <glgui/textfield.h>
#include <glgui/button.h>
#include <textures/texturesheet.h>
#include <glgui/checkbox.h>

#include "monitor_window.h"
#include "panel_container.h"

using namespace std;
using namespace glgui;
using namespace vb;

CMonitorMenu::CMonitorMenu(bool bHideOnClose)
	: CMenu("Options")
{
	m_bHideOnClose = bHideOnClose;

	// Add one or else the menu won't open.
	AddSubmenu("Quit", this, Quit);

	if (!m_bHideOnClose)
	{
		CTextureSheet oSheet("materials/buttons.txt");
		SetSheetTexture(oSheet.GetSheet("Viewback"), oSheet.GetArea("Viewback"), oSheet.GetSheetWidth("Viewback"), oSheet.GetSheetHeight("Viewback"));
	}
}

void CMonitorMenu::OnOpenMenu()
{
	ClearSubmenus();

#ifdef _DEBUG
	AddSubmenu("Open Console", this, Console);
#endif

	AddSubmenu("View Servers", this, ViewServers);
	AddSubmenu("Manual Connect", this, ManualConnect);

	if (MonitorWindow()->GetViewback()->HasConnection())
		AddSubmenu("Disconnect", this, Disconnect);
	else
		AddSubmenu("Connect To Best Server", this, FindServer);

	AddSubmenu("Quit", this, Quit);
}

void CMonitorMenu::Think()
{
	bool bHeightWasZero = (m_flMenuHeight == 0);

	BaseClass::Think();

	if (m_bHideOnClose && Application()->PlatformHasMenuKey() && !bHeightWasZero && m_flMenuHeight == 0)
		SetVisible(false);
}

void CMonitorMenu::ConsoleCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	Application()->ToggleConsole();
	CloseMenu();
}

void CMonitorMenu::ViewServersCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	CServerListPanel::Create();

	CloseMenu();
}

void CMonitorMenu::ManualConnectCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	CManualConnectPanel::Create();

	CloseMenu();
}

void CMonitorMenu::FindServerCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	MonitorWindow()->GetViewback()->FindServer();
	CloseMenu();
}

void CMonitorMenu::DisconnectCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	MonitorWindow()->GetViewback()->Disconnect();
	CloseMenu();
}

void CMonitorMenu::QuitCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	Application()->Close();
	CloseMenu();
}

CServerListPanel::CServerListPanel()
	: CMovablePanel("Available Servers")
{
}

CControlResource CServerListPanel::s_server_list;

void CServerListPanel::Create()
{
	if (!s_server_list)
		s_server_list = (new CServerListPanel())->shared_from_this();

	CServerListPanel* pServerList = s_server_list.DowncastStatic<CServerListPanel>();

	pServerList->SetSize(350, 550);
	pServerList->Layout();
	pServerList->MoveToCenter();
	pServerList->SetVisible(true);

	pServerList->m_refresh_time = Application()->GetTime() + 1.5f;
}

void CServerListPanel::SetParent(CControlHandle hParent)
{
	BaseClass::SetParent(hParent);

	if (!hParent)
		s_server_list.reset();
}

void CServerListPanel::Layout()
{
	BaseClass::Layout();

	ClearControls();

	CTextureSheet oSheet("materials/buttons.txt");
	CControl<CPictureButton> refresh_button = AddControl(new CPictureButton("Refresh"));
	refresh_button->SetSheetTexture(oSheet.GetSheet("Refresh"), oSheet.GetArea("Refresh"), oSheet.GetSheetWidth("Refresh"), oSheet.GetSheetHeight("Refresh"));
	refresh_button->SetSize(30, 30);
	refresh_button->Layout_AlignRight();
	refresh_button->SetTop(T_HEADER_HEIGHT + GetDefaultMargin());
	refresh_button->SetClickedListener(this, Refresh);

	vector<CServerListing> server_list = MonitorWindow()->GetViewback()->GetServers();

	if (server_list.size())
	{
		for (size_t i = 0; i < server_list.size(); i++)
		{
			in_addr in;
			in.s_addr = htonl(server_list[i].address);
			const char* string_address = inet_ntoa(in);

			CControl<CButton> server_listing = AddControl(new CButton(server_list[i].name + " " + string(tsprintf(" (%s:%d)", string_address, server_list[i].tcp_port).c_str())));
			server_listing->Layout_FullWidth();
			server_listing->SetTop(i * (server_listing->GetHeight() + 10) + refresh_button->GetBottom() + GetDefaultMargin());
			server_listing->SetClickedListener(this, Connect, tsprintf("%s:%d", string_address, server_list[i].tcp_port));
		}
	}
	else
	{
		CControl<CLabel> searching_label = AddControl(new CLabel("Searching for servers..."));
		searching_label->Layout_FullWidth();
		searching_label->SetTop(refresh_button->GetBottom() + GetDefaultMargin());

		if (!MonitorWindow()->GetViewback()->GetServers().size())
			m_refresh_time = Application()->GetTime() + 0.1f;
	}
}

void CServerListPanel::Think()
{
	BaseClass::Think();

	if (m_refresh_time > 0 && Application()->GetTime() > m_refresh_time)
	{
		m_refresh_time = 0;
		Layout();
	}
}

void CServerListPanel::RefreshCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	Layout();
}

void CServerListPanel::ConnectCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	tvector<tstring> tokens;
	tstrtok(sArgs, tokens, ":");

	MonitorWindow()->GetViewback()->Connect(tokens[0].c_str(), stoi(tokens[1]));

	if (MonitorWindow()->GetViewback()->HasConnection())
		SetVisible(false);

	Layout();
}

CControlResource CManualConnectPanel::s_panel;

CManualConnectPanel::CManualConnectPanel()
	: CMovablePanel("Manual Connect")
{
	m_pIPLabel = AddControl(new CLabel("IP:"));
	m_pIP = AddControl(new CTextField());
	m_pPortLabel = AddControl(new CLabel("Port:"));
	m_pPort = AddControl(new CTextField());
	m_pConnect = AddControl(new CButton("Connect"));
	m_pConnect->SetClickedListener(this, Connect);
}

void CManualConnectPanel::Create()
{
	if (!s_panel)
		s_panel = (new CManualConnectPanel())->shared_from_this();

	CManualConnectPanel* pPanel = s_panel.DowncastStatic<CManualConnectPanel>();

	pPanel->SetSize(350, 150);
	pPanel->Layout();
	pPanel->MoveToCenter();
	pPanel->SetVisible(true);

	pPanel->m_pIP->SetText(MonitorWindow()->GetLastSuccessfulIP());
	pPanel->m_pPort->SetText(MonitorWindow()->GetLastSuccessfulPort());

	CRootPanel::Get()->SetFocus(pPanel->m_pIP);
}

void CManualConnectPanel::SetParent(CControlHandle hParent)
{
	BaseClass::SetParent(hParent);

	if (!hParent)
		s_panel.reset();
}

void CManualConnectPanel::Layout()
{
	BaseClass::Layout();

	float flOutsideMargin = 25;
	float flInsideMargin = 10;

	m_pIPLabel->SetAlign(CLabel::TA_LEFTCENTER);
	m_pIPLabel->SetLeft(flOutsideMargin);
	m_pIPLabel->SetTop(40);
	m_pIPLabel->SetWidth(0);
	m_pIPLabel->EnsureTextFits();

	m_pPortLabel->SetAlign(CLabel::TA_LEFTCENTER);
	m_pPortLabel->SetLeft(220);
	m_pPortLabel->SetTop(40);
	m_pPortLabel->SetWidth(0);
	m_pPortLabel->EnsureTextFits();

	m_pIP->SetLeft(m_pIPLabel->GetRight() + flInsideMargin);
	m_pIP->SetTop(40);
	m_pIP->SetRight(m_pPortLabel->GetLeft() - flInsideMargin);

	m_pPort->SetLeft(m_pPortLabel->GetRight() + flInsideMargin);
	m_pPort->SetTop(40);
	m_pPort->SetRight(GetWidth() - flOutsideMargin);

	m_pConnect->SetPos(0, 90);
	m_pConnect->Layout_CenterHorizontal();
}

void CManualConnectPanel::ConnectCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	MonitorWindow()->GetViewback()->Connect(m_pIP->GetText().c_str(), stoi(m_pPort->GetText()));

	if (MonitorWindow()->GetViewback()->HasConnection())
	{
		SetVisible(false);
		MonitorWindow()->SetLastSuccessfulIP(m_pIP->GetText());
		MonitorWindow()->SetLastSuccessfulPort(m_pPort->GetText());
		MonitorWindow()->SaveConfig();
	}
}

CControlResource CAddSomethingPanel::s_panel;

CAddSomethingPanel::CAddSomethingPanel()
	: CMovablePanel("Add Something")
{
	m_search_label = AddControl(new CLabel("Search:"));
	m_search = AddControl(new CTextField());
	m_search->SetContentsChangedListener(this, ReSearch);

	m_search_results_panel = AddControl(new CPanel());
}

void CAddSomethingPanel::Create()
{
	if (!s_panel)
		s_panel = (new CAddSomethingPanel())->shared_from_this();

	CAddSomethingPanel* pPanel = s_panel.DowncastStatic<CAddSomethingPanel>();

	pPanel->m_search->SetText("");

	pPanel->SetSize(350, 550);
	pPanel->Layout();
	pPanel->MoveToCenter();
	pPanel->SetVisible(true);

	CRootPanel::Get()->SetFocus(pPanel->m_search);
}

void CAddSomethingPanel::SetParent(CControlHandle hParent)
{
	BaseClass::SetParent(hParent);

	if (!hParent)
		s_panel.reset();
}

void CAddSomethingPanel::Layout()
{
	float flOutsideMargin = 25;
	float flInsideMargin = 10;

	m_search_label->SetAlign(CLabel::TA_LEFTCENTER);
	m_search_label->SetLeft(flOutsideMargin);
	m_search_label->SetTop(40);
	m_search_label->SetWidth(0);
	m_search_label->EnsureTextFits();

	m_search->SetLeft(m_search_label->GetRight() + flInsideMargin);
	m_search->SetTop(40);
	m_search->SetRight(GetWidth() - flOutsideMargin);

	m_search_results_panel->SetTop(m_search_label->GetBottom() + flInsideMargin);
	m_search_results_panel->SetBottom(GetHeight() - flOutsideMargin);
	m_search_results_panel->Layout_FullWidth();
	m_search_results_panel->SetBackgroundColor(Color(17, 17, 17, 255));
	m_search_results_panel->SetVerticalScrollBarEnabled(true);
	m_search_results_panel->SetScissoring(true);

	for (size_t i = 0; i < m_search_results.size(); i++)
		m_search_results_panel->RemoveControl(m_search_results[i]);
	m_search_results.clear();

	float y_pos = flOutsideMargin;

	for (size_t i = 0; i < Viewback()->GetChannels().size(); i++)
	{
		auto channel = Viewback()->GetChannels()[i];

		// Yes I know this is horrible code.
		tstring name = tstring(channel.m_sName.c_str()).tolower();
		if (!strstr(name.c_str(), m_search->GetText().c_str()))
			continue;

		if (channel.m_eDataType != VB_DATATYPE_INT && channel.m_eDataType != VB_DATATYPE_FLOAT)
			continue;

		m_search_results.push_back(m_search_results_panel->AddControl(new CButton(name), true));
		m_search_results.back()->Layout_FullWidth();
		m_search_results.back()->SetTop(y_pos);
		m_search_results.back()->SetClickedListener(this, Add, tsprintf("%d", i));

		y_pos += m_search_results.back()->GetHeight() + flInsideMargin;
	}

	BaseClass::Layout();
}

void CAddSomethingPanel::ReSearchCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	Layout();
}

void CAddSomethingPanel::AddCallback(glgui::CBaseControl*, const tstring& sArgs)
{
	Close();

	Viewback()->Profile_AddChannel(atoi(sArgs.c_str()));
}
