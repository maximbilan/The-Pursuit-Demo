#ifndef _MENUCONTROLS_H_
#define _MENUCONTROLS_H_

#include "MenuElement.h"

class GUILabel;
class GUIFrame;

class Config;

//-----------------------------------------------------------------------------------
// Menu controls
//-----------------------------------------------------------------------------------
class MenuControls : public MenuElement
{
private:
	static const char* s_controlsTypesIds[];

public:
	MenuControls();
	virtual ~MenuControls();

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

	// Event when controls is changing
	void OnControlsChanged();

private:
	GUILabel*		m_autogasLabel;
	GUILabel*		m_flipLabel;
	GUILabel*		m_elementNameLabel;

	bool			m_isAutogasPressed;
	bool			m_isFlipPressed;

	unsigned int	m_currentControlId;

	FrameArray		m_controlsFrames;

	Config*			m_config;
};

#endif	// _MENUCONTROLS_H_