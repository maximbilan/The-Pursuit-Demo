#include "MenuAbout.h"
#include "MenuManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "IwDebug.h"

#include "../Common/Defines.h"
#include "../Engine/Font/FontManager.h"
#include "../Engine/Font/Font.h"
#include "../Engine/Input/TouchManager.h"
#include "../Utilities/Components/SmoothScroll.h"
#include "../Localization/Localization.h"

//-----------------------------------------------------------------------------------

MenuAbout::MenuAbout()
{
	m_typeId = MENUELEMENTTYPE_ABOUT;

	GetElementByName<GUIFrame*>( m_frame, "menu_about" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );

	m_fontManager = FontManager::GetInstance();
	m_touchManager = TouchManager::GetInstance();

	m_smoothScroll = new SmoothScroll();

	m_titlesX = static_cast<int>( IwGxGetScreenWidth() * 0.5 - ( IwGxGetScreenWidth() * 0.25 ) );
	m_titlesY = static_cast<int>( IwGxGetScreenHeight() );
}

//-----------------------------------------------------------------------------------

MenuAbout::~MenuAbout()
{
	delete m_smoothScroll;
	m_touchManager->FreeInstance();
	m_fontManager->FreeInstance();
}

//-----------------------------------------------------------------------------------

void MenuAbout::Init()
{
}

//-----------------------------------------------------------------------------------

void MenuAbout::Render()
{
	const int32 width = IwGxGetScreenWidth();
	const int32 height = IwGxGetScreenHeight();

	m_fontManager->DrawPreparedText( m_preparedText, m_titlesX, m_titlesY, static_cast<int>( width * 0.5 ), height, Font::FN_TAHOMA, Font::FS_MEDIUM, 
									 IwGxGetColFixed( IW_GX_COLOUR_WHITE ), IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP );
}

//-----------------------------------------------------------------------------------

void MenuAbout::Update( const s3e_uint64_t dt )
{
	m_smoothScroll->Update();

	if( m_smoothScroll->IsActive() )
	{
		m_titlesY -= static_cast<int>( m_smoothScroll->GetStepY() * dt );
	}
	else
	{
		m_titlesY -= static_cast<int>( 0.1 * dt );
	}

	int screenHeight = static_cast<int>( IwGxGetScreenHeight() );

	if( m_titlesY + m_preparedText.GetHeight() < 0 )
	{
		m_titlesY = screenHeight;
	}

	if( m_titlesY > screenHeight + 1 )
	{
		m_titlesY += -screenHeight - m_preparedText.GetHeight();
	}
}

//-----------------------------------------------------------------------------------

void MenuAbout::OnShow()
{
	MenuElement::OnShow();

	PrepareTitles();

	m_menuManager->Fade();
}

//-----------------------------------------------------------------------------------

void MenuAbout::OnHide()
{
	MenuElement::OnHide();
}

//-----------------------------------------------------------------------------------

bool MenuAbout::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//-----------------------------------------------------------------------------------

bool MenuAbout::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "ma_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PopMenu();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------

void MenuAbout::PrepareTitles()
{
	const char* amountStr = LocaliseFindString( "MA_TITLE_COUNT" );
	const int amount = atoi( amountStr );

	if( amount != 0 )
	{
		std::string str = "";
		str = LocaliseFindString( "MA_TITLE_START" );
		str += "\n";

		str += LocaliseFindString( "GAME_NAME" );
		str += "\n";

		str += GAMEVERSION;
		str += "\n";

		for( int i = 0; i < amount; ++i )
		{
			char title[64];
			sprintf( title, "MA_TITLE_%d", i + 1 );
			str += LocaliseFindString( title );
			str += "\n";
		}

		str += LocaliseFindString( "MA_TITLE_END" );

		const int32 width = IwGxGetScreenWidth();
		const int32 height = IwGxGetScreenHeight();

		m_titlesY = height;
		
		m_fontManager->PrepareText( m_preparedText, str.c_str(), m_titlesX, m_titlesY, static_cast<int>( width * 0.5 ), height * 1000, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP );
	}
}