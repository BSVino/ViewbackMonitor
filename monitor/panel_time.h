#pragma once

#include "panel_base.h"

class CPanel_Time : public CPanel_Base
{
	DECLARE_CLASS(CPanel_Time, CPanel_Base);

public:
	void RegistrationUpdate();

	void Paint(float x, float y, float w, float h);

private:
	tvector<glgui::CControl<glgui::CLabel>> m_apLabels;

	tvector<int> m_aiDataLabels; // Maps a data index to an m_apLabels position.
};
