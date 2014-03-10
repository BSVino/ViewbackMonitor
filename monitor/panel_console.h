#pragma once

#include <glgui/panel.h>

class CPanel_Console : public glgui::CPanel
{
	DECLARE_CLASS(CPanel_Console, glgui::CPanel);

public:
	CPanel_Console();

public:
	void Layout();

	void PrintConsole(const tstring& sText);

	virtual bool KeyPressed(int code, bool bCtrlDown = false);

private:
	glgui::CControl<glgui::CLabel>     m_hOutput;
	glgui::CControl<glgui::CTextField> m_hInput;

	bool					m_bBackground;

	tvector<tstring>		m_asHistory;
	int						m_iHistory;
};
