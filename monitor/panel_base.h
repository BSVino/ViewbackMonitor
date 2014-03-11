#pragma once

#include <glgui/panel.h>

class CPanel_Base : public glgui::CPanel
{
	DECLARE_CLASS(CPanel_Base, glgui::CPanel);

public:
	virtual void RegistrationUpdate() {} // Registration was updated. Remake controls or whatever.
};

