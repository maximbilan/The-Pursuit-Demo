#include "AccelerometerManager.h"
#include "s3eAccelerometer.h"

//---------------------------------------------------------------------------------------

void AccelerometerManager::Init()
{
	m_smoothed.x = m_smoothed.y = m_smoothed.z = 0;
	m_smoothFactor = ( IW_GEOM_ONE * 50 ) / 100;

	m_available = ( s3eAccelerometerGetInt( S3E_ACCELEROMETER_AVAILABLE ) != 0 );
	m_started = false;
}

//---------------------------------------------------------------------------------------

void AccelerometerManager::Start()
{
	if( m_available && !m_started )
	{
		s3eAccelerometerStart();
		m_started = true;
	}
}

//---------------------------------------------------------------------------------------

void AccelerometerManager::Stop()
{
	if( m_available && m_started )
	{
		s3eAccelerometerStop();
		m_started = false;
	}
}

//---------------------------------------------------------------------------------------

void AccelerometerManager::Update()
{
	// Read current accelerometer inputs and smooth them, if active
	if( m_started )
	{
		// Get new raw inputs from accelerometer
		const int32 lRawX = s3eAccelerometerGetX();
		const int32 lRawY = s3eAccelerometerGetY();
		const int32 lRawZ = s3eAccelerometerGetZ();

		// Combine into the smoothed values
		m_smoothed.x += IW_FIXED_MUL( m_smoothFactor, lRawX - m_smoothed.x );
		m_smoothed.y += IW_FIXED_MUL( m_smoothFactor, lRawY - m_smoothed.y );
		m_smoothed.z += IW_FIXED_MUL( m_smoothFactor, lRawZ - m_smoothed.z );
	}
}