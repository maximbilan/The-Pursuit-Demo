#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "IwSound.h"
#include "IwResManager.h"

#include "../../Common/Defines.h"
#include "../../Common/Singleton.h"

class Config;

//-----------------------------------------------------------------------------------------------
// SoundManager - singleton for playing sounds and music
//-----------------------------------------------------------------------------------------------
class SoundManager : public Singleton<SoundManager>
{
public:
	// Initialization
	void Init();

	// Remove the instance
	void Remove();

	// Update
	void Update();

	// Load resources
	void Load();

	// Play menu music
	void PlayMenuMusic();

	// Stop menu music
	void StopMenuMusic();

	// Play game music
	void PlayGameMusic();
	
	// Stop game music
	void StopGameMusic();

	// Play sound effect UI select
	void PlaySFXUiSelect();

	// Stop all sounds
	void StopAllSounds();

private:
	// Get resource
	template <typename T>
	void GetResource( T& res, const char* resName, const char* resType, const uint32 flags = 0 );

protected:
	SoundManager() {}
	friend class Singleton<SoundManager>;

private:
	Config*			m_config;
	CIwResGroup*	m_soundsResGroup;

	CIwSoundGroup*	m_menuMusicGroup;
	CIwSoundSpec*	m_menuMusicSpec;
		
	CIwSoundGroup*	m_gameplayMusicGroup;
	CIwSoundSpec*	m_gameplayMusicSpec;

	CIwSoundSpec*	m_uiSelect;
};

//-----------------------------------------------------------------------------------------------

template <typename T>
void SoundManager::GetResource( T& res, const char* resName, const char* resType, const uint32 flags )
{
	res = static_cast<T>( IwGetResManager()->GetResNamed( resName, resType, flags ) );
}

#endif	// _SOUNDMANAGER_H_