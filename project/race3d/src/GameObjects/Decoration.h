#ifndef _DECORATION_H_
#define _DECORATION_H_

#include "GameEntity.h"

class Collision;

//--------------------------------------------------------------------------------------------------
// Decoration - static or dynamic entity, doesn't have special functionality, just decoration
//--------------------------------------------------------------------------------------------------
class Decoration : public GameEntity
{
public:
	Decoration( const std::string groupName, const std::string modelName, const int collisionType = PHYSICS_COLLISION_BOX );
	virtual ~Decoration();

	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Physics update
	void UpdatePhysics( const s3e_uint64_t dt );

	// Render
	void Render();

	// Get amount of polygons
	unsigned int GetPolyCount();

private:
	std::string					m_groupName;
	std::string					m_modelName;

	CIwModel*					m_model;
	CIwFMat						m_modelMatrix;

	Collision*					m_collision;
	btTriangleIndexVertexArray*	m_indexVertexArrays;
	int*						m_colIndices;
	float*						m_colVertices;
};

#endif	// _DECORATION_H_