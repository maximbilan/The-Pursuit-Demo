#include "GS_GamePlay.h"

#include "s3eAccelerometer.h"

#include "../Engine/Input/TouchManager.h"
#include "../Engine/Input/KeyManager.h"
#include "../Engine/Input/AccelerometerManager.h"
#include "../Engine/Camera.h"
#include "../Engine/Physics.h"

#include "../GameLogic/GameLevel.h"
#include "../GameLogic/Actor/Player.h"
#include "../GameLogic/GameMode/GameMode.h"

#include "../Menus/MenuManager.h"
#include "../Menus/MenuElement.h"
#include "../Menus/MenuGameGUI.h"
#include "../Menus/GamePlayLog.h"
#include "../Menus/GamePlayCountdown.h"

#include "../Utilities/Config/Config.h"
#include "../Utilities/SoundManager/SoundManager.h"

#include "../Scripting/GameLevelScript.h"

//-----------------------------------------------------------------------------------

GS_GamePlay::GS_GamePlay( GameLevel* gameLevel )
{
	s_stateID = GameState::k_state_gameplay;

	m_physics = Physics::GetInstance();

	m_config = Config::GetInstance();

	m_keyManager = KeyManager::GetInstance();
	m_keyManager->Init();

	m_touchManager = TouchManager::GetInstance();

	m_accelerometerManager = AccelerometerManager::GetInstance();
	m_accelerometerManager->Start();

	m_menuManager = MenuManager::GetInstance();

	m_gameLevel = gameLevel;
	m_gameMode = m_config->GetGameMode( m_gameLevel->GetGameModeID() );
	m_gameMode->Init( m_gameLevel->GetPlayer() );

	m_gameGui = static_cast<MenuGameGui*>( m_menuManager->GetMenuElementById( MenuElement::MENUELEMENTTYPE_GAME_GUI ) );
	m_gameGui->SetGameModeId( m_gameLevel->GetGameModeID() );
	m_gameGui->SetGameMode( m_gameMode );
	m_menuManager->PushMenu( MenuElement::MENUELEMENTTYPE_GAME_GUI );

	m_menuManager->Fadein();

	m_soundManager = SoundManager::GetInstance();
	m_soundManager->PlayGameMusic();

	m_log = new GamePlayLog();
	m_countdown = new GamePlayCountdown();

	s_subState = k_substate_countdown;
	m_countdown->Enable();
}

//-----------------------------------------------------------------------------------

GS_GamePlay::~GS_GamePlay()
{
	delete m_countdown;
	delete m_log;

	m_soundManager->FreeInstance();

	delete m_gameLevel;

	m_menuManager->FreeInstance();
	m_keyManager->FreeInstance();
	m_accelerometerManager->FreeInstance();
	m_touchManager->FreeInstance();

	m_config->FreeInstance();

	m_physics->Remove();
	m_physics->FreeInstance();
}

//-----------------------------------------------------------------------------------

void GS_GamePlay::ResumeState()
{
}

//-----------------------------------------------------------------------------------

void GS_GamePlay::UpdateState( const s3e_uint64_t frameTime )
{
	m_menuManager->Update( frameTime );
	m_log->Update();

	switch( s_subState )
	{
	case k_substate_gameplay:
		{
			m_keyManager->Update();

			m_physics->Update( frameTime );
			m_gameLevel->Update( frameTime );

			if( m_gameMode->GetState() == GameMode::GMS_SUSPENDED )
			{
				m_gameMode->Resume();
			}

			m_gameMode->Update( frameTime );

			const int32 accelerometerValue = ( m_accelerometerManager->GetX() * IW_GEOM_ONE ) / S3E_ACCELEROMETER_1G;

			Player* player = m_gameLevel->GetPlayer();
			if( player )
			{
				const int controlsType = m_config->GetControlsType();

				switch( controlsType )
				{
					case Config::CCT_STANDART:
					{
						if( m_keyManager->IsPressedLeftKey() || m_gameGui->GetBtnEventValue( MenuGameGui::MGGBE_LEFT ) )
						{
							player->DoLeft();
						}

						if( m_keyManager->IsPressedRightKey() || m_gameGui->GetBtnEventValue( MenuGameGui::MGGBE_RIGHT ) )
						{
							player->DoRight();
						}
					}
					break;

					case Config::CCT_ACCELEROMETER:
					{
						if( m_keyManager->IsPressedLeftKey() || accelerometerValue < -100 )
						{
							player->DoLeft();
						}

						if( m_keyManager->IsPressedRightKey() || accelerometerValue > 100 )
						{
							player->DoRight();
						}
					}
					break;
				}

				if( m_keyManager->IsPressedUpKey() || m_gameGui->GetBtnEventValue( MenuGameGui::MGGBE_UP ) )
				{
					player->Gas();
				}

				if( m_keyManager->IsPressedDownKey() || m_gameGui->GetBtnEventValue( MenuGameGui::MGGBE_DOWN ) )
				{
					player->Brake();
				}
				else
				{
					if( m_config->IsAutogasEnabled() )
					{
						player->Gas();
					}
				}
			}
		}
		break;
	case k_substate_ingame_menu:
		{
			if( m_gameMode->GetState() == GameMode::GMS_ACTIVE )
			{
				m_gameMode->Suspend();
				m_config->Save();
			}
		}
		break;
	case k_substate_countdown:
		{
			if( GetPrevSubStateID() != k_substate_ingame_menu )
			{
				m_physics->Update( frameTime );
			}

			m_gameLevel->Update( frameTime );

			if( m_gameMode->GetState() == GameMode::GMS_ACTIVE )
			{
				m_gameMode->Suspend();
				m_config->Save();
			}

			if( m_countdown->IsEnabled() )
			{
				m_countdown->Update();
			}
			else
			{
				s_subState = k_substate_gameplay;
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------------

void GS_GamePlay::RenderState()
{
	m_gameLevel->Render();
	m_menuManager->Render();
	
	switch( s_subState )
	{
	case k_substate_gameplay:
	case k_substate_countdown:
		{
			m_log->Render();
			m_countdown->Render();
		}
		break;
	}
}
//-----------------------------------------------------------------------------------

void GS_GamePlay::RespawnPlayer()
{
	Player* player = m_gameLevel->GetPlayer();
	if( player )
	{
		m_gameLevel->GetScript()->SelectScriptFunction ( "OnRespawnPlayer" );
		m_gameLevel->GetScript()->Go();

		if( m_gameMode->GetID() == GameMode::GM_ID_TRAINING )
		{
			player->Respawn();
		}
	}
}

//-----------------------------------------------------------------------------------

void GS_GamePlay::RunLevelScript( const std::string scriptFunc )
{
	m_gameLevel->GetScript()->SelectScriptFunction( scriptFunc.c_str() );
	m_gameLevel->GetScript()->Go();
}

//-----------------------------------------------------------------------------------

void GS_GamePlay::RunCountdown()
{
	m_countdown->Enable();
}