#pragma once

#include <glgui/menu.h>

class CMonitorMenu : public glgui::CMenu
{
	DECLARE_CLASS(CMonitorMenu, glgui::CMenu);

public:
	CMonitorMenu();

public:
	void OnOpenMenu();

	void Think();

	EVENT_CALLBACK(CMonitorMenu, FindServer);
	EVENT_CALLBACK(CMonitorMenu, Disconnect);
	EVENT_CALLBACK(CMonitorMenu, Quit);
};



