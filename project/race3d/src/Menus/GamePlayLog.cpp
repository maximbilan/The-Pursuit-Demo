#include "GamePlayLog.h"
#include "GamePlayLogObj.h"

//-----------------------------------------------------------------------------------

const int		GamePlayLog::s_messageAmount	= 5;
const uint64	GamePlayLog::s_messageTime		= 2000;		// 2 sec
const int		GamePlayLog::s_xRenderPos		= 100;
const int		GamePlayLog::s_yRenderPos		= 10;
const int		GamePlayLog::s_yRenderOffset	= 50;

//-----------------------------------------------------------------------------------

GamePlayLog::GamePlayLog()
{
}

//-----------------------------------------------------------------------------------

GamePlayLog::~GamePlayLog()
{
}

//-----------------------------------------------------------------------------------

void GamePlayLog::Render()
{
	if( !m_currentMsg.empty() )
	{
		int x = s_xRenderPos;
		int y = s_yRenderPos;
		GPLogVector::const_iterator it, end;
		for( it = m_currentMsg.begin(), end = m_currentMsg.end(); it != end; ++it )
		{
			( *it )->Render( x, y );
			y += s_yRenderOffset;
		}
	}
}

//-----------------------------------------------------------------------------------

void GamePlayLog::Update()
{
	if( !m_msgQueue.empty() )
	{
		const int needSize = s_messageAmount - m_currentMsg.size();
		const int msgQueueSize = m_msgQueue.size();
		const int size = msgQueueSize <= needSize ? msgQueueSize : needSize;

		for( int i = 0; i < size; ++i )
		{
			GamePlayLogObj* obj = m_msgQueue.front();
			obj->Enable();
			m_currentMsg.push_back( obj );
			m_msgQueue.pop();
		}
	}

	if( !m_currentMsg.empty() )
	{
		GPLogVector::iterator it;
		for( it = m_currentMsg.begin(); it != m_currentMsg.end(); )
		{
			if( ( *it )->IsEnabled() )
			{
				( *it )->Update();
				it++;
			}
			else
			{
				delete ( *it );
				it = m_currentMsg.erase( it );
			}
		}
	}
}

//-----------------------------------------------------------------------------------

void GamePlayLog::AddMessage( const std::string text )
{
	GamePlayLogObj* obj = new GamePlayLogObj( text, s_messageTime );
	m_msgQueue.push( obj );
}