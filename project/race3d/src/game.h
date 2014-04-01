#ifndef _GAME_H
#define _GAME_H

#include "s3eKeyboard.h"

#include "IwGraphics.h"
#include "IwModel.h"
#include "IwModelBuilder.h"
#include "IwResManager.h"
#include "IwGxPrint.h"

#include "Common/Singleton.h"

//-----------------------------------------------------------------------------------------------

class MenuManager;
class AccelerometerManager;
class TouchManager;
class GameLevel;
class WebManager;
class Config;
class FontManager;
class SoundManager;
class ScriptManager;
class ShaderManager;
class PostProcessing;

//-----------------------------------------------------------------------------------------------
// Game - main class of the game
//-----------------------------------------------------------------------------------------------
class Game : public Singleton<Game>
{
public:
	static const unsigned int	s_fpsLimit;
	static const float			s_msPerFrame;

public:
	// Initialization
	void Init();

	// Running the game
	void Run();

	// Update
	void Update( const s3e_uint64_t dt );

	// Render
	void Render();

	// Shutdown the game
	void Shutdown();

	// Create game level
	GameLevel* CreateGameLevel( const int modeID, const int levelID );

protected:
	Game() {}
	friend class Singleton<Game>;

private:
	MenuManager*			m_menuManager;
	TouchManager*			m_touchManager;
	AccelerometerManager*	m_accelerometerManager;
	WebManager*				m_webManager;
	Config*					m_config;
	FontManager*			m_font;
	SoundManager*			m_soundManager;
	ScriptManager*			m_scriptManager;
	ShaderManager*			m_shaderManager;
	PostProcessing*			m_postProcessing;
};

#endif	// _GAME_H