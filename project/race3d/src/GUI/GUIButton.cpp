//-----------------------------------------------------------------------------
// GUIButton.h
// Button that responds to user input
//-----------------------------------------------------------------------------

#include "GUIButton.h"

// Library includes
#include "IwGx.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIDraw.h"
#include "GUIEventManager.h"
#include "GUIFrame.h"
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY(GUIButton);

//-----------------------------------------------------------------------------
// GUIButtonEventManager
//-----------------------------------------------------------------------------
GUIButtonEventManager::GUIButtonEventManager()
{
	IW_CALLSTACK("GUIButtonEventManager::GUIButtonEventManager");
	GetGUIEventManager()->RegisterEvent(EVENT_TOUCH, this);
}

//-----------------------------------------------------------------------------
// Check if touch pressed event is related to a GUIButton
//-----------------------------------------------------------------------------
bool GUIButtonEventManager::OnTouchPressed(const GUIElement* apElement, const CIwSVec2& arTouchPos)
{
	IW_CALLSTACK("GUIButtonEventManager::OnTouchPressed");

	if (apElement->TestFlags(GUIElement::GF_BUTTON))
	{
		// Urgh, casting away const.  Still, it's better to do this than make
		// the apElement pointer non-const.
		GUIElement* lpElement = const_cast<GUIElement*> (apElement);
		GUIButton* lpButton = reinterpret_cast<GUIButton*>(lpElement);
		lpButton->SetFlags(GUIElement::GF_BUTTON_PRESSED | GUIElement::GF_BUTTON_FOCUS);

		GUIButtonEvent lEvent(lpButton, false);
		GetGUIEventManager()->TriggerEvent(EVENT_BUTTON, &lEvent);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Check if touch moved event is related to a GUIButton
//-----------------------------------------------------------------------------
bool GUIButtonEventManager::OnTouchMoved(const GUIElement* apElement, const CIwSVec2& arTouchPos, const CIwSVec2& arTouchDelta)
{
	IW_CALLSTACK("GUIButtonEventManager::OnTouchMoved");

	if (apElement->TestFlags(GUIElement::GF_BUTTON))
	{
		// Urgh, casting away const.  Still, it's better to do this than make
		// the apElement pointer non-const.
		GUIElement* lpElement = const_cast<GUIElement*> (apElement);
		GUIButton* lpButton = reinterpret_cast<GUIButton*> (lpElement);

		// Clear pressed flag...
		lpButton->ClearFlags(GUIElement::GF_BUTTON_PRESSED);

		//  ...but set it again if still being touched
		if (lpButton->TestFlags(GUIElement::GF_BUTTON_FOCUS) &&
			lpButton->TestBounds(arTouchPos.x, arTouchPos.y))
		{
			lpButton->SetFlags(GUIElement::GF_BUTTON_PRESSED);
		}
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Check if touch released event is related to a GUIButton
//-----------------------------------------------------------------------------
bool GUIButtonEventManager::OnTouchReleased(const GUIElement* apElement, const CIwSVec2& arTouchPos)
{
	IW_CALLSTACK("GUIButtonEventManager::OnTouchReleased");

	if (apElement->TestFlags(GUIElement::GF_BUTTON))
	{
		// Urgh, casting away const.  Still, it's better to do this than make
		// the apElement pointer non-const.
		GUIElement* lpElement = const_cast<GUIElement*> (apElement);
		GUIButton* lpButton = reinterpret_cast<GUIButton*>(lpElement);

		// If button has touch focus and is released whilst still being held
		// we fire off a released event
		if (lpButton->TestFlags(GUIElement::GF_BUTTON_FOCUS) &&
			lpButton->TestBounds(arTouchPos.x, arTouchPos.y))
		{
			GUIButtonEvent lEvent(lpButton, true);
			GetGUIEventManager()->TriggerEvent(EVENT_BUTTON, &lEvent);
		}

		// Clear the pressed and focus flags
		lpButton->ClearFlags(GUIElement::GF_BUTTON_PRESSED | GUIElement::GF_BUTTON_FOCUS);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// GUIButton
//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIButton::GUIButton() :
	m_pUpFrame(NULL),
	m_pDownFrame(NULL),
	m_pDisabledFrame(NULL)
{
	IW_CALLSTACK("GUIButton::GUIButton");

	// Ensure touchable flag is set
	SetFlags(GF_TOUCHABLE | GF_BUTTON);
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUIButton::Serialise()
{
	IW_CALLSTACK("GUIButton::Serialise");

	GUIFrame::Serialise();
}

//-----------------------------------------------------------------------------
// Lookup resource references
//-----------------------------------------------------------------------------
void GUIButton::Resolve()
{
	IW_CALLSTACK("GUIButton::Resolve");

	GUIFrame::Resolve();

	// GUIButton uses the following three special case frames to render
	// its different pressed/released/disabled states.  These frames
	// are made invisible when not required.  All other child frames
	// of GUIButton will render normally.
	m_pUpFrame = reinterpret_cast<GUIFrame*>(FindChildByName("button_up"));
	m_pDownFrame = reinterpret_cast<GUIFrame*>(FindChildByName("button_down"));
	m_pDisabledFrame = reinterpret_cast<GUIFrame*>(FindChildByName("button_disabled"));
}

//-----------------------------------------------------------------------------
// Clone this element
//-----------------------------------------------------------------------------
void GUIButton::Clone(GUIElement* apCloneSource)
{
	IW_CALLSTACK("GUIButton::Clone");
	GUIFrame::Clone(apCloneSource);

	// Nothing else to do... yet!
}

//-----------------------------------------------------------------------------
// Called once per frame to render
//-----------------------------------------------------------------------------
void GUIButton::Render()
{
	IW_CALLSTACK("GUIButton::Render");

	// Enable/Disable the correct frames depending on current button status
	if (m_pDisabledFrame)
		m_pDisabledFrame->SetVisible(!IsEnabled());

	bool lPressed = TestFlags(GF_BUTTON_PRESSED);
	if (m_pUpFrame)
		m_pUpFrame->SetVisible(IsEnabled() && !lPressed);

	if (m_pDownFrame)
		m_pDownFrame->SetVisible(IsEnabled() && lPressed);

	// Render any visible child elements
	GUIFrame::Render();
}

