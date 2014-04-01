#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include <string>

#include "Entity.h"
#include "Renderable.h"
#include "Physical.h"

//-------------------------------------------------------------------------------------
// GameEntity - base class for the game entities, has render and physics
//-------------------------------------------------------------------------------------
class GameEntity : public Entity, public Renderable, public Physical
{
public:
	// Load binary data
	static void LoadData( const char* groupName );

	// Destroy data
	static void DestroyData();

public:
	GameEntity();
	virtual ~GameEntity();

	// Initialization of the entity
	virtual void Init() = 0;

	// Update
	virtual void Update( const s3e_uint64_t dt ) = 0;

	// Physics update
	virtual void UpdatePhysics( const s3e_uint64_t dt ) = 0;

	// Render
	virtual void Render() = 0;

	// Get amount of polygons
	virtual unsigned int GetPolyCount() = 0;

	// Check is inited
	bool IsInited() const;

	// Get position
	const CIwFVec3& GetPosition() const;

	// Set position
	void SetPosition( const CIwFVec3 position );

	// Get rotation
	const CIwFVec3& GetRotation() const;

	// Set rotation from vector
	void SetRotation( const CIwFVec3 rotation );

	// Set rotation from quaternion
	void SetRotation( const CIwFQuat quat );

	// Get scaling
	const CIwFVec3& GetScale() const;

	// Set scaling
	void SetScale( const CIwFVec3 scale );

private:
	// Load group with resources
	static void LoadGroup( const std::string& groupName );

	// Build resources
	static void BuildResources();

protected:
	static CIwArray<std::string> s_loadedGroups;

protected:
	bool			m_isInited;

	CIwFVec3		m_position;
	CIwFVec3		m_rotation;
	CIwFVec3		m_scale;
};

//-------------------------------------------------------------------------------------

inline bool GameEntity::IsInited() const
{
	return m_isInited;
}

//-------------------------------------------------------------------------------------

inline const CIwFVec3& GameEntity::GetPosition() const
{
	return m_position;
}

//-------------------------------------------------------------------------------------

inline void GameEntity::SetPosition( const CIwFVec3 position )
{
	if( m_isDynamic && m_isInited )
	{
		const btVector3 pos( btScalar( position.x ), btScalar( position.y ), btScalar( position.z ) );
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin( pos );
		const btQuaternion q = trans.getRotation();
		trans.setRotation( q );
		m_rigidBody->setWorldTransform( trans );
	}

	m_position = position;
}

//-------------------------------------------------------------------------------------

inline const CIwFVec3& GameEntity::GetRotation() const
{
	return m_rotation;
}

//-------------------------------------------------------------------------------------

inline void GameEntity::SetRotation( const CIwFVec3 rotation )
{
	if( m_isDynamic && m_isInited )
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin( m_rigidBody->getWorldTransform().getOrigin() );

		btQuaternion q;
		const CIwFVec3 rot = rotation * static_cast<float>( Physics::BPU_PI_180 );
		m_physics->EulerXYZToQuaternion( rot, q );
		trans.setRotation( q ); 

		m_rigidBody->setWorldTransform( trans );
	}

	m_rotation = rotation;
}

//-------------------------------------------------------------------------------------

inline void GameEntity::SetRotation( const CIwFQuat quat )
{
	CIwFVec3 rot;
	m_physics->QuaternionToEuler( quat, rot );
	rot *= static_cast<float>( m_physics->BPU_180_PI );

	if( m_isDynamic && m_isInited )
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin( m_rigidBody->getWorldTransform().getOrigin() );

		const btQuaternion q( quat.x, quat.y, quat.z, quat.s );
		trans.setRotation( q ); 

		m_rigidBody->setWorldTransform( trans );
	}
	
	m_rotation = rot;
}

//-------------------------------------------------------------------------------------

inline const CIwFVec3& GameEntity::GetScale() const
{
	return m_scale;
}

//-------------------------------------------------------------------------------------

inline void GameEntity::SetScale( const CIwFVec3 scale )
{
	m_scale = scale;
}

#endif	// _GAME_ENTITY_H_