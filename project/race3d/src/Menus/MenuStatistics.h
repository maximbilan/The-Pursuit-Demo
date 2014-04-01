#ifndef _MENUSTATISTICS_H_
#define _MENUSTATISTICS_H_

#include "MenuElement.h"

class GUILabel;
class Config;

//---------------------------------------------------------------------------------
// MenuStatistics
//---------------------------------------------------------------------------------
class MenuStatistics : public MenuElement
{
public:
	MenuStatistics();
	virtual ~MenuStatistics();

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
	void ShowTrainingStatistics();
	void ShowTimeAttackStatistics();

private:
	// Training mode statistics
	GUILabel*	m_labelTrainingTimeSpent;
	GUILabel*	m_labelTrainingMaxSpeed;

	// Time attack mode statistics
	GUILabel*	m_labelTimeAttackBestTime;
	GUILabel*	m_labelTimeAttackLaps;

	Config*		m_config;
};

#endif	// _MENUSTATISTICS_H_