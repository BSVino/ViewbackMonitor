#pragma once

#include "panel_base.h"

class CPanel_2D : public CPanel_Base
{
	DECLARE_CLASS(CPanel_2D, CPanel_Base);

public:
	void RegistrationUpdate();

	void Layout();

	void Paint(float x, float y, float w, float h);

	EVENT_CALLBACK(CPanel_2D, ToggleVisible)

private:
	tvector<glgui::CControl<glgui::CButton>> m_apLabels;

	tvector<int> m_aiDataLabels; // Maps a data index to an m_apLabels position.
};
