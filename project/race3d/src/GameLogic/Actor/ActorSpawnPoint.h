#ifndef _ACTORSPAWNPOINT_H_
#define _ACTORSPAWNPOINT_H_

#include <string>

#include "IwGeomFVec3.h"
#include "IwGeomFQuat.h"

//---------------------------------------------------------------------------------------
// ActorSpawnPoint - spawn point information for actor
//---------------------------------------------------------------------------------------
class ActorSpawnPoint
{
public:
	enum ActorSpawnPointType
	{
		ASPT_NONE			= -1,
		ASPT_START_POINT,
		ASPT_SPAWN_POINT,

		ASPT_COUNT
	};

private:
	static const std::string s_actorSpawnPointTypes[ASPT_COUNT];

public:
	ActorSpawnPoint();
	~ActorSpawnPoint();
	ActorSpawnPoint( const ActorSpawnPoint &point );
	ActorSpawnPoint& operator= ( const ActorSpawnPoint &point );

	// Get distance from point
	CIwFVec3 GetDistance( const CIwFVec3 point );

	// Set position
	void SetPosition( const CIwFVec3 pos );

	// Set rotation
	void SetRotation( const CIwFVec3 rot );

	// Get position
	const CIwFVec3& GetPosition() const;

	// Get rotation
	const CIwFVec3& GetRotation() const;

	// Set name
	void SetName( const std::string name );

	// Get name
	const std::string& GetName() const;

public:
	// Get actor spawn point type
	static unsigned int GetActorSpawnPointType( const char* type );

private:
	CIwFVec3	m_position;
	CIwFVec3	m_rotation;
	
	std::string	m_name;
};

//---------------------------------------------------------------------------------------

inline void ActorSpawnPoint::SetPosition( const CIwFVec3 pos )
{
	m_position = pos;
}

//---------------------------------------------------------------------------------------

inline void ActorSpawnPoint::SetRotation( const CIwFVec3 rot )
{
	m_rotation = rot;
}

//---------------------------------------------------------------------------------------

inline const CIwFVec3& ActorSpawnPoint::GetPosition() const
{
	return m_position;
}

//---------------------------------------------------------------------------------------

inline const CIwFVec3& ActorSpawnPoint::GetRotation() const
{
	return m_rotation;
}

//---------------------------------------------------------------------------------------

inline void ActorSpawnPoint::SetName( const std::string name )
{
	m_name = name;
}

//---------------------------------------------------------------------------------------

inline const std::string& ActorSpawnPoint::GetName() const
{
	return m_name;
}

#endif	// _ACTORSPAWNPOINT_H_