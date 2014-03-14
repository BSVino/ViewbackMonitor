#pragma once

#include "panel_base.h"

class CPanel_Time : public CPanel_Base
{
	DECLARE_CLASS(CPanel_Time, CPanel_Base);

public:
	CPanel_Time();

public:
	void RegistrationUpdate();

	void Paint(float x, float y, float w, float h);

	virtual bool MousePressed(int code, int mx, int my);
	virtual bool MouseReleased(int code, int mx, int my);
	virtual void CursorMoved(int mx, int my, int dx, int dy);

private:
	tvector<glgui::CControl<glgui::CLabel>> m_apLabels;

	tvector<int> m_aiDataLabels; // Maps a data index to an m_apLabels position.

	bool   m_bDragging;
	bool   m_bOverrideTime;
	double m_flOverrideTime;
	double m_flLastDragVelocity;

	double m_flFastForwardStartTime;
	double m_flFastForwardFromTime;

	double m_flShowTime;
};
