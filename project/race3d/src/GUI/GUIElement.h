//-----------------------------------------------------------------------------
// GUIElement.h
// Base class for all GUI elements (e.g. frames, buttons etc.)
//-----------------------------------------------------------------------------

#ifndef GUIELEMENT_H
#define GUIELEMENT_H

// Library includes
#include "IwGxTypes.h"

// Project includes
#include "GUIBounds.h"
#include "GUIOffset.h"
#include "IwResource.h"

// Forward declarations
class GUIFrame;

class GUIElement : public CIwResource
{
public:
	IW_MANAGED_DECLARE( GUIElement );
	GUIElement();

	//-------------------------------------------------------------------------
	// Enumerations
	//-------------------------------------------------------------------------
	enum GUIElementFlags
	{
		GF_FRAME				= (1 << 0),		// Should only be true for GUIFrame (and it's descendants)
		GF_TOUCHABLE			= (1 << 1),		// This element can respond to touch events
		GF_BUTTON				= (1 << 2),		// This element is derived from GUIButton
		GF_BUTTON_FOCUS			= (1 << 3),		// This button is currently tracking touch movement events
		GF_BUTTON_PRESSED		= (1 << 4),		// The button is currently in a pressed state
		GF_SCROLLFRAME			= (1 << 5),		// This element is derived from GUIScrollFrame
		GF_SCROLLFRAME_FOCUS	= (1 << 6),		// This scrollframe is currently tracking touch events
		GF_DEBUG				= (1 << 7)		// This GUIElement is in debug mode (can display extra info etc.)
	};

	//-------------------------------------------------------------------------
	// Virtual overrides from CIwManaged
	//-------------------------------------------------------------------------
	virtual void Serialise();
	virtual void Resolve();
	virtual void ParseOpen( CIwTextParserITX* apParser );
	virtual bool ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName );
	virtual void ParseClose( CIwTextParserITX* apParser );

	//-------------------------------------------------------------------------
	// Declared virtuals
	//-------------------------------------------------------------------------
	// Clone this element
	virtual void Clone( GUIElement* apCloneSource );

	// Called once per frame to update GUI element
	virtual void Update( int32 aTick );

	// Called once per frame to render GUI element
	virtual void Render();

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------
	// Flag management methods
	void SetFlags( uint32 aFlag );
	void ClearFlags( uint32 aFlag );
	bool TestFlags( uint32 aFlag ) const;

	// Parent accessors
	void SetParent( GUIFrame* apFrame );
	GUIFrame* GetParent() const;

	// Other accessors
	const GUIBounds& GetBounds() const;
	void SetBounds( const GUIBounds& arBounds );
	const CIwRect& GetScreenRect() const;

	// Enabled/disabled methods
	void SetEnabled( bool aEnabled );
	bool IsEnabled() const;

	// Visibility methods
	void SetVisible( bool aVisible );
	bool IsVisible() const;

	// Test whether the given point is within the bounds of this element
	bool TestBounds( int32 aX, int32 aY );

protected:
	// The bounding rectangle of this element (protected since we really
	// need to override standard setting of this parameter for GUIScrollFrame)
	GUIBounds	m_Bounds;

private:
	// Offset from another GUIElement
	GUIOffset	m_Offset;

	// The GUIFrame which contains this element
	GUIFrame*	m_pParent;

	// Element control flags
	uint32		m_ElementFlags;

	// The calculated screen rectangle.  This is only valid once the Update
	// method has been called on the element.
	CIwRect		m_ScreenRect;

	// Is this control enabled or disabled?
	bool		m_Enabled;

	// Is this control visible?
	bool		m_Visible;
};

//----------------------------------------------------------------------------------

inline void GUIElement::SetFlags( uint32 aFlag )
{
	m_ElementFlags |= aFlag;
}

//----------------------------------------------------------------------------------

inline void GUIElement::ClearFlags( uint32 aFlag )
{
	m_ElementFlags &= ~aFlag;
}

//----------------------------------------------------------------------------------

inline bool GUIElement::TestFlags( uint32 aFlag ) const
{
	return ( m_ElementFlags & aFlag ) == aFlag;
}

//----------------------------------------------------------------------------------

inline void GUIElement::SetParent( GUIFrame* apFrame )
{
	m_pParent = apFrame;
}

//----------------------------------------------------------------------------------

inline GUIFrame* GUIElement::GetParent() const
{
	return m_pParent;
}

//----------------------------------------------------------------------------------

inline const GUIBounds& GUIElement::GetBounds() const
{
	return m_Bounds;
}

//----------------------------------------------------------------------------------

inline void GUIElement::SetBounds( const GUIBounds& arBounds )
{
	m_Bounds.Copy( arBounds );
}

//----------------------------------------------------------------------------------

inline const CIwRect& GUIElement::GetScreenRect() const
{
	return m_ScreenRect;
}

//----------------------------------------------------------------------------------

inline void GUIElement::SetEnabled( bool aEnabled )
{
	m_Enabled = aEnabled;
}

//----------------------------------------------------------------------------------

inline bool GUIElement::IsEnabled() const
{
	return m_Enabled;
}

//----------------------------------------------------------------------------------

inline void GUIElement::SetVisible( bool aVisible )
{
	m_Visible = aVisible;
}

//----------------------------------------------------------------------------------

inline bool GUIElement::IsVisible() const
{
	return m_Visible;
}

#endif