#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "IwModel.h"

#include "GameEntity.h"

class Collision;

//------------------------------------------------------------------------------------------------
// Terrain
//------------------------------------------------------------------------------------------------
class Terrain : public GameEntity
{
public:
	Terrain( const std::string groupName, const std::string modelName );
	virtual ~Terrain();

	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Physics update
	void UpdatePhysics( const s3e_uint64_t dt );

	// Render
	void Render();

	// Get amount of polugons
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

#endif	// _TERRAIN_H_