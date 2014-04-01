#ifndef _MENULANGUAGES_H_
#define _MENULANGUAGES_H_

#include "MenuElement.h"

class Config;

//-----------------------------------------------------------------------------------
// MenuLanguages
//-----------------------------------------------------------------------------------
class MenuLanguages : public MenuElement
{
public:
	MenuLanguages();
	virtual ~MenuLanguages();

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
	// On language changed event
	void OnLanguageChanged();

private:
	int		m_currentLanguage;
	Config*	m_config;
};

#endif	// _MENULANGUAGES_H_