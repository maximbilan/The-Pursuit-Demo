#include "Car.h"

#include "s3e.h"
#include "math.h"
#include "tinyxml.h"

#include "IwResManager.h"
#include "IwGraphics.h"

#include "../Common/Defines.h"

#include "../Engine/Collision.h"
#include "../Engine/Camera.h"
#include "../Engine/Record/Record.h"
#include "../Engine/Record/RecordDB.h"
#include "../Engine/ShaderManager.h"
#include "../Engine/LightManager.h"

//--------------------------------------------------------------------------------------------------

// By default, Bullet Vehicle uses Y as up axis.
// You can override the up axis, for example Z-axis up. Enable this define to see how to:
//#define FORCE_ZAXIS_UP 1

//-------------------------------------------------------------------------------------------------

const char* Car::s_carGroupNames[] = {
										"car_eclipse_red", 
										"car_eclipse_blue",
										"car_eclipse_gray", 
										"car_eclipse_purple"
};

const char* Car::s_carSettingsPath		= "car_settings";
const char* Car::s_carSettingsXMLFormat	= "xml";
const char* Car::s_carSettingsBINFormat	= "bin";

//-------------------------------------------------------------------------------------------------

Car::Car( const unsigned int color ) :
	m_isForce( false ),
	m_isBackDraft( false ),
	m_shaderUniform( NULL )
{
	m_groupName = s_carGroupNames[color];
	m_modelName = "";

	m_type = ENTITY_CAR;

#ifdef FORCE_ZAXIS_UP
	m_rightIndex = 0;
	m_upIndex = 2; 
	m_forwardIndex = 1;

	m_wheelDirectionCS0.setX( 0 );
	m_wheelDirectionCS0.setY( 0 );
	m_wheelDirectionCS0.setZ( -1 );

	m_wheelAxleCS.setX( 1 );
	m_wheelAxleCS.setY( 0 );
	m_wheelAxleCS.setZ( 0 );
#else
	m_rightIndex = 0;
	m_upIndex = 1;
	m_forwardIndex = 2;

	m_wheelDirectionCS0.setX( 0 );
	m_wheelDirectionCS0.setY( -1 ); //-1
	m_wheelDirectionCS0.setZ( 0 );

	m_wheelAxleCS.setX( -1 );
	m_wheelAxleCS.setY( 0 );
	m_wheelAxleCS.setZ( 0 );
#endif

	m_gEngineForce = 0.f;
	m_gBreakingForce = 0.f;

	m_maxEngineForce = 10000.f;		// This should be engine/velocity dependent
	m_maxBreakingForce = 300.f;

	m_gVehicleSteering = 0.f;
	m_steeringIncrement = 0.1f;
	m_steeringClamp = 0.3f;
	m_wheelRadius = 60.f;
	m_wheelWidth = 55.f;
	m_wheelFriction = 1000.f;
	m_suspensionStiffness = 20.f;
	m_suspensionDamping = 2.3f;
	m_suspensionCompression = 4.4f;
	m_rollInfluence = 0.1f; // 1.0f;
	m_suspensionRestLength = 25.5f;

	m_connectionHeight = 115.f;

	m_mass = 50.0f;

	m_name = "car_eclipse";

	if( m_name.empty() )
	{
		IwAssert( APP, "Name of the car is missing !!!" );
	}
	
	sprintf( m_settingsFileXML, "%s/%s.%s", s_carSettingsPath, m_name.c_str(), s_carSettingsXMLFormat );
	sprintf( m_settingsFileBIN, "%s/%s.%s", s_carSettingsPath, m_name.c_str(), s_carSettingsBINFormat );

	m_settingsDB = new RecordDB( std::string( m_settingsFileBIN ) );

#ifdef IW_BUILD_RESOURCES
	LoadXML();
	SaveBin();
#else
	LoadBin();
#endif
}

//-------------------------------------------------------------------------------------------------

Car::~Car()
{
	delete m_settingsDB;
	delete m_compoundShape;
	delete m_vehicleRayCaster;
	delete m_raycastVehicle;
}

//--------------------------------------------------------------------------------------------------

void Car::Init()
{
	CIwResGroup* pGroup = IwGetResManager()->GetGroupNamed( m_groupName.c_str() );
	
	const std::string bodyStr = m_groupName + "_body";
	const std::string flwheelStr = m_groupName + "_flwheel";
	const std::string frwheelStr = m_groupName + "_frwheel";
	const std::string rrwheelStr = m_groupName + "_rrwheel";
	const std::string rlwheelStr = m_groupName + "_rlwheel";

	// car body
	m_modelBody = static_cast<CIwModel*>( pGroup->GetResNamed( bodyStr.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );
	m_collisionBody = static_cast<Collision*>( pGroup->GetResNamed( bodyStr.c_str(), "Collision" ) );

	m_modelWheel1 = static_cast<CIwModel*>( pGroup->GetResNamed( flwheelStr.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );
	m_modelWheel2 = static_cast<CIwModel*>( pGroup->GetResNamed( frwheelStr.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );
	m_modelWheel3 = static_cast<CIwModel*>( pGroup->GetResNamed( rrwheelStr.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );
	m_modelWheel4 = static_cast<CIwModel*>( pGroup->GetResNamed( rlwheelStr.c_str(), IW_GRAPHICS_RESTYPE_MODEL ) );

	m_collisionShape = m_collisionBody->GetShape();
	//m_collisionShape = new btBoxShape( btVector3( 50.f, 25.f, 100.f ) );

	m_compoundShape = new btCompoundShape();
	
	btTransform localTrans;
	localTrans.setIdentity();
	
	// localTrans effectively shifts the center of mass with respect to the chassis
#ifdef FORCE_ZAXIS_UP
	localTrans.setOrigin( btVector3( 0, 1, 0 ) );
#else
	localTrans.setOrigin( btVector3( 0, 0, 1 ) );
#endif

	m_compoundShape->addChildShape( localTrans, m_collisionShape );

	m_rigidBody = m_physics->CreateRigidBody( m_mass, m_position, m_rotation, m_compoundShape );

	const CIwFVec3 extent = m_physics->GetBodyExtent( m_rigidBody );

	/// create vehicle
	{
		m_vehicleRayCaster = new btDefaultVehicleRaycaster( m_physics->GetDynamicWorld() );
		m_raycastVehicle = new btRaycastVehicle( m_vehicleTuning, m_rigidBody, m_vehicleRayCaster );
		m_rigidBody->setActivationState( DISABLE_DEACTIVATION );		// Never deactivate the vehicle

		m_physics->AddVehicle( m_raycastVehicle );

		bool isFrontWheel = true;

		m_raycastVehicle->setCoordinateSystem( m_rightIndex, m_upIndex, m_forwardIndex );		// Choose coordinate system

		const float shiftWheel = 0.5f;

		const float shiftZFrontWheel = 1.6f;
		const float shiftZRearWheel = -1.75f;

#ifdef FORCE_ZAXIS_UP
		btVector3 connectionPointCS0( extent.x - ( shiftWheel * m_wheelWidth ), 2 * extent.x - m_wheelRadius, btScalar( m_connectionHeight ) );
#else
		btVector3 connectionPointCS0( btScalar( extent.x - ( shiftWheel * m_wheelWidth ) ), btScalar( m_connectionHeight ), btScalar( shiftZFrontWheel * extent.x - m_wheelRadius ) );
#endif
		m_raycastVehicle->addWheel( connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_vehicleTuning, isFrontWheel );

#ifdef FORCE_ZAXIS_UP
		connectionPointCS0 = btVector3( -extent.x + ( shiftWheel * m_wheelWidth ), 2 * extent.x - m_wheelRadius, btScalar( m_connectionHeight ) );
#else
		connectionPointCS0 = btVector3( btScalar( -extent.x + ( shiftWheel * m_wheelWidth ) ), btScalar( m_connectionHeight ), btScalar( shiftZFrontWheel * extent.x - m_wheelRadius ) );
#endif
		m_raycastVehicle->addWheel( connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_vehicleTuning, isFrontWheel );

#ifdef FORCE_ZAXIS_UP
		connectionPointCS0 = btVector3( -extent.x + ( shiftWheel * m_wheelWidth ), -2 * extent.x + m_wheelRadius, btScalar( m_connectionHeight ) );
#else
		connectionPointCS0 = btVector3( btScalar( -extent.x +( shiftWheel * m_wheelWidth ) ), btScalar( m_connectionHeight ), btScalar( shiftZRearWheel * extent.x + m_wheelRadius ) );
#endif
		isFrontWheel = false;
		m_raycastVehicle->addWheel( connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_vehicleTuning, isFrontWheel );

#ifdef FORCE_ZAXIS_UP
		connectionPointCS0 = btVector3( extent.x - ( shiftWheel * m_wheelWidth ), -2 * extent.x + m_wheelRadius, btScalar( m_connectionHeight ) );
#else
		connectionPointCS0 = btVector3( btScalar( extent.x - ( shiftWheel * m_wheelWidth ) ), btScalar( m_connectionHeight ), btScalar( shiftZRearWheel * extent.x + m_wheelRadius ) );
#endif
		m_raycastVehicle->addWheel( connectionPointCS0, m_wheelDirectionCS0, m_wheelAxleCS, m_suspensionRestLength, m_wheelRadius, m_vehicleTuning, isFrontWheel );
		
		for( int i = 0; i < m_raycastVehicle->getNumWheels(); ++i )
		{
			btWheelInfo& wheel = m_raycastVehicle->getWheelInfo( i );
			wheel.m_suspensionStiffness = m_suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = m_suspensionDamping;
			wheel.m_wheelsDampingCompression = m_suspensionCompression;
			wheel.m_frictionSlip = m_wheelFriction;
			wheel.m_rollInfluence = m_rollInfluence;
		}
	}

	Reset();

	LoadXML();

	m_shaderTechnique = m_shaderManager->GetShaderTechnique( ShaderManager::TECHNIQUE_TOON_SHADING );
	if( m_shaderTechnique )
	{
		m_shaderUniform = m_shaderTechnique->GetParam( "diffuseLightPosition" );
	}

	m_isInited = true;
}

//-------------------------------------------------------------------------------------------------

void Car::Reset()
{
	m_gVehicleSteering = 0.f;
	m_rigidBody->setCenterOfMassTransform( btTransform::getIdentity() );
	m_rigidBody->setLinearVelocity( btVector3( 0, 0, 0 ) );
	m_rigidBody->setAngularVelocity( btVector3( 0, 0, 0 ) );
	
	if( m_raycastVehicle )
	{
		m_raycastVehicle->resetSuspension();
		for( int i = 0; i < m_raycastVehicle->getNumWheels(); ++i )
		{
			// Synchronize the wheels with the (interpolated) chassis worldtransform
			m_raycastVehicle->updateWheelTransform( i, true );
		}
	}
}

//-------------------------------------------------------------------------------------------------

void Car::Update( const s3e_uint64_t dt )
{
	if( m_shaderUniform )
	{
		CIwFVec3& lightPos = m_lightManager->GetDiffusePosition();
		m_shaderUniform->Set( lightPos );
	}

	if( !m_isForce && m_gVehicleSteering == 0.0f )
	{
		m_gBreakingForce = m_maxBreakingForce / 4;
	}

	int wheelIndex = 2;
	m_raycastVehicle->applyEngineForce( m_gEngineForce, wheelIndex );
	m_raycastVehicle->setBrake( m_gBreakingForce, wheelIndex );

	wheelIndex = 3;
	m_raycastVehicle->applyEngineForce( m_gEngineForce, wheelIndex );
	m_raycastVehicle->setBrake( m_gBreakingForce, wheelIndex );

	m_gEngineForce = 0.0f;

	wheelIndex = 0;
	m_raycastVehicle->setSteeringValue( m_gVehicleSteering, wheelIndex );
	wheelIndex = 1;
	m_raycastVehicle->setSteeringValue( m_gVehicleSteering, wheelIndex );

	m_gVehicleSteering = 0.0f;

#ifdef CALCULATE_SLIP_ANGLE
	double delta = dt * 0.0001;
	
	for( int i = 2; i < 4; i++ )
	{
		if( m_gEngineForce > 0.f )
		{
			m_raycastVehicle->getWheelInfo( i ).m_frictionSlip = btScalar( m_wheelFriction * 0.85 * ( 1.f - ( m_gEngineForce * m_maxEngineForce * ( 1.f - 0.65 ) ) / m_maxEngineForce ) );
		}
		else
		{
			m_raycastVehicle->getWheelInfo( i ).m_frictionSlip = btScalar( m_wheelFriction * 0.85 );
		}
				
		if( m_gBreakingForce > 0.f )
		{
			m_raycastVehicle->getWheelInfo( i ).m_frictionSlip = btScalar ( m_wheelFriction * 0.85 * ( 1.f + ( m_gBreakingForce * m_maxBreakingForce * ( 0.85 - 1.f ) ) / m_maxBreakingForce ) );
		}
	}

	float slip_angle = 0.f;

	for( int i = 0; i < 4; i++ )
	{  
		// Calculate velocity of wheel wrt ground

		btVector3 centerOfMass = m_rigidBody->getCenterOfMassPosition();
		btVector3 rel_pos1 = m_raycastVehicle->getWheelInfo( i ).m_raycastInfo.m_contactPointWS;
		rel_pos1 -= centerOfMass;
		btVector3 vel = m_rigidBody->getVelocityInLocalPoint( rel_pos1 );
		btVector3 forward = m_raycastVehicle->getForwardVector();
		btVector3 right( 0.f, 0.f, 1.f );
		right.cross( forward );

		float vz = -1.f * forward.dot( vel );
		float vx = right.dot( vel );
		slip_angle += static_cast<float>( atan2( vx, fabs( vz ) ) );
	}

	slip_angle /= 4.f;
	for( int i = 0; i < 2; ++i )
	{
		m_raycastVehicle->getWheelInfo( i ).m_steering -= btScalar( slip_angle * delta * ( 1.f - m_raycastVehicle->getWheelInfo( i + 2 ).m_skidInfo) * 55.0f );

		if( fabs( m_raycastVehicle->getWheelInfo( i ).m_steering ) > m_steeringClamp )
		{
			if( m_raycastVehicle->getWheelInfo(i).m_steering < 0.f )
			{
				m_raycastVehicle->getWheelInfo(i).m_steering = -m_steeringClamp;
			}
			else
			{
				m_raycastVehicle->getWheelInfo(i).m_steering = m_steeringClamp;
			}
		}
	}

	btVector3 forw = m_raycastVehicle->getForwardVector();
	forw *= 1.f / forw.length();

	if( m_raycastVehicle->getCurrentSpeedKmHour() > 1000.f )
	{
		forw *= btScalar( delta * m_raycastVehicle->getCurrentSpeedKmHour() * 1000.f * delta );
	}

	m_rigidBody->applyCentralImpulse( forw );
#endif

	UpdatePhysics( dt );

	m_isForce = false;
	m_isBackDraft = false;
}

//--------------------------------------------------------------------------------------------------

void Car::UpdatePhysics( const s3e_uint64_t dt )
{
	m_bbox = m_physics->GetBBoxFromRigidBody( m_rigidBody );
	m_bsphere = m_physics->GetBSphereFromRigidBody( m_rigidBody );
	
	const btTransform btTrans = m_rigidBody->getCenterOfMassTransform();
	const btVector3 pos = btTrans.getOrigin();
	
	const float posX = pos.getX();
	const float posY = pos.getY();
	const float posZ = pos.getZ();

	const CIwFVec3 position( posX, posY, posZ );

	const btQuaternion q = m_rigidBody->getOrientation();
	CIwFVec3 rot;
	m_physics->QuaternionToEuler( q, rot );

	CIwFMat mat = CIwFMat::g_Identity;
	mat.PostRotateX( -rot.x );
	mat.PostRotateY( -rot.y );
	mat.PostRotateZ( -rot.z );

	m_modelBodyMatrix.CopyRot( mat );
	m_modelBodyMatrix.SetTrans( position );

	m_modelWheel1Matrix = m_physics->GetMatrixFromWheelInfo( m_raycastVehicle->getWheelInfo( 0 ), m_rigidBody );
	m_modelWheel2Matrix = m_physics->GetMatrixFromWheelInfo( m_raycastVehicle->getWheelInfo( 1 ), m_rigidBody );
	m_modelWheel3Matrix = m_physics->GetMatrixFromWheelInfo( m_raycastVehicle->getWheelInfo( 2 ), m_rigidBody );
	m_modelWheel4Matrix = m_physics->GetMatrixFromWheelInfo( m_raycastVehicle->getWheelInfo( 3 ), m_rigidBody );

	m_rotation = rot;
	m_rotation *= static_cast<float>( m_physics->BPU_180_PI );
	m_position = CIwFVec3( m_rigidBody->getWorldTransform().getOrigin().getX(), m_rigidBody->getWorldTransform().getOrigin().getY(), m_rigidBody->getWorldTransform().getOrigin().getZ() );
}

void Car::Render()
{
	IwGxSetModelMatrix( &m_modelWheel1Matrix );
	m_modelWheel1->Render();

	IwGxSetModelMatrix( &m_modelWheel2Matrix );
	m_modelWheel2->Render();

	IwGxSetModelMatrix( &m_modelWheel3Matrix );
	m_modelWheel3->Render();

	IwGxSetModelMatrix( &m_modelWheel4Matrix );
	m_modelWheel4->Render();

	IwGxSetModelMatrix( &m_modelBodyMatrix );
	m_modelBody->Render();

#ifdef ENABLE_RENDER_BSPHERE
	IwGxDebugPrimSphere( m_bsphere, IwGxGetColFixed( IW_GX_COLOUR_BLUE ) );
#endif

#ifdef ENABLE_RENDER_BBOX
	IwGxDebugPrimBBox( m_bbox, IwGxGetColFixed( IW_GX_COLOUR_YELLOW ) );
#endif

#ifdef PRINT_CAR_DEBUG_INFO
	char speedStr[256];
	sprintf( speedStr, "Speed: %f", m_raycastVehicle->getCurrentSpeedKmHour() );
	IwGxPrintString( 50, 150, speedStr );
	sprintf( speedStr, "EngineForce: %f", m_gEngineForce );
	IwGxPrintString( 50, 180, speedStr );
#endif
}

//-------------------------------------------------------------------------------------------------

void Car::Force()
{
	m_isForce = true;

	const float speed = static_cast<float>( m_raycastVehicle->getCurrentSpeedKmHour() * 0.001 );

#ifdef PRINT_CAR_DEBUG_INFO
	char speedStr[256];
	sprintf( speedStr, "Car::force Speed: %f", speed );
	IwGxPrintString( 50, 250, speedStr );
#endif

	if( speed > 1.0f )
	{
		m_gEngineForce = m_maxEngineForce / speed;
	}
	else
	{
		m_gEngineForce = m_maxEngineForce;
	}

	m_gBreakingForce = 0.f;
}

//-------------------------------------------------------------------------------------------------

void Car::BackDraft()
{
	m_isBackDraft = true;

	const float speed = static_cast<float>( m_raycastVehicle->getCurrentSpeedKmHour() * 0.001 );

#ifdef PRINT_CAR_DEBUG_INFO
	char speedStr[256];
	sprintf( speedStr, "Car::force Speed: %f", speed );
	IwGxPrintString( 50, 250, speedStr );
#endif

	if( speed < -1.0f )
	{
		m_gEngineForce = static_cast<float>( -m_maxEngineForce * 0.5 / fabs( speed ) );
	}
	else
	{
		m_gEngineForce = static_cast<float>( -m_maxEngineForce * 0.5 );
	}

	m_gBreakingForce = m_maxBreakingForce;
}

//-------------------------------------------------------------------------------------------------

void Car::Brake()
{
	m_gBreakingForce = m_maxBreakingForce; 
	m_gEngineForce = 0.f;
}

//-------------------------------------------------------------------------------------------------

void Car::Left()
{
	m_gVehicleSteering += m_steeringIncrement;
	
	if( m_gVehicleSteering > m_steeringClamp )
	{
		m_gVehicleSteering = m_steeringClamp;
	}
}

//-------------------------------------------------------------------------------------------------

void Car::Right()
{
	m_gVehicleSteering -= m_steeringIncrement;

	if( m_gVehicleSteering < -m_steeringClamp )
	{
		m_gVehicleSteering = -m_steeringClamp;
	}
}

//-------------------------------------------------------------------------------------------------

unsigned int Car::GetPolyCount()
{
	return m_modelBody->GetNumVerts() + m_modelWheel1->GetNumVerts() + m_modelWheel2->GetNumVerts() + m_modelWheel3->GetNumVerts() + m_modelWheel4->GetNumVerts();
}

//-------------------------------------------------------------------------------------------------

void Car::SaveBin()
{
	if( m_settingsDB->IsExist() )
	{
		m_settingsDB->Reset();
	}

	m_settingsDB->AddRecord( "RightIndex", m_rightIndex );
	m_settingsDB->AddRecord( "ForwardIndex", m_forwardIndex );
	m_settingsDB->AddRecord( "UpIndex", m_upIndex );
	m_settingsDB->AddRecord( "WheelDirectionCS0_X", m_wheelDirectionCS0.getX() );
	m_settingsDB->AddRecord( "WheelDirectionCS0_Y", m_wheelDirectionCS0.getY() );
	m_settingsDB->AddRecord( "WheelDirectionCS0_Z", m_wheelDirectionCS0.getZ() );
	m_settingsDB->AddRecord( "WheelAxleCS_X", m_wheelAxleCS.getX() );
	m_settingsDB->AddRecord( "WheelAxleCS_Y", m_wheelAxleCS.getY() );
	m_settingsDB->AddRecord( "WheelAxleCS_Z", m_wheelAxleCS.getZ() );
	m_settingsDB->AddRecord( "MaxEngineForce", m_maxEngineForce );
	m_settingsDB->AddRecord( "MaxBreakingForce", m_maxBreakingForce );
	m_settingsDB->AddRecord( "SteeringIncrement", m_steeringIncrement );
	m_settingsDB->AddRecord( "SteeringClamp", m_steeringClamp );
	m_settingsDB->AddRecord( "WheelRadius", m_wheelRadius );
	m_settingsDB->AddRecord( "WheelWidth", m_wheelWidth );
	m_settingsDB->AddRecord( "WheelFriction", m_wheelFriction );
	m_settingsDB->AddRecord( "SuspensionStiffness", m_suspensionStiffness );
	m_settingsDB->AddRecord( "SuspensionDamping", m_suspensionDamping );
	m_settingsDB->AddRecord( "SuspensionCompression", m_suspensionCompression );
	m_settingsDB->AddRecord( "RollInfluence", m_rollInfluence );
	m_settingsDB->AddRecord( "SuspensionRestLength", m_suspensionRestLength );
	m_settingsDB->AddRecord( "ConnectionHeight", m_connectionHeight );
	m_settingsDB->AddRecord( "Mass", m_mass );

	m_settingsDB->Save();
}

//-------------------------------------------------------------------------------------------------

void Car::LoadBin()
{
	if( m_settingsDB->IsExist() )
	{
		m_settingsDB->Load();

		m_rightIndex = m_settingsDB->GetRecordAsInt( "RightIndex" );
		m_forwardIndex = m_settingsDB->GetRecordAsInt( "ForwardIndex" );
		m_upIndex = m_settingsDB->GetRecordAsInt( "UpIndex" );
		m_wheelDirectionCS0.setX( m_settingsDB->GetRecordAsFloat( "WheelDirectionCS0_X" ) );
		m_wheelDirectionCS0.setY( m_settingsDB->GetRecordAsFloat( "WheelDirectionCS0_Y" ) );
		m_wheelDirectionCS0.setZ( m_settingsDB->GetRecordAsFloat( "WheelDirectionCS0_Z" ) );
		m_wheelAxleCS.setX( m_settingsDB->GetRecordAsFloat( "WheelAxleCS_X" ) );
		m_wheelAxleCS.setY( m_settingsDB->GetRecordAsFloat( "WheelAxleCS_Y" ) );
		m_wheelAxleCS.setZ( m_settingsDB->GetRecordAsFloat( "WheelAxleCS_Z" ) );
		m_maxEngineForce = m_settingsDB->GetRecordAsFloat( "MaxEngineForce" );
		m_maxBreakingForce = m_settingsDB->GetRecordAsFloat( "MaxBreakingForce" );
		m_steeringIncrement = m_settingsDB->GetRecordAsFloat( "SteeringIncrement" );
		m_steeringClamp = m_settingsDB->GetRecordAsFloat( "SteeringClamp" );
		m_wheelRadius = m_settingsDB->GetRecordAsFloat( "WheelRadius" );
		m_wheelWidth = m_settingsDB->GetRecordAsFloat( "WheelWidth" );
		m_wheelFriction = m_settingsDB->GetRecordAsFloat( "WheelFriction" );
		m_suspensionStiffness = m_settingsDB->GetRecordAsFloat( "SuspensionStiffness" );
		m_suspensionDamping = m_settingsDB->GetRecordAsFloat( "SuspensionDamping" );
		m_suspensionCompression = m_settingsDB->GetRecordAsFloat( "SuspensionCompression" );
		m_rollInfluence = m_settingsDB->GetRecordAsFloat( "RollInfluence" );
		m_suspensionRestLength = m_settingsDB->GetRecordAsFloat( "SuspensionRestLength" );
		m_connectionHeight = m_settingsDB->GetRecordAsFloat( "ConnectionHeight" );
		m_mass = m_settingsDB->GetRecordAsFloat( "Mass" );
	}
	else
	{
		IwAssert( APP, "Binary settings don't exists !!!" );
	}
}

//-------------------------------------------------------------------------------------------------

void Car::LoadXML()
{
	char fileData[CAR_XML_SETTINGS_BUFFER];
	const std::string filename = m_settingsFileXML;

	if( filename.empty() )
	{
		IwAssert( APP, "XML settings filename is missing !!!" );
		return;
	}

	if( !s3eFileCheckExists( filename.c_str() ) )
	{
		IwAssert( APP, "XML settings don't exists !!!" );
		return;
	}

	s3eFile* file = s3eFileOpen( filename.c_str(), "rb" );
	if( file != NULL )
	{
		uint32 fileSize = s3eFileGetSize( file );
		s3eFileRead( fileData, fileSize, 1, file );
		s3eFileClose( file );
	}
	else
	{
		s3eFileGetError();
		s3eDebugOutputString( s3eFileGetErrorString() );
		return;
	}

	TiXmlDocument doc;
	TiXmlHandle root( NULL );
	TiXmlElement* element;
	TiXmlNode* child;
	
	doc.Parse( fileData, 0 );
	element = doc.FirstChildElement( "Car" );
	root = TiXmlHandle( element );
	
	for ( child = root.FirstChild().ToElement(); child != 0; child = child->NextSibling() )
	{
		TiXmlElement* e = child->ToElement();

		if( e )
		{
			const char* attrib = e->Value();
			const char* value = e->GetText();

			if( !strcmp( attrib, "RightIndex" ) )
			{
				sscanf( value, "%d", &m_rightIndex );
			}
			else if( !strcmp( attrib, "ForwardIndex" ) )
			{
				sscanf( value, "%d", &m_forwardIndex );
			}
			else if( !strcmp( attrib, "UpIndex" ) )
			{
				sscanf( value, "%d", &m_upIndex );
			}
			else if( !strcmp( attrib, "WheelDirectionCS0_X" ) )
			{
				int x = 0;
				sscanf( value, "%d", &x );
				m_wheelDirectionCS0.setX( btScalar( x ) );
			}
			else if( !strcmp( attrib, "WheelDirectionCS0_Y" ) )
			{
				int y = 0;
				sscanf( value, "%d", &y );
				m_wheelDirectionCS0.setY( btScalar( y ) );
			}
			else if( !strcmp( attrib, "WheelDirectionCS0_Z" ) )
			{
				int z = 0;
				sscanf( value, "%d", &z );
				m_wheelDirectionCS0.setZ( btScalar( z ) );
			}
			else if( !strcmp( attrib, "WheelAxleCS_X" ) )
			{
				int x = 0;
				sscanf( value, "%d", &x );
				m_wheelAxleCS.setX( btScalar( x ) );
			}
			else if( !strcmp( attrib, "WheelAxleCS_Y" ) )
			{
				int y = 0;
				sscanf( value, "%d", &y );
				m_wheelAxleCS.setY( btScalar( y ) );
			}
			else if( !strcmp( attrib, "WheelAxleCS_Z" ) )
			{
				int z = 0;
				sscanf( value, "%d", &z );
				m_wheelAxleCS.setZ( btScalar( z ) );
			}
			else if( !strcmp( attrib, "MaxEngineForce" ) )
			{
				sscanf( value, "%f", &m_maxEngineForce );
			}
			else if( !strcmp( attrib, "MaxBreakingForce" ) )
			{
				sscanf( value, "%f", &m_maxBreakingForce );
			}
			else if( !strcmp( attrib, "SteeringIncrement" ) )
			{
				sscanf( value, "%f", &m_steeringIncrement );
			}
			else if( !strcmp( attrib, "SteeringClamp" ) )
			{
				sscanf( value, "%f", &m_steeringClamp );
			}
			else if( !strcmp( attrib, "WheelRadius" ) )
			{
				sscanf( value, "%f", &m_wheelRadius );
			}
			else if( !strcmp( attrib, "WheelWidth" ) )
			{
				sscanf( value, "%f", &m_wheelWidth );
			}
			else if( !strcmp( attrib, "WheelFriction" ) )
			{
				sscanf( value, "%f", &m_wheelFriction );
			}
			else if( !strcmp( attrib, "SuspensionStiffness" ) )
			{
				sscanf( value, "%f", &m_suspensionStiffness );
			}
			else if( !strcmp( attrib, "SuspensionDamping" ) )
			{
				sscanf( value, "%f", &m_suspensionDamping );
			}
			else if( !strcmp( attrib, "SuspensionCompression" ) )
			{
				sscanf( value, "%f", &m_suspensionCompression );
			}
			else if( !strcmp( attrib, "RollInfluence" ) )
			{
				sscanf( value, "%f", &m_rollInfluence );
			}
			else if( !strcmp( attrib, "SuspensionRestLength" ) )
			{
				sscanf( value, "%f", &m_suspensionRestLength );
			}
			else if( !strcmp( attrib, "ConnectionHeight" ) )
			{
				sscanf( value, "%f", &m_connectionHeight );
			}
			else if( !strcmp( attrib, "Mass" ) )
			{
				sscanf( value, "%f", &m_mass );
			}
		}
	}

}