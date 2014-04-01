#ifndef _GM_TRAINING_H_
#define _GM_TRAINING_H_

#include "GameMode.h"

//-------------------------------------------------------------------------------------
// GM_Training - game mode with simple gameplay, just moving through the level, and
// count simple statistics ( time in the game, max speed )
//-------------------------------------------------------------------------------------
class GM_Training : public GameMode
{
public:
	GM_Training();
	~GM_Training();

	// Initialization
	void Init( Player* player );

	// Resume
	void Resume();

	// Suspend
	void Suspend();

	// Update
	void Update( const s3e_uint64_t dt );

	// Save to file
	void Save( RecordDB &db );

	// Load from file
	void Load( RecordDB &db );

	// Create
	void Create( RecordDB &db );

	// Reset all the settings
	void Reset();

	// Get time in the game mode
	double	GetTime() const;

	// Get distance
	double	GetDistance() const;

	// Get max speed
	float	GetMaxSpeed() const;	

private:
	uint64	m_startTime;
	double	m_time;
	double	m_distance;
	float	m_maxSpeed;
};

//-------------------------------------------------------------------------------------

inline double GM_Training::GetTime() const
{
	return m_time;
}

//-------------------------------------------------------------------------------------

inline double GM_Training::GetDistance() const
{
	return m_distance;
}

//-------------------------------------------------------------------------------------

inline float GM_Training::GetMaxSpeed() const
{
	return m_maxSpeed;
}

#endif	// _GM_TRAINING_H_