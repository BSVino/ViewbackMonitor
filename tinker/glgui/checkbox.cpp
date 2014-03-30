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

#include "checkbox.h"

#include <renderer/renderingcontext.h>

#include "rootpanel.h"

using namespace glgui;

#ifdef T_TOUCH_PLATFORM
#define T_CHECKBOX_SIZE 25
#else
#define T_CHECKBOX_SIZE 15
#endif

CCheckBox::CCheckBox()
: CButton(0, 0, T_CHECKBOX_SIZE, T_CHECKBOX_SIZE, "", true)
{
	SetAlign(TA_LEFTCENTER);
	SetBorder(BT_NONE);
}

void CCheckBox::Paint(float x, float y, float w, float h)
{
	PaintRect(x, y, h-1, h-1, glgui::g_clrPanel, 3);

	if (m_bDown)
		CRootPanel::PaintRect(x+2, y+2, h-4, h-4, g_clrBoxHi);

	BaseClass::Paint(x + h + 4, y, w, h);
}

void CCheckBox::PaintButton(float x, float y, float w, float h)
{
}
