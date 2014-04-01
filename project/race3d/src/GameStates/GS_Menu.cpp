#include "GS_Menu.h"
#include "../Menus/MenuManager.h"
#include "../Menus/MenuElement.h"

//-----------------------------------------------------------------------------------

GS_Menu::GS_Menu()
{
	s_stateID = GameState::k_state_menu;

	m_menuManager = MenuManager::GetInstance();
	m_menuManager->PushMenu( MenuElement::MENUELEMENTTYPE_SPLASH_SCREEN );
}

//-----------------------------------------------------------------------------------

GS_Menu::~GS_Menu()
{
	m_menuManager->FreeInstance();
}

//-----------------------------------------------------------------------------------

void GS_Menu::ResumeState()
{
}

//-----------------------------------------------------------------------------------

void GS_Menu::UpdateState( const s3e_uint64_t frameTime )
{
	switch( s_subState )
	{
	case k_substate_splash_screen:
		{
		}
		break;

	case k_substate_main:
		{
		}
		break;
	}

	m_menuManager->Update( frameTime );
}

//-----------------------------------------------------------------------------------

void GS_Menu::RenderState()
{
	switch( s_subState )
	{
	case k_substate_splash_screen:
		{
		}
		break;

	case k_substate_main:
		{
		}
		break;
	}

	m_menuManager->Render();
}