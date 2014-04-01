//-----------------------------------------------------------------------------
// GUIButton.h
// Button that responds to user input
//-----------------------------------------------------------------------------

#ifndef GUIBUTTON_H
#define GUIBUTTON_H

// Project includes
#include "GUIEventManager.h"
#include "GUIFrame.h"
#include "GUIManager.h"

//-----------------------------------------------------------------------------
// GUIButtonEventManager
//-----------------------------------------------------------------------------
class GUIButtonEventManager : public GUISingleton<GUIButtonEventManager>, public GUITouchEventHandler
{
public:
	GUIButtonEventManager();

	//-------------------------------------------------------------------------
	// GUITouchEventHandler virtuals
	//-------------------------------------------------------------------------
	virtual bool OnTouchPressed( const GUIElement* apElement, const CIwSVec2& arTouchPos );
	virtual bool OnTouchMoved( const GUIElement* apElement, const CIwSVec2& arTouchPos, const CIwSVec2& arTouchDelta );
	virtual bool OnTouchReleased( const GUIElement* apElement, const CIwSVec2& arTouchPos );
};

GUISINGLETON_GET( GUIButtonEventManager );

//-----------------------------------------------------------------------------
// GUIButton
//-----------------------------------------------------------------------------
class GUIButton : public GUIFrame
{
public:
	IW_MANAGED_DECLARE( GUIButton );
	GUIButton();

	//-------------------------------------------------------------------------
	// Virtual overrides from CIwManaged
	//-------------------------------------------------------------------------
	virtual void Serialise();
	virtual void Resolve();

	//-------------------------------------------------------------------------
	// Virtual overrides from GUIElement
	//-------------------------------------------------------------------------
	// Clone this element
	virtual void Clone( GUIElement* apCloneSource );

	// Called once per frame to render
	virtual void Render();

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------
	bool IsPressed() const;

private:
	//-------------------------------------------------------------------------
	// Member variables
	//-------------------------------------------------------------------------

	GUIFrame*	m_pUpFrame;
	GUIFrame*	m_pDownFrame;
	GUIFrame*	m_pDisabledFrame;
};

//-----------------------------------------------------------------------------
// Button pressed/released events
//-----------------------------------------------------------------------------
#define EVENT_BUTTON		"ButtonEvent"

//-----------------------------------------------------------------------------
// GUIButtonEventHandler
// Classes should inherit from this to handle button press events
//-----------------------------------------------------------------------------
class GUIButtonEventHandler : public virtual GUIEventHandler
{
public:
	virtual bool OnButtonPressed( const GUIButton* apButton ) = 0;
	virtual bool OnButtonReleased( const GUIButton* apButton ) = 0;
};

//-----------------------------------------------------------------------------
// GUIButtonEvent
// Deals with calling the correct event handler for the button press event
//-----------------------------------------------------------------------------
class GUIButtonEvent : public GUIEvent
{
public:
	GUIButtonEvent( GUIButton* apButton, bool aReleased ) :
		m_pButton( apButton ),
		m_Released( aReleased )
	{
	}

	virtual bool Execute( GUIEventHandler* apHandler )
	{
		GUIButtonEventHandler* lpButtonHandler = dynamic_cast<GUIButtonEventHandler*>( apHandler );
		if( m_Released )
		{
			if( lpButtonHandler->OnButtonReleased( m_pButton ) )
				return true;
		}
		else
		{
			if( lpButtonHandler->OnButtonPressed( m_pButton ) )
				return true;
		}
		return false;
	}

private:
	GUIButton*	m_pButton;
	bool		m_Released;
};

//---------------------------------------------------------------------------

inline bool GUIButton::IsPressed() const
{
	return TestFlags( GF_BUTTON_PRESSED );
}

#endif