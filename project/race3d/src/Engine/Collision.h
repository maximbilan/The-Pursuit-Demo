#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "s3eKeyboard.h"
#include "s3ePointer.h"

#include "IwResManager.h"
#include "IwGraphics.h"
#include "IwModel.h"
#include "IwModelBuilder.h"

#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "btBulletDynamicsCommon.h"

#include "Physics.h"

//-----------------------------------------------------------------------------------------------
// Collision is a custom resource that is constructed from a geo at ModelParseClose time.
// It is then used to generate some procedural geometry based on a sub set of the model's faces
//-----------------------------------------------------------------------------------------------

class Collision : public CIwResource
{
public:
	IW_MANAGED_DECLARE( Collision );

	Collision();
	~Collision();

#ifdef IW_BUILD_RESOURCES
	// Add face
	void AddFace( CIwFace* pFace, CIwModel* pModel );
#endif

	// Serializing
	void Serialise();

	// Resolving
	void Resolve();

	// Helper function for looking up verts from CIwModel's vertex block
	const CIwFVec3& GetVert( const int32 i ) const;

	// Get vertex count
	int GetVertCount() const;

	// Get collision shape
	btCollisionShape* GetShape() const;

private:
	CIwModel*			m_pModel;		// The model this collision is based on
	CIwArray<uint16>	m_Points;		// A list of points. Each triplet is one triangle.
	CIwArray<CIwFVec3>	m_Norms;

	CIwArray<float>		m_btPoints;
	btCollisionShape*	m_collisionShape;
	bool				m_isUsedCollisionShape;

	Physics*			m_physics;
};

//---------------------------------------------------------------------------------------------

inline btCollisionShape* Collision::GetShape() const
{
	return m_collisionShape;
}

//---------------------------------------------------------------------------------------------

inline const CIwFVec3& Collision::GetVert( const int32 i ) const
{
	return ( ( CIwFVec3* ) m_pModel->GetVerts())[m_Points[i]];
}

//---------------------------------------------------------------------------------------------

inline int Collision::GetVertCount() const
{
	return m_Points.size();
}

#endif	// _COLLISION_H_