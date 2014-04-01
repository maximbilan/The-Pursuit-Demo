#include "Terrain.h"

#include "IwResManager.h"
#include "IwGraphics.h"

#include "../Engine/Physics.h"
#include "../Engine/Collision.h"

//------------------------------------------------------------------------------------------------

Terrain::Terrain( const std::string groupName, const std::string modelName ) :
	m_groupName( groupName ),
	m_modelName( modelName )
{
	m_mass = 0.0f;
	m_isDynamic = false;
	m_type = ENTITY_TERRAIN;
}

//------------------------------------------------------------------------------------------------

Terrain::~Terrain()
{
	free( m_colIndices );
	free( m_colVertices );

	delete m_indexVertexArrays;
}

//------------------------------------------------------------------------------------------------

void Terrain::Init()
{
	CIwResGroup* pGroup = IwGetResManager()->GetGroupNamed( m_groupName.c_str() );
	m_model = static_cast<CIwModel*>( pGroup->GetResNamed( m_modelName.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );
	m_collision = static_cast< Collision* >( pGroup->GetResNamed( m_modelName.c_str(), "Collision" ) );

	// find some way to delete old unused trimeshes
	const uint16 vertcount = m_collision->GetVertCount();
	m_colIndices = ( int* ) malloc( vertcount * sizeof( int ) );
	for( int j = 0; j < vertcount; j += 1 )
	{
		m_colIndices[j] = j;
	}

	m_colVertices = ( btScalar* ) malloc( vertcount * 3 * sizeof( btScalar ) );
	for( int j = 0; j < vertcount; ++j )
	{
		m_colVertices[ j * 3] = m_collision->GetVert( j ).x;
		m_colVertices[ j * 3 + 1] = m_collision->GetVert( j ).y;
		m_colVertices[ j * 3 + 2] = m_collision->GetVert( j ).z;
	}

	// bullet physics
	m_indexVertexArrays = new btTriangleIndexVertexArray( vertcount / 3, m_colIndices, 3 * sizeof( int ), vertcount, ( btScalar* ) m_colVertices, 3 * sizeof( btScalar ) );
	m_collisionShape = new btBvhTriangleMeshShape( m_indexVertexArrays, true );
	m_physics->AddCollisionShape( m_collisionShape );

	m_rigidBody = m_physics->CreateRigidBody( m_mass, m_position, m_rotation, m_collisionShape );
	m_rigidBody->setRestitution( 0.9f );

	m_bbox = m_physics->GetBBoxFromRigidBody( m_rigidBody );

	m_isInited = true;
}

//------------------------------------------------------------------------------------------------

void Terrain::Update( const s3e_uint64_t dt )
{
	m_modelMatrix = m_physics->GetMatrixFromStaticdBody( m_position, m_rotation );
}

//------------------------------------------------------------------------------------------------

void Terrain::UpdatePhysics( const s3e_uint64_t dt )
{
}

//------------------------------------------------------------------------------------------------

void Terrain::Render()
{
	IwGxSetModelMatrix( &m_modelMatrix );
	m_model->Render();

#ifdef ENABLE_RENDER_BBOX
	IwGxDebugPrimBBox( m_bbox, IwGxGetColFixed( IW_GX_COLOUR_YELLOW ) );
#endif
}

//------------------------------------------------------------------------------------------------

unsigned int Terrain::GetPolyCount()
{
	return m_model->GetNumVerts();
}