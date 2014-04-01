//-----------------------------------------------------------------------------
// GUILabel.h
// Displays a line of text on screen
//-----------------------------------------------------------------------------

#ifndef GUILABEL_H
#define GUILABEL_H

// Library includes
#include "IwGxFont.h"
#include "IwString.h"

// Project includes
#include "GUIColour.h"
#include "GUIElement.h"
#include "../Localization/Localization.h"

class GUILabel : public GUIElement
{
public:
	IW_MANAGED_DECLARE( GUILabel );
	GUILabel();

	//-------------------------------------------------------------------------
	// Virtual overrides from CIwManaged
	//-------------------------------------------------------------------------
	virtual void Serialise();
	virtual void Resolve();
	virtual bool ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName );

	//-------------------------------------------------------------------------
	// GUIElement declared virtuals
	//-------------------------------------------------------------------------
	// Clone this element
	virtual void Clone( GUIElement* apCloneSource );

	// Called once per frame to update GUI element
	virtual void Update( int32 aTick );

	// Called once per frame to render GUI element
	virtual void Render();

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------
	// Change the text being displayed
	void SetText( const char* apText );

	// Change the text being displayed to a localised string (given reference)
	void SetString( uint32 aTextHash );

	// Change the text being displayed to a localised string (given text identifier)
	void SetString( const char* apTextIdentifier );

	// Reset text
	void ResetText();

private:
	//-------------------------------------------------------------------------
	// Enumerations
	//-------------------------------------------------------------------------
	enum LabelFlags
	{
		LF_PREPARED		= (1 << 0)		// Set when the prepared text is valid for use
	};

	// Used when specifying a fixed text string in the ITX file
	CIwStringL	m_Text;

	// The font used to render the label
	CIwGxFont*	m_pFont;

	// Font alignment options
	uint8		m_HorzAlign;
	uint8		m_VertAlign;

	// Some flags to control when prepared text needs updating etc.
	uint16		m_LabelFlags;

	// Prepared font structure needed for rendering text
	CIwGxFontPreparedData	m_PreparedText;

	// Colour to render text
	GUIColour	m_Colour;

	// Hash of localised text string
	uint32 m_TextHash;
};

#endif