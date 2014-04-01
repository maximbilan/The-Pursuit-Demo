#ifndef _GAME_PLAY_LOG_OBJ_H_
#define _GAME_PLAY_LOG_OBJ_H_

#include <string>

#include "s3eTimer.h"

class FontManager;

//-----------------------------------------------------------------------------------
// GamePlayLogObj - class describe one message for the Gameplay Log
//-----------------------------------------------------------------------------------
class GamePlayLogObj
{
public:
	enum GL_MESSAGE_TYPE
	{
		GL_MESSAGE_TYPE_TEXT
	};

private:
	static const int s_rectWidth;
	static const int s_rectHeight;

public:
	GamePlayLogObj();
	GamePlayLogObj( const std::string text, const uint64 time );
	~GamePlayLogObj();

	// Render
	void Render( const int x, const int y );

	// Update
	void Update();

	// Enable
	void Enable();

	// Disable
	void Disable();

	// Check is enabled
	bool IsEnabled() const;

private:
	bool			m_isEnabled;
	unsigned int	m_type;
	uint64			m_time;
	uint64			m_startTime;
	std::string		m_text;

	FontManager*	m_fontManager;
};

//------------------------------------------------------------------------------------

inline void GamePlayLogObj::Enable()
{
	m_isEnabled = true;
	m_startTime = s3eTimerGetMs();
}

//-----------------------------------------------------------------------------------

inline void GamePlayLogObj::Disable()
{
	m_isEnabled = false;
}

//-----------------------------------------------------------------------------------

inline bool GamePlayLogObj::IsEnabled() const
{
	return m_isEnabled;
}

#endif	// _GAME_PLAY_LOG_OBJ_H_