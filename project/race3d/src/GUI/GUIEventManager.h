//-----------------------------------------------------------------------------
// GUIEventManager.h
// Singleton class that deals with registering and transmitting events
//-----------------------------------------------------------------------------

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

// Project includes
#include "GUISingleton.h"

// Maximum number of registerable events
#define MAX_EVENTS	32

//-----------------------------------------------------------------------------
// EventHandler
// Empty class that just allows all event handler classes to share a common
// base class
//-----------------------------------------------------------------------------
class GUIEventHandler
{
public:
	// This virtual method is required to force all derived classes
	// (which must also be derived as "public virtual") to have a correct vtable
	virtual void Dummy() {}
};

//-----------------------------------------------------------------------------
// Event
// Base class defining a particular type of event
//-----------------------------------------------------------------------------
class GUIEvent
{
public:
	virtual bool Execute( GUIEventHandler* apHandler ) =0;
};

//-----------------------------------------------------------------------------
// EventManager
//-----------------------------------------------------------------------------
class GUIEventManager : public GUISingleton<GUIEventManager>
{
public:
	GUIEventManager();

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------

	// Registers an event handler for a particular event type
	void RegisterEvent( const char* apEventName, GUIEventHandler* apHandler );

	// Look through list of registered events to see if the given event type
	// is registered, and if so fire off the event with the supplied data
	void TriggerEvent( const char* apEventName, GUIEvent* apEvent );
	void TriggerEvent( uint32 apEventHash, GUIEvent* apEvent );

private:
	struct GUIEventEntry
	{
		uint32				m_Hash;
		GUIEventHandler*	m_pHandler;
	};

	GUIEventEntry m_Events[MAX_EVENTS];
	uint32 m_NumEvents;
};

// Declare the static singleton access method
GUISINGLETON_GET( GUIEventManager );

#endif

