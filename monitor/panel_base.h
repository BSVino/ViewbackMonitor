#pragma once

#include <deque>

#include <glgui/panel.h>

class CPanel_Base : public glgui::CPanel, public glgui::IEventListener
{
	DECLARE_CLASS(CPanel_Base, glgui::CPanel);

public:
	CPanel_Base();

public:
	void Layout();

	virtual void RegistrationUpdate() {} // Registration was updated. Remake controls or whatever.

	EVENT_CALLBACK(CPanel_Base, Maximize);
	EVENT_CALLBACK(CPanel_Base, Minimize); // Not really minimize so much as un-maximize.
	void SetIsMaximized(bool bMaximized);

	template <typename T>
	size_t FindStartTime(const std::deque<T>& aData, double flTime)
	{
		if (aData.size() < 2)
			return 0;

		double flFirstTime = aData.front().time;
		double flTimeFrom = flTime;

		size_t iStart = 0;

		if (flFirstTime > flTimeFrom)
			iStart = 0;
		else
		{
			size_t iLast = aData.size() - 1;

			while (iLast - iStart > 2)
			{
				// Try to predict where the time we want will be. O(log log n) time baby!
				size_t iBisectPoint = (size_t)RemapValClamped(flTimeFrom, aData[iStart].time, aData[iLast].time, (double)iStart, (double)iLast);

				if (iBisectPoint == iStart)
					iBisectPoint = iStart + 1;
				else if (iBisectPoint == iLast)
					iBisectPoint = iLast - 1;

				if (aData[iBisectPoint].time < flTimeFrom)
					iStart = iBisectPoint;
				else
					iLast = iBisectPoint;
			}

			// We're not terribly far now, only one or two off.
			while (iStart+1 < aData.size() && aData[iStart].time < flTimeFrom)
				iStart++;
		}

		return iStart;
	}

private:
	glgui::CControl<glgui::CPictureButton> m_pMaximize;
};

