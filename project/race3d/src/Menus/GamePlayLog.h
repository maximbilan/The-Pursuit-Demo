#ifndef _GAME_PLAY_LOG_H_
#define _GAME_PLAY_LOG_H_

#include <string>
#include <queue>
#include <vector>

class GamePlayLogObj;

//-----------------------------------------------------------------------------------

typedef std::queue<GamePlayLogObj*> GPLogQueue;
typedef std::vector<GamePlayLogObj*> GPLogVector;

//-----------------------------------------------------------------------------------
// GamePlayLog - class for print messages on the screen in the gameplay
//-----------------------------------------------------------------------------------
class GamePlayLog
{
public:
	static const int	s_messageAmount;
	static const uint64	s_messageTime;
	static const int	s_xRenderPos;
	static const int	s_yRenderPos;
	static const int	s_yRenderOffset;

public:
	GamePlayLog();
	~GamePlayLog();

	// Render
	void Render();

	// Update
	void Update();

	// Add message to queue
	void AddMessage( const std::string text );

private:
	GPLogQueue	m_msgQueue;
	GPLogVector	m_currentMsg;
};

#endif // _GAME_PLAY_LOG_H_