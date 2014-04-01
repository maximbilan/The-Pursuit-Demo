#ifndef _MENUGAMEMODE_H_
#define _MENUGAMEMODE_H_

#include "MenuElement.h"

class Popup;

//-----------------------------------------------------------------------------------
// Menu Game Mode - menu for choosing game mode
//-----------------------------------------------------------------------------------
class MenuGameMode : public MenuElement
{
public:
	MenuGameMode();
	virtual ~MenuGameMode();

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

	// Get current car frame
	unsigned int GetCurrentCarFrame() const;

private:
	// Set current visible car frame
	void SetCurrentVisibleCarFrame();

private:
	FrameArray		m_carFrames;
	unsigned int	m_currentCarFrame;
	Popup*			m_noAvailableFeaturePopup;
};

//-----------------------------------------------------------------------------------

inline unsigned int MenuGameMode::GetCurrentCarFrame() const
{
	return m_currentCarFrame;
}

#endif	// _MENUGAMEMODE_H_