#include "MenuInGame.h"
#include "MenuManager.h"
#include "Popup.h"

#include "../GameStates/GameState.h"
#include "../GameStates/GS_Menu.h"
#include "../GameStates/GS_GamePlay.h"

#include "../Utilities/SoundManager/SoundManager.h"

//-----------------------------------------------------------------------------------

MenuInGame::MenuInGame()
{
	m_typeId = MENUELEMENTTYPE_IN_GAME;

	GetElementByName<GUIFrame*>( m_frame, "menu_ingame" );

	m_quitToMenuPopup = new Popup();
	m_quitToMenuPopup->SetString( "MM_QUIT_MSG" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );
}

//-----------------------------------------------------------------------------------

MenuInGame::~MenuInGame()
{
	delete m_quitToMenuPopup;
}

//-----------------------------------------------------------------------------------

void MenuInGame::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuInGame::Render()
{
}

//-----------------------------------------------------------------------------------

void MenuInGame::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MenuInGame::OnShow()
{
	MenuElement::OnShow();
}

//-----------------------------------------------------------------------------------

void MenuInGame::OnHide()
{
	MenuElement::OnHide();
}

//-----------------------------------------------------------------------------------

bool MenuInGame::OnButtonPressed( const GUIButton* apButton )
{
	if( m_quitToMenuPopup->IsEnabled() )
	{
		m_quitToMenuPopup->OnButtonPressed( apButton );

		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------------

bool MenuInGame::OnButtonReleased( const GUIButton* apButton )
{
	if( m_quitToMenuPopup->IsEnabled() )
	{
		m_quitToMenuPopup->OnButtonReleased( apButton );

		if( m_quitToMenuPopup->GetButtonStatus() == Popup::PB_RELEASED )
		{
			if( m_quitToMenuPopup->GetQuestionResult() == Popup::PQ_YES )
			{
				m_soundManager->PlaySFXUiSelect();
				m_quitToMenuPopup->Hide();
				m_soundManager->StopGameMusic();

				GameState::SetStateAndResetStack( new GS_Menu() );

				return true;
			}

			if( m_quitToMenuPopup->GetQuestionResult() == Popup::PQ_NO )
			{
				m_soundManager->PlaySFXUiSelect();
				m_quitToMenuPopup->Hide();
				OnShow();

				return true;
			}

			m_quitToMenuPopup->Reset();
		}

		return false;
	}

	if( apButton->m_Hash == IwHashString( "mig_resume_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		GameState::GetCurrentState()->SetSubState( GS_GamePlay::k_substate_countdown );

		const int currentStateId = GameState::GetStateID();
		if( currentStateId == GameState::k_state_gameplay )
		{
			GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
			if( gameplay )
			{
				gameplay->RunCountdown();
			}
		}

		m_menuManager->PushMenu( MENUELEMENTTYPE_GAME_GUI );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mig_options_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_OPTIONS );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mig_menu_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_quitToMenuPopup->Show();
		return true;
	}

	return false;
}