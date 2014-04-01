#include "MenuControls.h"
#include "MenuManager.h"

#include "../GUI/GUILabel.h"

#include "../Utilities/Config/Config.h"

//-----------------------------------------------------------------------------------

const char* MenuControls::s_controlsTypesIds[] = { "MC_ELEMENT_STANDART",
												   "MC_ELEMENT_ACCELEROMETER" };

//-----------------------------------------------------------------------------------

MenuControls::MenuControls() :
	m_isAutogasPressed( false ),
	m_isFlipPressed( false ),
	m_currentControlId( 0 )
{
	m_typeId = MENUELEMENTTYPE_CONTROLS;

	m_config = Config::GetInstance();

	GetElementByName<GUIFrame*>( m_frame, "menu_controls" );

	GetElementByName<GUILabel*>( m_autogasLabel, "mc_autogas_label" );
	GetElementByName<GUILabel*>( m_flipLabel, "mc_flip_label" );
	GetElementByName<GUILabel*>( m_elementNameLabel, "mc_element_name_label" );

	GUIFrame* standartControl = NULL;
	GUIFrame* accelerometerControl = NULL;

	GetElementByName<GUIFrame*>( standartControl, "mc_controls_element_01" );
	GetElementByName<GUIFrame*>( accelerometerControl, "mc_controls_element_02" );

	m_controlsFrames.push_back( standartControl );
	m_controlsFrames.push_back( accelerometerControl );

	for( FrameArray::const_iterator it = m_controlsFrames.begin(), end = m_controlsFrames.end(); it != end; ++it )
	{
		( *it )->SetVisible( false );
	}

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );

	SetSwitchButton( m_autogasLabel, m_isAutogasPressed );
	SetSwitchButton( m_flipLabel, m_isFlipPressed );
}

//-----------------------------------------------------------------------------------

MenuControls::~MenuControls()
{
	m_config->FreeInstance();
}

//-----------------------------------------------------------------------------------

void MenuControls::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuControls::Render()
{
}

//-----------------------------------------------------------------------------------

void MenuControls::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MenuControls::OnShow()
{
	MenuElement::OnShow();

	m_currentControlId = m_config->GetControlsType();
	m_isAutogasPressed = m_config->IsAutogasEnabled();
	m_isFlipPressed = m_config->IsFlipButtonsEnabled();

	SetSwitchButton( m_autogasLabel, m_isAutogasPressed );
	SetSwitchButton( m_flipLabel, m_isFlipPressed );

	OnControlsChanged();

	m_menuManager->Fade();
}

//-----------------------------------------------------------------------------------

void MenuControls::OnHide()
{
	MenuElement::OnHide();
}

//-----------------------------------------------------------------------------------

bool MenuControls::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//-----------------------------------------------------------------------------------

bool MenuControls::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "mc_arrow_left_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();

		if( m_currentControlId == Config::CCT_COUNT - 1 )
		{
			m_currentControlId = 0;
		}
		else
		{
			m_currentControlId++;
		}

		OnControlsChanged();

		return true;
	}

	if( apButton->m_Hash == IwHashString( "mc_arrow_right_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();

		if( m_currentControlId == 0 )
		{
			m_currentControlId = Config::CCT_COUNT - 1;
		}
		else		
		{
			m_currentControlId--;
		}
		
		OnControlsChanged();

		return true;
	}

	if( apButton->m_Hash == IwHashString( "mc_autogas_checkbox_on" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_isAutogasPressed = !m_isAutogasPressed;
		SetSwitchButton( m_autogasLabel, m_isAutogasPressed );
		m_config->SetAutogasEnabled( m_isAutogasPressed );
		m_config->Save();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mc_flip_checkbox_on" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_isFlipPressed = !m_isFlipPressed;
		SetSwitchButton( m_flipLabel, m_isFlipPressed );
		m_config->SetFlipButtonsEnabled( m_isFlipPressed );
		m_config->Save();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mc_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PopMenu();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------

void MenuControls::SetSwitchButton( GUILabel* label, const bool value )
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

//-----------------------------------------------------------------------------------

void MenuControls::OnControlsChanged()
{
	m_elementNameLabel->SetString( s_controlsTypesIds[m_currentControlId] );

	unsigned int index = 0;
	for( FrameArray::const_iterator it = m_controlsFrames.begin(), end = m_controlsFrames.end(); it != end; ++it )
	{
		bool visible = false;
		if( index == m_currentControlId )
		{
			visible = true;
		}

		( *it )->SetVisible( visible );
		++index;
	}

	m_config->SetControlsType( m_currentControlId );
	m_config->Save();
}