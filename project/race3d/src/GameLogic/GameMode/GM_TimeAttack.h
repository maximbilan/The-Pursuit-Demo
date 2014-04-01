#ifndef _GM_TIMEATTACK_H_
#define _GM_TIMEATTACK_H_

#include "GameMode.h"

//-------------------------------------------------------------------------------------
// GM_TimeAttack
//-------------------------------------------------------------------------------------
class GM_TimeAttack : public GameMode
{
public:
	GM_TimeAttack();
	~GM_TimeAttack();

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

	// Add lap
	void AddLap();

	// On start lap event
	void StartLap();

	// On finish lap event
	void FinishLap();

	// Returns amount of passed laps
	int GetLapsAmount() const;

	// Returns current time
	double GetCurrentTime() const;

	// Returns best time of lap
	double GetBestTime() const;

	// Get current lap
	int GetCurrentLap() const;

private:
	int		m_laps;
	int		m_currentLap;

	uint64	m_startTime;
	uint64	m_pauseTime;
	double	m_currentTime;
	double	m_bestTime;
};

//-----------------------------------------------------------------------------------

inline void GM_TimeAttack::AddLap()
{
	++m_laps;
}

//-----------------------------------------------------------------------------------

inline int GM_TimeAttack::GetLapsAmount() const
{
	return m_laps;
}

//---------------------------------------------------------------------------------

inline double GM_TimeAttack::GetCurrentTime() const
{
	return m_currentTime;
}

//---------------------------------------------------------------------------------

inline double GM_TimeAttack::GetBestTime() const
{
	return m_bestTime;
}

//---------------------------------------------------------------------------------

inline int GM_TimeAttack::GetCurrentLap() const
{
	return m_currentLap;
}

#endif	// _GM_TIMEATTACK_H_