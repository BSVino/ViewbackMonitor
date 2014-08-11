#ifdef _WIN32
#include <pthread.h>
#endif

#include <SDL_main.h>

#include <tinker_platform.h>

#include "monitor_window.h"
#include "SDL.h"

//#define ANDROID_GLOBAL_TEST

int CreateApplication(int argc, char** argv)
{
#ifdef _WIN32
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		return 1;

#ifdef PTW32_STATIC_LIB
	pthread_win32_process_attach_np();
#endif
#endif

#ifdef ANDROID_GLOBAL_TEST
	while (true)
	{
#endif

	CMonitorWindow oWindow(argc, argv);

	int iScreenWidth;
	int iScreenHeight;

	CApplication::GetScreenSize(iScreenWidth, iScreenHeight);

#if !defined(__ANDROID__)
	iScreenWidth = iScreenWidth*2/3;
	iScreenHeight = iScreenHeight*2/3;
#endif

	oWindow.OpenWindow(iScreenWidth, iScreenHeight, false, true);

	oWindow.Run();

#ifdef ANDROID_GLOBAL_TEST
	}
#endif

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}

extern "C"
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
	__except (CreateMinidump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
	{
	}
#endif
}
