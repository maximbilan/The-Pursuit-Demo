#include "MenuGameGUI.h"
#include "MenuManager.h"

#include "../GameStates/GS_Menu.h"
#include "../GameStates/GS_GamePlay.h"

#include "../GameLogic/GameLevel.h"
#include "../GameLogic/GameMode/GameMode.h"
#include "../GameLogic/GameMode/GM_TimeAttack.h"
#include "../GameLogic/GameMode/GM_Training.h"

#include "../Engine/Font/FontManager.h"
#include "../Engine/Font/Font.h"
#include "../Engine/Input/TouchManager.h"

#include "../Utilities/Config/Config.h"

//-----------------------------------------------------------------------------------

const int MenuGameGui::s_rectWidth = 100;
const int MenuGameGui::s_rectHeight = 25;

//-----------------------------------------------------------------------------------

#define BUFFER_RENDER_STR_OUT 64

//-----------------------------------------------------------------------------------

MenuGameGui::MenuGameGui() :
	m_gameModeId( -1 ),
	m_gameMode( NULL ),
	m_laps( -1 )
{
	m_fontManager = FontManager::GetInstance();
	m_touchManager = TouchManager::GetInstance();

	m_typeId = MENUELEMENTTYPE_GAME_GUI;

	m_config = Config::GetInstance();

	GetElementByName<GUIFrame*>( m_frame, "game_gui" );

	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART][MGGBE_UP], "game_gui_button_up_std" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART][MGGBE_DOWN], "game_gui_button_down_std" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART][MGGBE_LEFT], "game_gui_button_left_std" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART][MGGBE_RIGHT], "game_gui_button_right_std" );

	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART_FLIP][MGGBE_UP], "game_gui_button_up_std_flip" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART_FLIP][MGGBE_DOWN], "game_gui_button_down_std_flip" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART_FLIP][MGGBE_LEFT], "game_gui_button_left_std_flip" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_STANDART_FLIP][MGGBE_RIGHT], "game_gui_button_right_std_flip" );

	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER][MGGBE_UP], "game_gui_button_up_accel" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER][MGGBE_DOWN], "game_gui_button_down_accel" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER][MGGBE_LEFT], "game_gui_button_left_accel" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER][MGGBE_RIGHT], "game_gui_button_right_accel" );

	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER_FLIP][MGGBE_UP], "game_gui_button_up_accel_flip" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER_FLIP][MGGBE_DOWN], "game_gui_button_down_accel_flip" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER_FLIP][MGGBE_LEFT], "game_gui_button_left_accel_flip" );
	GetElementByName<GUIButton*>( m_controlButtons[MGGBE_TC_ACCELEROMETER_FLIP][MGGBE_RIGHT], "game_gui_button_right_accel_flip" );

	GetElementByName<GUILabel*>( m_indicators[MGGI_BEST_TIME_LABEL], "game_gui_best_time_label" );
	GetElementByName<GUILabel*>( m_indicators[MGGI_BEST_TIME_VALUE], "game_gui_best_time_value" );
	GetElementByName<GUILabel*>( m_indicators[MGGI_TIME_LABEL], "game_gui_time_label" );
	GetElementByName<GUILabel*>( m_indicators[MGGI_TIME_VALUE], "game_gui_time_value" );
	GetElementByName<GUILabel*>( m_indicators[MGGI_LAP_LABEL], "game_gui_lap_label" );
	GetElementByName<GUILabel*>( m_indicators[MGGI_LAP_VALUE], "game_gui_lap_value" );

	m_btnEvents = new bool[MGGBE_COUNT];
	ResetTouches();

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );

	SetVisibleControls( m_config->GetControlsType(), m_config->IsFlipButtonsEnabled() );
}

//-----------------------------------------------------------------------------------

MenuGameGui::~MenuGameGui()
{
	delete [] m_btnEvents;

	m_fontManager->FreeInstance();
	m_touchManager->FreeInstance();

	m_config->FreeInstance();
}

//-----------------------------------------------------------------------------------

void MenuGameGui::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuGameGui::Render()
{
	if( m_gameModeId != -1 )
	{
		if( m_gameMode )
		{
			switch( m_gameModeId )
			{
			case GameLevel::GAMELEVEL_TIME_ATTACK:
				{
					bool drawBestTimeDummy = true;
					bool drawCurrentTimeDummy = true;

					char out[BUFFER_RENDER_STR_OUT];
					const CIwRect& rectCurrentTime = m_indicators[MGGI_TIME_VALUE]->GetScreenRect();
					const CIwRect& rectBestTime = m_indicators[MGGI_BEST_TIME_VALUE]->GetScreenRect();
					CIwColour color;
					color.Set( IwGxGetColFixed( IW_GX_COLOUR_WHITE ) );

					GM_TimeAttack* timeAttack = static_cast<GM_TimeAttack*>( m_gameMode );
					if( timeAttack )
					{
						const int laps = timeAttack->GetCurrentLap();
						if( laps != m_laps )
						{
							m_laps = laps;
							sprintf( out, "%d", m_laps );
							m_indicators[MGGI_LAP_VALUE]->SetText( out );
						}

						const double currentTime = timeAttack->GetCurrentTime();
						if( currentTime > 0.0f )
						{
							const time_t t = static_cast<time_t>( currentTime * 0.001 );
							if( t >= 0 )
							{
								strftime( out, 6, "%M:%S", localtime( &t ) );
								m_fontManager->DrawText( out, rectCurrentTime.x, rectCurrentTime.y, s_rectWidth, s_rectHeight, Font::FN_TAHOMA, Font::FS_SMALL, color, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP );
								drawCurrentTimeDummy = false;
							}
						}

						const double bestTime = timeAttack->GetBestTime();
						if( bestTime > 0.0f )
						{
							const time_t t = static_cast<time_t>( bestTime * 0.001 );
							if( t >= 0 )
							{
								strftime( out, 6, "%M:%S", localtime( &t ) );
								m_fontManager->DrawText( out, rectBestTime.x, rectBestTime.y, s_rectWidth, s_rectHeight, Font::FN_TAHOMA, Font::FS_SMALL, color, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP );
								drawBestTimeDummy = false;
							}
						}
					}

					if( drawCurrentTimeDummy )
					{
						sprintf( out, "--:--" );
						m_fontManager->DrawText( out, rectCurrentTime.x, rectCurrentTime.y, s_rectWidth, s_rectHeight, Font::FN_TAHOMA, Font::FS_SMALL, color, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP );
					}

					if( drawBestTimeDummy )
					{
						sprintf( out, "--:--" );
						m_fontManager->DrawText( out, rectBestTime.x, rectBestTime.y, s_rectWidth, s_rectHeight, Font::FN_TAHOMA, Font::FS_SMALL, color, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP );
					}
				}
				break;
			case GameLevel::GAMELEVEL_TRAINING:
				{
				}
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------

void MenuGameGui::Update( const s3e_uint64_t dt )
{
	const bool wasTouches = MenuTouch::UpdateTouches();

	if( !wasTouches )
	{
		ResetTouches();
	}
}

//-----------------------------------------------------------------------------------

void MenuGameGui::HandleTouches( const Touch* touch )
{
	const CIwRect& upRect = m_upBtn->GetScreenRect();
	const CIwRect& downRect = m_downBtn->GetScreenRect();
	const CIwRect& leftRect = m_leftBtn->GetScreenRect();
	const CIwRect& rightRect = m_rightBtn->GetScreenRect();

	CheckOnTouchButton( upRect, MGGBE_UP );
	CheckOnTouchButton( downRect, MGGBE_DOWN );
	CheckOnTouchButton( leftRect, MGGBE_LEFT );
	CheckOnTouchButton( rightRect, MGGBE_RIGHT );
}

//-----------------------------------------------------------------------------------

void MenuGameGui::CheckOnTouchButton( const CIwRect rect, const unsigned buttonId )
{
	const int buttonX1 = rect.x;
	const int buttonY1 = rect.y;
	const int buttonX2 = rect.x + rect.w;
	const int buttonY2 = rect.y + rect.h;

	for( int i = 0; i < m_touchManager->GetTouchCount(); ++i )
	{
		Touch* touch = m_touchManager->GetTouch( i );

		const int x = touch->x;
		const int y = touch->y;

		if( ( x >= buttonX1 ) && ( x <= buttonX2 ) && ( y >= buttonY1 ) && ( y <= buttonY2 ) )
		{
			m_btnEvents[buttonId] = true;
			return;
		}
	}

	m_btnEvents[buttonId] = false;
}

//-----------------------------------------------------------------------------------

void MenuGameGui::ResetTouches()
{
	for( int i = 0; i < MGGBE_COUNT; ++i )
	{
		m_btnEvents[i] = false;
	}
}

//-----------------------------------------------------------------------------------

void MenuGameGui::OnShow()
{
	m_frame->SetEnabled( true );

	GetGUIManager()->ClearAllFrames();
	GetGUIManager()->AddFrame( m_frame );

	SetVisibleControls( m_config->GetControlsType(), m_config->IsFlipButtonsEnabled() );

	if( m_gameModeId != -1 )
	{
		switch( m_gameModeId )
		{
		case GameLevel::GAMELEVEL_TIME_ATTACK:
			{
				SetVisibleIndicators( true );
			}
			break;
		case GameLevel::GAMELEVEL_TRAINING:
			{
				SetVisibleIndicators( false );
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------------

void MenuGameGui::OnHide()
{
	m_frame->SetEnabled( false );
}

//-----------------------------------------------------------------------------------

bool MenuGameGui::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//-----------------------------------------------------------------------------------

bool MenuGameGui::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "game_gui_button_menu" ) )
	{
		GameState::GetCurrentState()->SetSubState( GS_GamePlay::k_substate_ingame_menu );

		m_menuManager->PushMenu( MENUELEMENTTYPE_IN_GAME );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "game_gui_button_respawn" ) )
	{
		const int currentStateId = GameState::GetStateID();
		const int currentSubStateId = GameState::GetSubStateID();

		if( currentStateId == GameState::k_state_gameplay && currentSubStateId != GS_GamePlay::k_substate_countdown )
		{
			GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
			if( gameplay )
			{
				gameplay->RespawnPlayer();
			}
		}

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------

void MenuGameGui::SetVisibleControls( const int controlsType, const bool flip )
{
	int controlsGuiType = MGGBE_TC_STANDART;

	switch( controlsType )
	{
		case Config::CCT_STANDART:
		{
			if( flip )
			{
				controlsGuiType = MGGBE_TC_STANDART_FLIP;
			}
			else
			{
				controlsGuiType = MGGBE_TC_STANDART;
			}
		}
		break;

		case Config::CCT_ACCELEROMETER:
		{
			if( flip )
			{
				controlsGuiType = MGGBE_TC_ACCELEROMETER_FLIP;
			}
			else
			{
				controlsGuiType = MGGBE_TC_ACCELEROMETER;
			}
		}
		break;
	}

	for( int i = 0; i < MGGBE_TC_COUNT; ++i )
	{
		for( int j = 0; j < MGGBE_COUNT; ++j )
		{
			bool visible = false;
			if( i == controlsGuiType )
			{
				visible = true;
			}

			if( visible )
			{
				switch( j )
				{
				case MGGBE_UP:
					{
						if( m_config->IsAutogasEnabled() )
						{
							visible = false;
						}

						m_upBtn = m_controlButtons[i][j];
					}
					break;

				case MGGBE_DOWN:
					{
						m_downBtn = m_controlButtons[i][j];
					}
					break;

				case MGGBE_LEFT:
					{
						m_leftBtn = m_controlButtons[i][j];
					}
					break;

				case MGGBE_RIGHT:
					{
						m_rightBtn = m_controlButtons[i][j];
					}
					break;
				}

			}

			m_controlButtons[i][j]->SetVisible( visible );
		}
	}

}

void MenuGameGui::SetVisibleIndicators( const bool visible )
{
	for( int i = 0; i < MGGI_COUNT; ++i )
	{
		m_indicators[i]->SetVisible( visible );
	}
}