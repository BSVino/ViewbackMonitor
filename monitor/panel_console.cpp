#include "panel_console.h"

#include <glgui/label.h>
#include <glgui/textfield.h>
#include <glgui/rootpanel.h>
#include <tinker/application.h>

#include "monitor_window.h"

using namespace glgui;

CPanel_Console::CPanel_Console()
{
	m_hOutput = AddControl(new CLabel(0, 0, 100, 100, ""));
	m_hOutput->SetAlign(CLabel::TA_BOTTOMLEFT);

	m_hInput = AddControl(new CTextField());
}

void CPanel_Console::Layout()
{
	m_hInput->SetSize(GetWidth(), 20);
	m_hInput->SetPos(0, GetHeight() - 20);

	m_hOutput->SetSize(GetWidth(), GetHeight() - 24);
	m_hOutput->SetPos(0, 0);

	BaseClass::Layout();
}

void CPanel_Console::PrintConsole(const tstring& sText)
{
	m_hOutput->AppendText(sText);

	if (m_hOutput->GetText().length() > 2500)
		m_hOutput->SetText(m_hOutput->GetText().substr(500));

	if (!Application()->IsOpen())
		return;

	if (IsVisible())
		Layout();
}

bool CPanel_Console::KeyPressed(int code, bool bCtrlDown)
{
	if (!IsVisible())
		return false;

	if (m_asHistory.size())
	{
		if (code == TINKER_KEY_DOWN)
		{
			if (m_iHistory >= 0 && m_iHistory < (int)m_asHistory.size() - 1)
			{
				m_iHistory++;

				m_hInput->SetText(m_asHistory[m_iHistory]);
				m_hInput->SetCursorPosition(-1);
			}
			else if (m_iHistory == (int)m_asHistory.size() - 1)
			{
				m_iHistory = -1;
				m_hInput->SetText("");
			}
		}
		else if (code == TINKER_KEY_UP)
		{
			if (m_iHistory == -1)
				m_iHistory = m_asHistory.size() - 1;
			else if (m_iHistory > 0)
				m_iHistory--;

			m_hInput->SetText(m_asHistory[m_iHistory]);
			m_hInput->SetCursorPosition(-1);
		}
		else
			m_iHistory = -1;
	}

	if (code == TINKER_KEY_ENTER || code == TINKER_KEY_KP_ENTER)
	{
		tstring sText = m_hInput->GetText();
		m_hInput->SetText("");

		PrintConsole(tstring("--> ") + sText + "\n");

		MonitorWindow()->GetViewback()->SendConsoleCommand(sText.c_str());

		if (trim(sText).length())
			m_asHistory.push_back(trim(sText));
		m_iHistory = -1;

		return true;
	}

	bool bReturn = BaseClass::KeyPressed(code, bCtrlDown);

	if (bReturn && !(code == TINKER_KEY_ENTER || code == TINKER_KEY_KP_ENTER))
		return true;

	return false;
}

