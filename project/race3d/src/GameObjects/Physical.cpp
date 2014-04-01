#include "Physical.h"

//-----------------------------------------------------------

Physical::Physical() :
	m_isDynamic( false ),
	m_collisionType( 0 ),
	m_collisionShape( NULL ),
	m_mass( 0.0f ),
	m_rigidBody( NULL )
{
	m_physics = Physics::GetInstance();
}

//-----------------------------------------------------------

Physical::~Physical()
{
	m_physics->FreeInstance();
}