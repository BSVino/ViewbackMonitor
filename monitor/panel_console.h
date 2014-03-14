#pragma once

#include "panel_base.h"

class CPanel_Console : public CPanel_Base
{
	DECLARE_CLASS(CPanel_Console, CPanel_Base);

public:
	CPanel_Console();

public:
	void Layout();

	void Think();

	void PrintConsole(const tstring& sText);

	virtual bool KeyPressed(int code, bool bCtrlDown = false);

private:
	glgui::CControl<glgui::CLabel>     m_hOutput;
	glgui::CControl<glgui::CTextField> m_hInput;

	glgui::CControl<glgui::CLabel>     m_hStatus;

	bool					m_bBackground;

	tvector<tstring>		m_asHistory;
	int						m_iHistory;
};
