#ifndef _GS_MAINMENU_H_
#define _GS_MAINMENU_H_

#include "GameState.h"

class Timer;
class MenuManager;

//-----------------------------------------------------------------------------------
// GS_Menu - gamestate for rendering and updating menus
//-----------------------------------------------------------------------------------
class GS_Menu : public GameState
{
public:
	static const int k_substate_none			= 0;
	static const int k_substate_splash_screen	= 1;
	static const int k_substate_main			= 2;

public:
	GS_Menu();
	virtual ~GS_Menu();

	// Render state
	void ResumeState();

	// Update State
	void UpdateState( const s3e_uint64_t frameTime );

	// Render state
	void RenderState();

private:
	MenuManager*	m_menuManager;
};

#endif	// _GS_MAINMENU_H_