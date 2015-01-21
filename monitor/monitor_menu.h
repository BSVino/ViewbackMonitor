#pragma once

#include <glgui/menu.h>

class CMonitorMenu : public glgui::CMenu
{
	DECLARE_CLASS(CMonitorMenu, glgui::CMenu);

public:
	CMonitorMenu(bool bHideOnClose);

public:
	void OnOpenMenu();

	void Think();

	EVENT_CALLBACK(CMonitorMenu, Console);
	EVENT_CALLBACK(CMonitorMenu, ViewServers);
	EVENT_CALLBACK(CMonitorMenu, ManualConnect);
	EVENT_CALLBACK(CMonitorMenu, FindServer);
	EVENT_CALLBACK(CMonitorMenu, Disconnect);
	EVENT_CALLBACK(CMonitorMenu, Quit);

private:
	bool m_bHideOnClose;
};

#include <glgui/movablepanel.h>

class CManualConnectPanel : public glgui::CMovablePanel
{
	DECLARE_CLASS(CManualConnectPanel, glgui::CMovablePanel);

private:
	CManualConnectPanel();

public:
	static void Create();
	virtual void SetParent(glgui::CControlHandle hParent);

	void Layout();

	EVENT_CALLBACK(CManualConnectPanel, Connect);

private:
	static glgui::CControlResource s_panel;

	glgui::CControl<glgui::CLabel>     m_pIPLabel;
	glgui::CControl<glgui::CTextField> m_pIP;
	glgui::CControl<glgui::CLabel>     m_pPortLabel;
	glgui::CControl<glgui::CTextField> m_pPort;
	glgui::CControl<glgui::CButton>    m_pConnect;
};

class CServerListPanel : public glgui::CMovablePanel
{
	DECLARE_CLASS(CServerListPanel, glgui::CMovablePanel);

private:
	CServerListPanel();

public:
	static void Create();
	virtual void SetParent(glgui::CControlHandle hParent);

	void Layout();
	void Think();

	EVENT_CALLBACK(CServerListPanel, Refresh);
	EVENT_CALLBACK(CServerListPanel, Connect);

private:
	static glgui::CControlResource s_server_list;

	double m_refresh_time;
};

class CAddSomethingPanel : public glgui::CMovablePanel
{
	DECLARE_CLASS(CAddSomethingPanel, glgui::CMovablePanel);

private:
	CAddSomethingPanel();

public:
	static void Create();
	virtual void SetParent(glgui::CControlHandle hParent);

	void Layout();

	EVENT_CALLBACK(CAddSomethingPanel, ReSearch);
	EVENT_CALLBACK(CAddSomethingPanel, Add);

private:
	static glgui::CControlResource s_panel;

	glgui::CControl<glgui::CLabel>     m_search_label;
	glgui::CControl<glgui::CTextField> m_search;

	glgui::CControl<glgui::CPanel>           m_search_results_panel;
	tvector<glgui::CControl<glgui::CButton>> m_search_results;
};

