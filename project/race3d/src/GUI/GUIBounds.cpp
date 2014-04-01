//-----------------------------------------------------------------------------
// GUIBounds.h
// Class used to store the bounding rectangle of a GUI element
//-----------------------------------------------------------------------------

#include "GUIBounds.h"

// Library includes
#include "IwGx.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIElement.h"
#include "GUIFrame.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIBounds::GUIBounds()
{
	IW_CALLSTACK( "GUIBounds::GUIBounds" );

	// Bounds defaults to full screen size
	m_X = 0;
	m_Y = 0;
	m_Width = IwGxGetScreenWidth();
	m_Height = IwGxGetScreenHeight();
	m_AspectRatio = 0;

	// No flags set by default
	m_Flags = 0;
}

//-----------------------------------------------------------------------------
// Serialise bounds data
//-----------------------------------------------------------------------------
void GUIBounds::Serialise()
{
	IW_CALLSTACK( "GUIBounds::Serialise" );

	IwSerialiseInt16( m_X );
	IwSerialiseInt16( m_Y );
	IwSerialiseInt16( m_Width );
	IwSerialiseInt16( m_Height );
	IwSerialiseInt32( m_AspectRatio );
	IwSerialiseUInt32( m_Flags );
}

//-----------------------------------------------------------------------------
// Parse attributes from text file
// A bounds parameter takes the following form in an ITX file
// bounds { ha va hap vap width height flags } where:-
//       ha = Horizontal alignment specifier
//       va = Verticall alignment specifier
//       hap = Horizontal anchor position
//       vap = Vertical anchor position
//       width = width of element
//       height = height of element
//       flags = optional set of flags to control bounds
// ha can be one of:-
//		left, centre, right
// va can be one of:-
//		top, centre, bottom
// hap, vap, width and height can all be specified as follows:-
//       nnn = absolute pixel position
//       nnn% = percentage of parent width
//       Precede by a minus sign to make relative to right/bottom of parent width/height
// Valid values for flags parameter are:-
//       aspect-width <width> <height> = compute width based on calculated height and supplied aspect ratio
//       aspect-height <width> <height> = compute height based on calculated width and supplied aspect ratio
//-----------------------------------------------------------------------------
const char* g_HorizontalAnchors[] = 
{
	"left",
	"centre",
	"right",
	NULL
};

const char* g_VerticalAnchors[] = 
{
	"top",
	"centre",
	"bottom",
	NULL
};

const char* g_Flags[] = 
{
	"aspect-width",
	"aspect-height",
	NULL
};

void GUIBounds::Parse( CIwTextParserITX* apParser )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIBounds::Parse" );

	CIwStringS lTmp;

	// First we should have the opening curly bracket, which we'll just skip, but
	// we'll assert if it's not there
	apParser->ReadString( lTmp );
	IwAssertMsg( GUI, lTmp[0] == '{', ( "Error parsing GUIBounds, missing {" ) );

	// First parameter is horizontal anchor
	uint32 lTmpAlign;
	apParser->ReadString( lTmp );
	apParser->FilterStringGetEnumAsserted( lTmp.c_str(), g_HorizontalAnchors, lTmpAlign );
	m_Flags |= ( lTmpAlign << GB_HORIZONTAL_ALIGN_SHIFT );

	// Second parameter is vertical anchor
	apParser->ReadString( lTmp );
	apParser->FilterStringGetEnumAsserted( lTmp.c_str(), g_VerticalAnchors, lTmpAlign );
	m_Flags |= ( lTmpAlign << GB_VERTICAL_ALIGN_SHIFT );

	// Third parameter is horizontal position
	apParser->ReadString( lTmp );
	ParseNumber( lTmp, m_X, GB_X_SHIFT );

	// Fourth parameter is vertical position
	apParser->ReadString( lTmp );
	ParseNumber( lTmp, m_Y, GB_Y_SHIFT );

	// Fifth parameter is width
	apParser->ReadString( lTmp );
	ParseNumber( lTmp, m_Width, GB_WIDTH_SHIFT );

	// Sixth parameter is height
	apParser->ReadString( lTmp );
	ParseNumber( lTmp, m_Height, GB_HEIGHT_SHIFT );

	// Any remaining parameters are special control flags
	while( true )
	{
		apParser->ReadString( lTmp );
		if( lTmp[0] == '}' )
			break;

		uint32 lFlag = 0;
		apParser->FilterStringGetEnumAsserted( lTmp.c_str(), g_Flags, lFlag );
		switch( lFlag )
		{
			case GBF_ASPECT_WIDTH:
			case GBF_ASPECT_HEIGHT:
			{
				int32 lAspectX;
				int32 lAspectY;
				apParser->ReadInt32( &lAspectX );
				apParser->ReadInt32( &lAspectY );
				if( lFlag == GBF_ASPECT_WIDTH )
				{
					m_AspectRatio = ( lAspectX << 12 ) / lAspectY;
					m_Flags |= GB_ASPECT_WIDTH;
				}
				else
				{
					m_AspectRatio = ( lAspectY << 12 ) / lAspectX;
					m_Flags |= GB_ASPECT_HEIGHT;
				}
				break;
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Get the bounds in pixel co-ordinates
//-----------------------------------------------------------------------------
void GUIBounds::GetScreenRect( CIwRect& arRect, GUIElement* apOwner ) const
{
	CIwRect lParentRect;

	GUIFrame* lpParent = apOwner->GetParent();
	if( lpParent )
	{
		// Find parent frames screen rectangle
		lpParent->GetBounds().GetScreenRect( lParentRect, lpParent );
	}
	else
	{
		// No parent, so we set the parent rectangle to be full screen
		lParentRect.x = 0;
		lParentRect.y = 0;
		lParentRect.w = IwGxGetScreenWidth();
		lParentRect.h = IwGxGetScreenHeight();
	}

	// Calculate the width
	if( m_Flags & GB_WIDTH_PERCENTAGE )
	{
		int16 lWidth = m_Width;
		if( m_Flags & GB_WIDTH_NEGATIVE )
			lWidth = 4096 - lWidth;

		arRect.w = ( lWidth * lParentRect.w ) >> 12;
	}
	else
	{
		if( m_Flags & GB_WIDTH_NEGATIVE )
			arRect.w = lParentRect.w - m_Width;
		else
			arRect.w = m_Width;
	}

	// Calculate the height
	if( m_Flags & GB_HEIGHT_PERCENTAGE )
	{
		int16 lHeight = m_Height;
		if( m_Flags & GB_HEIGHT_NEGATIVE )
			lHeight = 4096 - lHeight;

		arRect.h = ( lHeight * lParentRect.h ) >> 12;
	}
	else
	{
		if( m_Flags & GB_HEIGHT_NEGATIVE )
			arRect.h = lParentRect.h - m_Height;
		else
			arRect.h = m_Height;
	}

	// Check to see if we need to keep width or height at a specific aspect ratio
	if( m_Flags & GB_ASPECT_WIDTH )
	{
		arRect.w = IW_FIXED_MUL( arRect.h, m_AspectRatio );
	}
	else if( m_Flags & GB_ASPECT_HEIGHT )
	{
		arRect.h = IW_FIXED_MUL( arRect.w, m_AspectRatio );
	}

	// Work out left edge co-ordinate
	if( m_Flags & GB_X_PERCENTAGE )
	{
		if( m_Flags & GB_X_NEGATIVE )
			arRect.x = IW_FIXED_MUL( 4096 - m_X, lParentRect.w );
		else
			arRect.x = IW_FIXED_MUL( m_X, lParentRect.w );
	}
	else 
	{
		if( m_Flags & GB_X_NEGATIVE )
			arRect.x = lParentRect.w - m_X;
		else
			arRect.x = m_X;
	}
	arRect.x += lParentRect.x;

	GB_HorizontalAlign lHorzAlign = ( GB_HorizontalAlign ) ( ( m_Flags >> GB_HORIZONTAL_ALIGN_SHIFT ) & GB_ALIGN_MASK );
	switch( lHorzAlign )
	{
		case GBHA_CENTRE:
			arRect.x -= ( arRect.w >> 1 );
			break;

		case GBHA_RIGHT:
			arRect.x -= arRect.w;
			break;
	}

	// Work out top edge co-ordinate
	if( m_Flags & GB_Y_PERCENTAGE )
	{
		if( m_Flags & GB_Y_NEGATIVE )
			arRect.y = IW_FIXED_MUL( 4096 - m_Y, lParentRect.h );
		else
			arRect.y = IW_FIXED_MUL( m_Y, lParentRect.h );
	}
	else
	{
		if( m_Flags & GB_Y_NEGATIVE )
			arRect.y = lParentRect.h - m_Y;
		else
			arRect.y = m_Y;
	}
	arRect.y += lParentRect.y;

	GB_VerticalAlign lVertAlign = ( GB_VerticalAlign ) ( ( m_Flags >> GB_VERTICAL_ALIGN_SHIFT) & GB_ALIGN_MASK );
	switch( lVertAlign )
	{
		case GBVA_CENTRE:
			arRect.y -= ( arRect.h >> 1 );
			break;

		case GBHA_RIGHT:
			arRect.y -= arRect.h;
			break;
	}
}

//-----------------------------------------------------------------------------
// Set the X anchor position (in pixels)
// Note: negative values are offsets from right hand edge of bounding area
//-----------------------------------------------------------------------------
void GUIBounds::SetXPixel( int16 aX )
{
	IW_CALLSTACK( "GUIBounds::SetXPixel" );
	m_Flags &= ~( GB_X_PERCENTAGE | GB_X_NEGATIVE );
	if( aX >= 0 )
	{
		m_X = aX;
	}
	else
	{
		m_X = -aX;
		m_Flags |= GB_X_NEGATIVE;
	}
}

//-----------------------------------------------------------------------------
// Set the X anchor type
//-----------------------------------------------------------------------------
void GUIBounds::SetXAnchor( GB_HorizontalAlign aAnchorType )
{
	IW_CALLSTACK( "GUIBounds::SetXAnchor" );

	// Clear the current horizontal alignment flags, then stuff in new ones
	m_Flags &= ~( GB_ALIGN_MASK << GB_HORIZONTAL_ALIGN_SHIFT );
	m_Flags |= aAnchorType << GB_HORIZONTAL_ALIGN_SHIFT;
}

//-----------------------------------------------------------------------------
// Set the Y anchor position (in pixels)
// Note: negative values are offsets from bottom edge of bounding area
//-----------------------------------------------------------------------------
void GUIBounds::SetYPixel( int16 aY )
{
	IW_CALLSTACK( "GUIBounds::SetYPixel" );
	m_Flags &= ~( GB_Y_PERCENTAGE | GB_Y_NEGATIVE );
	if( aY >= 0 )
	{
		m_Y = aY;
	}
	else
	{
		m_Y = -aY;
		m_Flags |= GB_Y_NEGATIVE;
	}
}

//-----------------------------------------------------------------------------
// Set the Y anchor type
//-----------------------------------------------------------------------------
void GUIBounds::SetYAnchor( GB_VerticalAlign aAnchorType )
{
	IW_CALLSTACK( "GUIBounds::SetYAnchor" );

	// Clear the current vertical alignment flags, then stuff in new ones
	m_Flags &= ~( GB_ALIGN_MASK << GB_VERTICAL_ALIGN_SHIFT );
	m_Flags |= aAnchorType << GB_VERTICAL_ALIGN_SHIFT;
}

//-----------------------------------------------------------------------------
// Set the width (in pixels)
//-----------------------------------------------------------------------------
void GUIBounds::SetWidthPixel( int16 aWidth )
{
	IW_CALLSTACK( "GUIBounds::SetWidth" );

	m_Flags &= ~( GB_WIDTH_PERCENTAGE | GB_WIDTH_NEGATIVE | GB_ASPECT_WIDTH | GB_ASPECT_HEIGHT );
	if( aWidth >= 0 )
	{
		m_Width = aWidth;
	}
	else
	{
		m_Width = -aWidth;
		m_Flags |= GB_WIDTH_NEGATIVE;
	}
}

//-----------------------------------------------------------------------------
// Set the height (in pixels)
//-----------------------------------------------------------------------------
void GUIBounds::SetHeightPixel( int16 aHeight )
{
	IW_CALLSTACK( "GUIBounds::SetHeight" );

	m_Flags &= ~( GB_HEIGHT_PERCENTAGE | GB_HEIGHT_NEGATIVE | GB_ASPECT_WIDTH | GB_ASPECT_HEIGHT );
	if( aHeight >= 0 )
	{
		m_Height = aHeight;
	}
	else
	{
		m_Height = -aHeight;
		m_Flags |= GB_HEIGHT_NEGATIVE;
	}
}

//-----------------------------------------------------------------------------
// Copy the given bounds
//-----------------------------------------------------------------------------
void GUIBounds::Copy( const GUIBounds& arBounds )
{
	IW_CALLSTACK( "GUIBounds::Copy" );
	m_X = arBounds.m_X;
	m_Y = arBounds.m_Y;
	m_Width = arBounds.m_Width;
	m_Height = arBounds.m_Height;
	m_AspectRatio = arBounds.m_AspectRatio;
	m_Flags = arBounds.m_Flags;
}

//-----------------------------------------------------------------------------
// Parses the string input, taking note of whether the number is a negative
// or percentage value and setting flags appropriately
//-----------------------------------------------------------------------------
#ifdef IW_BUILD_RESOURCES
void GUIBounds::ParseNumber( CIwStringS& arStr, int16& arValue, uint32 aFlagShift )
{
	IW_CALLSTACK( "GUIBounds::ParseNumber" );

	float lValue;
	uint32 lFlags = 0;

	if( arStr[arStr.length() - 1] == '%' )
	{
		arStr.setLength( arStr.length() - 1 );
		lFlags |= GB_PERCENTAGE;
	}

	lValue = (float) atof( arStr.c_str() );
	if( lValue < 0.0f )
	{
		lValue = -lValue;
		lFlags |= GB_NEGATIVE;
	}

	if( lFlags & GB_PERCENTAGE )
	{
		// Convert percentage into fixed point value in range 0..4096 for 0..100%
		arValue = ( int16 ) ( ( int32 ) ( lValue * 4096.0f ) / 100 );
	}
	else
	{
		arValue = ( int16 ) lValue;
	}

	m_Flags |= ( lFlags << aFlagShift );
}
#endif