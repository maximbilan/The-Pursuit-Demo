//-----------------------------------------------------------------------------
// GuiManager.cpp
// Singleton class that takes care of rendering GUI elements and handling
// touch screen input
//-----------------------------------------------------------------------------

#include "GUIManager.h"

// Library includes
#include "IwGx.h"
#include "s3ePointer.h"

// Project includes
#include "GUIEventManager.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "GUIFrame.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIManager::GUIManager() :
	m_pActiveTouched( NULL ),
	m_pOverrideFrame( NULL )
{
	IW_CALLSTACK( "GUIManager::GUIManager" );

	new GUIButtonEventManager;

	ClearContext();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
GUIManager::~GUIManager()
{
	IW_CALLSTACK( "GUIManager::~GUIManager" );

	delete GetGUIButtonEventManager();
}

//-----------------------------------------------------------------------------
// Add a new frame to the list of frames to update/render
//-----------------------------------------------------------------------------
void GUIManager::AddFrame( GUIFrame* apFrame )
{
	IW_CALLSTACK( "GUIManager::AddFrame" );

	// Ensure the frame is ready for display
	apFrame->Update( 0 );
	m_Frames.Add( apFrame );
}

//-----------------------------------------------------------------------------
// Remove specified frame from list
//-----------------------------------------------------------------------------
void GUIManager::RemoveFrame( GUIFrame* apFrame )
{
	IW_CALLSTACK( "GUIManager::RemoveFrame" );
	m_Frames.RemoveSlow( apFrame );
}

//-----------------------------------------------------------------------------
// Remove all frames from GUIManager list
//-----------------------------------------------------------------------------
void GUIManager::ClearAllFrames()
{
	IW_CALLSTACK( "GUIManager::ClearAllFrames" );
	m_Frames.Clear();
	m_TouchElements.clear();
}

//-----------------------------------------------------------------------------
// Update all registered frames
//-----------------------------------------------------------------------------
void GUIManager::Update( int32 aTick )
{
	IW_CALLSTACK( "GUIManager::Update" );

	// Clear view offset and scissoring
	ClearContext();

	// Clear list of touch zones
	m_TouchElements.clear();

	// Update all frames
	if( m_pOverrideFrame )
	{
		m_pOverrideFrame->Update( aTick );
	}
	else
	{
		IW_MANAGED_LIST_ITERATE( m_Frames, itFrame )
		{
			GUIFrame* lpFrame = reinterpret_cast<GUIFrame*>( *itFrame );
			lpFrame->Update( aTick );
		}
	}

	// Previous loop (might) have populated the m_TouchElements list, so let's see if any
	// user input has been detected
	int32 lX = s3ePointerGetX();
	int32 lY = s3ePointerGetY();
	s3ePointerState lTouchState = s3ePointerGetState( S3E_POINTER_BUTTON_SELECT );

	// Before we handle any touch events, let's check that our currently active element is
	// still in the list of touchable elements.  If it isn't, clear our active touched pointer
	bool lActiveInList = false;
	for( uint32 i = 0; i < m_TouchElements.size(); i++ )
	{
		GUIElement* lpElement = m_TouchElements[i].m_pElement;
		if( lpElement == m_pActiveTouched )
			lActiveInList = true;
	}

	if( lActiveInList )
	{
		// If active element is still in the list, make it respond to movement
		// or released events
		if( lTouchState & S3E_POINTER_STATE_RELEASED )
		{
			// Trigger generic and element specific TouchReleased events
			GUITouchEvent lEvent( GUITouchEvent::GTE_RELEASED, m_pActiveTouched, lX, lY );
			GetGUIEventManager()->TriggerEvent( EVENT_TOUCH, &lEvent );

			m_pActiveTouched = NULL;
		}
		else if( ( lTouchState & S3E_POINTER_STATE_DOWN ) &&
				( ( lX != m_LastTouchX ) || ( lY != m_LastTouchY ) ) )
		{
			// Trigger generic and element specific TouchMove events
			GUITouchEvent lEvent( GUITouchEvent::GTE_MOVED, m_pActiveTouched, lX, lY, lX - m_LastTouchX, lY - m_LastTouchY );
			GetGUIEventManager()->TriggerEvent( EVENT_TOUCH, &lEvent );
		}
	}
	else if( lTouchState & S3E_POINTER_STATE_PRESSED )
	{
		// Clear the current active touched element
		m_pActiveTouched = NULL;

		// Now step through all zones
		for( uint32 i = 0; i < m_TouchElements.size(); i++ )
		{
			GUIElement* lpElement = m_TouchElements[i].m_pElement;
			CIwRect& lrZone = m_TouchElements[i].m_ScreenRect;

			if( ( lX >= lrZone.x ) && ( lY >= lrZone.y ) && ( lX <= ( lrZone.x + lrZone.w ) ) && ( lY <= (lrZone.y + lrZone.h ) ) )
			{
				m_pActiveTouched = lpElement;
			}
		}

		// Do we have an active touched element now?
		if( m_pActiveTouched )
		{
			// Trigger generic and element specific TouchPressed events
			GUITouchEvent lEvent( GUITouchEvent::GTE_PRESSED, m_pActiveTouched, lX, lY );
			GetGUIEventManager()->TriggerEvent( EVENT_TOUCH, &lEvent );
		}
	}

	m_LastTouchX = lX;
	m_LastTouchY = lY;
}

//-----------------------------------------------------------------------------
// Render all registered frames
//-----------------------------------------------------------------------------
void GUIManager::Render()
{
	IW_CALLSTACK( "GUIManager::Render" );

	// Clear view offset and scissoring
	ClearContext();

	// Don't want fogging on the GUI!
	IwGxFogOff();

	if( m_pOverrideFrame )
	{
		m_pOverrideFrame->Render();
	}
	else
	{
		IW_MANAGED_LIST_ITERATE( m_Frames, itFrame )
		{
			GUIFrame* lpFrame = reinterpret_cast<GUIFrame*>( *itFrame );
			if( lpFrame->IsVisible() )
				lpFrame->Render();
		}
	}
}

//-----------------------------------------------------------------------------
// Add given element to the master list of elements
//-----------------------------------------------------------------------------
void GUIManager::AddElement( GUIElement* apElement )
{
	IW_CALLSTACK( "GUIManager::AddElement" );
	m_Elements.Add( apElement, true );
}

//-----------------------------------------------------------------------------
// Try to find the specified GUIElement in the m_Elements list
//-----------------------------------------------------------------------------
GUIElement* GUIManager::FindElementByHash( uint32 aHash )
{
	IW_CALLSTACK( "GUIManager::FindElementByHash" );
	GUIElement* lpElement = reinterpret_cast<GUIElement*>( m_Elements.GetObjHashed( aHash, true ) );
	return lpElement;
}

//-----------------------------------------------------------------------------
// Try to find the named GUIElement in the m_Elements list
//-----------------------------------------------------------------------------
GUIElement* GUIManager::FindElementByName( const char* apName )
{
	IW_CALLSTACK( "GUIManager::FindElementByName" );
	return FindElementByHash( IwHashString( apName ) );
}

//-----------------------------------------------------------------------------
// Add given element to the list of touchable elements
//-----------------------------------------------------------------------------
void GUIManager::AddTouchElement( GUIElement* apElement )
{
	IW_CALLSTACK( "GUIManager::AddTouchElement" );

	TouchEntry lEntry;
	lEntry.m_pElement = apElement;

	// Copy the screen rectangle from the element first
	CIwRect r = apElement->GetScreenRect();

	// Add on any view offset (to allow for being in a GUIScrollFrame)
	CIwSVec2 lViewOffset = GetViewOffset();
	r.x += lViewOffset.x >> SUBPIXEL_SHIFT;
	r.y += lViewOffset.y >> SUBPIXEL_SHIFT;

	// Are we completely outside the scissor rectangle first?
	if( ( ( r.x + r.w ) < m_ScissorRect.x ) || ( r.x > ( m_ScissorRect.x + m_ScissorRect.w ) ) ||
		( ( r.y + r.h ) < m_ScissorRect.y ) || ( r.y > ( m_ScissorRect.y + m_ScissorRect.h ) ) )
	{
		return;
	}

	// Clip left edge
	if( r.x < m_ScissorRect.x )
	{
		r.w -= ( m_ScissorRect.x - r.x );
		r.x = m_ScissorRect.x;
	}
	// Clip right edge
	if( ( r.x + r.w ) >= ( m_ScissorRect.x + m_ScissorRect.w ) )
	{
		r.w -= ( r.x + r.w ) - ( m_ScissorRect.x + m_ScissorRect.w );
	}
	// Clip top edge
	if( r.y < m_ScissorRect.y )
	{
		r.h -= ( m_ScissorRect.y - r.y );
		r.y = m_ScissorRect.y;
	}
	// Clip bottom edge
	if( ( r.y + r.h ) >= ( m_ScissorRect.y + m_ScissorRect.h ) )
	{
		r.h -= ( r.y + r.h ) - ( m_ScissorRect.y + m_ScissorRect.h );
	}

	// Has width/height been reduced to zero?
	if( ( r.w == 0 ) || ( r.h == 0 ) )
		return;

	// If we get here then there's still some visible area of the element
	// so we can add it to the list
	lEntry.m_ScreenRect = r;
	m_TouchElements.push_back( lEntry );
}

//-----------------------------------------------------------------------------
// Push a view offset (in sub pixels)
//-----------------------------------------------------------------------------
void GUIManager::PushViewOffset( const CIwSVec2& arOffset )
{
	IW_CALLSTACK( "GUIManager::PushViewOffset" );
	m_ViewOffset += arOffset;
	IwGxSetScreenSpaceOrg( &m_ViewOffset );
}

//-----------------------------------------------------------------------------
// Pop a view offset (in sub pixels)
//-----------------------------------------------------------------------------
void GUIManager::PopViewOffset( const CIwSVec2& arOffset )
{
	IW_CALLSTACK( "GUIManager::PopViewOffset" );
	m_ViewOffset -= arOffset;
	IwGxSetScreenSpaceOrg( &m_ViewOffset );
}

//-----------------------------------------------------------------------------
// Set scissor rectangle
//-----------------------------------------------------------------------------
void GUIManager::SetScissorRectangle( const CIwRect& arRect )
{
	IW_CALLSTACK( "GUIManager::SetScissorRectangle" );
	m_ScissorRect = arRect;
	IwGxSetScissorScreenSpace( m_ScissorRect.x, m_ScissorRect.y, m_ScissorRect.w, m_ScissorRect.h );
}

//-----------------------------------------------------------------------------
// Clear scissor rectangle
//-----------------------------------------------------------------------------
void GUIManager::ClearScissorRectangle()
{
	IW_CALLSTACK( "GUIManager::ClearScissorRectangle" );

	m_ScissorRect.x = m_ScissorRect.y = 0;
	m_ScissorRect.w = IwGxGetScreenWidth();
	m_ScissorRect.h = IwGxGetScreenHeight();
	IwGxClearScissorScreenSpace();
}

//-----------------------------------------------------------------------------
// Initialise scissor and view offset for starting update/render
//-----------------------------------------------------------------------------
void GUIManager::ClearContext()
{
	IW_CALLSTACK( "GUIManager::ClearContext" );

	m_ViewOffset.x = m_ViewOffset.y = 0;
	IwGxSetScreenSpaceOrg( &m_ViewOffset );
	ClearScissorRectangle();
}

//-----------------------------------------------------------------------------
// Reset all strings
//-----------------------------------------------------------------------------
void GUIManager::ResetLabelStrings()
{
	IW_MANAGED_LIST_ITERATE( m_Elements, itElement )
	{
		GUIElement* lpElement = static_cast< GUIElement* >( *itElement );
		if( !stricmp( lpElement->GetClassName(), "GUILabel" ) )
		{
			GUILabel* lpLabel = static_cast< GUILabel* >( lpElement );
			if( lpLabel )
			{
				lpLabel->ResetText();
			}
		}
	}
}