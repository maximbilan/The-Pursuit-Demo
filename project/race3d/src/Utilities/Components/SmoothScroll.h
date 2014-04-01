#ifndef _SMOOTHSCROLL_H_
#define _SMOOTHSCROLL_H_

#include "s3eTypes.h"
#include "IwGeomVec2.h"

class TouchManager;

//-----------------------------------------------------------------------------------------------
// SmoothScroll - component for smoothing scrolling
//-----------------------------------------------------------------------------------------------
class SmoothScroll
{
private:
	static double s_lengthFactor;
	static double s_durationFactor;
	static double s_distanceDecrease;
	static double s_timeCoef;
	static double s_percentOfSmoothingStart;

public:
	SmoothScroll();
	~SmoothScroll();

	// Update
	void Update();

	// Get step by x coordinate
	double GetStepX() const;

	// Get step by y coordinate
	double GetStepY() const;

	// Check is active
	bool IsActive() const;

private:
	// Start scrolling
	void StartScroll( const unsigned int direction, const double distance, const uint64 duration );

	// Stop scrolling
	void StopScroll();

private:
	TouchManager*	m_touchManager;

	uint64			m_duration;
	uint64			m_startTime;
	double			m_stepX;
	double			m_stepY;

	bool			m_isActive;

	unsigned int	m_direction;
	double			m_distance;
};

//---------------------------------------------------------------------------------------------

inline double SmoothScroll::GetStepX() const
{
	return m_stepX;
}

//---------------------------------------------------------------------------------------------

inline double SmoothScroll::GetStepY() const
{
	return m_stepY;
}

//---------------------------------------------------------------------------------------------

inline bool SmoothScroll::IsActive() const
{
	return m_isActive;
}

#endif	// _SMOOTHSCROLL_H_