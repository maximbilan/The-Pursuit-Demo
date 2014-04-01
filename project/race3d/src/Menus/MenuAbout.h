#ifndef _MENUABOUT_H_
#define _MENUABOUT_H_

#include "IwGxFont.h"

#include "MenuElement.h"

class FontManager;
class TouchManager;
class SmoothScroll;

//-----------------------------------------------------------------------------------
// Menu About - constains information about authors and game
//-----------------------------------------------------------------------------------
class MenuAbout : public MenuElement
{
public:
	MenuAbout();
	virtual ~MenuAbout();

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
	// Prepare titles
	void PrepareTitles();

private:
	FontManager*			m_fontManager;
	TouchManager*			m_touchManager;
	SmoothScroll*			m_smoothScroll;
	CIwGxFontPreparedData	m_preparedText;
	int						m_titlesX;
	int						m_titlesY;
};

#endif	// _MENUABOUT_H_