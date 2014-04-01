#include "Entity.h"

//------------------------------------------------------------

unsigned int Entity::s_idTotal = 0;

//------------------------------------------------------------

Entity::Entity() :
	m_id( ++s_idTotal ),
	m_name( "" ),
	m_type( ENTITY_NONE ),
	m_isEnabled( false )
{
}

//------------------------------------------------------------

Entity::~Entity()
{
}

//------------------------------------------------------------

unsigned int Entity::GetEntityType( const char* name )
{
	if( !strcmp( name, "Terrain" ) )
	{
		return ENTITY_TERRAIN;
	}
	else if( !strcmp( name, "Decoration" ) )
	{
		return ENTITY_DECORATION;
	}
	else if( !strcmp( name, "Car" ) )
	{
		return ENTITY_CAR;
	}
	else if( !strcmp( name, "Trigger" ) )
	{
		return ENTITY_TRIGGER;
	}
	else
	{
		return ENTITY_NONE;
	}
}