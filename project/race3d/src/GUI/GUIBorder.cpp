//-----------------------------------------------------------------------------
// GUIBorder.cpp
// Easy way to specify a scaleable bordered image.  The corners, sides and
// middle of a single texture are used as tiles to build up a border image
// that can stretch to any required size
//-----------------------------------------------------------------------------

#include "GUIBorder.h"

// Library includes
#include "IwGx.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIColour.h"
#include "GUIDraw.h"
#include "GUIFrame.h"
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY( GUIBorder );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIBorder::GUIBorder()
{
	IW_CALLSTACK( "GUIBorder::GUIBorder" );

	m_Material.Init();
	m_Material.SetColAmbient( 0xffffffff );		// Default to full colour!
	m_Material.SetColDiffuse( 0xffffffff );		// Default to full colour!
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUIBorder::Serialise()
{
	IW_CALLSTACK( "GUIBorder::Serialise" );

	GUIElement::Serialise();

	m_Material.Serialise();

	// Have to serialise and reset the material colour since
	// CIwMaterial::Serialise doesn't seem to behave properly
	m_Colour.Serialise();
	m_Material.SetColAmbient( m_Colour.GetColour() );
	m_Material.SetColDiffuse( m_Colour.GetColour() );

	m_BRBounds.Serialise();
	m_TLBounds.Serialise();

	// Write out the UV data, as this won't change
	for( uint32 i = 0; i < 16; i++ )
	{
		m_UV[i].Serialise();
	}
}

//-----------------------------------------------------------------------------
// Lookup resource references
//-----------------------------------------------------------------------------
void GUIBorder::Resolve()
{
	IW_CALLSTACK( "GUIBorder::Resolve" );

	GUIElement::Resolve();

	m_Material.Resolve();
}

//-----------------------------------------------------------------------------
// Parse an attribute from an ITX file
//-----------------------------------------------------------------------------
bool GUIBorder::ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIBorder::ParseAttribute" );

	if( !strcmp( apAttrName, "bounds" ) )
	{
		IwAssertMsg( GUI, false, ("GUIBorder does not use 'bounds' parameter.  Use 'bounds_tl', 'bounds_mid' and 'bounds_br' instead" ) );
	}
	else if( !strcmp( apAttrName, "bounds_br" ) )
	{
		m_BRBounds.Parse( apParser );
	}
	else if( !strcmp( apAttrName, "bounds_tl" ) )
	{
		m_TLBounds.Parse( apParser );
	}
	else if( !strcmp( apAttrName, "material" ) )
	{
		CIwStringS lMaterialName;

		apParser->ReadString( lMaterialName );
		CIwMaterial* lpMaterial = ResourceFindByName<CIwMaterial>( lMaterialName.c_str(), "CIwMaterial" );
		IwAssertMsg( GUI, lpMaterial, ( "Material named %s not found", lMaterialName.c_str() ) );

		// Make a copy of the material so we can tweak colours etc later
		m_Material.Copy( *lpMaterial );
	}
	else if( !strcmp( apAttrName, "colour" ) )
	{
		GUIColour::ParseColour( apParser, &m_Colour );
		m_Material.SetColAmbient( m_Colour.GetColour() );
		m_Material.SetColDiffuse( m_Colour.GetColour() );
	}
	else if( !strcmp( apAttrName, "uv" ) )
	{
		// Read in pixel UV data
		int16 lUVData[8];
		apParser->ReadInt16Array( lUVData, 8 );
		float u[4], v[4];
		// Convert pixel UV positions into fractions of width/height
		for( uint32 i = 0; i < 8; i++ )
		{
			if( i & 1 )
			{
				v[i >> 1] = MakeUVValue( lUVData[i], m_Material.GetTexture()->GetHeight() );
			}
			else
			{
				u[i >> 1] = MakeUVValue( lUVData[i], m_Material.GetTexture()->GetWidth() );
			}
		}

		// Populate UV list with the above data
		for( uint32 i = 0; i < 16; i++ )
		{
			m_UV[i].x = u[i & 3];
			m_UV[i].y = v[i / 4];
		}
	}
	else
		return GUIElement::ParseAttribute( apParser, apAttrName );
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Clone this element
//-----------------------------------------------------------------------------
void GUIBorder::Clone( GUIElement* apCloneSource )
{
	IW_CALLSTACK( "GUIBorder::Clone" );

	GUIElement::Clone( apCloneSource );

	GUIBorder* lpBorderSource = reinterpret_cast<GUIBorder*>( apCloneSource );
	m_BRBounds.Copy( lpBorderSource->m_BRBounds );
	m_TLBounds.Copy( lpBorderSource->m_TLBounds );
	m_Material.Copy( lpBorderSource->m_Material );
	m_Colour.Copy( lpBorderSource->m_Colour );
	memcpy( m_UV, lpBorderSource->m_UV, sizeof( CIwFVec2 ) * 16 );
}

//-----------------------------------------------------------------------------
// Called once per frame to update GUI element
//-----------------------------------------------------------------------------
void GUIBorder::Update( int32 aTick )
{
	IW_CALLSTACK( "GUIBorder::Update" );

	// Calculate the screen co-ordinates for our three border sections
	CIwRect lBR, lTL;

	m_BRBounds.GetScreenRect( lBR, this );
	m_TLBounds.GetScreenRect( lTL, this );

	// Set up vertices...
	// Top row
	m_Pts[0].x = lTL.x;				m_Pts[0].y = lTL.y;
	m_Pts[1].x = lTL.x + lTL.w;		m_Pts[1].y = lTL.y;
	m_Pts[2].x = lBR.x;				m_Pts[2].y = lTL.y;
	m_Pts[3].x = lBR.x + lBR.w;		m_Pts[3].y = lTL.y;

	// Second row
	m_Pts[4].x = lTL.x;				m_Pts[4].y = lTL.y + lTL.h;
	m_Pts[5].x = lTL.x + lTL.w;		m_Pts[5].y = lTL.y + lTL.h;
	m_Pts[6].x = lBR.x;				m_Pts[6].y = lTL.y + lTL.h;
	m_Pts[7].x = lBR.x + lBR.w;		m_Pts[7].y = lTL.y + lTL.h;

	// Third row
	m_Pts[8].x = lTL.x;				m_Pts[8].y = lBR.y;
	m_Pts[9].x = lTL.x + lTL.w;		m_Pts[9].y = lBR.y;
	m_Pts[10].x = lBR.x;			m_Pts[10].y = lBR.y;
	m_Pts[11].x = lBR.x + lBR.w;	m_Pts[11].y = lBR.y;

	// Fourth row
	m_Pts[12].x = lTL.x;			m_Pts[12].y = lBR.y + lBR.h;
	m_Pts[13].x = lTL.x + lTL.w;	m_Pts[13].y = lBR.y + lBR.h;
	m_Pts[14].x = lBR.x;			m_Pts[14].y = lBR.y + lBR.h;
	m_Pts[15].x = lBR.x + lBR.w;	m_Pts[15].y = lBR.y + lBR.h;

	// Make all co-ordinates sub pixel
	for( uint32 i = 0; i < 16; i++ )
	{
		m_Pts[i].x <<= SUBPIXEL_SHIFT;
		m_Pts[i].y <<= SUBPIXEL_SHIFT;
	}
}

//-----------------------------------------------------------------------------
// Called once per frame to render GUI element
//-----------------------------------------------------------------------------

// Indices using degenerate triangles so we can render the entire border
// image in one draw call
static uint16 g_Indices[26] = { 0, 4, 1, 5, 2, 6, 3, 7, 7, 7,
								11, 6, 10, 5, 9, 4, 8, 8, 8,
								12, 9, 13, 10, 14, 11, 15 };

void GUIBorder::Render()
{
	IW_CALLSTACK( "GUIBorder::Render" );

	IwGxSetMaterial( &m_Material );
	IwGxSetVertStreamScreenSpaceSubPixel( m_Pts, 16 );
	IwGxSetColStream( NULL );
	IwGxSetUVStream( m_UV );
	IwGxDrawPrims( IW_GX_TRI_STRIP, g_Indices, 26 );
}