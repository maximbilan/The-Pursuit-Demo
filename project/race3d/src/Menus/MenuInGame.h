#ifndef _MENUINGAME_H_
#define _MENUINGAME_H_

#include "MenuElement.h"

class Popup;

//-----------------------------------------------------------------------------------
// MenuInGame
//-----------------------------------------------------------------------------------
class MenuInGame : public MenuElement
{
public:
	MenuInGame();
	virtual ~MenuInGame();

	// Initialization
	void Init();

	// Render
	void Render();

	// Update
	void Update( const s3e_uint64_t dt );
		
	// Event, when push menu to array
	void OnShow();

	// Event, when pop menu from array
	void OnHide();

	// Event when user button is pressed
	bool OnButtonPressed( const GUIButton* apButton );

	// Event when user button is released
	bool OnButtonReleased( const GUIButton* apButton );

private:
	Popup*	m_quitToMenuPopup;
};

#endif	// _MENUINGAME_H_