#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <string>
#include <set>
#include <map>

#include <s3eTypes.h>

#include "IwGeom.h"

#include "../../GameObjects/Trigger.h"

class ActorSpawnPoint;
class Car;
class Trigger;

//-----------------------------------------------------------------------------------------
struct ActorInTrigger
{
	unsigned int	triggerId;
	int				triggerState;
};

//-----------------------------------------------------------------------------------------

typedef std::map<unsigned int,int> ActorInTriggerMap;
typedef std::pair<unsigned int,int> ActorInTriggerPair;

//-----------------------------------------------------------------------------------------
// Actor - base class of the car controller
//-----------------------------------------------------------------------------------------
class Actor
{
public:
	enum EActorType
	{
		ACTOR_TYPE_NONE	= -1,
		ACTOR_TYPE_PLAYER,
		ACTOR_TYPE_NPC,

		ACTOR_TYPE_COUNT
	};

private:
	static const std::string s_actorTypes[ACTOR_TYPE_COUNT];

public:
	Actor( const unsigned int carColor = 0 );
	~Actor();

	// Get actor type
	static unsigned int GetActorType( const char* type );

	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Render
	void Render();

	// Sets start position for the actor, this method should be run before actor initialization
	void SetStartPosition( ActorSpawnPoint* startPoint );

	// Get car
	Car* GetCar() const;

	// Checks if actor intersects trigger
	void IntersectTrigger( Trigger* trigger );

	// Add trigger to the map
	void AddTrigger( const unsigned int triggerId );

	// Sets trigger state by trigger id
	void SetTriggerState( const unsigned int triggerId, const int triggerState );

	// Returns trigger state by trigger id
	int GetTriggerState( const unsigned int triggerId ) const;

protected:
	int					m_type;
	Car*				m_car;
	ActorSpawnPoint*	m_startPoint;
	ActorInTriggerMap	m_triggers;
};

//---------------------------------------------------------------------------------------

inline Car* Actor::GetCar() const
{
	return m_car;
}

//---------------------------------------------------------------------------------------

inline void Actor::SetStartPosition( ActorSpawnPoint* startPoint )
{
	m_startPoint = startPoint;
}

//---------------------------------------------------------------------------------------

inline void Actor::AddTrigger( const unsigned int triggerId )
{
	m_triggers.insert( ActorInTriggerPair( triggerId, Trigger::OBJECT_OUT ) );
}

//---------------------------------------------------------------------------------------

inline void Actor::SetTriggerState( const unsigned int triggerId, const int triggerState )
{
	ActorInTriggerMap::iterator findIt = m_triggers.find( triggerId );

	if( findIt != m_triggers.end() )
	{
		(*findIt).second = triggerState;
	}
}

//---------------------------------------------------------------------------------------

inline int Actor::GetTriggerState( const unsigned int triggerId ) const
{
	int triggerState = Trigger::OBJECT_OUT;

	ActorInTriggerMap::const_iterator findIt = m_triggers.find( triggerId );

	if( findIt != m_triggers.end() )
	{
		triggerState = (*findIt).second;
	}

	return triggerState;
}

#endif	// _ACTOR_H_