#include "monitor_menu.h"

#include "monitor_window.h"

using namespace glgui;

CMonitorMenu::CMonitorMenu()
	: CMenu("Options")
{
	AddSubmenu("Search for Servers", this, FindServer);
	AddSubmenu("Disconnect", this, Disconnect);
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


