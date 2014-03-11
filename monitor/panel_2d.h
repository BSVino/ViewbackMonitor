#pragma once

#include "panel_base.h"

class CPanel_2D : public CPanel_Base
{
	DECLARE_CLASS(CPanel_2D, CPanel_Base);

public:
	void RegistrationUpdate();

	void Paint(float x, float y, float w, float h);

private:
	tvector<glgui::CControl<glgui::CLabel>> m_apLabels;
};
