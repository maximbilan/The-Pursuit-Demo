#include "GM_TimeAttack.h"

#include "s3eTimer.h"

#include "../../Engine/Record/RecordDB.h"
#include "../../Utilities/Config/Config.h"

//-------------------------------------------------------------------------------------

GM_TimeAttack::GM_TimeAttack() :
	m_laps( 0 ),
	m_currentLap( 0 ),
	m_startTime( 0 ),
	m_pauseTime( 0 ),
	m_currentTime( 0.0f ),
	m_bestTime( 0.0f )
{
	m_state = GMS_ACTIVE;
	m_id = GM_ID_TIME_ATTACK;
}

//-------------------------------------------------------------------------------------

GM_TimeAttack::~GM_TimeAttack()
{
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Init( Player* player )
{
	GameMode::Init( player );

	m_currentLap = 0;
	m_startTime = 0;
	m_pauseTime = 0;
	m_currentTime = 0.0f;
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Resume()
{
	m_state = GMS_ACTIVE;

	StartLap();
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Suspend()
{
	m_state = GMS_SUSPENDED;
	m_pauseTime = s3eTimerGetMs();
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Update( const s3e_uint64_t dt )
{
	if( m_state == GMS_ACTIVE )
	{
		m_currentTime = static_cast<double>( s3eTimerGetMs() - m_startTime );
	}
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Save( RecordDB &db )
{
	db.SetRecord( "gm_time_attack_best_time", m_bestTime );
	db.SetRecord( "gm_time_attack_laps", m_laps );
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Load( RecordDB &db )
{
	m_bestTime = db.GetRecordAsDouble( "gm_time_attack_best_time" );
	m_laps = db.GetRecordAsInt( "gm_time_attack_laps" );
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Create( RecordDB &db )
{
	db.AddRecord( "gm_time_attack_best_time", m_bestTime );
	db.AddRecord( "gm_time_attack_laps", m_laps );
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::Reset()
{
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::StartLap()
{
	m_startTime = m_startTime + ( s3eTimerGetMs() - m_pauseTime );
}

//-------------------------------------------------------------------------------------

void GM_TimeAttack::FinishLap()
{
	++m_currentLap;
	++m_laps;

	if( ( m_currentTime < m_bestTime || m_bestTime == 0 ) && m_currentTime > 0.0f )
	{
		m_bestTime = m_currentTime;
	}

	Config* config = Config::GetInstance();
	config->Save();
	config->FreeInstance();
	
	m_startTime = 0;
	m_pauseTime = 0;
	m_currentTime = 0.0f;

	StartLap();
}