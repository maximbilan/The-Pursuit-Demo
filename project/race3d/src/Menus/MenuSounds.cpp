#include "MenuSounds.h"
#include "MenuManager.h"

#include "../GUI/GUILabel.h"
#include "../GUI/GUIButton.h"

#include "../Utilities/Config/Config.h"
#include "../Utilities/SoundManager/SoundManager.h"

#include "../GameStates/GameState.h"
#include "../GameStates/GS_Menu.h"
#include "../GameStates/GS_GamePlay.h"

//-----------------------------------------------------------------------------------

MenuSounds::MenuSounds()
{
	m_typeId = MENUELEMENTTYPE_SOUNDS;

	m_config = Config::GetInstance();

	GetElementByName<GUIFrame*>( m_frame, "menu_sounds" );

	GetElementByName<GUILabel*>( m_effectsLabel, "ms_effects_label" );
	GetElementByName<GUILabel*>( m_musicLabel, "ms_music_label" );
	GetElementByName<GUILabel*>( m_vibrationLabel, "ms_vibration_label" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );

	m_isEffectsBtnPressed = false;
	m_isMusicBtnPressed = false;
	m_isVibrationBtnPressed = false;

	SetSwitchButton( m_effectsLabel, m_isEffectsBtnPressed );
	SetSwitchButton( m_musicLabel, m_isMusicBtnPressed );
	SetSwitchButton( m_vibrationLabel, m_isVibrationBtnPressed );
}

//-----------------------------------------------------------------------------------

MenuSounds::~MenuSounds()
{
	m_config->FreeInstance();
}

//-----------------------------------------------------------------------------------

void MenuSounds::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuSounds::Render()
{
}

//-----------------------------------------------------------------------------------

void MenuSounds::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MenuSounds::OnShow()
{
	MenuElement::OnShow();

	m_isEffectsBtnPressed = m_config->IsSoundEffectsEnabled();
	m_isMusicBtnPressed = m_config->IsMusicEnabled();
	m_isVibrationBtnPressed = m_config->IsVibrationEnabled();

	SetSwitchButton( m_effectsLabel, m_isEffectsBtnPressed );
	SetSwitchButton( m_musicLabel, m_isMusicBtnPressed );
	SetSwitchButton( m_vibrationLabel, m_isVibrationBtnPressed );

	m_menuManager->Fade();
}

//-----------------------------------------------------------------------------------

void MenuSounds::OnHide()
{
	MenuElement::OnShow();
}

//-----------------------------------------------------------------------------------

bool MenuSounds::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//-----------------------------------------------------------------------------------

bool MenuSounds::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "effects_checkbox_on" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_isEffectsBtnPressed = !m_isEffectsBtnPressed;
		SetSwitchButton( m_effectsLabel, m_isEffectsBtnPressed );
		m_config->SetSoundEffectsEnabled( m_isEffectsBtnPressed );
		m_config->Save();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "music_checkbox_on" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_isMusicBtnPressed = !m_isMusicBtnPressed;
		SetSwitchButton( m_musicLabel, m_isMusicBtnPressed );
		m_config->SetMusicEnabled( m_isMusicBtnPressed );
		m_config->Save();

		if( !m_isMusicBtnPressed )
		{
			m_soundManager->StopAllSounds();
		}
		else
		{
			const int currentStateId = GameState::GetStateID();

			if( currentStateId == GameState::k_state_gameplay )
			{
				m_soundManager->PlayGameMusic();
			}
			else if( currentStateId == GameState::k_state_menu  )
			{
				m_soundManager->PlayMenuMusic();
			}
		}

		return true;
	}

	if( apButton->m_Hash == IwHashString( "vibration_checkbox_on" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_isVibrationBtnPressed = !m_isVibrationBtnPressed;
		SetSwitchButton( m_vibrationLabel, m_isVibrationBtnPressed );
		m_config->SetVibrationEnabled( m_isVibrationBtnPressed );
		m_config->Save();
		return true;
	}
	
	if( apButton->m_Hash == IwHashString( "ms_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PopMenu();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------

void MenuSounds::SetSwitchButton( GUILabel* label, const bool value )
{
	if( label )
	{
		if( value )
		{
			label->SetString( "STR_ON" );
		}
		else
		{
			label->SetString( "STR_OFF" );
		}
	}
}