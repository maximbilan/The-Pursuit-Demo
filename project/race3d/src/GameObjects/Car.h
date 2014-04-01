#ifndef _CAR_H_
#define _CAR_H_

//-------------------------------------------------------------------------------------

#include "GameEntity.h"

//-------------------------------------------------------------------------------------

// Calculate slip angle (not completed)
//#define CALCULATE_SLIP_ANGLE

// Print some debug information about the car
//#define PRINT_CAR_DEBUG_INFO

// Length of buffer for XML settings
#define CAR_XML_SETTINGS_BUFFER 4096
#define CAR_SETTINGS_NAME_BUFFER 255

//-------------------------------------------------------------------------------------

class Collision;
class RecordDB;

//-------------------------------------------------------------------------------------
// Car - entity simulates physics of the car and render the car
//-------------------------------------------------------------------------------------
class Car : public GameEntity
{
public:
	enum CAR_COLOR
	{
		CAR_COLOR_RED,
		CAR_COLOR_BLUE,
		CAR_COLOR_GRAY,
		CAR_COLOR_PURPLE,

		CAR_COLOR_COUNT
	};

	static const char* s_carGroupNames[CAR_COLOR_COUNT];
	static const char* s_carSettingsPath;
	static const char* s_carSettingsXMLFormat;
	static const char* s_carSettingsBINFormat;

public:
	Car( const unsigned int color = 0 );
	virtual ~Car();

	// Initialization
	void Init();

	// Reset car parameters
	void Reset();
	
	// Update
	void Update( const s3e_uint64_t dt );

	// Physics update
	void UpdatePhysics( const s3e_uint64_t dt );

	// Render
	void Render();

	// Get amount of polygons
	unsigned int GetPolyCount();

	// Force the car to move
	void Force();

	// Backdraft the car
	void BackDraft();

	// Brake the car
	void Brake();

	// Steering left
	void Left();

	// Steering right
	void Right();

	// Save parameters to binary file
	void SaveBin();

	// Load parameters from binary file
	void LoadBin();

	// Load parameters from xml file
	void LoadXML();

	// Get raycast vehicle
	btRaycastVehicle* GetRaycastVehicle() const;

private:
	std::string							m_groupName;
	std::string							m_modelName;

	btRaycastVehicle::btVehicleTuning	m_vehicleTuning;
	btVehicleRaycaster*					m_vehicleRayCaster;
	btRaycastVehicle*					m_raycastVehicle;

	CIwModel*							m_modelBody;
	CIwFMat								m_modelBodyMatrix;
	Collision*							m_collisionBody;

	CIwModel*							m_modelWheel1;
	CIwFMat								m_modelWheel1Matrix;
	CIwModel*							m_modelWheel2;
	CIwFMat								m_modelWheel2Matrix;
	CIwModel*							m_modelWheel3;
	CIwFMat								m_modelWheel3Matrix;
	CIwModel*							m_modelWheel4;
	CIwFMat								m_modelWheel4Matrix;

	btCompoundShape*					m_compoundShape;

	int									m_rightIndex;
	int									m_upIndex;
	int									m_forwardIndex;
	btVector3							m_wheelDirectionCS0;
	btVector3							m_wheelAxleCS;

	///btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
	///notice that for higher-quality slow-moving vehicles, another approach might be better
	///implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts
	float								m_gEngineForce;
	float								m_gBreakingForce;

	float								m_maxEngineForce;		 // this should be engine/velocity dependent
	float								m_maxBreakingForce;
	
	float								m_gVehicleSteering;
	float								m_steeringIncrement;
	float								m_steeringClamp;
	float								m_wheelRadius;
	float								m_wheelWidth;
	float								m_wheelFriction;
	float								m_suspensionStiffness;
	float								m_suspensionDamping;
	float								m_suspensionCompression;
	float								m_rollInfluence;
	float								m_suspensionRestLength;
	float								m_connectionHeight;

	bool								m_isForce;
	bool								m_isBackDraft;

	CIwGxShaderTechnique*				m_shaderTechnique;
	CIwGxShaderUniform*					m_shaderUniform;

	char								m_settingsFileXML[CAR_SETTINGS_NAME_BUFFER];
	char								m_settingsFileBIN[CAR_SETTINGS_NAME_BUFFER];
	RecordDB*							m_settingsDB;
};

//-------------------------------------------------------------------------------------

inline btRaycastVehicle* Car::GetRaycastVehicle() const
{
	return m_raycastVehicle;
}

#endif	// _CAR_H_