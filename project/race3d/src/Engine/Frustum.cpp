#include "Frustum.h"
#include "../GameObjects/GameEntity.h"

#include "IwGeomFBBox.h"
#include "IwGeomFVec3.h"

//---------------------------------------------------------------------------------------

Frustum::Frustum()
{
}

//---------------------------------------------------------------------------------------

Frustum::~Frustum()
{
}

//---------------------------------------------------------------------------------------

bool Frustum::ObjectInFrustum( GameEntity* obj )
{
	if( obj )
	{
		const CIwFBBox& bbox = obj->GetBBox();

		if( PointInFrustum( bbox.minV ) )
		{
			return true;
		}

		if( PointInFrustum( bbox.maxV ) )
		{
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------------------

bool Frustum::PointInFrustum( const CIwFVec3& point )
{
	// Transform the point from worldspace to clipspace
	CIwFVec3 p = IwGxGetWorldViewClipMatrix().TransformVec( point );

	// Clip the point against the view frustrum
	const uint16 flags = IwGxClipVec( p );

	return ( flags == 0 ) ? true : false;
}