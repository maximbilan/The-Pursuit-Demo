#ifndef _MENUELEMENT_H_
#define _MENUELEMENT_H_

#include <string>

#include "../GUI/GUIButton.h"
#include "../GUI/GUILabel.h"
#include "../Utilities/SoundManager/SoundManager.h"

class GUIFrame;
class MenuManager;

//-----------------------------------------------------------------------------------

typedef CIwArray<GUIFrame*> FrameArray;

//-----------------------------------------------------------------------------------
// Menu element - base class for menus
//-----------------------------------------------------------------------------------
class MenuElement : public GUIButtonEventHandler
{
public:
	enum MENUELEMENTTYPE
	{
		MENUELEMENTTYPE_NONE			= -1,
		MENUELEMENTTYPE_MAIN_MENU,
		MENUELEMENTTYPE_GAME_GUI,
		MENUELEMENTTYPE_ABOUT,
		MENUELEMENTTYPE_OPTIONS,
		MENUELEMENTTYPE_SOUNDS,
		MENUELEMENTTYPE_LANGUAGES,
		MENUELEMENTTYPE_CONTROLS,
		MENUELEMENTTYPE_SPLASH_SCREEN,
		MENUELEMENTTYPE_GAME_MODE,
		MENUELEMENTTYPE_IN_GAME,
		MENUELEMENTTYPE_STATISTICS,

		MENUELEMENTTYPE_COUNT
	};

public:
	MenuElement();
	virtual ~MenuElement();

	// Initialization
	virtual void Init() = 0;

	// Render
	virtual void Render() = 0;

	// Update
	virtual void Update( const s3e_uint64_t dt ) = 0;

	// Event, when push menu to array
	virtual void OnShow();

	// Event, when pop menu from array
	virtual void OnHide();

	// Event when user button is pressed
	virtual bool OnButtonPressed( const GUIButton* apButton ) = 0;

	// Event when user button is released
	virtual bool OnButtonReleased( const GUIButton* apButton ) = 0;

	// Get type ID
	int GetTypeId() const;

protected:
	// Get element by name
	template<typename T>
	void GetElementByName( T&, const char* name );

protected:
	int				m_typeId;

	GUIFrame*		m_frame;

	MenuManager*	m_menuManager;
	SoundManager*	m_soundManager;
};

//-----------------------------------------------------------------------------------

inline int MenuElement::GetTypeId() const
{
	return m_typeId;
}

//-----------------------------------------------------------------------------------

template<typename T>
void MenuElement::GetElementByName( T& element, const char* name )
{
	element = static_cast<T>( GetGUIManager()->FindElementByName( name ) );
}

#endif	// _MENUELEMENT_H_