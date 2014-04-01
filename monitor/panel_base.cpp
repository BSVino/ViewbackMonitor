#include "panel_base.h"

#include <glgui/picturebutton.h>
#include <textures/materiallibrary.h>
#include <textures/texturesheet.h>

#include "monitor_window.h"

using namespace glgui;

CPanel_Base::CPanel_Base()
{
	CTextureSheet oSheet("materials/buttons.txt");

	m_pMaximize = AddControl(new CPictureButton("Max", CMaterialHandle()));
	m_pMaximize->SetSheetTexture(oSheet.GetSheet("Maximize"), oSheet.GetArea("Maximize"), oSheet.GetSheetWidth("Maximize"), oSheet.GetSheetHeight("Maximize"));
	m_pMaximize->SetClickedListener(this, Maximize);
}

void CPanel_Base::Layout()
{
	m_pMaximize->SetSize(35, 35);

	m_pMaximize->Layout_AlignTop(nullptr, 0);
	m_pMaximize->Layout_AlignRight(0);
}

void CPanel_Base::MaximizeCallback(const tstring& sArgs)
{
	MonitorWindow()->SetMaximizedPanel(this);
}

void CPanel_Base::MinimizeCallback(const tstring& sArgs)
{
	MonitorWindow()->SetMaximizedPanel(nullptr);
}

void CPanel_Base::SetIsMaximized(bool bMaximized)
{
	if (bMaximized)
		m_pMaximize->SetClickedListener(this, Minimize);
	else
		m_pMaximize->SetClickedListener(this, Maximize);
}

