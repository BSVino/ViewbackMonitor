#include "monitor_menu.h"

#include "monitor_window.h"

using namespace glgui;

CMonitorMenu::CMonitorMenu()
	: CMenu("Options")
{
	// Add one or else the menu won't open.
	AddSubmenu("Quit", this, Quit);
}

void CMonitorMenu::OnOpenMenu()
{
	ClearSubmenus();

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


