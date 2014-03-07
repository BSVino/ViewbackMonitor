#include "monitor_window.h"

#include <tinker/renderer/renderer.h>
#include <tinker/profiler.h>

CMonitorWindow::CMonitorWindow(int argc, char** argv)
: CApplication(argc, argv)
{
}

CRenderer* CMonitorWindow::CreateRenderer()
{
	return new CRenderer(CApplication::Get()->GetWindowWidth(), CApplication::Get()->GetWindowHeight());
}

void ConsoleOutput(const char* pszText)
{
	MonitorWindow()->PrintConsole(tstring("From Viewback: ") + pszText);
}

void CMonitorWindow::Run()
{
	Viewback()->Initialize(&ConsoleOutput);

	while (IsOpen())
	{
		CProfiler::BeginFrame();

		{
			TPROF("Frame");

			{
				TPROF("CViewbackClient::Update()")
				vb.Update();
			}

			Render();
		}

		CProfiler::Render();
		SwapBuffers();
	}
}
