//-----------------------------------------------------------------------------
// GUIOffset.h
// Class used to reference another GUIElement for positioning purposes
//-----------------------------------------------------------------------------

#ifndef GUIOFFSET_H
#define GUIOFFSET_H

// Library includes
#include "s3eTypes.h"
#include "IwResource.h"		// Needed for IW_BUILD_RESOURCES definition...

// Forward declarations
class CIwTextParserITX;
class GUIElement;

// The following definitions must match the order of OffsetSide enum
#define OFF_LEFT	(1 << 0)		// Offset to the left of named element
#define OFF_RIGHT	(1 << 1)		// Offset to the right of named element
#define OFF_TOP		(1 << 2)		// Offset above named element
#define OFF_BOTTOM	(1 << 3)		// Offset below named element

#define OFF_FIRST_FLAG	4							// Bit position of first flag
#define OFF_FILL			(1 << (OFF_FIRST_FLAG))		// Resize width/height to fill remaining area (depending on offset type)
#define OFF_SPACING			(1 << (OFF_FIRST_FLAG + 1))	// Add specified spacing size
#define OFF_SPACING_PERC	(1 << (OFF_FIRST_FLAG + 2))	// Spacing value is a percentage

//-----------------------------------------------------------------------------
// GUIOffset
//-----------------------------------------------------------------------------
class GUIOffset
{
public:
	GUIOffset();

	// Serialise offset data
	void Serialise();

	// Resolve offset data
	void Resolve();

	// Parse attributes from text file
	void Parse( CIwTextParserITX* apParser );

	// Set the GUIBounds for the given GUIElement from this offset data
	void SetBounds( GUIElement* apOwner );

	// Copy the given offset
	void Copy( const GUIOffset& arOffset );

private:
	//-------------------------------------------------------------------------
	// Enumerations
	//-------------------------------------------------------------------------
	enum OffsetSide
	{
		OS_LEFT, OS_RIGHT, OS_TOP, OS_BOTTOM
	};

	enum OffsetFlags
	{
		OF_FILL
	};

	//-------------------------------------------------------------------------
	// Private members
	//-------------------------------------------------------------------------
#ifdef IW_BUILD_RESOURCES
	void ParseNumber( CIwStringS& arStr, int16& arValue, uint16 aPercentageFlag );
#endif // IW_BUILD_RESOURCES

	//-------------------------------------------------------------------------
	// Private members
	//-------------------------------------------------------------------------
	// Hash of the element we are relative to
	uint32	m_OffsetElementHash;

	// Flags
	uint16	m_OffsetFlags;

	// Spacing value
	int16	m_Spacing;

	// The GUIElement we are relative to
	GUIElement* m_pOffsetElement;
};

#endif // GUIOFFSET_H