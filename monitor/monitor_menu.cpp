#include "monitor_menu.h"

#include <application.h>

using namespace glgui;

CMonitorMenu::CMonitorMenu()
	: CMenu("Options")
{
	AddSubmenu("Quit", this, Quit);
}

void CMonitorMenu::Think()
{
	bool bHeightWasZero = (m_flMenuHeight == 0);

	BaseClass::Think();

	if (Application()->PlatformHasMenuKey() && !bHeightWasZero && m_flMenuHeight == 0)
		SetVisible(false);
}

void CMonitorMenu::QuitCallback(const tstring& sArgs)
{
	Application()->Close();
}


