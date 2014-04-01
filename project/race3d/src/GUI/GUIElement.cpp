//-----------------------------------------------------------------------------
// GUIElement.h
// Base class for all GUI elements (e.g. frames, buttons etc.)
//-----------------------------------------------------------------------------

#include "GUIElement.h"

// Library includes
#include "IwGx.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIDraw.h"
#include "GUIFrame.h"
#include "GUIManager.h"
#include "GUITemplate.h"

// This value is used to keep track of the current GUIElement nesting depth.  This is so that
// top level GUIFrames don't attempt to add themselves to a non-existant parent frame
static uint32 s_FrameDepth = 0;

IW_MANAGED_IMPLEMENT_FACTORY( GUIElement );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIElement::GUIElement() :
	m_pParent( NULL ),
	m_ElementFlags( 0 ),
	m_Enabled( true ),
	m_Visible( true )
{
	IW_CALLSTACK( "GUIElement::GUIElement" );

	m_ScreenRect.x = m_ScreenRect.y = m_ScreenRect.w = m_ScreenRect.h = 0;
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUIElement::Serialise()
{
	IW_CALLSTACK( "GUIElement::Serialise" );

	CIwResource::Serialise();

	IwSerialiseUInt32( m_ElementFlags );
	IwSerialiseBool( m_Enabled );
	IwSerialiseBool( m_Visible );

	m_Bounds.Serialise();
	m_Offset.Serialise();
}

//-----------------------------------------------------------------------------
// Lookup resource references
//-----------------------------------------------------------------------------
void GUIElement::Resolve()
{
	IW_CALLSTACK( "GUIElement::Resolve" );

	CIwResource::Resolve();

	GetGUIManager()->AddElement( this );

	m_Offset.Resolve();
}

//-----------------------------------------------------------------------------
// Started parsing a new GUIElement
//-----------------------------------------------------------------------------
void GUIElement::ParseOpen( CIwTextParserITX* apParser )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIElement::ParseOpen" );

	// Keep track of how nested our GUIElements are
	s_FrameDepth++;

	CIwResource::ParseOpen( apParser );
#endif
}

//-----------------------------------------------------------------------------
// Parse an attribute from an ITX file
//-----------------------------------------------------------------------------
bool GUIElement::ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIElement::ParseAttribute" );

	if( !strcmp( apAttrName, "bounds" ) )
	{
		m_Bounds.Parse( apParser );
	}
	else if( !strcmp( apAttrName, "addTemplate" ) )
	{
		CIwStringS lTemplateName;
		apParser->ReadString( lTemplateName );
		GUITemplate* lpTemplate = reinterpret_cast<GUITemplate*>( IwGetResManager()->GetResNamed( lTemplateName.c_str(), RESTYPE_GUITEMPLATE ) );
		IwAssertMsg( GUI, lpTemplate, ( "Unable to locate GUITemplate called %s", lTemplateName.c_str() ) );
		lpTemplate->CloneTemplateChildren( this );
	}
	else if( !strcmp( apAttrName, "useTemplate" ) )
	{
		CIwStringS lTemplateName;
		apParser->ReadString( lTemplateName );
		GUITemplate* lpTemplate = reinterpret_cast<GUITemplate*>( IwGetResManager()->GetResNamed(lTemplateName.c_str(), RESTYPE_GUITEMPLATE ) );
		IwAssertMsg( GUI, lpTemplate, ( "Unable to locate GUITemplate called %s", lTemplateName.c_str() ) );
		lpTemplate->CloneTemplate( this );
	}
	else if( !strcmp( apAttrName, "debug" ) )
	{
		SetFlags( GF_DEBUG );
	}
	else if( !strcmp( apAttrName, "enable" ) )
	{
		apParser->ReadBool( &m_Enabled );
	}
	else if( !strcmp( apAttrName, "offset" ) )
	{
		m_Offset.Parse( apParser );
	}
	else if( !strcmp( apAttrName, "touchable" ) )
	{
		SetFlags( GF_TOUCHABLE );
	}
	else
		return CIwResource::ParseAttribute( apParser, apAttrName );
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Finished parsing the object
//-----------------------------------------------------------------------------
void GUIElement::ParseClose( CIwTextParserITX* apParser )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIElement::ParseClose" );

	// Nesting level needs decreasing first
	s_FrameDepth--;

	// Get parent object from parser stack if we're still nested
	if( s_FrameDepth )
	{
		GUIElement* lpParent = reinterpret_cast<GUIElement*>( apParser->GetObject( -1 ) );
		IwAssertMsg( GUI, lpParent, ( "GUIElement %s declared without a parent", DebugGetName() ) );

		if( lpParent )
		{
			// Check parent really is a frame!
			IwAssertMsg( GUI, lpParent->TestFlags( GF_FRAME ), ( "GUIElement %s not declared within a GUIFrame", DebugGetName() ) );

			m_pParent = reinterpret_cast<GUIFrame*>( lpParent );
			m_pParent->AddChild( this );
		}
	}
	else
	{
		// Top level object, add to resource manager
		if( !strcmp( GetClassName(), "GUITemplate" ) )
			IwGetResManager()->AddRes( RESTYPE_GUITEMPLATE, this );
		else
			IwGetResManager()->AddRes( RESTYPE_GUIELEMENT, this );
	}
#endif
}

//-----------------------------------------------------------------------------
// Clone this element
//-----------------------------------------------------------------------------
void GUIElement::Clone( GUIElement* apCloneSource )
{
	IW_CALLSTACK( "GUIElement::Clone" );

	m_Bounds.Copy( apCloneSource->m_Bounds );
	m_Offset.Copy( apCloneSource->m_Offset );
	m_ElementFlags = apCloneSource->m_ElementFlags;
	m_Enabled = apCloneSource->m_Enabled;
	m_Visible = apCloneSource->m_Visible;
}

//-----------------------------------------------------------------------------
// Called once per frame to update GUI element
//-----------------------------------------------------------------------------
void GUIElement::Update( int32 aTick )
{
	IW_CALLSTACK( "GUIElement::Update" );

	// Set up bounds using any offset we might have
	m_Offset.SetBounds( this );

	// Calculate the screen rectangle for this element, so if it needs to
	// be touchable we will know the hit zone area
	m_Bounds.GetScreenRect( m_ScreenRect, this );

	// Is this a touchable element?
	if( m_Enabled && m_Visible && TestFlags( GF_TOUCHABLE ) )
		GetGUIManager()->AddTouchElement( this );
}

//-----------------------------------------------------------------------------
// Called once per frame to render GUI element
//-----------------------------------------------------------------------------
void GUIElement::Render()
{
	IW_CALLSTACK( "GUIElement::Render" );

	// m_ScreenRect is pre-calculated in GUIElement::Update
	if( TestFlags( GF_DEBUG ) )
	{
		DrawRectangleOutline( m_ScreenRect.x, m_ScreenRect.y, m_ScreenRect.w, m_ScreenRect.h, COL_MAGENTA );
	}
}

//-----------------------------------------------------------------------------
// Test whether the given point is within the bounds of this element
//-----------------------------------------------------------------------------
bool GUIElement::TestBounds( int32 aX, int32 aY )
{
	IW_CALLSTACK( "GUIElement::TestBounds" );

	return	( aX >= m_ScreenRect.x ) && ( aY >= m_ScreenRect.y ) &&
			( aX <= ( m_ScreenRect.x + m_ScreenRect.w ) ) &&
			( aY <= ( m_ScreenRect.y + m_ScreenRect.h ) );
}