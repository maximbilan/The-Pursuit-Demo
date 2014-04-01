#ifndef _MENUOPTIONS_H_
#define _MENUOPTIONS_H_

#include "MenuElement.h"

//-----------------------------------------------------------------------------------
// Menu options
//-----------------------------------------------------------------------------------
class MenuOptions : public MenuElement
{
public:
	MenuOptions();
	virtual ~MenuOptions();

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
};

#endif	// _MENUOPTIONS_H_