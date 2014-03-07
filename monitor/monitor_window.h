#pragma once

#include <tinker/application.h>

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
};
