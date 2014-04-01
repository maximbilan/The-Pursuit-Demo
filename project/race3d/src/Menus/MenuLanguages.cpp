#include "MenuLanguages.h"
#include "MenuManager.h"

#include "../Utilities/Config/Config.h"
#include "../Localization/Localization.h"

//-----------------------------------------------------------------------------------

MenuLanguages::MenuLanguages() :
	m_currentLanguage( 0 )
{
	m_config = Config::GetInstance();

	m_typeId = MENUELEMENTTYPE_LANGUAGES;

	GetElementByName<GUIFrame*>( m_frame, "menu_languages" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );
}

//-----------------------------------------------------------------------------------

MenuLanguages::~MenuLanguages()
{
	m_config->FreeInstance();
}

//-----------------------------------------------------------------------------------

void MenuLanguages::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuLanguages::Render()
{
}

//-----------------------------------------------------------------------------------

void MenuLanguages::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MenuLanguages::OnShow()
{
	MenuElement::OnShow();

	m_currentLanguage = m_config->GetLanguageCode();

	m_menuManager->Fade();
}

//-----------------------------------------------------------------------------------

void MenuLanguages::OnHide()
{
	MenuElement::OnShow();
}

//-----------------------------------------------------------------------------------

bool MenuLanguages::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//-----------------------------------------------------------------------------------

bool MenuLanguages::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "ml_flag_en_button" ) )
	{
		m_currentLanguage = Config::CLC_ENGLISH;
		OnLanguageChanged();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "ml_flag_ru_button" ) )
	{
		m_currentLanguage = Config::CLC_RUSSIAN;
		OnLanguageChanged();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "ml_flag_fr_button" ) )
	{
		m_currentLanguage = Config::CLC_FRENCH;
		OnLanguageChanged();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "ml_flag_ge_button" ) )
	{
		m_currentLanguage = Config::CLC_GERMAN;
		OnLanguageChanged();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "ml_flag_it_button" ) )
	{
		m_currentLanguage = Config::CLC_ITALIAN;
		OnLanguageChanged();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "ml_flag_sp_button" ) )
	{
		m_currentLanguage = Config::CLC_SPAINISH;
		OnLanguageChanged();
		return true;
	}

	if( apButton->m_Hash == IwHashString( "ml_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PopMenu();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------

void MenuLanguages::OnLanguageChanged()
{
	m_soundManager->PlaySFXUiSelect();

	m_config->SetLanguageCode( m_currentLanguage );
	m_config->Save();

	LocaliseLoadStrings( m_currentLanguage );

	m_menuManager->ResetTexts();
	m_menuManager->PopMenu();
}