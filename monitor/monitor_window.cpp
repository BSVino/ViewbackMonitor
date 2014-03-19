#include "monitor_window.h"

#include <glgui/rootpanel.h>
#include <tinker/renderer/renderer.h>
#include <tinker/profiler.h>
#include <tinker/cvar.h>
#include <tinker_platform.h>

#include "panel_container.h"
#include "panel_console.h"

using namespace glgui;

CMonitorWindow::CMonitorWindow(int argc, char** argv)
: CApplication(argc, argv)
{
}

CRenderer* CMonitorWindow::CreateRenderer()
{
	size_t x, y;
	Application()->GetViewportSize(x, y);

	return new CRenderer(x, y);
}

void RegistrationUpdate()
{
	MonitorWindow()->RegistrationUpdate();
}

void ConsoleOutput(const char* pszText)
{
	MonitorWindow()->GetConsolePanel()->PrintConsole(pszText);
}

void DebugOutput(const char* pszText)
{
	TMsg(sprintf(tstring("VB: %s"), pszText));
}

void CMonitorWindow::Run()
{
	EnableMulticast();

	if (!Viewback()->Initialize(&::RegistrationUpdate, &ConsoleOutput, &DebugOutput))
		TError("Could not initialize Viewback.\n");

	CVar::SetCVar("r_bloom", 0);

	SetupGUI();

	while (IsOpen())
	{
		CProfiler::BeginFrame();

		{
			TPROF("Frame");

			PollEvents();

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

void CMonitorWindow::SetupGUI()
{
	m_pPanelContainer = RootPanel()->AddControl(new CPanelContainer());

	m_pPanelContainer->Setup();
}

void CMonitorWindow::RegistrationUpdate()
{
	m_pPanelContainer->RegistrationUpdate();
}

void CMonitorWindow::WindowResize(int x, int y)
{
	if (m_pPanelContainer)
		m_pPanelContainer->Layout();

	BaseClass::WindowResize(x, y);
}

void CMonitorWindow::SetMaximizedPanel(const glgui::CControl<class CPanel_Base>& pPanel)
{
	if (m_pPanelContainer)
		m_pPanelContainer->SetMaximizedPanel(pPanel);
}

CControl<CPanel_Console> CMonitorWindow::GetConsolePanel()
{
	return m_pPanelContainer->GetConsolePanel();
}
