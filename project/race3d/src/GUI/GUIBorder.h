//-----------------------------------------------------------------------------
// GUIBorder.h
// Easy way to specify a scaleable bordered image.  The corners, sides and
// middle of a single texture are used as tiles to build up a border image
// that can stretch to any required size
//-----------------------------------------------------------------------------

#ifndef GUIBORDER_H
#define GUIBORDER_H

// Library includes
#include "IwMaterial.h"

// Project includes
#include "GUIColour.h"
#include "GUIElement.h"

class GUIBorder : public GUIElement
{
public:
	IW_MANAGED_DECLARE( GUIBorder );
	GUIBorder();

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

protected:
	// Used to specify bounding rectangles for the top left and bottom
	// right corners of the borders.  The m_Bounds member inherited from
	// GUIElement will not be used in GUIBorder
	GUIBounds	m_BRBounds;
	GUIBounds	m_TLBounds;

	// Material used to render this image
	CIwMaterial	m_Material;

	// Colour used to render the border
	GUIColour	m_Colour;

	// Vertex data required to render border as a tristrip
	CIwSVec2	m_Pts[16];

	// UV data required to render border
	CIwFVec2	m_UV[16];
};

#endif