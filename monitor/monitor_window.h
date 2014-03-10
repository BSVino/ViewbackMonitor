#pragma once

#include <tinker/application.h>

#include <client/viewback_client.h>

#include <glgui/basecontrol.h>

class CPanelContainer;

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

	virtual void WindowResize(int x, int y);

	CViewbackClient* GetViewback() { return &vb; }

	glgui::CControl<class CPanel_Console> GetConsolePanel();

private:
	CViewbackClient vb;

	glgui::CControl<CPanelContainer> m_pPanelContainer;
};

inline CMonitorWindow* MonitorWindow()
{
	return static_cast<CMonitorWindow*>(Application());
}

inline CViewbackClient* Viewback()
{
	return MonitorWindow()->GetViewback();
}
