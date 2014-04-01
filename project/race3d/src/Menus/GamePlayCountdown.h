#ifndef _GAME_PLAY_COUNTDOWN_H_
#define _GAME_PLAY_COUNTDOWN_H_

#include "s3eTimer.h"

class FontManager;

//-----------------------------------------------------------------------------------
// GamePlayCountdown - class for display countdown
//-----------------------------------------------------------------------------------
class GamePlayCountdown
{
private:
	static const int	s_messageCount;
	static const char*	s_messages[];
	static const uint64	s_messageTime;
	static const int	s_rectWidth;
	static const int	s_rectHeight;

public:
	GamePlayCountdown();
	~GamePlayCountdown();

	// Render
	void Render();

	// Update
	void Update();

	// Enable countdown
	void Enable();

	// Check if countdown enabled
	bool IsEnabled() const;

private:
	bool			m_isEnabled;
	int				m_messageIndex;
	uint64			m_startTime;
	uint64			m_time;

	FontManager*	m_fontManager;
};

//---------------------------------------------------------------------------------

inline void GamePlayCountdown::Enable()
{
	m_isEnabled = true;
	m_messageIndex = 0;
	m_startTime = s3eTimerGetMs();
}

//---------------------------------------------------------------------------------

inline bool GamePlayCountdown::IsEnabled() const
{
	return m_isEnabled;
}

#endif