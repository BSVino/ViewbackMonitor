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

void CMonitorWindow::Run()
{
	while (IsOpen())
	{
		CProfiler::BeginFrame();
		Render();
		CProfiler::Render();
		SwapBuffers();
	}
}
