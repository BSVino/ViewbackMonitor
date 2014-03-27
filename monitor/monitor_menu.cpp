#include "monitor_menu.h"

#include <glgui/label.h>
#include <glgui/textfield.h>
#include <glgui/button.h>
#include <textures/texturesheet.h>

#include "monitor_window.h"

using namespace glgui;

CMonitorMenu::CMonitorMenu()
	: CMenu("Options")
{
	// Add one or else the menu won't open.
	AddSubmenu("Quit", this, Quit);

	CTextureSheet oSheet("materials/buttons.txt");
	SetSheetTexture(oSheet.GetSheet("Viewback"), oSheet.GetArea("Viewback"), oSheet.GetSheetWidth("Viewback"), oSheet.GetSheetHeight("Viewback"));
}

void CMonitorMenu::OnOpenMenu()
{
	ClearSubmenus();

#ifdef _DEBUG
	AddSubmenu("Open Console", this, Console);
#endif

	AddSubmenu("Manual Connect", this, ManualConnect);

	if (MonitorWindow()->GetViewback()->HasConnection())
	{
		AddSubmenu("Disconnect", this, Disconnect);
	}
	else
	{
		AddSubmenu("Connect To Best Server", this, FindServer);
	}

	AddSubmenu("Quit", this, Quit);
}

void CMonitorMenu::Think()
{
	bool bHeightWasZero = (m_flMenuHeight == 0);

	BaseClass::Think();

	if (Application()->PlatformHasMenuKey() && !bHeightWasZero && m_flMenuHeight == 0)
		SetVisible(false);
}

void CMonitorMenu::ConsoleCallback(const tstring& sArgs)
{
	Application()->ToggleConsole();
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


