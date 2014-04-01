//-----------------------------------------------------------------------------
// GUIColour.cpp
// Used to specify a colour in GUI definitions
//-----------------------------------------------------------------------------

#include "GUIColour.h"

// Library includes
#include "IwDebug.h"
#include "IwResManager.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY( GUIColour );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIColour::GUIColour() :
	m_Colour( 0xFFFFFFFF )
{
	IW_CALLSTACK( "GUIColour::GUIColour" );
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUIColour::Serialise()
{
	IW_CALLSTACK( "GUIColour::Serialise" );

	CIwResource::Serialise();
	IwSerialiseUInt32( m_Colour );
}

//-----------------------------------------------------------------------------
// Parse an attribute from an ITX file
//-----------------------------------------------------------------------------
bool GUIColour::ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIColour::ParseAttribute" );

	if( !strcmp( apAttrName, "rgba" ) )
	{
		ParseColour( apParser, this );
	}
	else
		return CIwResource::ParseAttribute( apParser, apAttrName );
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Finished parsing the object
//-----------------------------------------------------------------------------
void GUIColour::ParseClose( CIwTextParserITX* apParser )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIColour::ParseClose" );

	// Top level object, add to resource manager
	IwGetResManager()->AddRes( RESTYPE_GUICOLOUR, this );
#endif
}

//-------------------------------------------------------------------------
// Static helper method to allow other GUI classes to easily parse a colour
//-------------------------------------------------------------------------
void GUIColour::ParseColour( CIwTextParserITX* apParser, GUIColour* apColour )
{
	char lBuf[64];
	apParser->PeekString( lBuf, 64 );

	if( lBuf[0] == '{' )
	{
		// We've got an RGBA quartet
		uint32 lColour[4] = { 255, 255, 255, 255 };

		apParser->ReadUInt32Array( lColour, 4 );

		IwAssertMsg( GUI, lColour[0] < 256, ( "GUIColour %s has invalid red value", apColour->DebugGetName() ) );
		IwAssertMsg( GUI, lColour[1] < 256, ( "GUIColour %s has invalid green value", apColour->DebugGetName() ) );
		IwAssertMsg( GUI, lColour[2] < 256, ( "GUIColour %s has invalid blue value", apColour->DebugGetName() ) );
		IwAssertMsg( GUI, lColour[3] < 256, ( "GUIColour %s has invalid alpha value", apColour->DebugGetName() ) );

		apColour->SetColour( ( lColour[0] << 0) | ( lColour[1] << 8) | ( lColour[2] << 16) | ( lColour[3] << 24 ) );
	}
	else
	{
		// We've been given a named reference to an existing GUIColour
		CIwStringS lColourName;
		apParser->ReadString( lColourName );
		GUIColour* lpColour = ResourceFindByName<GUIColour>( lColourName.c_str(), RESTYPE_GUICOLOUR );

		IwAssertMsg( GUI, lpColour, ( "Unable to locate specified GUIColour: %s", lColourName.c_str() ) );
		apColour->SetColour( lpColour->GetColour() );
	}
}