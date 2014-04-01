#ifndef _MENUGAMEGUI_H_
#define _MENUGAMEGUI_H_

#include "MenuElement.h"
#include "MenuTouch.h"

class Config;
class GameMode;
class FontManager;

//-----------------------------------------------------------------------------------
// MenuGameGui - game ui
//-----------------------------------------------------------------------------------
class MenuGameGui : public MenuElement, public MenuTouch
{
public:
	enum EMenuGameGuiBtnEvent
	{
		MGGBE_UP,
		MGGBE_DOWN,
		MGGBE_LEFT,
		MGGBE_RIGHT,

		MGGBE_COUNT
	};

	enum EMenuGameGuiTypeControls
	{
		MGGBE_TC_STANDART,
		MGGBE_TC_STANDART_FLIP,
		MGGBE_TC_ACCELEROMETER,
		MGGBE_TC_ACCELEROMETER_FLIP,

		MGGBE_TC_COUNT
	};

private:
	enum EMenuGameGuiIndicators
	{
		MGGI_BEST_TIME_LABEL,
		MGGI_BEST_TIME_VALUE,
		MGGI_TIME_LABEL,
		MGGI_TIME_VALUE,
		MGGI_LAP_LABEL,
		MGGI_LAP_VALUE,
		
		MGGI_COUNT
	};

	static const int s_rectWidth;
	static const int s_rectHeight;

public:
	MenuGameGui();
	virtual ~MenuGameGui();

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

	// Get button event value
	bool GetBtnEventValue( const unsigned int e ) const;

	// Reset button event
	void ResetBtnEvent( const unsigned int e );

	// Set game mode id
	void SetGameModeId( const int id );

	// Set game mode pointer
	void SetGameMode( GameMode* gameMode );

private:
	// Handle touches
	void HandleTouches( const Touch* touch );

	// On touch button event
	void CheckOnTouchButton( const CIwRect rect, const unsigned buttonId );

	// Reset touches
	void ResetTouches();

	// Set visibility for controls
	void SetVisibleControls( const int controlsType, const bool flip );

	// Set visibility of gameplay indicators
	void SetVisibleIndicators( const bool visible );

private:
	bool*			m_btnEvents;

	int				m_gameModeId;
	GameMode*		m_gameMode;

	GUIButton*		m_upBtn;
	GUIButton*		m_downBtn;
	GUIButton*		m_leftBtn;
	GUIButton*		m_rightBtn;

	GUIButton*		m_controlButtons[MGGBE_TC_COUNT][MGGBE_COUNT];
	GUILabel*		m_indicators[MGGI_COUNT];

	Config*			m_config;

	int				m_laps;

	FontManager*	m_fontManager;
	CIwGxFontPreparedData	m_currentTimePreparedText;
	CIwGxFontPreparedData	m_bestTimePreparedText;
};

//-----------------------------------------------------------------------------------

inline void MenuGameGui::ResetBtnEvent( const unsigned int e )
{
	if( e >= 0 && e < MGGBE_COUNT )
	{
		m_btnEvents[e] = false;
	}
}

//-----------------------------------------------------------------------------------

inline bool MenuGameGui::GetBtnEventValue( const unsigned int e ) const
{
	if( e >= 0 && e < MGGBE_COUNT )
	{
		return m_btnEvents[e];
	}

	return false;
}

//-----------------------------------------------------------------------------------

inline void MenuGameGui::SetGameModeId( const int id )
{
	m_gameModeId = id;
}

//-----------------------------------------------------------------------------------

inline void MenuGameGui::SetGameMode( GameMode* gameMode )
{
	m_gameMode = gameMode;
}

#endif	// _MENUGAMEGUI_H_