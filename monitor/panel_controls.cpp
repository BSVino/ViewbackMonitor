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

tstring ValuePrint(float value)
{
	return pretty_float(value, 2);
}

void CPanel_Controls::Layout()
{
	m_selectors.clear();

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

			m_selectors.push_back(button);
			break;
		}

		case VB_CONTROL_SLIDER_FLOAT:
		{
			CControl<CScrollSelector<float>> slider = AddControl(new CScrollSelector<float>(control.m_name));

			if (control.slider_float.steps >= 2)
			{
				for (int n = 0; n < control.slider_float.steps; n++)
				{
					float f = RemapVal((float)n, 0.0f, (float)control.slider_float.steps - 1, control.slider_float.range_min, control.slider_float.range_max);
					slider->AddSelection(CScrollSelection<float>(f, pretty_float(f, 2)));
				}
			}
			else
			{
				float min = control.slider_float.range_min;
				float max = control.slider_float.range_max;
				slider->SetContinuousRange(CScrollSelection<float>(min, pretty_float(min, 2)), CScrollSelection<float>(max, pretty_float(max, 2)), &ValuePrint);
			}

			slider->SetValue(control.slider_float.initial_value);

			slider->Layout_FullWidth();
			slider->SetTop(GetDefaultMargin() + (float)i * (control_height + GetDefaultMargin()));
			slider->SetHeight(control_height);
			slider->SetSelectedListener(this, FloatSliderMoved, tsprintf("%d %d", i, m_selectors.size()));

			m_selectors.push_back(slider);
			break;
		}

		case VB_CONTROL_SLIDER_INT:
		{
			CControl<CScrollSelector<int>> slider = AddControl(new CScrollSelector<int>(control.m_name));

			for (int n = control.slider_int.range_min; n <= control.slider_int.range_max; n += control.slider_int.step_size)
				slider->AddSelection(CScrollSelection<int>(n, tsprintf("%d", n)));

			slider->SetValue(control.slider_int.initial_value);

			slider->Layout_FullWidth();
			slider->SetTop(GetDefaultMargin() + (float)i * (control_height + GetDefaultMargin()));
			slider->SetHeight(control_height);
			slider->SetSelectedListener(this, IntSliderMoved, tsprintf("%d %d", i, m_selectors.size()));

			m_selectors.push_back(slider);
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

	MonitorWindow()->GetViewback()->ControlCallback(control, m_selectors[selector].DowncastStatic<CScrollSelector<float>>()->GetSelectionValue());
}

void CPanel_Controls::IntSliderMovedCallback(const tstring& sArgs)
{
	tvector<tstring> tokens;
	tstrtok(sArgs, tokens);

	if (tokens.size() < 2)
		return;

	unsigned int control = stoi(tokens[0]);
	unsigned int selector = stoi(tokens[1]);

	MonitorWindow()->GetViewback()->ControlCallback(control, m_selectors[selector].DowncastStatic<CScrollSelector<int>>()->GetSelectionValue());
}

void CPanel_Controls::ControlUpdated(size_t control_id, float f_value, int i_value)
{
	auto& controls = MonitorWindow()->GetViewback()->GetControls();

	TAssert(control_id < controls.size());
	TAssert(control_id < m_selectors.size());

	auto the_real_control = m_selectors[control_id];

	switch (controls[control_id].m_type)
	{
	case VB_CONTROL_SLIDER_FLOAT:
	{
		CScrollSelector<float>* slider = the_real_control.DowncastStatic<CScrollSelector<float>>();
		if (!slider->IsMovingHandle())
			slider->SetValue(f_value);
		break;
	}
	case VB_CONTROL_SLIDER_INT:
	{
		CScrollSelector<int>* slider = the_real_control.DowncastStatic<CScrollSelector<int>>();
		if (!slider->IsMovingHandle())
			slider->SetValue(i_value);
		break;
	}
	}
}
