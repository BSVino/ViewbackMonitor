#pragma once

#include <glgui/menu.h>

class CMonitorMenu : public glgui::CMenu
{
	DECLARE_CLASS(CMonitorMenu, glgui::CMenu);

public:
	CMonitorMenu();

public:
	void Think();

	EVENT_CALLBACK(CMonitorMenu, Quit);
};



