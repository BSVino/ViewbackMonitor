#include "monitor_window.h"

#include <glgui/rootpanel.h>
#include <tinker/renderer/renderer.h>
#include <tinker/profiler.h>
#include <tinker/cvar.h>
#include <tinker_platform.h>
#include <datamanager/dataserializer.h>

#include "panel_container.h"
#include "panel_console.h"
#include "panel_controls.h"
#include "monitor_menu.h"

using namespace glgui;

CMonitorWindow::CMonitorWindow(int argc, char** argv)
: CApplication(argc, argv)
{
}

CMonitorWindow::~CMonitorWindow()
{
	Viewback()->Shutdown();
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
	TMsg(tsprintf("VB: %s", pszText));
}

void ControlUpdated(size_t control_id, float f_value, int i_value)
{
	MonitorWindow()->GetPanelContainer()->GetControlsPanel()->ControlUpdated(control_id, f_value, i_value);
}

void CMonitorWindow::Run()
{
	RootPanel()->SetDesignHeight(720);

	FILE* fp = tfopen(MonitorWindow()->GetAppDataDirectory("viewback.txt"), "r");

	if (fp)
	{
		std::shared_ptr<CData> pData(new CData());
		CDataSerializer::Read(fp, pData.get());

		m_sLastSuccessfulIP = pData->FindChildValueString("LastIP", "");
		m_sLastSuccessfulPort = pData->FindChildValueString("LastPort", "");

		fclose(fp);
	}

	//EnableMulticast();

	if (!Viewback()->Initialize(&::RegistrationUpdate, &ConsoleOutput, &DebugOutput))
		TError("Could not initialize Viewback.\n");

	Viewback()->SetControlUpdatedCallback(ControlUpdated);

	CVar::SetCVar("r_bloom", 0);

	SetupGUI();

	double frame_end_time = 0;
	double frame_start_time = 0;

	while (IsOpen())
	{
		CProfiler::BeginFrame();

		frame_end_time = GetTime();

		{
			TPROF("Sleep");

			double next_frame_time = frame_start_time + (1.0f / 30);
			double time_to_sleep_seconds = next_frame_time - frame_end_time;
			if (time_to_sleep_seconds > 0.001)
				SleepMS((size_t)(time_to_sleep_seconds * 1000));
		}

		frame_start_time = GetTime();

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

	m_pMenu = RootPanel()->AddControl(new CMonitorMenu(true), true);
	m_pMenu->SetSize(60, 20);
	m_pMenu->Layout_AlignBottom(nullptr, 0);
	m_pMenu->Layout_CenterHorizontal();
	m_pMenu->SetVisible(false);

	CServerListPanel::Create();
}

void CMonitorWindow::RegistrationUpdate()
{
	m_pPanelContainer->RegistrationUpdate();

	if (GetViewback()->HasConnection())
	{
		Viewback()->ActivateProfile(0);

		if (m_pPanelContainer)
			m_pPanelContainer->Layout();
	}
	else
		CServerListPanel::Create();
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

CControl<CPanelContainer> CMonitorWindow::GetPanelContainer()
{
	return m_pPanelContainer;
}

CControl<CPanel_Console> CMonitorWindow::GetConsolePanel()
{
	return m_pPanelContainer->GetConsolePanel();
}

CControl<CPanel_Controls> CMonitorWindow::GetControlsPanel()
{
	return m_pPanelContainer->GetControlsPanel();
}

void CMonitorWindow::SaveConfig()
{
	FILE* fp = tfopen(MonitorWindow()->GetAppDataDirectory("viewback.txt"), "w");

	if (!fp)
		return;

	std::shared_ptr<CData> pData(new CData());

	if (m_sLastSuccessfulIP.length())
		pData->AddChild("LastIP", m_sLastSuccessfulIP);

	if (m_sLastSuccessfulPort.length())
		pData->AddChild("LastPort", m_sLastSuccessfulPort);

	CDataSerializer::Save(fp, pData.get());

	fclose(fp);
}

bool CMonitorWindow::ButtonPanelSide() const
{
	return m_iWindowWidth > m_iWindowHeight;
}

