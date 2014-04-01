//-----------------------------------------------------------------------------
// GUILabel.cpp
// Displays a line of text on screen
//-----------------------------------------------------------------------------

#include "GUILabel.h"

// Library includes
#include "IwGx.h"
#include "IwGxFont.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIColour.h"
#include "GUIFrame.h"
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY( GUILabel );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUILabel::GUILabel() :
	m_pFont( NULL ),
	m_LabelFlags( 0 ),
	m_TextHash( 0xFFFFFFFF )
{
	IW_CALLSTACK( "GUILabel::GUILabel" );
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUILabel::Serialise()
{
	IW_CALLSTACK( "GUILabel::Serialise" );

	GUIElement::Serialise();

	IwSerialiseManagedHash( &m_pFont );
	IwSerialiseUInt8( m_HorzAlign );
	IwSerialiseUInt8( m_VertAlign );
	IwSerialiseUInt32( m_TextHash );

	m_Text.Serialise();
	m_Colour.Serialise();
}

//-----------------------------------------------------------------------------
// Lookup resource references
//-----------------------------------------------------------------------------
void GUILabel::Resolve()
{
	IW_CALLSTACK( "GUILabel::Resolve" );

	GUIElement::Resolve();

	IwResolveManagedHash( &m_pFont, IW_GX_RESTYPE_FONT, IW_RES_SEARCH_ALL_F );
}

//-----------------------------------------------------------------------------
// Parse an attribute from an ITX file
//-----------------------------------------------------------------------------
const char* g_HorizontalAligns[] = 
{
	"left",
	"centre",
	"right",
	NULL
};

const char* g_VerticalAligns[] = 
{
	"top",
	"centre",
	"bottom",
	NULL
};

bool GUILabel::ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUILabel::ParseAttribute" );

	if( !strcmp( apAttrName, "align" ) )
	{
		CIwStringS lAlignment;
		uint32 lTmpAlign;

		apParser->ReadString( lAlignment );
		IwAssertMsg( GUI, lAlignment[0] == '{', ("Error parsing GUILabel align parameter, missing {" ) );

		// Note - The following code uses the convenient fact that the IwGxFontAlignHor
		// and IwGxFontAlignVer enumerations are defined in the order expected in our
		// two lists of enumeration filters.  If text starts to be aligned strangely
		// after upgrading Marmalade SDK, check these enums aren't the problem (unlikely!)

		// Horizontal alignment
		apParser->ReadString( lAlignment );
		apParser->FilterStringGetEnumAsserted( lAlignment.c_str(), g_HorizontalAligns, lTmpAlign );
		m_HorzAlign = ( uint8 ) lTmpAlign;

		// Vertical alignment
		apParser->ReadString(lAlignment);
		apParser->FilterStringGetEnumAsserted(lAlignment.c_str(), g_VerticalAligns, lTmpAlign);
		m_VertAlign = (uint8) lTmpAlign;

		apParser->ReadString( lAlignment );
		IwAssertMsg( GUI, lAlignment[0] == '}', ("Error parsing GUILabel align parameter, missing }" ) );
	}
	else if( !strcmp( apAttrName, "colour" ) )
	{
		GUIColour::ParseColour( apParser, &m_Colour );
	}
	else if( !strcmp( apAttrName, "font" ) )
	{
		CIwStringS lFontName;
		apParser->ReadString( lFontName );
		m_pFont = ResourceFindByName<CIwGxFont>( lFontName.c_str(), IW_GX_RESTYPE_FONT );
	}
	else if( !strcmp( apAttrName, "text" ) )
	{
		apParser->ReadString( m_Text );
	}
	else if( !strcmp( apAttrName, "string" ) )
	{
		CIwStringL lTextID;
		apParser->ReadString( lTextID );
		m_TextHash = LocaliseCalculateIDHash( lTextID.c_str() );
	}
	else
		return GUIElement::ParseAttribute( apParser, apAttrName );
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Clone this element
//-----------------------------------------------------------------------------
void GUILabel::Clone( GUIElement* apCloneSource )
{
	IW_CALLSTACK( "GUILabel::Clone" );
	GUIElement::Clone( apCloneSource );

	GUILabel* lpCloneLabel = reinterpret_cast<GUILabel*>( apCloneSource );

	m_Text = lpCloneLabel->m_Text;
	m_pFont = lpCloneLabel->m_pFont;
	m_HorzAlign = lpCloneLabel->m_HorzAlign;
	m_VertAlign = lpCloneLabel->m_VertAlign;
	m_LabelFlags = lpCloneLabel->m_LabelFlags & ~GUILabel::LF_PREPARED;
	m_Colour.Copy(lpCloneLabel->m_Colour);
	m_TextHash = lpCloneLabel->m_TextHash;
}

//-----------------------------------------------------------------------------
// Called once per frame to update GUI element
//-----------------------------------------------------------------------------
void GUILabel::Update( int32 aTick )
{
	IW_CALLSTACK( "GUILabel::Update" );

	// Call this first to ensure screen rectangle has been calculated
	GUIElement::Update( aTick );

	// Check if the text needs re-preparing
	if( !( m_LabelFlags & LF_PREPARED ) )
	{
		// Set font to use
		IwGxFontSetFont( m_pFont );

		// Set rectangle bounds
		const CIwRect& lrRect = GetScreenRect();
		IwGxFontSetRect( lrRect );

		// Text alignment flags
		IwGxFontSetAlignmentHor( ( IwGxFontAlignHor ) m_HorzAlign );
		IwGxFontSetAlignmentVer( ( IwGxFontAlignVer ) m_VertAlign );

		// Prepare the text for rendering
		const char* lpText = m_Text.c_str();
 		if( m_TextHash != 0xFFFFFFFF )
 			lpText = LocaliseFindString( m_TextHash );

		IwGxFontPrepareText( m_PreparedText, lpText );
		m_LabelFlags |= LF_PREPARED;
	}
}

//-----------------------------------------------------------------------------
// Called once per frame to render GUI element
//-----------------------------------------------------------------------------
void GUILabel::Render()
{
	IW_CALLSTACK( "GUILabel::Render" );

	GUIElement::Render();

	if( m_LabelFlags & LF_PREPARED )
	{
		// Must have lighting enabled in order for fonts to be drawn coloured
		IwGxLightingOn();
		IwGxFontSetCol( m_Colour.GetColour() );

		// Ensure screen rectangle is passed to IwGxFont too
		const CIwRect& lrRect = GetScreenRect();

		// Since we are using sub pixel screen space rendering everywhere else
		// in the GUI code, IwGxFont fails to draw in the correct position
		// as it doesn't.  Hence we need to find the current screen space
		// origin, scale it back to normal pixel size, render, then set
		// the origin back to its original value.
		CIwSVec2 lScreenOrg = GetGUIManager()->GetViewOffset();
		CIwSVec2 lNewScreenOrg( lScreenOrg.x >> SUBPIXEL_SHIFT, lScreenOrg.y >> SUBPIXEL_SHIFT );
		lNewScreenOrg -= lScreenOrg;
		GetGUIManager()->PushViewOffset( lNewScreenOrg );

		IwGxFontSetRect( lrRect );
		IwGxFontSetFont( m_pFont );
		IwGxFontDrawText( m_PreparedText );
		IwGxLightingOff();

		GetGUIManager()->PopViewOffset( lNewScreenOrg );
	}
}

//-----------------------------------------------------------------------------
// Change the text being displayed
//-----------------------------------------------------------------------------
void GUILabel::SetText( const char* apText )
{
	IW_CALLSTACK( "GUILabel::SetText" );
	m_LabelFlags &= ~LF_PREPARED;
	m_Text = apText;
	m_TextHash = 0xFFFFFFFF;
}


//-----------------------------------------------------------------------------
// Change the text being displayed to a localised string
//-----------------------------------------------------------------------------
void GUILabel::SetString( uint32 aTextHash )
{
	IW_CALLSTACK( "GUILabel::SetString" );
	m_LabelFlags &= ~LF_PREPARED;
	m_Text.setLength( 0 );
	m_TextHash = aTextHash;
}

//-----------------------------------------------------------------------------
// Change the text being displayed to a localised string (given text identifier)
//-----------------------------------------------------------------------------
void GUILabel::SetString( const char* apTextIdentifier )
{
	IW_CALLSTACK( "GUILabel::SetString" );
	SetString( LocaliseCalculateIDHash( apTextIdentifier ) );
}

//-----------------------------------------------------------------------------
// Reset text
//-----------------------------------------------------------------------------
void GUILabel::ResetText()
{
	IW_CALLSTACK( "GUILabel::ResetText" );
	m_LabelFlags &= ~LF_PREPARED;
}