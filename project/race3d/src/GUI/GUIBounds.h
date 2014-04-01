//-----------------------------------------------------------------------------
// GUIBounds.h
// Class used to store the bounding rectangle of a GUI element
//-----------------------------------------------------------------------------

#ifndef GUIBOUNDS_H
#define GUIBOUNDS_H

// Library includes
#include "s3eTypes.h"
#include "IwString.h"
#include "IwResource.h"		// Needed for IW_BUILD_RESOURCES definition...

// Forward declarations
class CIwRect;
class CIwTextParserITX;
class GUIElement;

// Defines for the m_Flags parameter
#define GB_ALIGN_MASK				3
#define GB_HORIZONTAL_ALIGN_SHIFT	0
#define GB_VERTICAL_ALIGN_SHIFT		2

#define GB_NEGATIVE					(1 << 0)
#define GB_PERCENTAGE				(1 << 1)

#define GB_X_SHIFT					4
#define GB_X_NEGATIVE				(1 << 4)
#define GB_X_PERCENTAGE				(1 << 5)

#define GB_Y_SHIFT					6
#define GB_Y_NEGATIVE				(1 << 6)
#define GB_Y_PERCENTAGE				(1 << 7)

#define GB_WIDTH_SHIFT				8
#define GB_WIDTH_NEGATIVE			(1 << 8)
#define GB_WIDTH_PERCENTAGE			(1 << 9)

#define GB_HEIGHT_SHIFT				10
#define GB_HEIGHT_NEGATIVE			(1 << 10)
#define GB_HEIGHT_PERCENTAGE		(1 << 11)

// Additional flags
enum GB_Flags
{
	GBF_ASPECT_WIDTH,
	GBF_ASPECT_HEIGHT
};

#define GB_FIRST_FLAG				12
#define GB_ASPECT_WIDTH				(1 << (GB_FIRST_FLAG + GBF_ASPECT_WIDTH))
#define GB_ASPECT_HEIGHT			(1 << (GB_FIRST_FLAG + GBF_ASPECT_HEIGHT))

enum	GB_HorizontalAlign	 	{ GBHA_LEFT, GBHA_CENTRE, GBHA_RIGHT };
enum	GB_VerticalAlign 		{ GBVA_TOP, GBVA_CENTRE, GBVA_BOTTOM };

//-----------------------------------------------------------------------------
// GUIBounds
//-----------------------------------------------------------------------------
class GUIBounds
{
public:
	GUIBounds();

	// Serialise bounds data
	void Serialise();

	// Parse attributes from text file
	void Parse( CIwTextParserITX* apParser );

	// Get the bounds in pixel co-ordinates
	void GetScreenRect( CIwRect& arRect, GUIElement* apOwner ) const;

	// Set the X anchor position (in pixels)
	void SetXPixel( int16 aX );

	// Set the X anchor type
	void SetXAnchor( GB_HorizontalAlign aAnchorType );

	// Set the Y anchor position (in pixels)
	void SetYPixel( int16 aY );

	// Set the Y anchor type
	void SetYAnchor( GB_VerticalAlign aAnchorType );

	// Set the width (in pixels)
	void SetWidthPixel( int16 aWidth );

	// Set the height (in pixels)
	void SetHeightPixel( int16 aHeight );

	// Copy the given bounds
	void Copy( const GUIBounds& arBounds );

private:
	//----------------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------------
#ifdef IW_BUILD_RESOURCES
	void ParseNumber( CIwStringS& arStr, int16& arValue, uint32 aFlagShift );
#endif

	//----------------------------------------------------------------------------
	// Private members
	//----------------------------------------------------------------------------
	int16	m_X;
	int16	m_Y;
	int16	m_Width;
	int16	m_Height;
	int32	m_AspectRatio;
	uint32	m_Flags;
};

#endif