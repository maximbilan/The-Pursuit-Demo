#ifndef _MENUSOUNDS_H_
#define _MENUSOUNDS_H_

#include "MenuElement.h"

class GUILabel;
class GUIButton;

class Config;

//-----------------------------------------------------------------------------------
// MenuSounds
//-----------------------------------------------------------------------------------
class MenuSounds : public MenuElement
{
public:
	MenuSounds();
	virtual ~MenuSounds();

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
	// Set switch button
	void SetSwitchButton( GUILabel* label, const bool value );

private:
	GUILabel*	m_effectsLabel;
	GUILabel*	m_musicLabel;
	GUILabel*	m_vibrationLabel;

	bool		m_isEffectsBtnPressed;
	bool		m_isMusicBtnPressed;
	bool		m_isVibrationBtnPressed;

	Config*		m_config;
};

#endif	// _MENUSOUNDS_H_