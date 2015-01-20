#pragma once

#include <glgui/panel.h>

class CPanelContainer : public glgui::CPanel, public glgui::IEventListener
{
	DECLARE_CLASS(CPanelContainer, glgui::CPanel);

public:
	virtual void Setup();
	virtual void RegistrationUpdate();
	virtual void Layout();

	void SetMaximizedPanel(const glgui::CControl<class CPanel_Base>& pPanel);

	EVENT_CALLBACK(CPanelContainer, ShowProfile);

	glgui::CControl<class CPanel_Console> GetConsolePanel();
	glgui::CControl<class CPanel_Controls> GetControlsPanel();

private:
	glgui::CControl<class CPanel_Console> m_pPanelConsole;
	glgui::CControl<class CPanel_2D>      m_pPanel2D;
	glgui::CControl<class CPanel_Time>    m_pPanelTime;
	glgui::CControl<class CPanel_Controls> m_pPanelControls;

	glgui::CControl<class CPanel_Base>    m_pMaximizedPanel;

	glgui::CControl<glgui::CMenu> m_pViewbackButton;

	glgui::CControl<glgui::CMenu> m_pProfilesButton;
};
