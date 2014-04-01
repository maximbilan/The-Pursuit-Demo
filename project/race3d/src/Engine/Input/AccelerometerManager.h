#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include "s3eTypes.h"
#include "IwGeomFVec3.h"

#include "../../Common/Singleton.h"

//-------------------------------------------------------------------------------------------
// AccelerometerManager - manager for work with accelerometer
//-------------------------------------------------------------------------------------------
class AccelerometerManager : public Singleton<AccelerometerManager>
{
public:
	// Initialization
	void Init();

	// Start accelerometers running
	void Start();

	// Stop accelerometer input
	void Stop();

	// Update with current accelerometer inputs
	void Update();

	// Return true if accelerometer available
	bool IsAvailable() const;

	// Keyboard status methods
	int32 GetX() const;
	int32 GetY() const;
	int32 GetZ() const;

protected:
	AccelerometerManager() {}
	friend class Singleton<AccelerometerManager>;

private:		
	bool	m_available;	// Set true if accelerometer inputs are available
	bool	m_started;		// Set true when accelerometer input is active
	CIwVec3	m_smoothed;		// Set true when accelerometer input is active

	// The smoothing factor to apply.  If set to IW_GEOM_ONE no smoothing
	// will occur. Smaller values apply more smoothing but add more lag
	// to the controls
	int32	m_smoothFactor;
};

//-----------------------------------------------------------------------------------------

inline bool AccelerometerManager::IsAvailable() const
{
	return m_available;
}

//---------------------------------------------------------------------------------------

inline int32 AccelerometerManager::GetX() const
{
	return m_smoothed.x;
}

//---------------------------------------------------------------------------------------

inline int32 AccelerometerManager::GetY() const
{
	return m_smoothed.y;
}

//---------------------------------------------------------------------------------------

inline int32 AccelerometerManager::GetZ() const
{
	return m_smoothed.z;
}

#endif	// _ACCELEROMETER_H_