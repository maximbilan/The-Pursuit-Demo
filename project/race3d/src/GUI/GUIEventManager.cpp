//-----------------------------------------------------------------------------
// GUIEventManager.cpp
// Singleton class that deals with registering and transmitting events
//-----------------------------------------------------------------------------

#include "GUIEventManager.h"

// Library includes
#include "IwHashString.h"

//-----------------------------------------------------------------------------
// EventManager
//-----------------------------------------------------------------------------
GUIEventManager::GUIEventManager() :
	m_NumEvents( 0 )
{
}

//-----------------------------------------------------------------------------
// Registers an event handler for a particular event type
//-----------------------------------------------------------------------------
void GUIEventManager::RegisterEvent( const char* apEventName, GUIEventHandler* apHandler )
{
	IwAssertMsg( GUI, m_NumEvents < MAX_EVENTS, ( "Attempting to register too many event handlers.  Increase size of MAX_EVENTS define" ) );

	m_Events[m_NumEvents].m_Hash = IwHashString( apEventName );
	m_Events[m_NumEvents].m_pHandler = apHandler;
	m_NumEvents++;
}

//-----------------------------------------------------------------------------
// Look through list of registered events to see if the given event type
// is registered, and if so fire off the event with the supplied data
//-----------------------------------------------------------------------------
void GUIEventManager::TriggerEvent( const char* apEventName, GUIEvent* apEvent )
{
	TriggerEvent( IwHashString( apEventName ), apEvent );
}

void GUIEventManager::TriggerEvent( uint32 apEventHash, GUIEvent* apEvent )
{
	for( uint32 i = 0; i < m_NumEvents; i++ )
	{
		if( m_Events[i].m_Hash == apEventHash )
		{
			// If Execute method returns true, the event has been handled
			if( apEvent->Execute( m_Events[i].m_pHandler ) )
				return;
		}
	}
}