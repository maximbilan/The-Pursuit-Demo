#include "Config.h"

#include "../../Engine/Record/RecordDB.h"

#include "../../GameLogic/GameMode/GameMode.h"
#include "../../GameLogic/GameMode/GM_Training.h"
#include "../../GameLogic/GameMode/GM_TimeAttack.h"

//---------------------------------------------------------------------------------------------

void Config::Init()
{
	m_db = new RecordDB( CONFIG_COMMON_NAME );

	GM_Training* m_trainingMode = new GM_Training();
	GM_TimeAttack* m_timeAttackMode = new GM_TimeAttack();

	m_gameModes.push_back( m_trainingMode );
	m_gameModes.push_back( m_timeAttackMode );

	if( s3eFileCheckExists( CONFIG_COMMON_NAME ) )
	{
		Load();
	}
	else
	{
		m_isExist = false;

		Reset();

		m_db->AddRecord( "language_code", m_languageCode );
		m_db->AddRecord( "is_music_enabled", m_isMusicEnabled );
		m_db->AddRecord( "is_sound_effects_enabled", m_isSoundEffectsEnabled );
		m_db->AddRecord( "is_vibration_enabled", m_isVibrationEnabled );
		m_db->AddRecord( "controls_type", m_controlsType );
		m_db->AddRecord( "is_autogas_enabled", m_isAutogasEnabled );
		m_db->AddRecord( "is_flip_buttons_enabled", m_isFlipButtonsEnabled );
		
		for( GameModeArray::const_iterator it = m_gameModes.begin(), end = m_gameModes.end(); it != end; ++it )
		{
			( *it )->Create( *m_db );
		}

		Save();
	}
}

//---------------------------------------------------------------------------------------------

void Config::Reset()
{
	m_languageCode = s3eDeviceGetInt( S3E_DEVICE_LANGUAGE );
	m_isMusicEnabled = true;
	m_isSoundEffectsEnabled = true;
	m_isVibrationEnabled = false;
	m_controlsType = 0;
	m_isAutogasEnabled = false;
	m_isFlipButtonsEnabled = false;

	for( GameModeArray::const_iterator it = m_gameModes.begin(), end = m_gameModes.end(); it != end; ++it )
	{
		( *it )->Reset();
	}
}

//---------------------------------------------------------------------------------------------

void Config::Remove()
{
	for( GameModeArray::const_iterator it = m_gameModes.begin(), end = m_gameModes.end(); it != end; ++it )
	{
		delete ( *it );
	}

	delete m_db;
}

//---------------------------------------------------------------------------------------------

void Config::Save()
{
	m_db->SetRecord( "language_code", m_languageCode );
	m_db->SetRecord( "is_music_enabled", m_isMusicEnabled );
	m_db->SetRecord( "is_sound_effects_enabled", m_isSoundEffectsEnabled );
	m_db->SetRecord( "is_vibration_enabled", m_isVibrationEnabled );
	m_db->SetRecord( "controls_type", m_controlsType );
	m_db->SetRecord( "is_autogas_enabled", m_isAutogasEnabled );
	m_db->SetRecord( "is_flip_buttons_enabled", m_isFlipButtonsEnabled );

	for( GameModeArray::const_iterator it = m_gameModes.begin(), end = m_gameModes.end(); it != end; ++it )
	{
		( *it )->Save( *m_db );
	}

	m_db->Save();
	m_isExist = true;
}

//---------------------------------------------------------------------------------------------

void Config::Load()
{
	m_db->Load();
	m_languageCode = m_db->GetRecordAsInt( "language_code" );
	m_isMusicEnabled = m_db->GetRecordAsBool( "is_music_enabled" );
	m_isSoundEffectsEnabled = m_db->GetRecordAsBool( "is_sound_effects_enabled" );
	m_isVibrationEnabled = m_db->GetRecordAsBool( "is_vibration_enabled" );
	m_controlsType = m_db->GetRecordAsInt( "controls_type" );
	m_isAutogasEnabled = m_db->GetRecordAsBool( "is_autogas_enabled" );
	m_isFlipButtonsEnabled = m_db->GetRecordAsBool( "is_flip_buttons_enabled" );

	for( GameModeArray::const_iterator it = m_gameModes.begin(), end = m_gameModes.end(); it != end; ++it )
	{
		( *it )->Load( *m_db );
	}

	m_isExist = true;
}

//---------------------------------------------------------------------------------------------

GameMode* Config::GetGameMode( const int id )
{
	GameMode* gameMode = NULL;

	if( id > GameMode::GM_ID_NONE && id < GameMode::GM_ID_COUNT )
	{
		for( GameModeArray::const_iterator it = m_gameModes.begin(), end = m_gameModes.end(); it != end; ++it )
		{
			if( ( *it )->GetID() == id )
			{
				gameMode = *it;
				break;
			}
		}
	}

	return gameMode;
}