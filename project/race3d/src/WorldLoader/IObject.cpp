#include "IObject.h"

//----------------------------------------------------------------------------------------------

IObject::IObject() :
	m_name( "" ),
	m_modelName( "" ),
	m_groupName( "" ),
	m_entityType( "" ),
	m_collisionType( "" ),
	m_position( CIwFVec3() ),
	m_rotation( CIwFQuat() ),
	m_isVisible( false ),
	m_isEnabled( false ),
	m_mass( 0.0f ),
	m_scale( 0.0f )
{
}

//----------------------------------------------------------------------------------------------

IObject::~IObject()
{
}