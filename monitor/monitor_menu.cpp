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

void CMonitorMenu::ConsoleCallback(const tstring& sArgs)
{
	Application()->ToggleConsole();
	CloseMenu();
}

void CMonitorMenu::ViewServersCallback(const tstring& sArgs)
{
	CServerListPanel::Create();

	CloseMenu();
}

void CMonitorMenu::ManualConnectCallback(const tstring& sArgs)
{
	CManualConnectPanel::Create();

	CloseMenu();
}

void CMonitorMenu::FindServerCallback(const tstring& sArgs)
{
	MonitorWindow()->GetViewback()->FindServer();
	CloseMenu();
}

void CMonitorMenu::DisconnectCallback(const tstring& sArgs)
{
	MonitorWindow()->GetViewback()->Disconnect();
	CloseMenu();
}

void CMonitorMenu::QuitCallback(const tstring& sArgs)
{
	Application()->Close();
	CloseMenu();
}

CServerListPanel::CServerListPanel()
	: CMovablePanel("Available Servers")
{
}

void CServerListPanel::Create()
{
	static CServerListPanel* pPanel = NULL;

	if (!pPanel)
		pPanel = new CServerListPanel();

	pPanel->SetSize(350, 550);
	pPanel->Layout();
	pPanel->MoveToCenter();
	pPanel->SetVisible(true);

	pPanel->m_refresh_time = Application()->GetTime() + 1.5f;
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
			in.S_un.S_addr = htonl(server_list[i].address);
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
	}
}

void CServerListPanel::Think()
{
	BaseClass::Think();

	if (m_refresh_time > 0 && Application()->GetTime() > m_refresh_time)
	{
		m_refresh_time = 0;
		Layout();

		if (!MonitorWindow()->GetViewback()->GetServers().size())
			m_refresh_time = Application()->GetTime() + 0.1f;
	}
}

void CServerListPanel::RefreshCallback(const tstring& sArgs)
{
	Layout();
}

void CServerListPanel::ConnectCallback(const tstring& sArgs)
{
	tvector<tstring> tokens;
	tstrtok(sArgs, tokens, ":");

	MonitorWindow()->GetViewback()->Connect(tokens[0].c_str(), stoi(tokens[1]));

	if (MonitorWindow()->GetViewback()->HasConnection())
		SetVisible(false);
}

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
	static CManualConnectPanel* pPanel = NULL;

	if (!pPanel)
		pPanel = new CManualConnectPanel();

	pPanel->SetSize(350, 150);
	pPanel->Layout();
	pPanel->MoveToCenter();
	pPanel->SetVisible(true);

	pPanel->m_pIP->SetText(MonitorWindow()->GetLastSuccessfulIP());
	pPanel->m_pPort->SetText(MonitorWindow()->GetLastSuccessfulPort());

	CRootPanel::Get()->SetFocus(pPanel->m_pIP);
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

void CManualConnectPanel::ConnectCallback(const tstring& sArgs)
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

CChannelPanel::CChannelPanel()
: CMovablePanel("Channels")
{
	SetSize(250, 350);
	MoveToCenter();
	SetVisible(false);
	SetScissoring(true);
	SetVerticalScrollBarEnabled(true);
}

CChannelPanel* CChannelPanel::Get()
{
	static CChannelPanel* pPanel = NULL;

	if (!pPanel)
		pPanel = new CChannelPanel();

	return pPanel;
}

void CChannelPanel::Create()
{
	Get()->Layout();
	Get()->SetVisible(true);
}

void CChannelPanel::RegistrationUpdate()
{
	m_ahChannels.clear();
	ClearControls();

	const auto& aChannels = Viewback()->GetChannels();
	const auto& aMeta = Viewback()->GetMeta();

	for (size_t i = 0; i < aChannels.size(); i++)
	{
		CControl<CCheckBox> pChannel = AddControl(new CCheckBox());
		pChannel->SetPos(20, T_HEADER_HEIGHT + 10 + (float)i * (pChannel->GetHeight() + 10));
		pChannel->SetWidth(GetWidth() - 40);
		pChannel->SetText(aChannels[i].m_sName);
		pChannel->SetClickedListener(this, ChannelOn, tsprintf("%d", i));
		pChannel->SetUnclickedListener(this, ChannelOff, tsprintf("%d", i));

		m_ahChannels.push_back(pChannel);
	}

	Layout();
}

void CChannelPanel::Layout()
{
	const auto& aChannels = Viewback()->GetChannels();

	for (size_t i = 0; i < m_ahChannels.size(); i++)
		m_ahChannels[i]->SetState(aChannels[i].m_bActive, false);

	BaseClass::Layout();
}

void CChannelPanel::ChannelOnCallback(const tstring& sArgs)
{
	auto& aChannels = Viewback()->GetChannels();

	size_t iChannel = stoi(sArgs);
	if (iChannel < 0)
		return;

	if (iChannel >= aChannels.size())
		return;

	Viewback()->ActivateChannel(iChannel);

	MonitorWindow()->GetPanelContainer()->Layout();
}

void CChannelPanel::ChannelOffCallback(const tstring& sArgs)
{
	auto& aChannels = Viewback()->GetChannels();

	size_t iChannel = stoi(sArgs);
	if (iChannel < 0)
		return;

	if (iChannel >= aChannels.size())
		return;

	Viewback()->DeactivateChannel(iChannel);

	MonitorWindow()->GetPanelContainer()->Layout();
}

CGroupPanel::CGroupPanel()
	: CMovablePanel("Activate a group")
{
	SetVisible(false);
	SetScissoring(true);
	SetVerticalScrollBarEnabled(true);
}

void CGroupPanel::Create()
{
	static CGroupPanel* pPanel = NULL;

	if (!pPanel)
		pPanel = new CGroupPanel();

	pPanel->SetSize(350, 350);
	pPanel->Layout();
	pPanel->MoveToCenter();
	pPanel->SetVisible(true);
}

void CGroupPanel::Layout()
{
	ClearControls();

	const auto& aGroups = Viewback()->GetGroups();

	for (size_t i = 0; i < aGroups.size(); i++)
	{
		CControl<CButton> group_button = AddControl(new CButton(aGroups[i].m_sName));
		group_button->SetPos(20, T_HEADER_HEIGHT + 10 + (float)i * (group_button->GetHeight() + 10));
		group_button->SetWidth(GetWidth() - 40);
		group_button->SetClickedListener(this, ActivateGroup, tsprintf("%d", i));
	}

	BaseClass::Layout();
}

void CGroupPanel::ActivateGroupCallback(const tstring& sArgs)
{
	auto& aGroups = Viewback()->GetGroups();

	size_t group_index = stoi(sArgs);
	if (group_index < 0)
		return;

	if (group_index >= aGroups.size())
		return;

	Viewback()->ActivateGroup(group_index);

	MonitorWindow()->GetPanelContainer()->Layout();

	SetVisible(false);
}
