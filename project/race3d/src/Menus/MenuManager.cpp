#include "MenuManager.h"
#include "MainMenu.h"
#include "MenuSplashScreen.h"
#include "MenuGameGUI.h"
#include "MenuAbout.h"
#include "MenuOptions.h"
#include "MenuSounds.h"
#include "MenuLanguages.h"
#include "MenuControls.h"
#include "MenuSplashScreen.h"
#include "MenuGameMode.h"
#include "MenuInGame.h"
#include "MenuStatistics.h"

#include "s3eDevice.h"
#include "s3ePointer.h"
#include "s3eTimer.h"

#include "IwGx.h"
#include "IwResManager.h"

#include "transitions2d.h"

#include "../Common/Defines.h"

#include "../GUI/GUI.h"
#include "../GUI/GUIManager.h"

//-----------------------------------------------------------------------------------

uint8 MenuManager::s_fadeTransitionSpeed = 100;
uint8 MenuManager::s_fadeInTransitionSpeed = 20;
uint8 MenuManager::s_fadeOutTransitionSpeed = 20;
uint8 MenuManager::s_slideTransitionSpeed = 60;

//-----------------------------------------------------------------------------------

void MenuManager::Init()
{
	m_currentTypeId = MenuElement::MENUELEMENTTYPE_NONE;
	m_lastTypeId = MenuElement::MENUELEMENTTYPE_NONE;

	m_postProcessing = PostProcessing::GetInstance();

	GUIInit();

	//IwGxLightingEmissive( true );

	CIwResGroup* lpUIResGroup = IwGetResManager()->LoadGroup( UI_GROUP_PATH );

	CreateMenu( MenuElement::MENUELEMENTTYPE_MAIN_MENU );
	CreateMenu( MenuElement::MENUELEMENTTYPE_GAME_GUI );
	CreateMenu( MenuElement::MENUELEMENTTYPE_ABOUT );
	CreateMenu( MenuElement::MENUELEMENTTYPE_OPTIONS );
	CreateMenu( MenuElement::MENUELEMENTTYPE_SOUNDS );
	CreateMenu( MenuElement::MENUELEMENTTYPE_LANGUAGES );
	CreateMenu( MenuElement::MENUELEMENTTYPE_CONTROLS );
	CreateMenu( MenuElement::MENUELEMENTTYPE_SPLASH_SCREEN );
	CreateMenu( MenuElement::MENUELEMENTTYPE_GAME_MODE );
	CreateMenu( MenuElement::MENUELEMENTTYPE_IN_GAME );
	CreateMenu( MenuElement::MENUELEMENTTYPE_STATISTICS );
}

//-----------------------------------------------------------------------------------

void MenuManager::Render()
{
	GetGUIManager()->Render();

	if( m_currentTypeId > MenuElement::MENUELEMENTTYPE_NONE && m_currentTypeId < MenuElement::MENUELEMENTTYPE_COUNT )
	{
		m_menus[m_currentTypeId]->Render();
	}
}

//-----------------------------------------------------------------------------------

void MenuManager::Update( const s3e_uint64_t dt )
{
	const int32 frameTime = static_cast<int32>( dt );
	GetGUIManager()->Update( frameTime );

	if( m_currentTypeId > MenuElement::MENUELEMENTTYPE_NONE && m_currentTypeId < MenuElement::MENUELEMENTTYPE_COUNT )
	{
		m_menus[ m_currentTypeId ]->Update( dt );
	}
}

//-----------------------------------------------------------------------------------

void MenuManager::CreateMenu( const int id )
{
	MenuElement* menu = NULL;

	switch( id )
	{
	case MenuElement::MENUELEMENTTYPE_MAIN_MENU:
		{
			menu = new MainMenu();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_GAME_GUI:
		{
			menu = new MenuGameGui();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_ABOUT:
		{
			menu = new MenuAbout();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_OPTIONS:
		{
			menu = new MenuOptions();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_SOUNDS:
		{
			menu = new MenuSounds();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_LANGUAGES:
		{
			menu = new MenuLanguages();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_CONTROLS:
		{
			menu = new MenuControls();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_SPLASH_SCREEN:
		{
			menu = new MenuSplashScreen();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_GAME_MODE:
		{
			menu = new MenuGameMode();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_IN_GAME:
		{
			menu = new MenuInGame();
		}
		break;
	case MenuElement::MENUELEMENTTYPE_STATISTICS:
		{
			menu = new MenuStatistics();
		}
		break;
	}

	if( menu )
	{
		menu->Init();
		m_menus.push_back( menu );
	}
}

//-----------------------------------------------------------------------------------

void MenuManager::PushMenu( const int id )
{
	if( id > MenuElement::MENUELEMENTTYPE_NONE && id < MenuElement::MENUELEMENTTYPE_COUNT )
	{
		if( m_currentTypeId > MenuElement::MENUELEMENTTYPE_NONE )
		{
			m_menus[m_currentTypeId]->OnHide();
		}

		m_lastTypeId = m_currentTypeId;
		m_currentTypeId = id;
		m_menus[m_currentTypeId]->OnShow();
	}
}

//-----------------------------------------------------------------------------------

void MenuManager::ReplaceMenu( const int id )
{
	if( id > MenuElement::MENUELEMENTTYPE_NONE && id < MenuElement::MENUELEMENTTYPE_COUNT )
	{
		if( m_currentTypeId > MenuElement::MENUELEMENTTYPE_NONE )
		{
			m_menus[m_currentTypeId]->OnHide();
		}

		m_currentTypeId = id;
		m_menus[ m_currentTypeId ]->OnShow();
	}
}

//-----------------------------------------------------------------------------------

void MenuManager::PopMenu()
{
	if( m_currentTypeId > MenuElement::MENUELEMENTTYPE_NONE && m_lastTypeId > MenuElement::MENUELEMENTTYPE_NONE )
	{
		m_menus[m_currentTypeId]->OnHide();
		int temp = m_currentTypeId;
		m_currentTypeId = m_lastTypeId;
		m_lastTypeId = temp;
		m_menus[m_currentTypeId]->OnShow();
	}
}

//-----------------------------------------------------------------------------------

void MenuManager::Remove()
{
	m_postProcessing->FreeInstance();

	for( MenuElementArray::const_iterator it = m_menus.begin(), end = m_menus.end(); it != end; ++it )
	{
		delete ( *it );
	}
	m_menus.clear();

	GUITerminate();
}

//-----------------------------------------------------------------------------------

void MenuManager::ResetTexts()
{
	GUIResetTexts();
}

//-----------------------------------------------------------------------------------

void MenuManager::Fade()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	m_postProcessing->PreRender();
	Render();
	m_postProcessing->PostRender();
	Transitions2D::CaptureEndScreen();
	Transitions2D::Fade( s_fadeTransitionSpeed, false, Transitions2D::FADE_TYPE_IN );
#endif
}

//-----------------------------------------------------------------------------------

void MenuManager::Fadein()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	IwGxSetColClear( 0, 0, 0, 0 );
	IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );
	Transitions2D::CaptureEndScreen();
	Transitions2D::Fade( s_fadeInTransitionSpeed, false, Transitions2D::FADE_TYPE_IN );
#endif
}

//-----------------------------------------------------------------------------------

void MenuManager::Fadeout()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	IwGxSetColClear( 0, 0, 0, 255 );
	IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );
	Transitions2D::CaptureEndScreen();
	Transitions2D::Fade( s_fadeOutTransitionSpeed, true, Transitions2D::FADE_TYPE_OUT );
#endif
}

//-----------------------------------------------------------------------------------

void MenuManager::SildeRight()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	Render();
	Transitions2D::CaptureEndScreen();
	Transitions2D::SlideRight( s_slideTransitionSpeed );
#endif
}

//-----------------------------------------------------------------------------------

void MenuManager::SlideLeft()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	Render();
	Transitions2D::CaptureEndScreen();
	Transitions2D::SlideLeft( s_slideTransitionSpeed );
#endif
}

//-----------------------------------------------------------------------------------

void MenuManager::SlideUp()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	Render();
	Transitions2D::CaptureEndScreen();
	Transitions2D::SlideUp( s_slideTransitionSpeed );
#endif
}

//-----------------------------------------------------------------------------------

void MenuManager::SlideDown()
{
#ifdef USE_TRANSITION_EFFECTS
	Transitions2D::CaptureStartScreen();
	Render();
	Transitions2D::CaptureEndScreen();
	Transitions2D::SlideDown( s_slideTransitionSpeed );
#endif
}