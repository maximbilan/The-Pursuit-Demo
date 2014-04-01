#include "MenuGameMode.h"
#include "MenuManager.h"
#include "Popup.h"

#include "../GameObjects/Car.h"
#include "../GameStates/GS_Loading.h"
#include "../GameLogic/GameLevel.h"
#include "../GameLogic/GameMode/GameMode.h"

#include "../Utilities/SoundManager/SoundManager.h"

//-----------------------------------------------------------------------------------

MenuGameMode::MenuGameMode() :
	m_currentCarFrame( Car::CAR_COLOR_RED )
{
	m_typeId = MENUELEMENTTYPE_GAME_MODE;

	GetElementByName<GUIFrame*>( m_frame, "menu_gamemode" );

	GUIFrame* redCar = NULL;
	GUIFrame* blueCar = NULL;
	GUIFrame* grayCar = NULL;
	GUIFrame* purpleCar = NULL;

	GetElementByName<GUIFrame*>( redCar, "menu_gamemode_car_red_frame" );
	GetElementByName<GUIFrame*>( blueCar, "menu_gamemode_car_blue_frame" );
	GetElementByName<GUIFrame*>( grayCar, "menu_gamemode_car_gray_frame" );
	GetElementByName<GUIFrame*>( purpleCar, "menu_gamemode_car_purple_frame" );

	m_carFrames.push_back( redCar );
	m_carFrames.push_back( blueCar );
	m_carFrames.push_back( grayCar );
	m_carFrames.push_back( purpleCar );

	for( FrameArray::const_iterator it = m_carFrames.begin(), end = m_carFrames.end(); it !=end; ++it )
	{
		(*it)->SetVisible( false );
	}

	m_noAvailableFeaturePopup = new Popup( Popup::PT_OK );
	m_noAvailableFeaturePopup->SetString( "NO_AVAILABLE_FEATURE" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );
}

//-----------------------------------------------------------------------------------

MenuGameMode::~MenuGameMode()
{
	delete m_noAvailableFeaturePopup;
}

//-----------------------------------------------------------------------------------

void MenuGameMode::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuGameMode::Render()
{
}

//-----------------------------------------------------------------------------------

void MenuGameMode::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MenuGameMode::OnShow()
{
	MenuElement::OnShow();

	m_currentCarFrame = Car::CAR_COLOR_RED;
	SetCurrentVisibleCarFrame();

	m_menuManager->Fade();
}

//-----------------------------------------------------------------------------------

void MenuGameMode::OnHide()
{
	MenuElement::OnHide();
}

//-----------------------------------------------------------------------------------

bool MenuGameMode::OnButtonPressed( const GUIButton* apButton )
{
	if( m_noAvailableFeaturePopup->IsEnabled() )
	{
		m_noAvailableFeaturePopup->OnButtonPressed( apButton );

		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------------

bool MenuGameMode::OnButtonReleased( const GUIButton* apButton )
{
	if( m_noAvailableFeaturePopup->IsEnabled() )
	{
		m_noAvailableFeaturePopup->OnButtonReleased( apButton );

		if( m_noAvailableFeaturePopup->GetButtonStatus() == Popup::PB_RELEASED )
		{
			m_soundManager->PlaySFXUiSelect();
			m_noAvailableFeaturePopup->Hide();
			OnShow();

			return true;
		}

		return false;
	}

	if( apButton->m_Hash == IwHashString( "mgm_arrow_left_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();

		if( m_currentCarFrame == 0 )
		{
			m_currentCarFrame = Car::CAR_COLOR_COUNT - 1;
		}
		else
		{
			m_currentCarFrame--;
		}
		SetCurrentVisibleCarFrame();

		return true;
	}

	if( apButton->m_Hash == IwHashString( "mgm_arrow_right_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();

		if( m_currentCarFrame == Car::CAR_COLOR_COUNT - 1 )
		{
			m_currentCarFrame = 0;
		}
		else
		{
			m_currentCarFrame++;
		}
		SetCurrentVisibleCarFrame();

		return true;
	}

	if( apButton->m_Hash == IwHashString( "mgm_timeattack_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_soundManager->StopMenuMusic();

		GameState::SetState( new GS_Loading( GameMode::GM_ID_TIME_ATTACK, GameLevel::GAMELEVEL_TIME_ATTACK ) );

		return true;
	}

	if( apButton->m_Hash == IwHashString( "mgm_training_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_soundManager->StopMenuMusic();

		GameState::SetState( new GS_Loading( GameMode::GM_ID_TRAINING, GameLevel::GAMELEVEL_TRAINING ) );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mgm_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MenuElement::MENUELEMENTTYPE_MAIN_MENU );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------

void MenuGameMode::SetCurrentVisibleCarFrame()
{
	unsigned int index = 0;
	for( FrameArray::const_iterator it = m_carFrames.begin(), end = m_carFrames.end(); it !=end; ++it )
	{
		bool visible = false;
		if( index == m_currentCarFrame )
		{
			visible = true;
		}

		( *it )->SetVisible( visible );
		++index;
	}
}