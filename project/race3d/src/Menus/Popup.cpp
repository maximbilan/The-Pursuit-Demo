#include "Popup.h"

#include "../GUI/GUI.h"
#include "../GUI/GUIManager.h"
#include "../GUI/GUIFrame.h"
#include "../GUI/GUIButton.h"
#include "../GUI/GUILabel.h"

//---------------------------------------------------------------------------------

Popup::Popup( const unsigned int type ) :
	m_questionResult( 0 ),
	m_buttonStatus( 0 ),
	m_isEnabled( false ),
	m_type( type )
{
	switch( m_type )
	{
	case PT_QUESTION:
		{
			m_frame = static_cast<GUIFrame*>( GetGUIManager()->FindElementByName( "popup_yes_no" ) );
			m_label = static_cast<GUILabel*>( m_frame->FindChildByName( "popup_question" ) );
		}
		break;

	case PT_OK:
		{
			m_frame = static_cast<GUIFrame*>( GetGUIManager()->FindElementByName( "popup_ok" ) );
			m_label = static_cast<GUILabel*>( m_frame->FindChildByName( "popup_text" ) );
		}
		break;
	}

	m_frame->SetEnabled( false );
}

//---------------------------------------------------------------------------------

Popup::~Popup()
{
}

//---------------------------------------------------------------------------------

void Popup::OnButtonPressed( const GUIButton* apButton )
{
	switch( m_type )
	{
	case PT_QUESTION:
		{
			if( apButton->m_Hash == IwHashString( "popup_yes" ) )
			{
				m_buttonStatus = PB_PRESSED;
				m_questionResult = PQ_YES;
			}

			if( apButton->m_Hash == IwHashString( "popup_no" ) )
			{
				m_buttonStatus = PB_PRESSED;
				m_questionResult = PQ_NO;
			}
		}
		break;

	case PT_OK:
		{
			if( apButton->m_Hash == IwHashString( "popup_ok" ) )
			{
				m_buttonStatus = PB_PRESSED;
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------------

void Popup::OnButtonReleased( const GUIButton* apButton )
{
	switch( m_type )
	{
	case PT_QUESTION:
		{
			if( apButton->m_Hash == IwHashString( "popup_yes" ) )
			{
				m_buttonStatus = PB_RELEASED;
				m_questionResult = PQ_YES;
			}

			if( apButton->m_Hash == IwHashString( "popup_no" ) )
			{
				m_buttonStatus = PB_RELEASED;
				m_questionResult = PQ_NO;
			}
		}
		break;

	case PT_OK:
		{
			if( apButton->m_Hash == IwHashString( "popup_ok" ) )
			{
				m_buttonStatus = PB_RELEASED;
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------------

void Popup::Reset()
{
	m_buttonStatus = PB_NONE;
	m_questionResult = PQ_NONE;
}

//---------------------------------------------------------------------------------

void Popup::Show()
{
	Reset();

	m_isEnabled = true;
	m_frame->SetEnabled( m_isEnabled );

	GetGUIManager()->AddFrame( m_frame );
}

//---------------------------------------------------------------------------------

void Popup::Hide()
{
	Reset();

	m_isEnabled = false;
	m_frame->SetEnabled( m_isEnabled );
}

//---------------------------------------------------------------------------------

void Popup::SetString( const char* str )
{
	m_label->SetString( str );
}