#include "MenuSplashScreen.h"
#include "MenuManager.h"

#include "IwSound.h"

#include "../Engine/Font/Font.h"
#include "../Engine/Font/FontManager.h"
#include "../Engine/Input/TouchManager.h"
#include "../Engine/PostProcessing/PostProcessing.h"

#include "../Utilities/SoundManager/SoundManager.h"

#include "../Localization/Localization.h"

//---------------------------------------------------------------------------------

const int unsigned	MenuSplashScreen::s_touchTextAlphaRestart	= 1000;		// 1 sec
const int			MenuSplashScreen::s_touchTextAlphaDescrease	= 5;

//---------------------------------------------------------------------------------

MenuSplashScreen::MenuSplashScreen()
{
	m_typeId = MENUELEMENTTYPE_SPLASH_SCREEN;

	GetElementByName<GUIFrame*>( m_frame, "menu_splashscreen" );

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );

	m_postProcessing = PostProcessing::GetInstance();
	m_fontManager = FontManager::GetInstance();
}

//---------------------------------------------------------------------------------

MenuSplashScreen::~MenuSplashScreen()
{
	m_fontManager->FreeInstance();
	m_postProcessing->FreeInstance();
}

//---------------------------------------------------------------------------------

void MenuSplashScreen::Init()
{
}

//---------------------------------------------------------------------------------

void MenuSplashScreen::Render()
{
	const char* text = LocaliseFindString( "SS_TOUCH_TEXT" );
	const int width = IwGxGetScreenWidth();
	const int height = 200;
	const int x = 0;
	const int y = IwGxGetScreenHeight() - height;

	CIwColour color;
	color.Set( 255, 255, 255, m_touchTextAlpha );

	m_fontManager->DrawText( text, x, y, width, height, Font::FN_TAHOMA, Font::FS_MEDIUM, color, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE );
}

//---------------------------------------------------------------------------------

void MenuSplashScreen::Update( const s3e_uint64_t dt )
{
	MenuTouch::UpdateTouches();

	if( s3eTimerGetMs() - m_touchTextStartTime > s_touchTextAlphaRestart )
	{
		m_touchTextAlpha = 255;
		m_touchTextStartTime = s3eTimerGetMs();
	}
	else
	{
		m_touchTextAlpha -= s_touchTextAlphaDescrease;
	}
}

//---------------------------------------------------------------------------------

void MenuSplashScreen::HandleTouches( const Touch* touch )
{
	const int x = touch->x;
	const int y = touch->y;

	const int width = IwGxGetScreenWidth();
	const int height = IwGxGetScreenHeight();

	if( x > 0 && x < width && y > 0 && y < height )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_MAIN_MENU );
	}
}

//---------------------------------------------------------------------------------

void MenuSplashScreen::OnShow()
{
	MenuElement::OnShow();
	m_menuManager->Fadein();
	m_postProcessing->PreRender();
	m_menuManager->Render();
	m_postProcessing->PostRender();
	m_menuManager->Fadeout();
	m_soundManager->PlayMenuMusic();
	m_touchTextStartTime = s3eTimerGetMs();
}

//---------------------------------------------------------------------------------

void MenuSplashScreen::OnHide()
{
	MenuElement::OnHide();

	m_menuManager->Fadein();
}

//---------------------------------------------------------------------------------

bool MenuSplashScreen::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//---------------------------------------------------------------------------------

bool MenuSplashScreen::OnButtonReleased( const GUIButton* apButton )
{
	return false;
}