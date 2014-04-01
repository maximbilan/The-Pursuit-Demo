#include "Physics.h"

#include <limits>

#include "IwResManager.h"
#include "IwGeomCore.h"

#include "btBulletDynamicsCommon.h"
#include "../Engine/Collision.h"
#include "../game.h"

//---------------------------------------------------------------------------------------

const double Physics::BPU_PI = PI;
const double Physics::BPU_PI2 = PI / 2.;
const double Physics::BPU_360_PI2 = 360. / ( BPU_PI2 );
const double Physics::BPU_PI_180 = BPU_PI / 180.;
const double Physics::BPU_180_PI = 180. / BPU_PI;

//---------------------------------------------------------------------------------------

const btVector3 Physics::s_gravity = btVector3( 0.0f, -100.0f, 0.0f );
const int		Physics::s_maxSubSteps = 6;

//---------------------------------------------------------------------------------------

void Physics::Init()
{
	/// Collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	
	/// Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new btCollisionDispatcher( m_collisionConfiguration );

	m_broadphase = new btDbvtBroadphase();

	/// The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;

	m_dynamicsWorld = new btDiscreteDynamicsWorld( m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration );
	m_dynamicsWorld->setGravity( s_gravity );
}

//-------------------------------------------------------------------------------------

void Physics::Update( const s3e_uint64_t dt )
{
	if( m_dynamicsWorld )
	{
		m_dynamicsWorld->stepSimulation( Game::s_msPerFrame, s_maxSubSteps ) ;
	}
}

//-------------------------------------------------------------------------------------

void Physics::Remove()
{
	// Cleanup in the reverse order of creation/initialization

	// Remove the rigidbodies from the dynamics world and delete them
	for( int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; --i )
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast( obj );
		if( body && body->getMotionState() )
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	// Delete collision shapes
	for( int j = 0; j < m_collisionShapes.size(); ++j )
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}

	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfiguration;
}

//-------------------------------------------------------------------------------------

void Physics::AddObject( btCollisionShape* shape, btRigidBody* body )
{
	if( shape )
	{
		m_collisionShapes.push_back( shape );
	}

	if( body )
	{
		m_dynamicsWorld->addRigidBody( body );
	}
}

//-------------------------------------------------------------------------------------

void Physics::AddRigidBody( btRigidBody* body )
{
	if( body )
	{
		m_dynamicsWorld->addRigidBody( body );
	}
}

//-------------------------------------------------------------------------------------

void Physics::AddCollisionShape( btCollisionShape* shape )
{
	if( shape )
	{
		m_collisionShapes.push_back( shape );
	}
}

//-------------------------------------------------------------------------------------

void Physics::AddVehicle( btActionInterface* vehicle )
{
	m_dynamicsWorld->addVehicle( vehicle );
}

//-------------------------------------------------------------------------------------

btRigidBody* Physics::CreateRigidBody( const float mass, const CIwFVec3 position, const CIwFVec3 rotation, btCollisionShape* shape )
{
	btRigidBody* rigidBody;
	btTransform startTransform;

	startTransform.setIdentity();
	startTransform.setOrigin( btVector3( position.x, position.y, position.z ) );
	
	btQuaternion q;
	CIwFVec3 rot = rotation * static_cast<float>( BPU_PI_180 );
	EulerXYZToQuaternion( rot, q );
	startTransform.setRotation( q );

	bool isDynamic = ( mass != 0.f );	// Rigidbody is dynamic if and only if mass is non zero, otherwise static

	btVector3 localInertia( 0, 0, 0 );
	if( isDynamic )
	{
		shape->calculateLocalInertia( mass, localInertia );
	}

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* motionState = new btDefaultMotionState( startTransform );
	btRigidBody::btRigidBodyConstructionInfo rInfo( mass, motionState, shape, localInertia );
	rigidBody = new btRigidBody( rInfo );
#else
	rigidBody = new btRigidBody( mass, 0, shape, localInertia );
	rigidBody->setWorldTransform( startTransform );
#endif

	m_dynamicsWorld->addRigidBody( rigidBody );

	return rigidBody;
}

//-------------------------------------------------------------------------------------

void Physics::QuaternionToEuler( float w, float x, float y, float z, float ( &result )[3] )
{	
	const double sqx = x * x;
	const double sqy = y * y;
	const double sqz = z * z;

	const float valueZY = btScalar( 2.0 * ( z * w + x * y ) );
	const float valueZX = btScalar( 1.0 - 2.0 * ( sqy + sqz ) );
	const float valueXY = btScalar( 2.0 * ( x * w + y * z ) );
	const float valueXX = btScalar( 1.0 - 2.0 * ( sqx + sqy ) );
	float valueY = btScalar( 2.0 * ( -x * z + y * w ) );

	if( valueY > 1.0f )
	{
		valueY = 1.0f;
	}
	else
	{
		if( valueY < -1.0f )
		{
			valueY = -1.0f;
		}
	}

	result[0] = atan2f( valueXY, valueXX );
	result[1] = asinf( valueY );
	result[2] = atan2f( valueZY, valueZX );
}

//-------------------------------------------------------------------------------------

void Physics::QuaternionToEuler( const btQuaternion &TQuat, btVector3 &TEuler )
{
	float a[3];

	const float w = TQuat.getW();
	const float x = TQuat.getX();
	const float y = TQuat.getY();
	const float z = TQuat.getZ();
	
	QuaternionToEuler( w, x, y, z, a );

	TEuler.setX( a[0] );
	TEuler.setY( a[1] );
	TEuler.setZ( a[2] );
}

//-------------------------------------------------------------------------------------

void Physics::QuaternionToEuler( const btQuaternion &TQuat, CIwFVec3 &TEuler )
{
	float a[3];

	const float w = TQuat.getW();
	const float x = TQuat.getX();
	const float y = TQuat.getY();
	const float z = TQuat.getZ();

	QuaternionToEuler( w, x, y, z, a );

	// heading
	TEuler.z = a[2];
	// bank
	TEuler.x = a[0];
	// attitude
	TEuler.y = a[1];
}

//-------------------------------------------------------------------------------------

void Physics::QuaternionToEuler( const CIwFQuat& TQuat, btVector3 &TEuler )
{
	float a[3];

	const float w = TQuat.s;
	const float x = TQuat.x;
	const float y = TQuat.y;
	const float z = TQuat.z;

	QuaternionToEuler( w, x, y, z, a );

	TEuler.setX( a[0] );
	TEuler.setY( a[1] );
	TEuler.setZ( a[2] );
}

//-------------------------------------------------------------------------------------

void Physics::QuaternionToEuler( const CIwFQuat& TQuat, CIwFVec3 &TEuler )
{
	float a[3];

	const float w = TQuat.s;
	const float x = TQuat.x;
	const float y = TQuat.y;
	const float z = TQuat.z;

	QuaternionToEuler( w, x, y, z, a );

	// heading
	TEuler.z = a[2];
	// bank
	TEuler.x = a[0];
	// attitude
	TEuler.y = a[1];
}

//-------------------------------------------------------------------------------------

void Physics::EulerXYZToQuaternion( const CIwFVec3 &TEuler, btQuaternion &TQuat )
{
	const double _heading = TEuler.z * 0.5;
	const double _attitude = TEuler.y * 0.5;
	const double _bank = TEuler.x * 0.5;
	const double c1 = cos( _heading );
	const double s1 = sin( _heading );
	const double c2 = cos( _attitude );
	const double s2 = sin( _attitude );
	const double c3 = cos( _bank );
	const double s3 = sin( _bank );
	const double c1c2 = c1 * c2;
	const double s1s2 = s1 * s2;

	const float w = btScalar( c1c2 * c3 + s1s2 * s3 );
	const float x = btScalar( c1c2 * s3 - s1s2 * c3 );
	const float y = btScalar( c1 * s2 * c3 + s1 * c2 * s3 );
	const float z = btScalar( s1 * c2 * c3 - c1 * s2 * s3 );

	TQuat.setW( w );	
	TQuat.setX( x );
	TQuat.setY( y );
	TQuat.setZ( z );
}

//-------------------------------------------------------------------------------------

CIwFMat Physics::GetMatrixFromRigidBody( const btRigidBody* b )
{
	CIwFMat modelMatrix;
	
	btMatrix3x3 mRot;
	btTransform btTrans = b->getWorldTransform();
	btVector3 pos = btTrans.getOrigin();
	mRot = btTrans.getBasis();

	modelMatrix.SetIdentity();

	for( int i = 0, j = 0; i < 3; ++i, j = 0 )
	{
		modelMatrix.m[i][j] = ( mRot.getRow( i ).x() );
		modelMatrix.m[i][++j] = ( mRot.getRow( i ).y() );
		modelMatrix.m[i][++j] = ( mRot.getRow( i ).z() );
	}
	
	modelMatrix.SetTrans( CIwFVec3( pos[0], pos[1], pos[2] ) );

	return modelMatrix;
}

//-------------------------------------------------------------------------------------

CIwFBBox Physics::GetBBoxFromRigidBody( const btRigidBody* b )
{
	CIwFBBox bbox;

	btVector3 vecMaxEdge, vecMinEdge;
	b->getAabb( vecMinEdge, vecMaxEdge );
	bbox.minV = CIwFVec3( vecMinEdge.getX(), vecMinEdge.getY(), vecMinEdge.getZ() );
	bbox.maxV = CIwFVec3( vecMaxEdge.getX(), vecMaxEdge.getY(), vecMaxEdge.getZ() );

	return bbox;
}

//-------------------------------------------------------------------------------------

CIwFSphere Physics::GetBSphereFromRigidBody( const btRigidBody* b )
{
	CIwFVec3 pos;
	const btVector3 centerOfMass = b->getCenterOfMassPosition();
	pos.x = centerOfMass.getX();
	pos.y = centerOfMass.getY();
	pos.z = centerOfMass.getZ();

	btVector3 vecMaxEdge, vecMinEdge;
	b->getAabb( vecMinEdge, vecMaxEdge );

	const float r = static_cast<float>( fabs( vecMaxEdge.getZ() - vecMinEdge.getZ() ) );
	const CIwFSphere sphere = CIwFSphere( pos, r );

	return sphere;
}

//-------------------------------------------------------------------------------------

CIwFMat Physics::GetMatrixFromWheelInfo( const btWheelInfo w, const btRigidBody* b )
{
	CIwFMat modelMatrix( CIwMat::g_Identity );

	const btTransform btTrans = w.m_worldTransform;
	const btVector3 pos = btTrans.getOrigin();

	const float posX = pos.getX();
	const float posY = pos.getY();
	const float posZ = pos.getZ();

	const CIwFVec3 position( posX, posY, posZ );

	const btQuaternion q = btTrans.getRotation();
	CIwFVec3 rot;
	QuaternionToEuler( q, rot );

	const btQuaternion qBody = b->getOrientation();
	CIwFVec3 rotBody;
	QuaternionToEuler( qBody, rotBody );
	
	CIwFMat mat = CIwFMat::g_Identity;
	mat.PostRotateX( rot.x );
	mat.PostRotateY( -rotBody.y );		// mat.PostRotateY( rot.y ); fix for rotating wheels by y axis, there are glitches after 360 degrees rotating
	mat.PostRotateZ( -rotBody.z );
	
	modelMatrix.CopyRot( mat );
	modelMatrix.SetTrans( position );

	return modelMatrix;
}

//-------------------------------------------------------------------------------------

CIwFMat Physics::GetMatrixFromStaticdBody( const CIwFVec3 pos, const CIwFVec3 rot )
{
	CIwFMat modelMatrix;

	CIwFMat rotX, rotY, rotZ;
	rotX.SetRotX( rot.x );
	rotY.SetRotY( rot.y );
	rotZ.SetRotZ( rot.z );
	modelMatrix.CopyRot( rotZ * rotY * rotX );
	modelMatrix.SetTrans( pos );

	return modelMatrix;
}

//-------------------------------------------------------------------------------------

CIwFVec3 Physics::GetBodyExtent( const btRigidBody* body )
{
	CIwFBBox bbox = GetBBoxFromRigidBody( body );
	const CIwFVec3 extent = ( bbox.maxV - bbox.minV ) * 0.5f;

	return extent;
}

//-------------------------------------------------------------------------------------

CIwFVec3 Physics::GetBodyExtent( const CIwFBBox bbox )
{
	const CIwFVec3 extent = ( bbox.maxV - bbox.minV ) * 0.5f;

	return extent;
}

//-------------------------------------------------------------------------------------

CIwFVec3 Physics::GetRotationFromRigidBody( const btRigidBody* b )
{
	CIwFVec3 rotation;

	const btTransform btTrans = b->getWorldTransform();
	const btQuaternion q = btTrans.getRotation();
	QuaternionToEuler( q, rotation );
	rotation *= static_cast<float>( BPU_180_PI );

	return rotation;
}

//-------------------------------------------------------------------------------------

#ifdef IW_BUILD_RESOURCES
void Physics::BuildCollision( const char* pUserString, CIwModel* pModel )
{
	// Check user string to test whether to affect this asset
	if( strcmp( pUserString, "collision" ) != 0 )
	{
		return;
	}
	
	// Create a new collision resource and name it after its model partner
	Collision* pCollision = new Collision;
	pCollision->SetName( pModel->DebugGetName() );

	for( uint32 i = 0; i < IwGetModelBuilder()->GetNumFaces(); ++i )
	{
		pCollision->AddFace( IwGetModelBuilder()->GetFace( i ), pModel );
	}

	// Add the collision resource to IwResManager - it will be serialised as part of the current group.
	IwGetResManager()->AddRes( "Collision", pCollision );
}
#endif