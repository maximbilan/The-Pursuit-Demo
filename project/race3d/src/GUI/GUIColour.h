//-----------------------------------------------------------------------------
// GUIColour.h
// Used to specify a colour in GUI definitions
//-----------------------------------------------------------------------------

#ifndef GUICOLOUR_H
#define GUICOLOUR_H

// Library includes
#include "IwGxTypes.h"

// Project includes
#include "IwResource.h"

#define RESTYPE_GUICOLOUR	"GUICOLOUR"

class GUIColour : public CIwResource
{
public:
	IW_MANAGED_DECLARE( GUIColour );
	GUIColour();

	//-------------------------------------------------------------------------
	// Virtual overrides from CIwManaged
	//-------------------------------------------------------------------------
	virtual void Serialise();
	virtual bool ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName );
	virtual void ParseClose( CIwTextParserITX* apParser );

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------
	// Accessors
	const uint32 GetColour() const;
	void SetColour( uint32 aColour );
	void Copy( const GUIColour& arColour );

	//-------------------------------------------------------------------------
	// Helper method to allow other GUI classes to easily parse a colour
	//-------------------------------------------------------------------------
	static void ParseColour( CIwTextParserITX* apParser, GUIColour* apColour );

private:
	// ABGR colour value
	uint32	m_Colour;
};

//-----------------------------------------------------------------------------

inline const uint32 GUIColour::GetColour() const
{
	return m_Colour;
}

//---------------------------------------------------------------------------

inline void GUIColour::SetColour( uint32 aColour )
{
	m_Colour = aColour;
}

//---------------------------------------------------------------------------

inline void GUIColour::Copy( const GUIColour& arColour )
{
	m_Colour = arColour.GetColour();
}

#endif