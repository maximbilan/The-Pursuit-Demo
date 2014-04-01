#include "Collision.h"

//-----------------------------------------------------------------------------

IW_MANAGED_IMPLEMENT( Collision );
IW_CLASS_FACTORY( Collision );

//-----------------------------------------------------------------------------

Collision::Collision() :
	m_pModel( NULL ),
	m_collisionShape( NULL )
{
	m_physics = Physics::GetInstance();
}

//-----------------------------------------------------------------------------

Collision::~Collision()
{
	m_physics->FreeInstance();
}

//-----------------------------------------------------------------------------

#ifdef IW_BUILD_RESOURCES
void Collision::AddFace( CIwFace* pFace, CIwModel* pModel )
{
	// Remember the model
	IwAssert( GRAPHICS, !m_pModel || m_pModel == pModel );
	m_pModel = pModel;

	// Model build info contains information about how the data in the model was re-arranged.
	// Verts may be reordered for cache performance, etc.
	CIwModelBuildInfoMap& map = m_pModel->GetModelBuildInfo().GetMap( IW_MB_STREAM_VERTS );

	// If this face is a triangle...
	if( pFace->GetNumPoints() == 3 )
	{
		for( uint32 i = 0; i < 3; ++i )
		{
			CIwArray<uint16> deps;
			map.GetDependents( pFace->GetVertID( i ), deps );
			m_Points.push_back( deps[0] );
		}
	}
	else
	{
		// Only support quads
		IwAssert( GRAPHICS, pFace->GetNumPoints() == 4 );

		// Add the quad as two triangles
		uint32 i;
		for( i = 0; i < 3; ++i )
		{
			CIwArray<uint16> deps;
			map.GetDependents( pFace->GetVertID( i ), deps );
			m_Points.push_back( deps[0] );
		}
		for( i = 0; i < 4; i == 0 ? i += 2 : i++ )
		{
			CIwArray<uint16> deps;
			map.GetDependents( pFace->GetVertID( i ), deps );
			m_Points.push_back( deps[0] );
		}
	}
}
#endif

//-----------------------------------------------------------------------------

void Collision::Serialise()
{
	CIwManaged::Serialise();

	IwSerialiseManagedHash( &m_pModel );
	m_Points.SerialiseHeader();
	IwSerialiseUInt16( m_Points[0], m_Points.size() );
}

//-----------------------------------------------------------------------------

void Collision::Resolve()
{
	CIwManaged::Resolve();

	IwResolveManagedHash( &m_pModel, IW_GRAPHICS_RESTYPE_MODEL );

	//Build face normals (done on resolve to save disk space)
	for( uint32 i = 0; i < m_Points.size(); i += 3 )
	{
		const CIwFVec3 v1 = (CIwFVec3)GetVert( i );
		const CIwFVec3 v2 = (CIwFVec3)GetVert( i + 1 );
		const CIwFVec3 v3 = (CIwFVec3)GetVert( i + 2 );

		CIwFVec3 cross = (CIwFVec3)( v2 - v1 ).Cross( v3 - v1 );
		if( cross != CIwFVec3::g_Zero )
		{
			cross.Normalise();
		}
		m_Norms.push_back( cross );
	}

	//Build points structure for bullet
	for( uint32 i = 0; i < m_Points.size(); ++i )
	{
		const CIwFVec3 v1 = GetVert( i );
		const btVector3 posconv( btScalar( v1.x ), btScalar( v1.y ), btScalar( v1.z ) );
		m_btPoints.push_back( posconv.x() );
		m_btPoints.push_back( posconv.y() );
		m_btPoints.push_back( posconv.z() );
	}

	const int stride = sizeof( float ) * 3;
	btConvexHullShape* convexHullShape = new btConvexHullShape( m_btPoints.data(), m_btPoints.size() / 3, stride );
	btShapeHull* shapeHullOptimized = new btShapeHull( convexHullShape );
	shapeHullOptimized->buildHull( convexHullShape->getMargin() );

	m_collisionShape = new btConvexHullShape( ( btScalar* )( shapeHullOptimized->getVertexPointer() ), shapeHullOptimized->numVertices() );
	m_physics->AddCollisionShape( m_collisionShape );

	delete convexHullShape;
	delete shapeHullOptimized;
}