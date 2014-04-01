#ifndef _GS_GAMEPLAY_H_
#define _GS_GAMEPLAY_H_

#include <string>

#include "GameState.h"

class KeyManager;
class TouchManager;
class AccelerometerManager;
class Physics;
class GameLevel;
class GameMode;
class MenuManager;
class MenuGameGui;
class Config;
class SoundManager;
class GamePlayLog;
class GamePlayCountdown;

//-----------------------------------------------------------------------------------
// GS_GamePlay - gamestate for rendering and updating gameplay
//-----------------------------------------------------------------------------------
class GS_GamePlay : public GameState
{
public:
	static const int k_substate_none		= 0;
	static const int k_substate_gameplay	= 1;
	static const int k_substate_ingame_menu	= 2;
	static const int k_substate_countdown	= 3;

public:
	GS_GamePlay( GameLevel* gameLevel );
	virtual ~GS_GamePlay();

	// Resume state
	void ResumeState();

	// Update state
	void UpdateState( const s3e_uint64_t frameTime );

	// Render state
	void RenderState();

	// Respawn Player
	void RespawnPlayer();

	// Run level script function
	void RunLevelScript( const std::string scriptFunc );

	// Run countdown
	void RunCountdown();

	// Returns pointer to gamelevel
	GameLevel* GetGameLevel() const;

	// Returns pointer to gamemode
	GameMode* GetGameMode() const;

	// Returns gameplay log
	GamePlayLog* GetLog() const;

private:
	TouchManager*			m_touchManager;
	AccelerometerManager*	m_accelerometerManager;
	KeyManager*				m_keyManager;
	Physics*				m_physics;
	GameLevel*				m_gameLevel;
	GameMode*				m_gameMode;
	MenuManager*			m_menuManager;
	MenuGameGui*			m_gameGui;
	Config*					m_config;
	SoundManager*			m_soundManager;
	GamePlayLog*			m_log;
	GamePlayCountdown*		m_countdown;
};

//-----------------------------------------------------------------------------------

inline GameLevel* GS_GamePlay::GetGameLevel() const
{
	return m_gameLevel;
}

//-----------------------------------------------------------------------------------

inline GameMode* GS_GamePlay::GetGameMode() const
{
	return m_gameMode;
}

//-----------------------------------------------------------------------------------

inline GamePlayLog* GS_GamePlay::GetLog() const
{
	return m_log;
}

#endif	// _GS_GAMEPLAY_H_