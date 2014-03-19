#include "monitor_window.h"

#include <glgui/rootpanel.h>
#include <tinker/renderer/renderer.h>
#include <tinker/profiler.h>
#include <tinker/cvar.h>
#include <tinker_platform.h>

#include "panel_container.h"
#include "panel_console.h"
#include "monitor_menu.h"

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

	m_pMenu = RootPanel()->AddControl(new CMonitorMenu(), true);
	m_pMenu->SetSize(60, 20);
	m_pMenu->Layout_AlignBottom(nullptr, 0);
	m_pMenu->Layout_CenterHorizontal();

	if (PlatformHasMenuKey())
		m_pMenu->SetVisible(false);
}

void CMonitorWindow::RegistrationUpdate()
{
	m_pPanelContainer->RegistrationUpdate();
}

void CMonitorWindow::WindowResize(int x, int y)
{
	if (m_pPanelContainer)
		m_pPanelContainer->Layout();

	m_pMenu->Layout_AlignBottom(nullptr, 0);
	m_pMenu->Layout_CenterHorizontal();

	BaseClass::WindowResize(x, y);
}

void CMonitorWindow::SetMaximizedPanel(const glgui::CControl<class CPanel_Base>& pPanel)
{
	if (m_pPanelContainer)
		m_pPanelContainer->SetMaximizedPanel(pPanel);
}

bool CMonitorWindow::KeyPress(int c)
{
	if (c == TINKER_KEY_APP_MENU || c == TINKER_KEY_F8)
	{
		if (m_pMenu->IsOpen())
			m_pMenu->CloseMenu();
		else
		{
			m_pMenu->OpenMenu();
			m_pMenu->SetVisible(true);
		}

		return true;
	}

	return BaseClass::KeyPress(c);
}

CControl<CPanel_Console> CMonitorWindow::GetConsolePanel()
{
	return m_pPanelContainer->GetConsolePanel();
}
