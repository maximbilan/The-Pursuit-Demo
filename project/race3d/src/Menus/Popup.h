#ifndef _POPUP_H_
#define _POPUP_H_

#include "s3eTypes.h"

class GUIFrame;
class GUIButton;
class GUILabel;

//---------------------------------------------------------------------------------
// Popup - ok popup and question popup with yes and no
//---------------------------------------------------------------------------------
class Popup
{
public:
	enum POPUP_TYPE
	{
		PT_QUESTION,
		PT_OK
	};

	enum POPUP_QUESTION_RESULT
	{
		PQ_NONE	= -1,
		PQ_NO,
		PQ_YES
	};

	enum POPUP_BUTTON_STATUS
	{
		PB_NONE		= -1,
		PB_PRESSED,
		PB_RELEASED
	};

public:
	Popup( const unsigned int type = PT_QUESTION );
	~Popup();

	// Event when user button is pressed
	void OnButtonPressed( const GUIButton* apButton );

	// Event when user button is released
	void OnButtonReleased( const GUIButton* apButton );

	// Reset all the settings
	void Reset();

	// Show
	void Show();

	// Hide
	void Hide();

	// Set text
	void SetString( const char* str );

	// Get question result
	unsigned int GetQuestionResult() const;

	// Get button status
	unsigned int GetButtonStatus() const;

	// Check is enabled
	bool IsEnabled() const;

private:
	unsigned int	m_type;
	GUIFrame*		m_frame;
	GUILabel*		m_label;
	unsigned int	m_questionResult;
	unsigned int	m_buttonStatus;
	bool			m_isEnabled;
};

//---------------------------------------------------------------------------------

inline unsigned int Popup::GetQuestionResult() const
{
	return m_questionResult;
}

//---------------------------------------------------------------------------------

inline unsigned int Popup::GetButtonStatus() const
{
	return m_buttonStatus;
}

//---------------------------------------------------------------------------------

inline bool Popup::IsEnabled() const
{
	return m_isEnabled;
}

#endif	// _POPUP_H_