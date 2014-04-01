#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "s3eTypes.h"
#include "IwGeomFVec3.h"
#include "IwGeomFMat.h"
#include "IwGeomFQuat.h"
#include "IwGeomFSphere.h"
#include "IwModel.h"
#include "../Common/Singleton.h"
#include "LinearMath/btAlignedObjectArray.h"

// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
#define USE_MOTIONSTATE 1

struct btWheelInfo;
struct btCollisionAlgorithmCreateFunc;

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btDynamicsWorld;
class btRigidBody;
class btQuaternion;
class btVector3;
class btActionInterface;
class btActionInterface;
class btTransform;

//-----------------------------------------------------------------------------------------
// Physics
//-----------------------------------------------------------------------------------------
class Physics : public Singleton<Physics>
{
public:
	static const double BPU_PI;
	static const double BPU_PI2;
	static const double BPU_360_PI2;
	static const double BPU_PI_180;
	static const double BPU_180_PI;

	static const btVector3	s_gravity;
	static const int		s_maxSubSteps;

public:
	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Remove the instance
	void Remove();

	// Add object
	void AddObject( btCollisionShape* shape, btRigidBody* body );

	// Add rigid body
	void AddRigidBody( btRigidBody* body );

	// Add collision shape
	void AddCollisionShape( btCollisionShape* shape );

	// Add vehicle
	void AddVehicle( btActionInterface* vehicle );

	// Create rigid body
	btRigidBody* CreateRigidBody( const float mass, const CIwFVec3 position, const CIwFVec3 rotation, btCollisionShape* shape );

	// Quaternion to euler geometric convertion (from btQuaternion to btVector3)
	void QuaternionToEuler( const btQuaternion &TQuat, btVector3 &TEuler );

	// Quaternion to euler geometric convertion (from btQuaternion to CIwFVec3)
	void QuaternionToEuler( const btQuaternion &TQuat, CIwFVec3 &TEuler );

	// Quaternion to euler geometric convertion (from CIwFQuat to btVector3)
	void QuaternionToEuler( const CIwFQuat& TQuat, btVector3 &TEuler );

	// Quaternion to euler geometric convertion (from CIwFQuat to CIwFVec3)
	void QuaternionToEuler( const CIwFQuat& TQuat, CIwFVec3 &TEuler );

	// Euler to quaternion geometric convertion (from CIwFVec3 to btQuaternion)
	void EulerXYZToQuaternion( const CIwFVec3 &TEuler, btQuaternion &TQuat );

	// Get matrix from rigid body
	CIwFMat GetMatrixFromRigidBody( const btRigidBody* b );

	// Get bounding box from rigid body
	CIwFBBox GetBBoxFromRigidBody( const btRigidBody* b );

	// Get bounding sphere from rigid body
	CIwFSphere GetBSphereFromRigidBody( const btRigidBody* b );

	// Get matrix from wheel information
	CIwFMat GetMatrixFromWheelInfo( const btWheelInfo w, const btRigidBody* b );

	// Get matrix from static body
	CIwFMat GetMatrixFromStaticdBody( const CIwFVec3 pos, const CIwFVec3 rot );

	// Get rotation from rigid body
	CIwFVec3 GetRotationFromRigidBody( const btRigidBody* b );

	// Get body extent
	CIwFVec3 GetBodyExtent( const btRigidBody* body );

	// Get body extent
	CIwFVec3 GetBodyExtent( const CIwFBBox bbox );

	// Build collision
	static void BuildCollision( const char* pUserString, CIwModel* pModel );

	// Get dynamic world
	btDynamicsWorld* GetDynamicWorld() const;

private:
	// Quaternion to euler geometric conversion
	void QuaternionToEuler( float w, float x, float y, float z, float ( &result )[3] );

protected:
	Physics() {}
	friend class Singleton<Physics>;

private:
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface*					m_broadphase;
	btCollisionDispatcher*					m_dispatcher;
	btConstraintSolver*						m_solver;
	btDefaultCollisionConfiguration*		m_collisionConfiguration;
	btDynamicsWorld*						m_dynamicsWorld;
};

//---------------------------------------------------------------------------------------

inline btDynamicsWorld* Physics::GetDynamicWorld() const
{
	return m_dynamicsWorld;
}

#endif	// _PHYSICS_H_