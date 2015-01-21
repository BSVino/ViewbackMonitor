#pragma once

#include "panel_base.h"

class CModifyChannelPanel : public glgui::CPanel, public glgui::IEventListener
{
	DECLARE_CLASS(CModifyChannelPanel, glgui::CPanel);

	EVENT_CALLBACK(CModifyChannelPanel, RemoveChannel);

public:
	CModifyChannelPanel(size_t control)
	{
		m_control = control;
		SetBackgroundColor(Color(0, 0, 0, 255));
	}

	void Layout();

public:
	size_t m_control;
};

class CPanel_Time : public CPanel_Base
{
	DECLARE_CLASS(CPanel_Time, CPanel_Base);

public:
	CPanel_Time();

public:
	void RegistrationUpdate();

	void Layout();

	double GetCurrentViewTime();

	void Think();
	void Paint(float x, float y, float w, float h);

	virtual bool MousePressed(int code, int mx, int my);
	virtual bool MouseReleased(int code, int mx, int my);
	virtual void CursorMoved(int mx, int my, int dx, int dy);

	EVENT_CALLBACK(CPanel_Time, ToggleVisible);
	EVENT_CALLBACK(CPanel_Time, AddChannel);

private:
	tvector<glgui::CControl<glgui::CButton>> m_apLabels;

	tvector<int> m_aiDataLabels; // Maps a data index to an m_apLabels position.

	glgui::CControl<glgui::CPictureButton> m_add;

	bool   m_bDragging;
	bool   m_bOverrideTime;
	double m_flOverrideTime;

	std::deque<double> m_aflLastDragVelocity;

	double m_flFastForwardStartTime;
	double m_flFastForwardFromTime;

	double m_flShowTime;

	glgui::CControl<CModifyChannelPanel> m_modify_panel;
};
