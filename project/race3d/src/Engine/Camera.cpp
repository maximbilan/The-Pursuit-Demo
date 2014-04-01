#include "Camera.h"
#include "IwGeomCore.h"
#include "IwGraphics.h"

#include "../GameObjects/GameEntity.h"
#include "../Engine/Physics.h"

//---------------------------------------------------------------------------------------

const CIwFVec3 Camera::s_fromAboveOffset = CIwFVec3( 0.0f, -7200.0f, 4000.0f );

//---------------------------------------------------------------------------------------

Camera::Camera() :
	m_viewDir( CIwFVec3( 0.0f, 0.0f, -1.0f ) ),
	m_rightVector( CIwFVec3( 1.0f, 0.0f, 0.0f ) ),
	m_upVector( CIwFVec3( 0.0f, 1.0f, 0.0f ) ),
	m_position( CIwFVec3( 0.0f, 0.0f, 0.0f ) ),
	m_rotatedX( 0.0f ),
	m_rotatedY( 0.0f ),
	m_rotatedZ( 0.0f ),
	m_fov( 0.0f ),
	m_farZ( 0.0f ),
	m_nearZ( 0.0f ),
	m_type( CAMERA_TYPE_SPECTATOR )
{
}

//---------------------------------------------------------------------------------------

Camera::~Camera()
{
}

//---------------------------------------------------------------------------------------

Camera::Camera( const Camera& camera )
{
	m_viewDir = camera.m_viewDir;
	m_rightVector = camera.m_rightVector;
	m_upVector = camera.m_upVector;
	m_position = camera.m_position;
	m_rotatedX = camera.m_rotatedX;
	m_rotatedY = camera.m_rotatedY;
	m_rotatedZ = camera.m_rotatedZ;
	m_fov = camera.m_fov;
	m_farZ = camera.m_farZ;
	m_nearZ = camera.m_nearZ;
	m_type = camera.m_type;
}

//---------------------------------------------------------------------------------------

void Camera::Init( const CIwFVec3 position, const float fov, const float farZ, const float nearZ )
{
	SetFov( fov );
	SetFarZNearZ( farZ, nearZ );
	Move( position );
}

//---------------------------------------------------------------------------------------

void Camera::Apply()
{
	CIwFVec3 viewPoint( 0.0f, 0.0f, 0.0f );

	switch( m_type )
	{
		case CAMERA_TYPE_SPECTATOR:
		{
			viewPoint = m_position + m_viewDir;
		}
		break;

		case CAMERA_TYPE_FOLLOW_OBJECT_FROM_ABOVE:
		{
			m_position = m_gameObject->GetPosition() - s_fromAboveOffset;
			viewPoint = m_gameObject->GetPosition();
			m_upVector = -CIwFVec3::g_AxisZ;
		}
		break;

	}

	CIwFMat viewMat;
	viewMat.LookAt( m_position, viewPoint, m_upVector );
	
	viewMat.t.x = m_position.x;
	viewMat.t.y = m_position.y;
	viewMat.t.z = m_position.z;

	IwGxSetViewMatrix( &viewMat );
}

//---------------------------------------------------------------------------------------

void Camera::Move( const CIwFVec3 direction )
{
	m_position += direction;
}

//---------------------------------------------------------------------------------------

void Camera::RotateX( const float angle )
{
	m_rotatedX += angle;

	CIwFVec3 viewDir = ( m_viewDir * static_cast<float>( cos( angle * ( PI / 180 ) ) ) ) - ( m_upVector * static_cast<float>( sin( angle * ( PI / 180 ) ) ) );
	m_viewDir = viewDir.GetNormalised();
	m_upVector = m_viewDir.Cross( m_rightVector ) * -1;
}

//---------------------------------------------------------------------------------------

void Camera::RotateY( const float angle )
{
	m_rotatedY += angle;

	CIwFVec3 viewDir = ( m_viewDir * static_cast<float>( cos( angle * ( PI / 180 ) ) ) ) - ( m_rightVector * static_cast<float>( sin( angle * ( PI / 180 ) ) ) );
	m_viewDir = viewDir.GetNormalised();
	m_rightVector = m_viewDir.Cross( m_upVector );
}

//---------------------------------------------------------------------------------------

void Camera::RotateZ( const float angle )
{
	m_rotatedZ += angle;

	CIwFVec3 rightVector = ( m_rightVector * static_cast<float>( cos( angle * ( PI / 180 ) ) ) ) + ( m_upVector * static_cast<float>( sin( angle * ( PI / 180 ) ) ) );
	m_rightVector = rightVector.GetNormalised();
	m_upVector = m_viewDir.Cross( m_rightVector ) * -1;
}

//---------------------------------------------------------------------------------------

void Camera::MoveForward( const float distance )
{
	m_position += ( m_viewDir * ( -distance ) );
}

//---------------------------------------------------------------------------------------

void Camera::MoveUpward( const float distance )
{
	m_position += ( m_upVector * distance );
}

//---------------------------------------------------------------------------------------

void Camera::StrafeRight( const float distance )
{
	m_position += ( m_rightVector * distance );
}

//---------------------------------------------------------------------------------------

void Camera::SetFov( const float fov )
{
	const float perspMul =  static_cast<float>( IwGxGetDeviceWidth() / ( 2 * tan( fov / 2 ) ) );
	IwGxSetPerspMul( perspMul );
}

//---------------------------------------------------------------------------------------

void Camera::SetFarZNearZ( const float farZ, const float nearZ )
{
	IwGxSetFarZNearZ( farZ, nearZ );
}

//---------------------------------------------------------------------------------------

void Camera::FollowGameObject( GameEntity* object )
{
	m_type = CAMERA_TYPE_FOLLOW_OBJECT_FROM_ABOVE;
	m_gameObject = object;
}