#include "GamePlayCountdown.h"

#include "IwGx.h"

#include "../Engine/Font/Font.h"
#include "../Engine/Font/FontManager.h"

//---------------------------------------------------------------------------------

const int		GamePlayCountdown::s_messageCount				= 4;
const char*		GamePlayCountdown::s_messages[s_messageCount]	= { "3", "2", "1", "GO" };
const uint64	GamePlayCountdown::s_messageTime				= 1000;		// 1 sec
const int		GamePlayCountdown::s_rectWidth					= 100;
const int		GamePlayCountdown::s_rectHeight					= 100;

//---------------------------------------------------------------------------------

GamePlayCountdown::GamePlayCountdown() :
	m_isEnabled( false ),
	m_messageIndex( 0 ),
	m_startTime( 0 ),
	m_time( 0 )
{
	m_fontManager = FontManager::GetInstance();
}

//---------------------------------------------------------------------------------

GamePlayCountdown::~GamePlayCountdown()
{
	m_fontManager->FreeInstance();
}

//---------------------------------------------------------------------------------

void GamePlayCountdown::Update()
{
	if( m_isEnabled )
	{
		m_time = s3eTimerGetMs() - m_startTime;
		if( m_time > s_messageTime )
		{
			m_startTime = s3eTimerGetMs();
			++m_messageIndex;

			if( m_messageIndex >= s_messageCount )
			{
				m_isEnabled = false;
				m_messageIndex = 0;
			}
		}
	}
}

//---------------------------------------------------------------------------------

void GamePlayCountdown::Render()
{
	if( m_isEnabled )
	{
		if( m_messageIndex >= 0 && m_messageIndex <= s_messageCount )
		{
			const double coef = static_cast<double>( m_time ) / static_cast<double>( s_messageTime );
			uint8 alpha = 255;
			alpha = ( 1.0f - coef ) > 0.5f ? 255 : static_cast<uint8>( 255 * ( 1.0f - coef ) );

			CIwColour color;
			color.Set( 255, 255, 255, alpha );

			const int16 midx = static_cast<int16>( IwGxGetScreenWidth() * 0.5f - s_rectWidth * 0.5 );
			const int16 midy = static_cast<int16>( IwGxGetScreenHeight() * 0.5f - s_rectHeight * 0.5 );

			m_fontManager->DrawText( s_messages[m_messageIndex], midx, midy, s_rectWidth, s_rectHeight, Font::FN_TAHOMA, Font::FS_LARGE, color, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE );
		}
	}
}