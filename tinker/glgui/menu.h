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

#ifndef TINKER_MENU_H
#define TINKER_MENU_H

#include "panel.h"
#include "picturebutton.h"

namespace glgui
{
#ifdef T_PLATFORM_TOUCH
// Nice fat finger friendly size.
#define T_MENU_SPACING 10
#define T_MENU_HEIGHT 30
#else
#define T_MENU_SPACING 10
#define T_MENU_HEIGHT 22
#endif

	typedef enum
	{
		MENUOPEN_BOTTOM,
		MENUOPEN_SIDE,
	} menuopen_t;

	class CMenuBar : public CPanel
	{
	public:
									CMenuBar();

		void						Layout();

		void						SetActive(class CMenu* pMenu);
	};

	class CMenu : public CPictureButton, public IEventListener
	{
		DECLARE_CLASS(CMenu, CPictureButton);

	public:
									CMenu(const tstring& sTitle, bool bSubmenu = false);
		virtual						~CMenu();

	public:
		virtual void				Think();
		virtual void				Layout();
		virtual void				Paint(float x, float y, float w, float h);
		virtual void				PostPaint();

		virtual bool				IsCursorListener() { return true; };
		virtual void				CursorIn();
		virtual void				CursorOut();

		virtual void				SetMenuListener(IEventListener* pListener, IEventListener::Callback pfnCallback);

		EVENT_CALLBACK(CMenu, Open);
		EVENT_CALLBACK(CMenu, Close);
		EVENT_CALLBACK(CMenu, Clicked);

		void						OpenMenu();
		void						CloseMenu();
		bool                        IsOpen() { return m_flMenuHeightGoal > 0; }

		virtual void OnOpenMenu() {};
		virtual void OnCloseMenu() {};

		virtual void				AddSubmenu(const tstring& sTitle, IEventListener* pListener = NULL, IEventListener::Callback pfnCallback = NULL);
		virtual void				AddSubmenu(CMenu* pMenu, IEventListener* pListener = NULL, IEventListener::Callback pfnCallback = NULL);
		virtual void				ClearSubmenus();
		virtual size_t              GetNumSubmenus() const { return m_ahEntries.size(); }
		virtual CControl<CMenu>     GetSubmenu(size_t iMenu) const { if (iMenu >= GetNumSubmenus()) return nullptr; return m_ahEntries[iMenu]; }

		void SetMenuOpen(menuopen_t eMenuOpen) { m_eMenuOpen = eMenuOpen; }

		virtual size_t				GetSelectedMenu();

		virtual void                DirtyVisible();

	protected:
		class CSubmenuPanel : public CPanel
		{
			friend class CMenu;

			DECLARE_CLASS(CSubmenuPanel, CPanel);
		public:
									CSubmenuPanel(CControl<CMenu> hMenu);

		public:
			void					Think();

			void					Paint(float x, float y, float w, float h);
			void					PostPaint();

			virtual void            CalculateVisible();

			void					SetFakeHeight(float flFakeHeight) { m_flFakeHeight = flFakeHeight; };

		protected:
			float					m_flFakeHeight;

			tvector<float>			m_aflControlHighlightGoal;
			tvector<float>			m_aflControlHighlight;

			CControl<CMenu>			m_hMenu;
		};

		bool						m_bSubmenu;

		float						m_flHighlightGoal;
		float						m_flHighlight;

		float						m_flMenuHighlightGoal;
		float						m_flMenuHighlight;
		float						m_flMenuHeightGoal;
		float						m_flMenuHeight;
		float						m_flMenuSelectionHighlightGoal;
		float						m_flMenuSelectionHighlight;
		FRect						m_MenuSelectionGoal;
		FRect						m_MenuSelection;

		menuopen_t                  m_eMenuOpen;

		IEventListener::Callback	m_pfnMenuCallback;
		IEventListener*				m_pMenuListener;

		CControl<CSubmenuPanel>		m_hMenu;

		tvector<CControl<CMenu>>	m_ahEntries;
	};
};

#endif
