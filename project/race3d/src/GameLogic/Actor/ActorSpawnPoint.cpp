#include "ActorSpawnPoint.h"
#include "math.h"

//---------------------------------------------------------------------------------------

const std::string ActorSpawnPoint::s_actorSpawnPointTypes[ASPT_COUNT] = { "StartPoint", "SpawnPoint" };

//---------------------------------------------------------------------------------------

ActorSpawnPoint::ActorSpawnPoint() :
	m_position( CIwFVec3( 0.0f, 0.0f, 0.0f ) ),
	m_rotation( CIwFVec3( 0.0f, 0.0f, 0.0f ) ),
	m_name( "" )
{
}

//---------------------------------------------------------------------------------------

ActorSpawnPoint::~ActorSpawnPoint()
{
}

//---------------------------------------------------------------------------------------

ActorSpawnPoint::ActorSpawnPoint( const ActorSpawnPoint &point )
{
	operator=( point );
}

//---------------------------------------------------------------------------------------

ActorSpawnPoint& ActorSpawnPoint::operator= ( const ActorSpawnPoint &point )
{
	if( this == &point )
	{
		return *this;
	}

	m_position = point.m_position;
	m_rotation = point.m_rotation;

	return *this;
}

//---------------------------------------------------------------------------------------

CIwFVec3 ActorSpawnPoint::GetDistance( const CIwFVec3 point )
{
	CIwFVec3 fabsDistance = CIwFVec3();

	const CIwFVec3 distance = m_position - point;
	fabsDistance.x = static_cast<float>( fabs( distance.x ) );
	fabsDistance.y = static_cast<float>( fabs( distance.y ) );
	fabsDistance.z = static_cast<float>( fabs( distance.z ) );

	return fabsDistance;
}

//---------------------------------------------------------------------------------------

unsigned int ActorSpawnPoint::GetActorSpawnPointType( const char* type )
{
	if( !strcmp( type, "StartPoint" ) )
	{
		return ASPT_START_POINT;
	}
	else if( !strcmp( type, "SpawnPoint" ) )
	{
		return ASPT_SPAWN_POINT;
	}
	else
	{
		return ASPT_NONE;
	}
}