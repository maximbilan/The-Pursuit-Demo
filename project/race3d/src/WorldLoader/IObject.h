#ifndef _IOBJECT_H_
#define _IOBJECT_H_

#include "IwString.h"
#include "IwGeom.h"
#include "IwGeomFQuat.h"

//------------------------------------------------------------------------------------------------
// IObject - base class for parsing from .world files
//------------------------------------------------------------------------------------------------
class IObject
{
public:
	IObject();
	virtual ~IObject();

public:
	// Get object name
	const CIwStringL& GetName() const;

	// Get model name
	const CIwStringL& GetModelName() const;

	// Get group name
	const CIwStringL& GetGroupName() const;
	
	// Get entity type
	const CIwStringL& GetEntityType() const;
	
	// Get collision type
	const CIwStringL& GetCollisionType() const;

	// Get position
	const CIwFVec3& GetPosition() const;
	
	// Get rotation
	const CIwFQuat& GetRotation() const;

	// Check is visible
	bool IsVisible() const;

	// Check is enabled
	bool IsEnabled() const;

	// Get mass
	float GetMass() const;

	// Get scale
	float GetScale() const;

protected:
	CIwStringL	m_name;
	CIwStringL	m_modelName;
	CIwStringL	m_groupName;
	CIwStringL	m_entityType;
	CIwStringL	m_collisionType;
		
	CIwFVec3	m_position;
	CIwFQuat	m_rotation;

	bool		m_isVisible;
	bool		m_isEnabled;

	float		m_mass;
	float		m_scale;
};

//----------------------------------------------------------------------------------------------

inline const CIwStringL& IObject::GetName() const
{
	return m_name;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& IObject::GetModelName() const
{
	return m_modelName;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& IObject::GetGroupName() const
{
	return m_groupName;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& IObject::GetEntityType() const
{
	return m_entityType;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& IObject::GetCollisionType() const
{
	return m_collisionType;
}

//----------------------------------------------------------------------------------------------

inline const CIwFVec3& IObject::GetPosition() const
{
	return m_position;
}

//----------------------------------------------------------------------------------------------

inline const CIwFQuat& IObject::GetRotation() const
{
	return m_rotation;
}

//----------------------------------------------------------------------------------------------

inline bool IObject::IsVisible() const
{
	return m_isVisible;
}

//----------------------------------------------------------------------------------------------

inline bool IObject::IsEnabled() const
{
	return m_isEnabled;
}

//----------------------------------------------------------------------------------------------

inline float IObject::GetMass() const
{
	return m_mass;
}

//----------------------------------------------------------------------------------------------

inline float IObject::GetScale() const
{
	return m_scale;
}

#endif	// _IOBJECT_H_