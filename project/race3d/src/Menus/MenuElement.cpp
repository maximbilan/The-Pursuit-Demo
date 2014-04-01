#include "MenuElement.h"
#include "MenuManager.h"

//-----------------------------------------------------------------------------------

MenuElement::MenuElement()
{
	m_menuManager = MenuManager::GetInstance();
	m_soundManager = SoundManager::GetInstance();
}

//-----------------------------------------------------------------------------------

MenuElement::~MenuElement()
{
	m_soundManager->FreeInstance();
	m_menuManager->FreeInstance();
}

//-----------------------------------------------------------------------------------

void MenuElement::OnShow()
{
	m_frame->SetEnabled( true );

	GetGUIManager()->ClearAllFrames();
	GetGUIManager()->AddFrame( m_frame );
}

//-----------------------------------------------------------------------------------

void MenuElement::OnHide()
{
	m_frame->SetEnabled( false );
}