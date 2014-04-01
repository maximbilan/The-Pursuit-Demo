#include "GamePlayLogObj.h"

#include "../Engine/Font/Font.h"
#include "../Engine/Font/FontManager.h"

//-----------------------------------------------------------------------------------

const int GamePlayLogObj::s_rectWidth	= 300;
const int GamePlayLogObj::s_rectHeight	= 100;

//-----------------------------------------------------------------------------------

GamePlayLogObj::GamePlayLogObj() :
	m_isEnabled( false ),
	m_type( 0 ),
	m_text( "" ),
	m_time( 0 ),
	m_startTime( 0 )
{
	m_fontManager = FontManager::GetInstance();
}

//-----------------------------------------------------------------------------------

GamePlayLogObj::GamePlayLogObj( const std::string text, const uint64 time ) :
	m_isEnabled( false ),
	m_type( GL_MESSAGE_TYPE_TEXT ),
	m_text( text ),
	m_time( time ),
	m_startTime( 0 )
{
	m_fontManager = FontManager::GetInstance();
}

//-----------------------------------------------------------------------------------

GamePlayLogObj::~GamePlayLogObj()
{
	m_fontManager->FreeInstance();
}

//-----------------------------------------------------------------------------------

void GamePlayLogObj::Update()
{
	if( s3eTimerGetMs() - m_startTime > m_time )
	{
		m_isEnabled = false;
	}
}

//-----------------------------------------------------------------------------------

void GamePlayLogObj::Render( const int x, const int y )
{
	if( m_isEnabled )
	{
		switch( m_type )
		{
		case GL_MESSAGE_TYPE_TEXT:
			{
				CIwColour color;
				color.Set( 255, 255, 255, 255 );
				m_fontManager->DrawText( m_text.c_str(), x, y, s_rectWidth, s_rectHeight, Font::FN_TAHOMA, Font::FS_MEDIUM, color, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE );
			}
			break;
		}
	}
}