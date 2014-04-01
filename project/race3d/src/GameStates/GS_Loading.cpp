#include "GS_Loading.h"
#include "GS_Menu.h"
#include "GS_GamePlay.h"

#include "s3eKeyboard.h"

#include "IwResManager.h"
#include "IwGraphics.h"

#include "../game.h"
#include "../Menus/MenuManager.h"
#include "../Menus/LoadingScreen.h"
#include "../Localization/Localization.h"
#include "../GameLogic/GameLevel.h"
#include "../GameLogic/GameMode/GameMode.h"
#include "../Engine/Font/FontManager.h"
#include "../Utilities/Config/Config.h"
#include "../Utilities/SoundManager/SoundManager.h"
#include "../Engine/ShaderManager.h"
#include "../Engine/PostProcessing/PostProcessing.h"

//-----------------------------------------------------------------------------------

const uint32 GS_Loading::s_renderCallbackPeriod = 10000;

// Loading render callback, called from IwSerialiseSetCallback
void LoadingRenderCallback();

//-----------------------------------------------------------------------------------

GS_Loading::GS_Loading() :
	m_levelID( GameLevel::GAMELEVEL_NONE ),
	m_gameModeID( GameMode::GM_ID_NONE )
{
	s_stateID = GameState::k_state_loading;
	s_subState = k_substate_start_init;
	m_loadingScreen = new LoadingScreen();
}

//-----------------------------------------------------------------------------------

GS_Loading::GS_Loading( const int modeID, const int levelID ) :
	m_gameModeID( modeID ),
	m_levelID( levelID )
{
	s_stateID = GameState::k_state_loading;
	s_subState = k_substate_level;
	m_loadingScreen = new LoadingScreen();
}

//-----------------------------------------------------------------------------------

GS_Loading::~GS_Loading()
{
	delete m_loadingScreen;
}

//-----------------------------------------------------------------------------------

void GS_Loading::ResumeState()
{
}

//-----------------------------------------------------------------------------------

void GS_Loading::UpdateState( const s3e_uint64_t frameTime )
{
	IwSerialiseSetCallbackPeriod( s_renderCallbackPeriod );
	IwSerialiseSetCallback( LoadingRenderCallback );

	switch( s_subState )
	{
		case k_substate_start_init:
		{
			Config* config = Config::GetInstance();

			FontManager* font = FontManager::GetInstance();
			font->Init();

			MenuManager* menuManager = MenuManager::GetInstance();
			menuManager->Init();

			uint32 lLanguage;

			if( config->IsExist() )
			{
				lLanguage = static_cast<unsigned int>( config->GetLanguageCode() );
			}
			else
			{
				lLanguage = s3eDeviceGetInt( S3E_DEVICE_LANGUAGE );
			}

			ShaderManager* shaderManager = ShaderManager::GetInstance();
			shaderManager->Load();
			shaderManager->FreeInstance();

			PostProcessing* postProcessing = PostProcessing::GetInstance();
			postProcessing->LoadShaders();

			LocaliseLoadStrings( lLanguage );

			SoundManager* soundManager = SoundManager::GetInstance();
			soundManager->Load();
			soundManager->FreeInstance();

			//postProcessing->SetState( PostProcessing::PP_SEPIA );
			postProcessing->FreeInstance();
			
			menuManager->FreeInstance();
			font->FreeInstance();
			config->FreeInstance();

			GameState::SetState( new GS_Menu() );
		}
		break;

		case k_substate_level:
		{
			Game* game = Game::GetInstance();
			GameLevel* gameLevel = game->CreateGameLevel( m_gameModeID, m_levelID );
			game->FreeInstance();

			GameState::SetState( new GS_GamePlay( gameLevel ) );
		}
		break;
	}

	IwSerialiseSetCallback( NULL );
	s3eKeyboardClearState();
	s3eDeviceCheckPauseRequest();
}

//-----------------------------------------------------------------------------------

void GS_Loading::RenderState()
{
	IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );

	m_loadingScreen->Render();
}

//-----------------------------------------------------------------------------------

void LoadingRenderCallback()
{
	IwGxTickUpdate();

	Game* game = Game::GetInstance();
	game->Render();
	s3eDeviceYield( 0 );
	game->FreeInstance();
}