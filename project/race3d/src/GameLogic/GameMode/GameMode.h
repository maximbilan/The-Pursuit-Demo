#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include <s3eTypes.h>

class RecordDB;
class Player;

//---------------------------------------------------------------------------------------
// GameMode - base class for description and behavior of the game mode
//---------------------------------------------------------------------------------------
class GameMode
{
public:
	enum GameModeState
	{
		GMS_ACTIVE,
		GMS_SUSPENDED
	};

	enum GameModeID
	{
		GM_ID_NONE			= -1,
		GM_ID_TRAINING,
		GM_ID_TIME_ATTACK,

		GM_ID_COUNT
	};

public:
	GameMode();
	virtual ~GameMode();

	// Initialization
	virtual void Init( Player* player );

	// Resume
	virtual void Resume() = 0;

	// Suspend
	virtual void Suspend() = 0;

	// Update
	virtual void Update( const s3e_uint64_t dt ) = 0;

	// Save to file
	virtual void Save( RecordDB &db ) = 0;

	// Load from file
	virtual void Load( RecordDB &db ) = 0;

	// Create
	virtual void Create( RecordDB &db ) = 0;

	// Reset all the settings
	virtual void Reset() = 0;

	// Get state
	unsigned int GetState() const;

	// Get ID
	int GetID() const;

protected:
	Player*			m_player;
	unsigned int	m_state;
	int				m_id;
};

//-------------------------------------------------------------------------------------

inline unsigned int GameMode::GetState() const
{
	return m_state;
}

//-------------------------------------------------------------------------------------

inline int GameMode::GetID() const
{
	return m_id;
}

#endif	// _GAMEMODE_H_