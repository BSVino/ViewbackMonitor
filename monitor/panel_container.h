#pragma once

#include <glgui/panel.h>

class CPanelContainer : public glgui::CPanel
{
	DECLARE_CLASS(CPanelContainer, glgui::CPanel);

public:
	virtual void Setup();
	virtual void Layout();

	glgui::CControl<class CPanel_Console> GetConsolePanel();

private:
	glgui::CControl<class CPanel_Console> m_pPanelConsole;
	glgui::CControl<class CPanel_2D>      m_pPanel2D;
	glgui::CControl<class CPanel_Time>    m_pPanelTime;
};
