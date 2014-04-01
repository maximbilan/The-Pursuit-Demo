#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "IwGeomFVec3.h"
#include "IwGeomFMat.h"
#include "IwGeomFPlane.h"
#include "IwGeomFQuat.h"

class GameEntity;

//-----------------------------------------------------------------------------------------
// Camera
//-----------------------------------------------------------------------------------------
class Camera
{
private:
	enum CameraType
	{
		CAMERA_TYPE_SPECTATOR,
		CAMERA_TYPE_FOLLOW_OBJECT_FROM_ABOVE
	};

private:
	static const CIwFVec3 s_fromAboveOffset;

public:
	Camera();
	Camera ( const Camera& camera );
	~Camera();

	// Initialization
	void Init( const CIwFVec3 position, const float fov, const float farZ = 0x400, const float nearZ = 0x10 );

	// Compute view matrix
	void Apply();

	// Move to direction
	void Move( const CIwFVec3 direction );

	// Rotate x
	void RotateX( const float angle );

	// Rotate y
	void RotateY( const float angle );

	// Rotate z
	void RotateZ( const float angle );

	// Move forward to distance
	void MoveForward( const float distance );

	// Move upward to distance
	void MoveUpward( const float distance );

	// Strafe right
	void StrafeRight( const float distance );

	// Set field of view
	void SetFov( const float fov );

	// Set far z and near z
	void SetFarZNearZ( const float farZ, const float nearZ );

	// Follow game object
	void FollowGameObject( GameEntity* object );

	// Get position
	const CIwFVec3& GetPosition() const;

private:
	CIwFVec3		m_viewDir;
	CIwFVec3		m_rightVector;	
	CIwFVec3		m_upVector;
	CIwFVec3		m_position;

	float			m_rotatedX;
	float			m_rotatedY;
	float			m_rotatedZ;

	float			m_fov;
	float			m_farZ;
	float			m_nearZ;

	unsigned int	m_type;

	GameEntity*		m_gameObject;
};

//---------------------------------------------------------------------------------------

inline const CIwFVec3& Camera::GetPosition() const
{
	return m_position;
}

#endif	// _CAMERA_H_