#ifdef _WIN32
#include <Windows.h>
#endif

#include <tinker_platform.h>
#include <monitor_window.h>

int CreateApplication(int argc, char** argv)
{
	CMonitorWindow oWindow(argc, argv);

	int iScreenWidth;
	int iScreenHeight;

	GetScreenSize(iScreenWidth, iScreenHeight);

	oWindow.OpenWindow(iScreenWidth*2/3, iScreenHeight*2/3, false, true);

	oWindow.Run();

	return 0;
}

int main(int argc, char** argv)
{
#ifdef _WIN32
	// Make sure we open up an assert messagebox window instead of just aborting like it does for console apps.
	_set_error_mode(_OUT_TO_MSGBOX);

#ifndef _DEBUG
	__try
	{
#endif
#endif

	return CreateApplication(argc, argv);

#if defined(_WIN32) && !defined(_DEBUG)
	}
	__except (CreateMinidump(GetExceptionInformation(), "ViewbackMonitor"), EXCEPTION_EXECUTE_HANDLER)
	{
	}
#endif
}
