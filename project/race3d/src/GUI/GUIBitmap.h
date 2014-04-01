//-----------------------------------------------------------------------------
// GUIBitmap.h
// Displays a bitmap on screen
//-----------------------------------------------------------------------------

#ifndef GUIBITMAP_H
#define GUIBITMAP_H

// Library includes
#include "IwMaterial.h"

// Project includes
#include "GUIColour.h"
#include "GUIElement.h"

class GUIBitmap : public GUIElement
{
public:
	IW_MANAGED_DECLARE( GUIBitmap );
	GUIBitmap();
	virtual ~GUIBitmap();

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

	// Called once per frame to render GUI element
	virtual void Render();

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------
	void SetAngle( iwangle aAngle );
	iwangle GetAngle() const;

	// Replace the materials texture with the one represented by the
	// supplied hash value
	void SetTexture( uint32 aTextureHash );

private:
	//-------------------------------------------------------------------------
	// Enumerations
	//-------------------------------------------------------------------------
	enum GBMirror
	{
		GB_MIRROR_X = 1,
		GB_MIRROR_Y
	};

	//-------------------------------------------------------------------------
	// Private members
	//-------------------------------------------------------------------------
	// Material used to render this image
	CIwMaterial	m_Material;

	// Colour used to render the bitmap
	GUIColour	m_Colour;

	// Allocated list of vertex positions (as ratios of width/height)
	CIwSVec2*	m_pVertRatios;

	// Allocated list of UV data (if needed)
	CIwFVec2*	m_pUV;

	// Rotation angle for drawing
	iwangle		m_Angle;

	// Number of vertices/UVs
	int16		m_VertexCount;

	// Mirror flags
	int16		m_Mirror;

	//-------------------------------------------------------------------------
	// Static members
	//-------------------------------------------------------------------------
	static CIwSVec2	s_VertRatios_MirrorNone[4];
	static CIwSVec2	s_VertRatios_MirrorX[6];
	static CIwSVec2	s_VertRatios_MirrorY[6];
	static CIwSVec2	s_VertRatios_MirrorXY[10];

	//-------------------------------------------------------------------------
	// Private methods
	//-------------------------------------------------------------------------
	// Create our vertex and UV arrays for rendering the bitmap
	void CreateVertexAndUVData();
};

//-----------------------------------------------------------------------------

inline void GUIBitmap::SetAngle( iwangle aAngle )
{
	m_Angle = aAngle;
}

//-----------------------------------------------------------------------------

inline iwangle GUIBitmap::GetAngle() const
{
	return m_Angle;
}

#endif