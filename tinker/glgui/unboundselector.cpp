/*
Copyright (c) 2012, Lunar Workshop, Inc.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
   This product includes software developed by Lunar Workshop, Inc.
4. Neither the name of the Lunar Workshop nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY LUNAR WORKSHOP INC ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LUNAR WORKSHOP BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "unboundselector.h"

#include "rootpanel.h"
#include "panel.h"
#include "label.h"

#include <tinker/application.h>
#include <renderer/renderingcontext.h>

using namespace glgui;

CUnboundSelector::CUnboundSelector(const tstring& label, const tstring& font_name, size_t font_size)
	: CPanel(0, 0, 100, 16), m_active(0, 10), m_handle(0, 5)
{
	m_moving_handle = false;

	m_selected_callback = NULL;
	m_selected_listener = NULL;

	m_label = AddControl(new CLabel(0, 0, 100, 100, label));
	m_label->SetWrap(false);
	m_label->SetFont(font_name, font_size);

	m_option = AddControl(new CLabel(0, 0, 100, 100, ""));
	m_option->SetWrap(false);
	m_option->SetFont(font_name, font_size);
}

void CUnboundSelector::Layout()
{
	m_label->SetSize(1, 1);
	m_label->EnsureTextFits();
	m_label->SetPos(0, GetHeight() / 2 - m_label->GetHeight() / 2);

	float flRemainingWidth = GetWidth() - m_label->GetWidth();

	m_option->SetSize(1, 1);

	// Make sure there's some text to be fit to.
	if (m_option->GetText() == "")
		m_option->SetText("-");

	m_option->EnsureTextFits();
	m_option->SetPos(m_label->GetWidth() + flRemainingWidth / 2 - m_option->GetWidth() / 2, GetHeight() - 15);

	CPanel::Layout();
}

void CUnboundSelector::Think()
{
	if (IsMovingHandle())
		m_active.m_goal = 1;
	else
		m_active.m_goal = 0;

	if (m_moving_handle)
	{
		int mx, my;
		CRootPanel::GetFullscreenMousePos(mx, my);

		float x, y, w, h;
		GetAbsDimensions(x, y, w, h);

		x += m_label->GetWidth();
		w -= m_label->GetWidth();

		m_handle.m_goal = RemapVal((float)mx, (float)x, (float)(x + w), 0.0f, 1.0f);
	}

	m_handle.Think((float)CRootPanel::Get()->GetFrameTime());
	m_active.Think((float)CRootPanel::Get()->GetFrameTime());

	float value = RemapVal(m_handle.m_goal, 0.0f, 1.0f, m_range_min, m_range_max);

	m_option->SetText(m_value_print(value));
}

void DrawTracks(float x, float y, float w, float h, const Color& c, float solid_width)
{
	::CRenderingContext r(nullptr, true);

	r.UseProgram("unbound_tracks");

	r.SetBlend(BLEND_ALPHA);
	r.SetUniform("vecColor", c);

	r.SetUniform("vecDimensions", Vector4D(x, y, w, h));
	r.SetUniform("flSolidWidth", solid_width);

	r.BeginRenderVertexArray(RootPanel()->GetQuad());
	r.SetPositionBuffer((size_t)0u, 20);
	r.SetTexCoordBuffer(12, 20);
	r.EndRenderVertexArray(6);
}

void CUnboundSelector::Paint(float x, float y, float w, float h)
{
	//CRootPanel::PaintRect(x, y, w, h, g_clrBoxHi);

	float flLeft = x + HANDLE_SIZE / 2;
	float flWidth = w - HANDLE_SIZE;

	flLeft += m_label->GetWidth();
	flWidth -= m_label->GetWidth();

	if (!m_active.m_curr)
		DrawTracks(flLeft, y + h / 2 - 4, flWidth, 8, Color(150, 150, 150, 0), flWidth);

	CRootPanel::PaintRect(flLeft, y + h / 2 - 5, Application()->GetGUIScale(), 10, Color(200, 200, 200, (int)(255 * (1-m_active.m_curr))));
	CRootPanel::PaintRect(flLeft + flWidth, y + h / 2 - 5, Application()->GetGUIScale(), 10, Color(200, 200, 200, (int)(255 * (1 - m_active.m_curr))));

	if (m_range_min < 0 && m_range_max > 0)
		CRootPanel::PaintRect(RemapVal(0, m_range_min, m_range_max, flLeft, flLeft + flWidth), y + h / 2 - 5, Application()->GetGUIScale(), 10, Color(200, 200, 200, 255));

	if (!IsMovingHandle())
		CRootPanel::PaintRect(HandleX() + 2, HandleY() + 2, HANDLE_SIZE - 4, HANDLE_SIZE - 4, g_clrBoxHi, 2);

	CPanel::Paint(x, y, w, h);
}

void CUnboundSelector::PostPaint()
{
	float expand_width = 150;

	if (m_active.m_curr)
	{
		float x, y, w, h;
		GetAbsDimensions(x, y, w, h);

		float flLeft = x + HANDLE_SIZE / 2;
		float flWidth = w - HANDLE_SIZE;

		flLeft += m_label->GetWidth();
		flWidth -= m_label->GetWidth();

		DrawTracks(flLeft - m_active.m_curr*expand_width / 2, y + h / 2 - 4, flWidth + m_active.m_curr * expand_width, 8, Color(150, 150, 150, 0), flWidth);
	}

	if (IsMovingHandle())
	{
		CRootPanel::PaintRect(HandleX() + 2, HandleY() + 2, HANDLE_SIZE - 4, HANDLE_SIZE - 4, g_clrBoxHi, 2);
	}

	CPanel::PostPaint();
}

bool CUnboundSelector::MousePressed(int code, int mx, int my)
{
	float x, y, w, h;
	GetAbsDimensions(x, y, w, h);

	x += m_label->GetWidth();
	w -= m_label->GetWidth();

	float hx, hy;
	hx = HandleX();
	hy = HandleY();

	if (mx >= hx && mx < hx + HANDLE_SIZE && my >= hy && my < hy + HANDLE_SIZE)
		m_moving_handle = true;
	else
	{
		m_handle.m_goal = RemapValClamped((float)mx, (float)x + HANDLE_SIZE / 2, (float)(x + w - HANDLE_SIZE / 2), 0.0f, 1.0f);

		if (m_selected_listener)
			m_selected_callback(m_selected_listener, this, m_selected_args);
	}

	return true;
}

bool CUnboundSelector::MouseReleased(int code, int mx, int my)
{
	if (m_moving_handle)
	{
		DoneMovingHandle();
		return true;
	}

	return CPanel::MouseReleased(code, mx, my);
}

void CUnboundSelector::DoneMovingHandle()
{
	m_moving_handle = false;

	if (m_selected_listener)
		m_selected_callback(m_selected_listener, this, m_selected_args);

	float value = GetValue();
	if (value > 0)
	{
		m_range_min = -value;
		m_range_max = value * 3;
		m_handle.m_goal = 0.5f;
	}
}

bool CUnboundSelector::IsMovingHandle()
{
	return m_moving_handle;
}

void CUnboundSelector::SetRange(float min, float max, ValuePrint pfnValuePrint)
{
	m_range_min = min;
	m_range_max = max;

	m_value_print = pfnValuePrint;
}

void CUnboundSelector::SetValue(float value)
{
	m_handle.m_goal = RemapValClamped(value, m_range_min, m_range_max, 0.0f, 1.0f);
}

float CUnboundSelector::GetValue()
{
	return RemapVal(m_handle.m_goal, 0.0f, 1.0f, m_range_min, m_range_max);
}

float CUnboundSelector::HandleX()
{
	float x, y, w, h;
	GetAbsDimensions(x, y, w, h);

	x += m_label->GetWidth();
	w -= m_label->GetWidth();

	float flLeft = x + HANDLE_SIZE / 2;
	float flWidth = w - HANDLE_SIZE;
	return flLeft + flWidth*m_handle.m_curr - HANDLE_SIZE / 2;
}

float CUnboundSelector::HandleY()
{
	float x, y, w, h;
	GetAbsDimensions(x, y, w, h);

	return y + h / 2 - HANDLE_SIZE / 2;
}

void CUnboundSelector::SetSelectedListener(IEventListener* pListener, IEventListener::Callback pfnCallback, const tstring& args)
{
	m_selected_callback = pfnCallback;
	m_selected_listener = pListener;
	m_selected_args = args;
}
