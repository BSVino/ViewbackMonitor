#include "panel_controls.h"

#include <glgui/button.h>
#include <glgui/rootpanel.h>
#include <glgui/selector.h>
#include <tinker/profiler.h>
#include <renderer/renderingcontext.h>

#include "monitor_window.h"

using namespace glgui;

CPanel_Controls::CPanel_Controls()
{
}

void CPanel_Controls::RegistrationUpdate()
{
	Layout();
}

void CPanel_Controls::Layout()
{
	m_float_selectors.clear();

	ClearControls();

	auto& controls = MonitorWindow()->GetViewback()->GetControls();

	float control_height = 30;

	for (size_t i = 0; i < controls.size(); i++)
	{
		auto& control = controls[i];

		switch (control.m_type)
		{
		case VB_CONTROL_BUTTON:
		{
			CControl<CButton> button = AddControl(new CButton(control.m_name));
			button->Layout_FullWidth();
			button->SetTop(GetDefaultMargin() + (float)i * (control_height + GetDefaultMargin()));
			button->SetClickedListener(this, ButtonPressed, tsprintf("%d", i));
			break;
		}

		case VB_CONTROL_SLIDER_FLOAT:
		{
			CControl<CScrollSelector<float>> slider = AddControl(new CScrollSelector<float>(control.m_name));

			for (int n = 0; n < control.slider_float.steps; n++)
			{
				float f = RemapVal((float)n, 0.0f, (float)control.slider_float.steps-1, control.slider_float.range_min, control.slider_float.range_max);
				slider->AddSelection(CScrollSelection<float>(f, pretty_float(f, 2)));
			}

			slider->Layout_FullWidth();
			slider->SetTop(GetDefaultMargin() + (float)i * (control_height + GetDefaultMargin()));
			slider->SetHeight(control_height);
			slider->SetSelectedListener(this, FloatSliderMoved, tsprintf("%d %d", i, m_float_selectors.size()));

			m_float_selectors.push_back(slider);
			break;
		}

		default:
			break;
		}
	}

	BaseClass::Layout();
}

void CPanel_Controls::Think()
{
	BaseClass::Think();
}

void CPanel_Controls::Paint(float x, float y, float w, float h)
{
	TPROF("CPanel_Controls::Paint()");

	BaseClass::Paint(x, y, w, h);
}

void CPanel_Controls::ButtonPressedCallback(const tstring& sArgs)
{
	unsigned int i = stoi(sArgs);

	MonitorWindow()->GetViewback()->ControlCallback(i);
}

void CPanel_Controls::FloatSliderMovedCallback(const tstring& sArgs)
{
	tvector<tstring> tokens;
	tstrtok(sArgs, tokens);

	if (tokens.size() < 2)
		return;

	unsigned int control = stoi(tokens[0]);
	unsigned int selector = stoi(tokens[1]);

	MonitorWindow()->GetViewback()->ControlCallback(control, m_float_selectors[selector].DowncastStatic<CScrollSelector<float>>()->GetSelectionValue());
}
