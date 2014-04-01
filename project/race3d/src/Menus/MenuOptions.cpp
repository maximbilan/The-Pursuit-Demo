#include "MenuOptions.h"
#include "MenuManager.h"

#include "../GameStates/GameState.h"
#include "../GameStates/GS_Menu.h"
#include "../GameStates/GS_GamePlay.h"

//-----------------------------------------------------------------------------------

MenuOptions::MenuOptions()
{
	m_typeId = MENUELEMENTTYPE_OPTIONS;

	GetElementByName<GUIFrame*>( m_frame, "menu_options" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );
}

//-----------------------------------------------------------------------------------

MenuOptions::~MenuOptions()
{
}

//-----------------------------------------------------------------------------------

void MenuOptions::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuOptions::Render()
{
}

//-----------------------------------------------------------------------------------

void MenuOptions::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MenuOptions::OnShow()
{
	MenuElement::OnShow();

	m_menuManager->Fade();
}

//-----------------------------------------------------------------------------------

void MenuOptions::OnHide()
{
	MenuElement::OnShow();
}

//-----------------------------------------------------------------------------------

bool MenuOptions::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//-----------------------------------------------------------------------------------

bool MenuOptions::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "mo_sounds_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_SOUNDS );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mo_conrols_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_CONTROLS );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mo_language_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_LANGUAGES );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mo_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		const int currentStateId = GameState::GetStateID();

		if( currentStateId == GameState::k_state_gameplay )
		{
			m_menuManager->PushMenu( MENUELEMENTTYPE_IN_GAME );
		}
		else
		{
			if( currentStateId == GameState::k_state_menu )
			{
				m_menuManager->PushMenu( MENUELEMENTTYPE_MAIN_MENU );
			}
		}
		
		return true;
	}

	return false;
}