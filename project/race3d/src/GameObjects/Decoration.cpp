#include "Decoration.h"

#include "IwResManager.h"
#include "IwGraphics.h"

#include "../Engine/Physics.h"
#include "../Engine/Collision.h"
#include "../Common/Defines.h"

//------------------------------------------------------------------------------------------------

Decoration::Decoration( const std::string groupName, const std::string modelName, const int collisionType ) :
	m_groupName( groupName ),
	m_modelName( modelName )
{
	m_collisionType = collisionType;
	m_mass = 0.0f;

	m_type = ENTITY_DECORATION;
}

//------------------------------------------------------------------------------------------------

Decoration::~Decoration()
{
}

//------------------------------------------------------------------------------------------------

void Decoration::Init()
{
	CIwResGroup* pGroup = IwGetResManager()->GetGroupNamed( m_groupName.c_str() );
	m_model = static_cast<CIwModel*>( pGroup->GetResNamed( m_modelName.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );

	switch( m_collisionType )
	{
		case PHYSICS_COLLISION_BOX:
		{
			CIwFVec3* vertArray = static_cast<CIwFVec3*>( m_model->GetVerts() );
			m_bbox.BoundVec( ( CIwFVec3* )(&m_position) );
			m_bbox.BoundVecs( vertArray, m_model->GetNumVerts() );

			const CIwFVec3 extent = ( m_bbox.maxV - m_bbox.minV ) * 0.5f;
			m_collisionShape = new btBoxShape( btVector3( btScalar( extent.x ), btScalar( extent.y ), btScalar( extent.z ) ) );
			m_physics->AddCollisionShape( m_collisionShape );
		}
		break;

		case PHYSICS_COLLISION_MESH:
		{
			m_collision = static_cast<Collision*>( pGroup->GetResNamed( m_modelName.c_str(), "Collision" ) );
			m_collisionShape = m_collision->GetShape();
		}
		break;
	}

	m_rigidBody = m_physics->CreateRigidBody( m_mass, m_position, m_rotation, m_collisionShape );

	m_isInited = true;
}

//------------------------------------------------------------------------------------------------

void Decoration::Update( const s3e_uint64_t dt )
{
	IwGxClearFlags( IW_GX_DISABLE_CUSTOM_SHADERS_F );

	UpdatePhysics( dt );
}

//------------------------------------------------------------------------------------------------

void Decoration::UpdatePhysics( const s3e_uint64_t dt )
{
	m_bbox = m_physics->GetBBoxFromRigidBody( m_rigidBody );
	const CIwFVec3 extent = m_physics->GetBodyExtent( m_bbox );

	const btTransform btTrans = m_rigidBody->getWorldTransform();
	const btVector3 pos = btTrans.getOrigin();
	btQuaternion q = btTrans.getRotation();
	
	float posX = pos.getX();
	float posY = pos.getY();
	float posZ = pos.getZ();

	if( m_collisionType == PHYSICS_COLLISION_BOX )
	{
		posY -= extent.y;
	}

	const CIwFVec3 position( posX, posY, posZ );

	CIwFVec3 rot;
	m_physics->QuaternionToEuler( q, rot );

	CIwFMat rotX, rotY, rotZ;
	rotX.SetRotX( rot.x );
	rotY.SetRotY( rot.y );
	rotZ.SetRotZ( rot.z );

	q = m_rigidBody->getOrientation();
	m_physics->QuaternionToEuler( q, rot );

	CIwFMat mat = CIwFMat::g_Identity;
	mat.PostRotateX( -rot.x );
	mat.PostRotateY( -rot.y );
	mat.PostRotateZ( -rot.z );

	m_modelMatrix.CopyRot( mat );
	m_modelMatrix.SetTrans( position );

	m_rotation = rot;
	m_rotation *= static_cast<float>( m_physics->BPU_180_PI );
	m_position = CIwFVec3( m_rigidBody->getWorldTransform().getOrigin().getX(), m_rigidBody->getWorldTransform().getOrigin().getY(), m_rigidBody->getWorldTransform().getOrigin().getZ() );
}

//------------------------------------------------------------------------------------------------

void Decoration::Render()
{
	IwGxSetModelMatrix( &m_modelMatrix );
	m_model->Render();

#ifdef ENABLE_RENDER_BSPHERE
	IwGxDebugPrimSphere( ( *m_model->GetBSphere() ), IwGxGetColFixed( IW_GX_COLOUR_BLUE ) );
#endif

#ifdef ENABLE_RENDER_BBOX
	IwGxDebugPrimBBox( m_bbox, IwGxGetColFixed( IW_GX_COLOUR_YELLOW ) );
#endif
}

//------------------------------------------------------------------------------------------------

unsigned int Decoration::GetPolyCount()
{
	return m_model->GetNumVerts();
}