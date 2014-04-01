//-----------------------------------------------------------------------------
// GuiManager.h
// Singleton class that takes care of rendering GUI elements and handling
// touch screen input
//-----------------------------------------------------------------------------

#ifndef GUIMANAGER_H
#define GUIMANAGER_H

// Library includes
#include "IwArray.h"
#include "IwManagedList.h"
#include "IwGeomSVec2.h"
#include "IwGxTypes.h"
#include "IwResManager.h"

// Project includes
#include "GUIEventManager.h"
#include "GUISingleton.h"

// Forward declarations
class GUIElement;
class GUIFrame;

#define RESTYPE_GUIELEMENT	"GUIELEMENT"
#define RESTYPE_GUITEMPLATE	"GUITEMPLATE"

#define SUBPIXEL_SHIFT	3

//-----------------------------------------------------------------------------
// Locate a resource by hash value, optionally limiting to the specified group
// Return NULL if not found
//-----------------------------------------------------------------------------
template<class tResType> tResType* ResourceFindByHash( uint32 aResHash, const char* apResType )
{
	CIwResource* lpResource = IwGetResManager()->GetResHashed( aResHash, apResType, IW_RES_SEARCH_ALL_F );
	return reinterpret_cast<tResType*>( lpResource );
}

//-----------------------------------------------------------------------------
// Locate a resource by name, optionally limiting to the specified group
//-----------------------------------------------------------------------------
template<class tResType> tResType* ResourceFindByName( const char* apResName, const char* apResType )
{
	CIwResource* lpResource = IwGetResManager()->GetResNamed( apResName, apResType, IW_RES_SEARCH_ALL_F );
	return reinterpret_cast<tResType*>( lpResource );
}

//-----------------------------------------------------------------------------
// GUIManager
//-----------------------------------------------------------------------------
class GUIManager : public GUISingleton<GUIManager>
{
public:
	GUIManager();
	~GUIManager();

	// Add a new frame to the list of frames to update/render
	void AddFrame( GUIFrame* apFrame );

	// Remove specified frame from list
	void RemoveFrame( GUIFrame* apFrame );

	// Remove all frames from GUIManager list
	void ClearAllFrames();

	// Set the override frame();
	void SetOverrideFrame( GUIFrame* apFrame );

	// Update all registered frames
	void Update( int32 aTick );

	// Render all registered frames
	void Render();

	// Add given element to the master list of elements
	void AddElement( GUIElement* apElement );

	// Try to find the GUIElement with given hash in the m_Elements list
	GUIElement* FindElementByHash( uint32 aHash );

	// Try to find the named GUIElement in the m_Elements list
	GUIElement* FindElementByName (const char* apName );

	// Add given element to the list of touchable elements
	void AddTouchElement( GUIElement* apElement );

	// Push a view offset (in sub pixels)
	void PushViewOffset( const CIwSVec2& arOffset );

	// Pop a view offset (in sub pixels)
	void PopViewOffset( const CIwSVec2& arOffset );

	// Get current view offset
	const CIwSVec2& GetViewOffset() const;

	// Set scissor rectangle
	void SetScissorRectangle( const CIwRect& arRect );

	// Clear scissor rectangle
	void ClearScissorRectangle();

	// Reset all strings
	void ResetLabelStrings();

private:
	//-------------------------------------------------------------------------
	// Private class
	//-------------------------------------------------------------------------
	class TouchEntry
	{
	public:
		GUIElement*		m_pElement;
		CIwRect			m_ScreenRect;
	};

	//-------------------------------------------------------------------------
	// Private methods
	//-------------------------------------------------------------------------
	// Initialise scissor and view offset for starting update/render
	void ClearContext();

	//-------------------------------------------------------------------------
	// Private members
	//-------------------------------------------------------------------------
	// List of all GUIElements that the GUIManager knows about
	CIwManagedList m_Elements;

	// List of GUIFrames we need to update/render
	CIwManagedList m_Frames;

	// List of all currently visible GUIElements that are touch areas
	CIwArray<TouchEntry> m_TouchElements;

	// Override frame (for loading screen).  If set, forces the frames
	// listed in m_Frames to be ignored.
	GUIFrame* m_pOverrideFrame;

	// The currently actively touched element (if any)
	GUIElement* m_pActiveTouched;

	// Position of last screen touch
	int32 m_LastTouchX;
	int32 m_LastTouchY;

	// The current view offset (used for GUIScrollFrame)
	CIwSVec2 m_ViewOffset;

	// Current scissor rect
	CIwRect m_ScissorRect;
};

// Declare the static singleton access method
GUISINGLETON_GET( GUIManager );

//-----------------------------------------------------------------------------

inline void GUIManager::SetOverrideFrame( GUIFrame* apFrame )
{
	m_pOverrideFrame = apFrame;
}

//-----------------------------------------------------------------------------

inline const CIwSVec2& GUIManager::GetViewOffset() const
{
	return m_ViewOffset;
}

//-----------------------------------------------------------------------------
// Event structures
//-----------------------------------------------------------------------------

#define EVENT_TOUCH				"Touch"

//-----------------------------------------------------------------------------
// GUITouchEventHandler
// Classes should inherit from this in order to respond to touch events
//-----------------------------------------------------------------------------
class GUITouchEventHandler : public virtual GUIEventHandler
{
public:
	virtual bool OnTouchPressed( const GUIElement* apElement, const CIwSVec2& arTouchPos ) = 0;
	virtual bool OnTouchMoved( const GUIElement* apElement, const CIwSVec2& arTouchPos, const CIwSVec2& arTouchDelta ) = 0;
	virtual bool OnTouchReleased( const GUIElement* apElement, const CIwSVec2& arTouchPos ) = 0;
};

//-----------------------------------------------------------------------------
// GUITouchEvent
// Class for representing a touch event of some kind
//-----------------------------------------------------------------------------
class GUITouchEvent : public GUIEvent
{
public:
	enum GUITouchEventType
	{
		GTE_PRESSED, GTE_MOVED, GTE_RELEASED
	};

	GUITouchEvent( GUITouchEventType aType, GUIElement* apElement, int32 aX, int32 aY, int32 aDeltaX = 0, int32 aDeltaY = 0 ) :
		m_Type( aType ),
		m_pElement( apElement )
	{
		m_Pos.x = aX;
		m_Pos.y = aY;
		m_Delta.x = aDeltaX;
		m_Delta.y = aDeltaY;
	}

	virtual bool Execute( GUIEventHandler* apHandler )
	{
		GUITouchEventHandler* lpTouchHandler = dynamic_cast<GUITouchEventHandler*>( apHandler );
		switch( m_Type )
		{
			case GTE_PRESSED:
				if( lpTouchHandler->OnTouchPressed( m_pElement, m_Pos ) )
					return true;
				break;
			case GTE_MOVED:
				if( lpTouchHandler->OnTouchMoved( m_pElement, m_Pos, m_Delta ) )
					return true;
				break;
			case GTE_RELEASED:
				if( lpTouchHandler->OnTouchReleased( m_pElement, m_Pos ) )
					return true;
				break;
		}
		return false;
	}

private:
	GUITouchEventType	m_Type;
	GUIElement*			m_pElement;
	CIwSVec2			m_Pos;
	CIwSVec2			m_Delta;
};

#endif