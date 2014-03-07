#pragma once

#include <tinker/application.h>

#include <client/viewback_client.h>

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

	CViewbackClient* GetViewback() { return &vb; }

private:
	CViewbackClient vb;
};

inline CMonitorWindow* MonitorWindow()
{
	return static_cast<CMonitorWindow*>(Application());
}

inline CViewbackClient* Viewback()
{
	return MonitorWindow()->GetViewback();
}
