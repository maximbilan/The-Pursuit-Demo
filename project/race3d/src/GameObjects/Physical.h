#ifndef _PHYSICAL_H_
#define _PHYSICAL_H_

#include "IwGeom.h"
#include "IwGeomFBBox.h"

#include "btBulletDynamicsCommon.h"
#include "../Engine/Physics.h"

//------------------------------------------------------------
// Physical - interface for physical entities
//------------------------------------------------------------
class Physical
{
public:
	enum
	{
		PHYSICS_COLLISION_NONE		= -1,
		PHYSICS_COLLISION_BOX,
		PHYSICS_COLLISION_MESH,
		PHYSICS_COLLISION_SPHERE
	};

public:
	Physical();
	virtual ~Physical();

	// Update physics
	virtual void UpdatePhysics( const s3e_uint64_t dt ) = 0;

	// Check if is the object dynamical
	bool IsDynamic() const;

	// Set dynamic
	void SetDynamic( const bool isDynamic );

	// Get mass
	float GetMass() const;

	// Set mass
	void SetMass( const float mass );

	// Get rigid body
	btRigidBody* GetRigidBody() const;

	// Get bounding box
	const CIwFBBox& GetBBox() const;

	// Get bounding sphere
	const CIwFSphere& GetBSphere() const;

protected:
	bool				m_isDynamic;
	int					m_collisionType;

	btCollisionShape*	m_collisionShape;
	btScalar			m_mass;
	btRigidBody*		m_rigidBody;
	CIwFBBox			m_bbox;
	CIwFSphere			m_bsphere;

	Physics*			m_physics;
};

//-----------------------------------------------------------

inline bool Physical::IsDynamic() const
{
	return m_isDynamic;
}

//-----------------------------------------------------------

inline void Physical::SetDynamic( const bool isDynamic )
{
	m_isDynamic = isDynamic;
}

//-----------------------------------------------------------

inline float Physical::GetMass() const
{
	return m_mass;
}

//-----------------------------------------------------------

inline void  Physical::SetMass( const float mass )
{
	m_mass = mass;
}

//-----------------------------------------------------------

inline btRigidBody* Physical::GetRigidBody() const
{
	return m_rigidBody;
}

//-----------------------------------------------------------

inline const CIwFBBox& Physical::GetBBox() const
{
	return m_bbox;
}

//-----------------------------------------------------------

inline const CIwFSphere& Physical::GetBSphere() const
{
	return m_bsphere;
}

#endif	// _PHYSICAL_H_