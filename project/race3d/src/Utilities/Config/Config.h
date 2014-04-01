#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

#include "s3e.h"
#include <IwArray.h>

#include "../../Common/Singleton.h"
#include "../../Common/Defines.h"

class RecordDB;
class GameMode;

//-----------------------------------------------------------------------------------------------

typedef CIwArray<GameMode*> GameModeArray;

//-----------------------------------------------------------------------------------------------
// Config - singleton for saving and loading all settings of the game
//-----------------------------------------------------------------------------------------------
class Config : public Singleton<Config>
{
public:
	enum CONFIG_CONTROLS_TYPE
	{
		CCT_STANDART,
		CCT_ACCELEROMETER,

		CCT_COUNT
	};

	enum CONFIG_LANG_CODE
	{
		CLC_ENGLISH		= S3E_DEVICE_LANGUAGE_ENGLISH,
		CLC_RUSSIAN		= S3E_DEVICE_LANGUAGE_RUSSIAN,
		CLC_FRENCH		= S3E_DEVICE_LANGUAGE_FRENCH,
		CLC_GERMAN		= S3E_DEVICE_LANGUAGE_GERMAN,
		CLC_ITALIAN		= S3E_DEVICE_LANGUAGE_ITALIAN,
		CLC_SPAINISH	= S3E_DEVICE_LANGUAGE_SPANISH,

		CLC_COUNT		= 6
	};

public:
	// Initialization
	void Init();

	// Remove the instance
	void Remove();

	// Reset all settings to default values
	void Reset();

	// Saving the settings to binary file
	void Save();

	// Loading the settings from file
	void Load();

	// Checks is config exist
	bool IsExist() const;

	// Get language code
	int	GetLanguageCode() const;

	// Get controls type
	int	GetControlsType() const;

	// Get game mode by id
	GameMode* GetGameMode( const int id );

	// Check is music enabled
	bool IsMusicEnabled() const;

	// Check is sound effects enabled
	bool IsSoundEffectsEnabled() const;

	// Check is vibration enabled
	bool IsVibrationEnabled() const;

	// Check is autogas function enabled
	bool IsAutogasEnabled() const;

	// Check is flip buttons enabled
	bool IsFlipButtonsEnabled() const;

	// Set language code
	void SetLanguageCode( const int languageCode );

	// Set controls type
	void SetControlsType( const int controlsType );

	// Enable or disable music
	void SetMusicEnabled( const bool enable );

	// Enable or disable sound effects
	void SetSoundEffectsEnabled( const bool enable );

	// Enable or disable vibration
	void SetVibrationEnabled( const bool enable );

	// Enable or disable autogas function
	void SetAutogasEnabled( const bool enable );

	// Enable or disable flip buttons
	void SetFlipButtonsEnabled( const bool enable );

private:
	bool			m_isExist;

	int				m_languageCode;
	bool			m_isMusicEnabled;
	bool			m_isSoundEffectsEnabled;
	bool			m_isVibrationEnabled;
	int				m_controlsType;
	bool			m_isAutogasEnabled;
	bool			m_isFlipButtonsEnabled;

	RecordDB*		m_db;

	GameModeArray	m_gameModes;

protected:
	Config() {}
	friend class Singleton<Config>;
};

//---------------------------------------------------------------------------------------------

inline int Config::GetLanguageCode() const
{
	return m_languageCode;
}

//---------------------------------------------------------------------------------------------

inline int Config::GetControlsType() const
{
	return m_controlsType;
}

//---------------------------------------------------------------------------------------------

inline bool Config::IsExist() const
{
	return m_isExist;
}

//---------------------------------------------------------------------------------------------

inline bool Config::IsMusicEnabled() const
{
	return m_isMusicEnabled;
}

//---------------------------------------------------------------------------------------------

inline bool Config::IsSoundEffectsEnabled() const
{
	return m_isSoundEffectsEnabled;
}

//---------------------------------------------------------------------------------------------

inline bool Config::IsVibrationEnabled() const
{
	return m_isVibrationEnabled;
}

//---------------------------------------------------------------------------------------------

inline bool Config::IsAutogasEnabled() const
{
	return m_isAutogasEnabled;
}

//---------------------------------------------------------------------------------------------

inline bool Config::IsFlipButtonsEnabled() const
{
	return m_isFlipButtonsEnabled;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetLanguageCode( const int languageCode )
{
	m_languageCode = languageCode;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetControlsType( const int controlsType )
{
	m_controlsType = controlsType;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetMusicEnabled( const bool enable )
{
	m_isMusicEnabled = enable;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetSoundEffectsEnabled( const bool enable )
{
	m_isSoundEffectsEnabled = enable;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetVibrationEnabled( const bool enable )
{
	m_isVibrationEnabled = enable;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetAutogasEnabled( const bool enable )
{
	m_isAutogasEnabled = enable;
}

//---------------------------------------------------------------------------------------------

inline void Config::SetFlipButtonsEnabled( const bool enable )
{
	m_isFlipButtonsEnabled = enable;
}

#endif	// _CONFIG_H_