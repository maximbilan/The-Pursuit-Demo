#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "IwGx.h"
#include "IwGeomFMat.h"

class GameEntity;

//-----------------------------------------------------------------------------------------
// Frustum - pyramidal clipping
//-----------------------------------------------------------------------------------------
class Frustum
{
public:
	Frustum();
	~Frustum();

	// Check is object in frustum
	bool ObjectInFrustum( GameEntity* obj );

private:
	// Check is point in frustum
	bool PointInFrustum( const CIwFVec3& point );
};

#endif	// _FRUSTUM_H_