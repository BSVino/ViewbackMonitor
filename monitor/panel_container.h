#pragma once

#include <glgui/panel.h>

class CPanelContainer : public glgui::CPanel
{
	DECLARE_CLASS(CPanelContainer, glgui::CPanel);

public:
	virtual void Setup();
	virtual void RegistrationUpdate();
	virtual void Layout();

	void SetMaximizedPanel(const glgui::CControl<class CPanel_Base>& pPanel);

	glgui::CControl<class CPanel_Console> GetConsolePanel();

private:
	glgui::CControl<class CPanel_Console> m_pPanelConsole;
	glgui::CControl<class CPanel_2D>      m_pPanel2D;
	glgui::CControl<class CPanel_Time>    m_pPanelTime;

	glgui::CControl<class CPanel_Base>    m_pMaximizedPanel;
};
