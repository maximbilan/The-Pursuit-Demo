#include "game.h"

#include "s3e.h"

#include "IwSound.h"
#include "IwTextParserITX.h"

#include "Engine/Input/TouchManager.h"
#include "Engine/Input/AccelerometerManager.h"
#include "Engine/Font/FontManager.h"
#include "Engine/Physics.h"
#include "Engine/PostProcessing/PostProcessing.h"

#include "Scripting/ScriptManager.h"

#include "Localization/Localization.h"

#include "Utilities/Config/Config.h"
#include "Utilities/SoundManager/SoundManager.h"
#include "Utilities/Online/Web/WebManager.h"

#include "Menus/MenuManager.h"
#include "Menus/MenuElement.h"

#include "GameStates/GameState.h"
#include "GameStates/GS_Menu.h"
#include "GameStates/GS_Loading.h"

#include "GameLogic/GameLevel.h"

#include "WorldLoader/WorldLoader.h"

#include "Engine/ShaderManager.h"

//---------------------------------------------------------------------------------------------

const unsigned int Game::s_fpsLimit = 30;
const float Game::s_msPerFrame = 1000 / s_fpsLimit;

//---------------------------------------------------------------------------------------------

void Game::Init()
{
	IwGxInit();
	IwGraphicsInit();

	IwResManagerInit();

#ifdef IW_BUILD_RESOURCES
	IwGetTextParserITX()->SetAssertOnUnrecognisedTokens( false );
	IwGetResManager()->AddHandler( new CIwResHandlerWAV );
#endif

	IW_CLASS_REGISTER( Collision );
	IW_CLASS_REGISTER( CStatic );
	IW_CLASS_REGISTER( CDynamic );
	IW_CLASS_REGISTER( CLocator );

	IwGxSetColClear( 0x40, 0x40, 0x40, 0x00 );

	LocaliseInit();

	m_font = FontManager::GetInstance();

	m_menuManager = MenuManager::GetInstance();

	m_touchManager = TouchManager::GetInstance();
	m_touchManager->Init();

	m_accelerometerManager = AccelerometerManager::GetInstance();
	m_accelerometerManager->Init();

	m_webManager = WebManager::GetInstance();
	m_webManager->Init();

	m_config = Config::GetInstance();
	m_config->Init();

	m_soundManager = SoundManager::GetInstance();
	m_soundManager->Init();

	m_scriptManager = ScriptManager::GetInstance();
	m_scriptManager->Init();

	m_shaderManager = ShaderManager::GetInstance();
	m_shaderManager->Init();

	m_postProcessing = PostProcessing::GetInstance();
	m_postProcessing->Init();

	GameState::InitStatesStack();
	GameState::SetState( new GS_Loading() );
}

//---------------------------------------------------------------------------------------------

void Game::Run()
{
	uint64 now = s3eTimerGetMs(); 

	while( true )
	{
		s3eDeviceYield( 0 );
		s3eKeyboardUpdate();

		m_touchManager->Update();
		m_accelerometerManager->Update();

		m_webManager->Update();

		if( ( s3eKeyboardGetState( s3eKeyEsc ) & S3E_KEY_STATE_DOWN ) || ( s3eKeyboardGetState( s3eKeyAbsBSK ) & S3E_KEY_STATE_DOWN ) || ( s3eDeviceCheckQuitRequest() ) )
		{
			break;
		}

		const uint64 newTime = s3eTimerGetMs();
		const uint64 deltaTime = newTime - now;
		now = newTime;

		Update( deltaTime );
		Render();

		while( ( s3eTimerGetMs() - newTime) < s_msPerFrame )
		{
			const int32 yield = static_cast<int32>( s_msPerFrame - ( s3eTimerGetMs() - newTime ) );
			if( yield < 0 )
			{
				break;
			}

			s3eDeviceYield( yield );
		}
	}
}

//---------------------------------------------------------------------------------------------

void Game::Update( const s3e_uint64_t dt )
{
	GameState::UpdateCurrentState( dt );
	m_soundManager->Update();
	IwGxTickUpdate();
}

//---------------------------------------------------------------------------------------------

void Game::Render()
{
	IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );

	m_postProcessing->PreRender();

	GameState::RenderCurrentState();

	m_postProcessing->PostRender();

	IwGxFlush();
	IwGxSwapBuffers();
}

//---------------------------------------------------------------------------------------------

void Game::Shutdown()
{
	m_webManager->FreeInstance();

	GameState::ReleaseStatesStack();

	m_postProcessing->Remove();
	m_postProcessing->FreeInstance();

	m_shaderManager->Remove();
	m_shaderManager->FreeInstance();

	m_scriptManager->Remove();
	m_scriptManager->FreeInstance();

	m_soundManager->Remove();
	m_soundManager->FreeInstance();

	m_config->Remove();
	m_config->FreeInstance();

	m_accelerometerManager->Stop();
	m_accelerometerManager->FreeInstance();

	m_touchManager->Remove();
	m_touchManager->FreeInstance();

	m_menuManager->Remove();
	m_menuManager->FreeInstance();

	m_font->Remove();
	m_font->FreeInstance();

	LocaliseTerminate();

	WorldLoader::ClearAndFree();

	IwResManagerTerminate();

	IwGraphicsTerminate();
	IwGxTerminate();
}

//---------------------------------------------------------------------------------------------

GameLevel* Game::CreateGameLevel( const int modeID, const int levelID )
{
	Physics* physics = Physics::GetInstance();
	physics->Init();

	GameLevel* gameLevel = new GameLevel( modeID, levelID );
	gameLevel->Init();

	physics->FreeInstance();

	return gameLevel;
}