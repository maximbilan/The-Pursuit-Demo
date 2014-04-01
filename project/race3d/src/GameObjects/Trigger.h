#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include "GameEntity.h"

#include <string>

class GameObject;
class Actor;

//------------------------------------------------------------------------------------------------
// Trigger - zone, which collides with objects, and it runs some events (onEnter, onLeave)
//------------------------------------------------------------------------------------------------
class Trigger : public GameEntity
{
public:
	enum TriggerState
	{
		OBJECT_OUT = -1,
		OBJECT_ENTER,
		OBJECT_IN,
		OBJECT_LEAVE
	};

public:
	Trigger();
	virtual ~Trigger();

	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Physics update
	void UpdatePhysics( const s3e_uint64_t dt );

	// Render
	void Render();

	// Get amount of polygons
	unsigned int GetPolyCount();

	// Checks the object intersects the trigger
	bool IsIntersect( GameEntity* obj );

	// Checks if actor intersect this trigger
	void CheckIntersect( Actor* actor );

	// Event when the object enters into trigger
	void OnEnter();

	// Event when the object leaves the trigger
	void OnLeave();

	// Sets OnEnter script name
	void SetOnEnterScriptName( const char* scriptName );

	// Returns OnEnter script name
	const std::string& GetOnEnterScriptName() const;

	// Sets OnLeave sript name
	void SetOnLeaveScriptName( const char* scriptName );

	// Returns OnLeave script name
	const std::string& GetOnLeaveScriptName() const;

private:
	// Runs lua level script from GamePlay state
	void RunLevelScript( const std::string functionName );

private:
	std::string	m_onEnterScriptName;
	std::string	m_onLeaveScriptName;
};

//------------------------------------------------------------------------------------------------

inline void Trigger::SetOnEnterScriptName( const char* scriptName )
{
	m_onEnterScriptName = scriptName;
}

//------------------------------------------------------------------------------------------------

inline const std::string& Trigger::GetOnEnterScriptName() const
{
	return m_onEnterScriptName;
}

//------------------------------------------------------------------------------------------------

inline void Trigger::SetOnLeaveScriptName( const char* scriptName )
{
	m_onLeaveScriptName = scriptName;
}

//------------------------------------------------------------------------------------------------

inline const std::string& Trigger::GetOnLeaveScriptName() const
{
	return m_onLeaveScriptName;
}

#endif	// _TRIGGER_H_