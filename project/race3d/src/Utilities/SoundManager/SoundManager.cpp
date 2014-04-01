#include "SoundManager.h"

#include "../Config/Config.h"

//---------------------------------------------------------------------------------------------

void SoundManager::Init()
{
	m_config = Config::GetInstance();
	IwSoundInit();
}

//---------------------------------------------------------------------------------------------

void SoundManager::Update()
{
	IwGetSoundManager()->Update();
}

//---------------------------------------------------------------------------------------------

void SoundManager::Remove()
{
	IwSoundTerminate();
	m_config->FreeInstance();
}

//---------------------------------------------------------------------------------------------

void SoundManager::Load()
{
	m_soundsResGroup = IwGetResManager()->LoadGroup( SOUNDS_GROUP_PATH );

	GetResource<CIwSoundGroup*>( m_menuMusicGroup, "menu_music", "CIwSoundGroup" );
	GetResource<CIwSoundSpec*>( m_menuMusicSpec, "mainmenu", "CIwSoundSpec" );
	
	GetResource<CIwSoundGroup*>( m_gameplayMusicGroup, "gameplay_group", "CIwSoundGroup" );
	GetResource<CIwSoundSpec*>( m_gameplayMusicSpec, "gameplay", "CIwSoundSpec" );

	GetResource<CIwSoundSpec*>( m_uiSelect, "select", "CIwSoundSpec", IW_RES_PERMIT_NULL_F );
}

//---------------------------------------------------------------------------------------------

void SoundManager::PlayMenuMusic()
{
	if( m_menuMusicSpec && m_config->IsMusicEnabled() )
	{
		m_menuMusicSpec->Play();
	}
}

//---------------------------------------------------------------------------------------------

void SoundManager::StopMenuMusic()
{
	if( m_menuMusicGroup )
	{
		m_menuMusicGroup->Stop();
	}
}

//---------------------------------------------------------------------------------------------

void SoundManager::PlayGameMusic()
{
	if( m_gameplayMusicSpec && m_config->IsMusicEnabled() )
	{
		m_gameplayMusicSpec->Play();
	}
}

//---------------------------------------------------------------------------------------------

void SoundManager::StopGameMusic()
{
	if( m_gameplayMusicGroup )
	{
		m_gameplayMusicGroup->Stop();
	}
}

//---------------------------------------------------------------------------------------------

void SoundManager::PlaySFXUiSelect()
{
	if( m_uiSelect && m_config->IsSoundEffectsEnabled() )
	{
		m_uiSelect->Play();
	}
}

//---------------------------------------------------------------------------------------------

void SoundManager::StopAllSounds()
{
	StopMenuMusic();
	StopGameMusic();
}