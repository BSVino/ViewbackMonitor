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

	EVENT_CALLBACK(CMonitorMenu, ManualConnect);
	EVENT_CALLBACK(CMonitorMenu, FindServer);
	EVENT_CALLBACK(CMonitorMenu, Disconnect);
	EVENT_CALLBACK(CMonitorMenu, Quit);
};

#include <glgui/movablepanel.h>

class CManualConnectPanel : public glgui::CMovablePanel
{
	DECLARE_CLASS(CManualConnectPanel, glgui::CMovablePanel);

private:
	CManualConnectPanel();

public:
	static void Create();

	void Layout();

	EVENT_CALLBACK(CManualConnectPanel, Connect);

private:
	glgui::CControl<glgui::CLabel>     m_pIPLabel;
	glgui::CControl<glgui::CTextField> m_pIP;
	glgui::CControl<glgui::CLabel>     m_pPortLabel;
	glgui::CControl<glgui::CTextField> m_pPort;
	glgui::CControl<glgui::CButton>    m_pConnect;
};



