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

#pragma once

#include <animatedfloat.h>

#include "panel.h"

namespace glgui
{
	class CUnboundSelector : public CPanel
	{
		DECLARE_CLASS(CUnboundSelector, CPanel);

		typedef tstring(*ValuePrint)(float value);

	public:
		CUnboundSelector(const tstring& sLabel = "", const tstring& sFont = "sans-serif", size_t iSize = 13);

		virtual void Layout();
		virtual void Think();
		virtual void Paint(float x, float y, float w, float h);
		virtual void PostPaint();

		bool MousePressed(int code, int mx, int my);
		bool MouseReleased(int code, int mx, int my);
		void DoneMovingHandle();
		bool IsMovingHandle();

		void  SetRange(float min, float max, ValuePrint pfnValuePrint);
		void  SetValue(float value);
		float GetValue();
		float HandleX();
		float HandleY();
		void  SetSelectedListener(IEventListener* pListener, IEventListener::Callback pfnCallback, const tstring& args);

	protected:
		// Super hack!
		ValuePrint m_value_print;

		CControl<CLabel> m_label;
		CControl<CLabel> m_option;

		float m_range_min;
		float m_range_max;

		CAnimatedFloat m_handle;
		CAnimatedFloat m_active;

		IEventListener::Callback m_selected_callback;
		IEventListener*          m_selected_listener;
		tstring                  m_selected_args;

		bool m_moving_handle;
	};
};
