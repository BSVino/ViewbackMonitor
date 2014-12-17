#pragma once

#include "panel_base.h"

#include <tvector.h>

class CPanel_Controls : public CPanel_Base
{
	DECLARE_CLASS(CPanel_Controls, CPanel_Base);

public:
	CPanel_Controls();

public:
	void RegistrationUpdate();

	void Layout();

	void Think();
	void Paint(float x, float y, float w, float h);

	EVENT_CALLBACK(CPanel_Controls, ButtonPressed);
	EVENT_CALLBACK(CPanel_Controls, FloatSliderMoved);
	EVENT_CALLBACK(CPanel_Controls, IntSliderMoved);

	void ControlUpdated(size_t control_id, float f_value, int i_value);

private:
	tvector<glgui::CControlHandle> m_selectors;
};
