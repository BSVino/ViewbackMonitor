#pragma once

#include <tinker/application.h>

#include <client/viewback_client.h>

#include <glgui/basecontrol.h>

class CPanelContainer;
class CMonitorMenu;

class CMonitorWindow : public CApplication
{
	DECLARE_CLASS(CMonitorWindow, CApplication);

public:
	CMonitorWindow(int argc, char** argv);

public:
	virtual tstring			WindowTitle() { return "Viewback Monitor"; }
	virtual tstring			AppDirectory() { return "Viewback"; }

	class CRenderer* CreateRenderer();

	void Run();

	virtual void SetupGUI();
	virtual void RegistrationUpdate();

	virtual void WindowResize(int x, int y);

	void SetMaximizedPanel(const glgui::CControl<class CPanel_Base>& pPanel);

	bool KeyPress(int c);

	CViewbackClient* GetViewback() { return &vb; }

	glgui::CControl<class CPanel_Console> GetConsolePanel();

	void SaveConfig();

	tstring GetLastSuccessfulIP() { return m_sLastSuccessfulIP; }
	void SetLastSuccessfulIP(const tstring& sIP) { m_sLastSuccessfulIP = sIP; }
	tstring GetLastSuccessfulPort() { return m_sLastSuccessfulPort; }
	void SetLastSuccessfulPort(const tstring& sPort) { m_sLastSuccessfulPort = sPort; }

	float ButtonPanelWidth() const { return 60; }

private:
	CViewbackClient vb;

	glgui::CControl<CPanelContainer> m_pPanelContainer;
	glgui::CControl<CMonitorMenu>    m_pMenu;

	tstring m_sLastSuccessfulIP;
	tstring m_sLastSuccessfulPort;
};

inline CMonitorWindow* MonitorWindow()
{
	return static_cast<CMonitorWindow*>(Application());
}

inline CViewbackClient* Viewback()
{
	return MonitorWindow()->GetViewback();
}
