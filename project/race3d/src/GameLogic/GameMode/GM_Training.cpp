#include "GM_Training.h"

#include "s3e.h"

#include "../../GameObjects/Car.h"
#include "../../GameLogic/Actor/Player.h"
#include "../../Engine/Record/RecordDB.h"

//-------------------------------------------------------------------------------------

GM_Training::GM_Training() :
	m_time( 0.0f ),
	m_startTime( 0 ),
	m_distance( 0.0f ),
	m_maxSpeed( 0.0f )
{
	m_state = GMS_ACTIVE;
	m_id = GM_ID_TRAINING;
}

//-------------------------------------------------------------------------------------

GM_Training::~GM_Training()
{
}

//-------------------------------------------------------------------------------------

void GM_Training::Init( Player* player )
{
	GameMode::Init( player );

	m_startTime = s3eTimerGetMs();
}

//-------------------------------------------------------------------------------------

void GM_Training::Resume()
{
	m_state = GMS_ACTIVE;
	m_startTime = s3eTimerGetMs();
}

//-------------------------------------------------------------------------------------

void GM_Training::Suspend()
{
	m_state = GMS_SUSPENDED;

	const uint64 currentTime = s3eTimerGetMs();
	const double min = ( ( currentTime - m_startTime ) * 0.001 ) / 60;
	m_time += min;
}

//-------------------------------------------------------------------------------------

void GM_Training::Update( const s3e_uint64_t dt )
{
	const float speed = static_cast<float>( m_player->GetCar()->GetRaycastVehicle()->getCurrentSpeedKmHour() * 0.01 );
	if( speed > m_maxSpeed )
	{
		m_maxSpeed = speed;
	}
}

//-------------------------------------------------------------------------------------

void GM_Training::Save( RecordDB &db )
{
	db.SetRecord( "gm_training_time", m_time );
	//db.setRecord( "gm_training_distance", m_distance );
	db.SetRecord( "gm_training_max_speed", m_maxSpeed );
}

//-------------------------------------------------------------------------------------

void GM_Training::Load( RecordDB &db )
{
	m_time = db.GetRecordAsDouble( "gm_training_time" );
	//m_distance = db.getRecordAsDouble( "gm_training_distance" );
	m_maxSpeed = db.GetRecordAsFloat( "gm_training_max_speed" );
}

//-------------------------------------------------------------------------------------

void GM_Training::Create( RecordDB &db )
{
	db.AddRecord( "gm_training_time", m_time );
	//db.addRecord( "gm_training_distance", m_distance );
	db.AddRecord( "gm_training_max_speed", m_maxSpeed );
}

//-------------------------------------------------------------------------------------

void GM_Training::Reset()
{
	m_time = 0.0f;
	m_distance = 0.0f;
	m_maxSpeed = 0.0f;
}