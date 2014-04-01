#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "IwModel.h"
#include "IwGeomFMat.h"

#include "MenuElement.h"

//#define DEBUG_3D_SCENE

class Popup;
class WebManager;

//-----------------------------------------------------------------------------------
// MainMenu
//-----------------------------------------------------------------------------------
class MainMenu : public MenuElement
{
public:
	MainMenu();
	virtual ~MainMenu();

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
	Popup*		m_quitPopup;
	Popup*		m_noAvailableFeaturePopup;
	WebManager*	m_webManager;
#ifdef DEBUG_3D_SCENE
	CIwModel*	m_model;
	CIwFMat		m_modelMatrix;
#endif
};

#endif	// _MAINMENU_H_