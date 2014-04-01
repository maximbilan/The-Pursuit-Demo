#include "MainMenu.h"
#include "Popup.h"
#include "MenuManager.h"

#include <string>

#include "s3eDevice.h"
#include "IwResManager.h"
#include "IwGraphics.h"

#include "../Utilities/Online/Web/WebManager.h"
#include "../Localization/Localization.h"

//-----------------------------------------------------------------------------------

MainMenu::MainMenu()
{
	m_typeId = MENUELEMENTTYPE_MAIN_MENU;

	GetElementByName<GUIFrame*>( m_frame, "main_menu" );
	m_quitPopup = new Popup( Popup::PT_QUESTION );
	m_quitPopup->SetString( "MM_QUIT_MSG" );

	m_noAvailableFeaturePopup = new Popup( Popup::PT_OK );
	m_noAvailableFeaturePopup->SetString( "NO_AVAILABLE_FEATURE" );

	// Hide facebook and twitter buttons for ios platform
	const int deviceClass = s3eDeviceGetInt( S3E_DEVICE_CLASS );
	if( deviceClass == S3E_DEVICE_CLASS_IPHONE )
	{
		GUIButton* facebookBtn = NULL;
		GetElementByName<GUIButton*>( facebookBtn, "mm_facebook_button" );
		facebookBtn->SetVisible( false );

		GUIButton* twitterBtn = NULL;
		GetElementByName<GUIButton*>( twitterBtn, "mm_twitter_button" );
		twitterBtn->SetVisible( false );
	}

	m_webManager = WebManager::GetInstance();

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );
}

//-----------------------------------------------------------------------------------

MainMenu::~MainMenu()
{
	m_webManager->FreeInstance();
	delete m_quitPopup;
	delete m_noAvailableFeaturePopup;
}

//-----------------------------------------------------------------------------------

void MainMenu::Init()
{
}

//-----------------------------------------------------------------------------------

void MainMenu::Render()
{
#ifdef DEBUG_3D_SCENE
	IwGxLightingOff();

	CIwFVec3 s_Angles = CIwFVec3( 0, -PI, 0 );

	CIwFMat s_ModelMatrix;

	CIwFMat rotX, rotY;
	rotX.SetRotX(s_Angles.x);
	rotY.SetRotY(s_Angles.y);
	s_ModelMatrix.CopyRot(rotY * rotX);
	s_ModelMatrix.SetTrans( CIwFVec3( 0, 0, 1600.0f ) );

	IwGxSetModelMatrix( &s_ModelMatrix );
	m_model->Render();
#endif
}

//-----------------------------------------------------------------------------------

void MainMenu::Update( const s3e_uint64_t dt )
{
}

//-----------------------------------------------------------------------------------

void MainMenu::OnShow()
{
	MenuElement::OnShow();

	m_menuManager->Fade();
#ifdef DEBUG_3D_SCENE
	IwGetResManager()->LoadGroup( "podium.group" );
	CIwResGroup* pGroup = IwGetResManager()->GetGroupNamed( "podium" );
	m_model = static_cast< CIwModel* >( pGroup->GetResNamed( "podium", IW_GRAPHICS_RESTYPE_MODEL ) );

	// Set field of view
	float fov = static_cast< float >( IwGxGetDeviceWidth() / IwGxGetDeviceHeight() );
	float perspMul =  static_cast< float >( IwGxGetDeviceWidth() / ( 2 * tan( fov / 2 ) ) );
	IwGxSetPerspMul( perspMul );

	// Set near and far planes
	IwGxSetFarZNearZ( 2048.0f, 0.01f );

	CIwFMat view;
	view.SetRotZ( PI );
	view.t.y = 128;
	view.t.z = -128;
	IwGxSetViewMatrix( &view );
#endif
}

//-----------------------------------------------------------------------------------

void MainMenu::OnHide()
{
	MenuElement::OnHide();
}

//-----------------------------------------------------------------------------------

bool MainMenu::OnButtonPressed( const GUIButton* apButton )
{
	if( m_quitPopup->IsEnabled() )
	{
		m_quitPopup->OnButtonPressed( apButton );

		return false;
	}

	if( m_noAvailableFeaturePopup->IsEnabled() )
	{
		m_noAvailableFeaturePopup->OnButtonPressed( apButton );

		return false;
	}

	return false;
}

//-----------------------------------------------------------------------------------

bool MainMenu::OnButtonReleased( const GUIButton* apButton )
{
	if( m_quitPopup->IsEnabled() )
	{
		m_quitPopup->OnButtonReleased( apButton );

		if( m_quitPopup->GetButtonStatus() == Popup::PB_RELEASED )
		{
			if( m_quitPopup->GetQuestionResult() == Popup::PQ_YES )
			{
				m_soundManager->PlaySFXUiSelect();
				s3eDeviceRequestQuit();
			}

			if( m_quitPopup->GetQuestionResult() == Popup::PQ_NO )
			{
				m_soundManager->PlaySFXUiSelect();
				m_quitPopup->Hide();
				OnShow();

				return true;
			}

			m_quitPopup->Reset();
		}

		return false;
	}

	if( m_noAvailableFeaturePopup->IsEnabled() )
	{
		m_noAvailableFeaturePopup->OnButtonReleased( apButton );

		if( m_noAvailableFeaturePopup->GetButtonStatus() == Popup::PB_RELEASED )
		{
			m_noAvailableFeaturePopup->Hide();
			OnShow();

			return true;
		}

		return false;
	}

	if( apButton->m_Hash == IwHashString( "mm_startgame_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_GAME_MODE );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mm_options_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_OPTIONS );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mm_statistics_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_STATISTICS );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mm_info_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_ABOUT );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mm_facebook_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		const std::string title = LocaliseFindString( "FB_SHARE_TITLE" );
		const std::string summary = LocaliseFindString( "FB_SHARE_TEXT" );
		m_webManager->ShareFacebook( title.c_str(), summary.c_str() );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mm_twitter_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		const std::string text = LocaliseFindString( "TWITTER_SHARE_TEXT" );
		m_webManager->ShareTwitter( text.c_str() );
		return true;
	}

	if( apButton->m_Hash == IwHashString( "mm_exit_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_quitPopup->Show();
		return true;
	}

	return false;
}