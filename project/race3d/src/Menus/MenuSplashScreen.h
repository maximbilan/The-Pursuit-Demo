#ifndef _MENUSPLASHSCREEN_H_
#define _MENUSPLASHSCREEN_H_

#include "MenuElement.h"
#include "MenuTouch.h"

class FontManager;
class PostProcessing;

//---------------------------------------------------------------------------------
// MenuSplashScreen
//---------------------------------------------------------------------------------
class MenuSplashScreen : public MenuElement, public MenuTouch
{
private:
	static const int unsigned	s_touchTextAlphaRestart;
	static const int			s_touchTextAlphaDescrease;

public:
	MenuSplashScreen();
	virtual ~MenuSplashScreen();

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
	// Handle touches
	void HandleTouches( const Touch* touch );

private:
	FontManager*	m_fontManager;
	PostProcessing*	m_postProcessing;

	uint8			m_touchTextAlpha;
	uint64			m_touchTextStartTime;
};

#endif	// _MENUSPLASHSCREEN_H_