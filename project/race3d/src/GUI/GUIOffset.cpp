//-----------------------------------------------------------------------------
// GUIOffset.cpp
// Class used to reference another GUIElement for positioning purposes
//-----------------------------------------------------------------------------

#include "GUIOffset.h"

// Library includes
#include "IwResource.h"
#include "IwRuntime.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIElement.h"
#include "GUIFrame.h"
#include "GUIManager.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIOffset::GUIOffset() :
	m_OffsetElementHash( 0 ),
	m_OffsetFlags( 0 ),
	m_pOffsetElement( NULL ),
	m_Spacing( 0 )
{
	IW_CALLSTACK( "GUIOffset::GUIOffset" );
}

//-----------------------------------------------------------------------------
// Serialise offset data
//-----------------------------------------------------------------------------
void GUIOffset::Serialise()
{
	IW_CALLSTACK( "GUIOffset::Serialise" );

	IwSerialiseUInt32( m_OffsetElementHash );
	IwSerialiseUInt16( m_OffsetFlags );
	IwSerialiseInt16( m_Spacing );
}

//-----------------------------------------------------------------------------
// Resolve offset data
//-----------------------------------------------------------------------------
void GUIOffset::Resolve()
{
	IW_CALLSTACK( "GUIOffset::Resolve" );

	if( m_OffsetElementHash )
	{
		m_pOffsetElement = GetGUIManager()->FindElementByHash( m_OffsetElementHash );
		IwAssertMsg( GUI, m_pOffsetElement, ( "Unable to locate GUIElement hashed %08x.", m_OffsetElementHash ) );
	}
}

//-----------------------------------------------------------------------------
// Parse attributes from text file
//-----------------------------------------------------------------------------
// Offset side (must match order of OffsetSide enum)
static const char* g_OffsetSideNames[] =
{
	"left", "right", "top", "bottom", NULL
};

static const char* g_OffsetFlags[] =
{
	"fill", "spacing", "spacing-perc", NULL
};

void GUIOffset::Parse( CIwTextParserITX* apParser )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIOffset::Parse" );

	CIwStringS lTmp;

	// First we should have the opening curly bracket, which we'll just skip, but
	// we'll assert if it's not there
	apParser->ReadString( lTmp );
	IwAssertMsg( GUI, lTmp[0] == '{', ("Error parsing GUIOffset, missing {" ) );

	// First parameter is the name of the GUIElement we want to be offset from
	apParser->ReadStringHash( &m_OffsetElementHash );

	// Next is the side of the parent GUIElement we want to join on to
	apParser->ReadString( lTmp );
	uint32 lFlagTmp = 0;
	apParser->FilterStringGetEnumAsserted( lTmp.c_str(), g_OffsetSideNames, lFlagTmp );
	m_OffsetFlags = 1 << lFlagTmp;

	// Any remaining parameters are special control flags
	while( true )
	{
		apParser->ReadString( lTmp );
		if( lTmp[0] == '}' )
			break;

		uint32 lFlag = 0;
		apParser->FilterStringGetEnumAsserted( lTmp.c_str(), g_OffsetFlags, lFlag );
		lFlag = 1 << ( OFF_FIRST_FLAG + lFlag );

		m_OffsetFlags |= lFlag;
		switch( lFlag )
		{
			case OFF_SPACING:
			{
				// Next value is a spacing value
				apParser->ReadString( lTmp );
				ParseNumber( lTmp, m_Spacing, OFF_SPACING_PERC );
				break;
			}
		}
	}
#endif // IW_BUILD_RESOURCES
}

//-----------------------------------------------------------------------------
// Set the GUIBounds for the given GUIElement from this offset data
//-----------------------------------------------------------------------------
void GUIOffset::SetBounds( GUIElement* apOwner )
{
	IW_CALLSTACK( "GUIOffset::SetBounds" );

	// Do we have an element to offset to?
	if( !m_pOffsetElement )
		return;

	GUIBounds lBounds;
	lBounds.Copy( m_pOffsetElement->GetBounds() );

	CIwRect lRect;
	lBounds.GetScreenRect( lRect, m_pOffsetElement );
	const CIwRect& lrParentRect = m_pOffsetElement->GetParent()->GetScreenRect();

	int16 lSpacing = 0;
	// Any spacing requested?
	if( m_OffsetFlags & OFF_SPACING )
	{
		bool lWidth = ( m_OffsetFlags & (OFF_LEFT | OFF_RIGHT ) ) != 0;
		if( m_OffsetFlags & OFF_SPACING_PERC )
			lSpacing = IW_FIXED_MUL( lWidth ? lrParentRect.w : lrParentRect.h, m_Spacing );
		else
			lSpacing = m_Spacing;
	}

	int16 lNewPos = 0;

	if( m_OffsetFlags & OFF_LEFT )
	{
		// We want to offset to the left
		lNewPos = lRect.x - lSpacing - lrParentRect.x;
		lBounds.SetXPixel( lNewPos );
		lBounds.SetXAnchor( GBHA_RIGHT );

		// Fill control out to extents of parent control
		if( m_OffsetFlags & OFF_FILL )
		{
			lBounds.SetWidthPixel( lNewPos );
		}
	}
	else if( m_OffsetFlags & OFF_RIGHT )
	{
		// We want to offset to the right
		lNewPos = lRect.x + lRect.w + lSpacing - lrParentRect.x;
		lBounds.SetXPixel( lNewPos );
		lBounds.SetXAnchor( GBHA_LEFT );

		// Fill control out to extents of parent control
		if( m_OffsetFlags & OFF_FILL )
		{
			lBounds.SetWidthPixel( lrParentRect.w - lNewPos );
		}
	}
	else if( m_OffsetFlags & OFF_TOP )
	{
		// We want to offset above
		lNewPos = lRect.y - lSpacing - lrParentRect.y;
		lBounds.SetYPixel( lNewPos );
		lBounds.SetYAnchor( GBVA_BOTTOM );

		// Fill control out to extents of parent control
		if( m_OffsetFlags & OFF_FILL )
		{
			lBounds.SetHeightPixel( lNewPos );
		}
	}
	else if( m_OffsetFlags & OFF_BOTTOM )
	{
		// We want to offset below
		lNewPos = lRect.y + lRect.h + lSpacing - lrParentRect.y;
		lBounds.SetYPixel( lNewPos );
		lBounds.SetYAnchor( GBVA_TOP );

		// Fill control out to extents of parent control
		if( m_OffsetFlags & OFF_FILL )
		{
			lBounds.SetHeightPixel( lrParentRect.h - lNewPos );
		}
	}

	apOwner->SetBounds( lBounds );
}

//-----------------------------------------------------------------------------
// Copy the given offset
//-----------------------------------------------------------------------------
void GUIOffset::Copy( const GUIOffset& arOffset )
{
	IW_CALLSTACK( "GUIOffset::Copy" );

	m_OffsetElementHash = arOffset.m_OffsetElementHash;
	m_OffsetFlags = arOffset.m_OffsetFlags;
	m_Spacing = arOffset.m_Spacing;
	m_pOffsetElement = arOffset.m_pOffsetElement;
}

//-----------------------------------------------------------------------------
// Parses the string input, taking note of whether the number is a percentage
// value and setting the specified flag accordingly
//-----------------------------------------------------------------------------
#ifdef IW_BUILD_RESOURCES
void GUIOffset::ParseNumber( CIwStringS& arStr, int16& arValue, uint16 aPercentageFlag )
{
	IW_CALLSTACK( "GUIOffset::ParseNumber" );

	float lValue;
	bool lPercentage = false;

	if( arStr[arStr.length() - 1] == '%' )
	{
		arStr.setLength( arStr.length() - 1 );
		m_OffsetFlags |= aPercentageFlag;
		lPercentage = true;
	}

	lValue = ( float ) atof( arStr.c_str() );

	if( lPercentage )
	{
		// Convert percentage into fixed point value in range 0..4096 for 0..100%
		arValue = ( int16 ) ( ( int32 ) ( lValue * 4096.0f ) / 100 );
	}
	else
	{
		arValue = ( int16 ) lValue;
	}
}
#endif