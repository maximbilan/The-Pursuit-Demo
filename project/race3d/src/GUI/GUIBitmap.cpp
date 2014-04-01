//-----------------------------------------------------------------------------
// GUIBitmap.cpp
// Displays a bitmap on screen
//-----------------------------------------------------------------------------

#include "GUIBitmap.h"

// Library includes
#include "IwGx.h"
#include "IwTextParserITX.h"

// Project includes
#include "GUIColour.h"
#include "GUIDraw.h"
#include "GUIFrame.h"
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY( GUIBitmap );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIBitmap::GUIBitmap() :
	m_pVertRatios( NULL ),
	m_pUV( NULL ),
	m_Angle( 0 ),
	m_Mirror( 0 ),
	m_VertexCount( 0 )
{
	IW_CALLSTACK( "GUIBitmap::GUIBitmap" );

	m_Material.Init();
	m_Material.SetColAmbient( 0xffffffff );		// Default to full colour!
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
GUIBitmap::~GUIBitmap()
{
	IW_CALLSTACK( "GUIBitmap::~GUIBitmap" );
	delete[] m_pUV;
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUIBitmap::Serialise()
{
	IW_CALLSTACK( "GUIBitmap::Serialise" );

	GUIElement::Serialise();

	IwSerialiseInt32( m_Angle );
	IwSerialiseInt16( m_VertexCount );
	IwSerialiseInt16( m_Mirror );

	m_Material.Serialise();

	// Have to serialise and reset the material colour since
	// CIwMaterial::Serialise doesn't seem to behave properly
	m_Colour.Serialise();
	m_Material.SetColAmbient( m_Colour.GetColour() );
	m_Material.SetColDiffuse( m_Colour.GetColour() );

	// Do we have UVs specified?
	bool lHasUVs = ( m_pUV != NULL );
	IwSerialiseBool( lHasUVs );

	if( lHasUVs )
	{
		// If boolean is true but pointer is null we are reading from
		// binary data and therefore need to allocate our UV buffer
		if( !m_pUV )
			m_pUV = new CIwFVec2[4];

		// Serialise the actual UV data
		for( uint32 i = 0; i < 4; i++ )
		{
			m_pUV[i].Serialise();
		}
	}
}

//-----------------------------------------------------------------------------
// Lookup resource references
//-----------------------------------------------------------------------------
void GUIBitmap::Resolve()
{
	IW_CALLSTACK( "GUIBitmap::Resolve" );

	GUIElement::Resolve();

	m_Material.Resolve();

	// We need to generate some points and UVs to support mirroring
	CreateVertexAndUVData();
}

//-----------------------------------------------------------------------------
// Parse an attribute from an ITX file
//-----------------------------------------------------------------------------
bool GUIBitmap::ParseAttribute( CIwTextParserITX* apParser, const char* apAttrName )
{
#ifdef IW_BUILD_RESOURCES
	IW_CALLSTACK( "GUIBitmap::ParseAttribute" );

	if( !strcmp( apAttrName, "material" ) )
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
		int16 lTexelUVData[4];
		float lUVData[4];
		apParser->ReadInt16Array( lTexelUVData, 4 );

		// Convert pixel UV positions into fractions of width/height
		lUVData[0] = MakeUVValue( lTexelUVData[0], m_Material.GetTexture()->GetWidth() );
		lUVData[1] = MakeUVValue( lTexelUVData[1], m_Material.GetTexture()->GetHeight() );
		lUVData[2] = MakeUVValue( lTexelUVData[2], m_Material.GetTexture()->GetWidth() );
		lUVData[3] = MakeUVValue( lTexelUVData[3], m_Material.GetTexture()->GetHeight() );

		// See g_FullTextureUV in Draw.cpp for ordering of UV data
		m_VertexCount = 4;
		m_pUV = new CIwFVec2[m_VertexCount];
		m_pUV[0].x = lUVData[0];	m_pUV[0].y = lUVData[1];
		m_pUV[1].x = lUVData[0];	m_pUV[1].y = lUVData[3];
		m_pUV[2].x = lUVData[2];	m_pUV[2].y = lUVData[3];
		m_pUV[3].x = lUVData[2];	m_pUV[3].y = lUVData[1];
	}
	else if( !strcmp( apAttrName, "angle" ) )
	{
		apParser->ReadAngleDegrees( &m_Angle );
	}
	else if( !strcmp( apAttrName, "mirrorx" ) )
	{
		m_Mirror |= GB_MIRROR_X;
	}
	else if( !strcmp( apAttrName, "mirrory" ) )
	{
		m_Mirror |= GB_MIRROR_Y;
	}
	else
		return GUIElement::ParseAttribute( apParser, apAttrName );
#endif
	return true;
}

//-----------------------------------------------------------------------------
// Clone this element
//-----------------------------------------------------------------------------
void GUIBitmap::Clone( GUIElement* apCloneSource )
{
	IW_CALLSTACK( "GUIBitmap::Clone" );

	GUIElement::Clone( apCloneSource );

	GUIBitmap* lpCloneBitmap = reinterpret_cast<GUIBitmap*>( apCloneSource );
	m_Material.Copy( lpCloneBitmap->m_Material );
	m_Colour.Copy( lpCloneBitmap->m_Colour );
	m_Angle = lpCloneBitmap->m_Angle;
	m_VertexCount = lpCloneBitmap->m_VertexCount;
	m_Mirror = lpCloneBitmap->m_Mirror;
	if( lpCloneBitmap->m_pUV )
	{
		m_pUV = new CIwFVec2[m_VertexCount];
		memcpy( m_pUV, lpCloneBitmap->m_pUV, sizeof( CIwFVec2 ) * m_VertexCount );
	}
}

//-----------------------------------------------------------------------------
// Called once per frame to render GUI element
//-----------------------------------------------------------------------------
void GUIBitmap::Render()
{
	IW_CALLSTACK( "GUIBitmap::Render" );

	GUIElement::Render();

	// Find screen position and sine/cosine of rotation angle
	CIwRect lrRect = GetScreenRect();
	const int32 cx = ( lrRect.x + ( lrRect.w >> 1 ) ) << 3;
	const int32 cy = ( lrRect.y + ( lrRect.h >> 1 ) ) << 3;
	const iwfixed lSine = IwGeomSin( m_Angle );
	const iwfixed lCosine = IwGeomCos( m_Angle );

	// Rotate and translate points
	CIwSVec2* lPts = IW_GX_ALLOC( CIwSVec2, m_VertexCount );
	for( int16 i = 0; i < m_VertexCount; i++ )
	{
		int32 lTmpWidth = IW_FIXED_MUL( m_pVertRatios[i].x, lrRect.w );
		int32 lTmpHeight = IW_FIXED_MUL( m_pVertRatios[i].y, lrRect.h );

		lPts[i].x = cx + IW_FIXED_MUL( lTmpWidth, lCosine ) - IW_FIXED_MUL( lTmpHeight, lSine );
		lPts[i].y = cy + IW_FIXED_MUL( lTmpWidth, lSine ) + IW_FIXED_MUL( lTmpHeight, lCosine );
	}

	IwGxSetMaterial( &m_Material );
	IwGxSetVertStreamScreenSpaceSubPixel( lPts, m_VertexCount );
	IwGxSetColStream( NULL );
	IwGxSetUVStream( m_pUV );
	IwGxDrawPrims( IW_GX_TRI_FAN, NULL, m_VertexCount );
}

//-----------------------------------------------------------------------------
// Replace the materials texture with the one represented by the
// supplied hash value
//-----------------------------------------------------------------------------
void GUIBitmap::SetTexture( uint32 aTextureHash )
{
	IW_CALLSTACK( "GUIBitmap::SetTexture" );
	CIwTexture* lpTexture = ResourceFindByHash<CIwTexture>( aTextureHash, "CIwTexture" );
	m_Material.SetTexture( lpTexture );
}

//-----------------------------------------------------------------------------
// Create our vertex and UV arrays for rendering the bitmap
//-----------------------------------------------------------------------------
// We're using subpixel screen space coords
#define GB_ONE	(IW_GEOM_ONE << 2)

CIwSVec2 GUIBitmap::s_VertRatios_MirrorNone[4] =
{
	CIwSVec2( -GB_ONE, -GB_ONE ), CIwSVec2( -GB_ONE, GB_ONE ),
	CIwSVec2( GB_ONE, GB_ONE ), CIwSVec2( GB_ONE, -GB_ONE )
};

CIwSVec2 GUIBitmap::s_VertRatios_MirrorX[6] =
{
	CIwSVec2( 0, -GB_ONE ), CIwSVec2( -GB_ONE, -GB_ONE ), CIwSVec2( -GB_ONE, GB_ONE ),
	CIwSVec2( 0, GB_ONE ), CIwSVec2( GB_ONE, GB_ONE ), CIwSVec2( GB_ONE, -GB_ONE )
};

CIwSVec2 GUIBitmap::s_VertRatios_MirrorY[6] =
{
	CIwSVec2( GB_ONE, 0 ), CIwSVec2( GB_ONE, -GB_ONE ), CIwSVec2( -GB_ONE, -GB_ONE ),
	CIwSVec2( -GB_ONE, 0 ), CIwSVec2( -GB_ONE, GB_ONE ), CIwSVec2( GB_ONE, GB_ONE )
};

CIwSVec2 GUIBitmap::s_VertRatios_MirrorXY[10] =
{
	CIwSVec2( 0, 0 ), CIwSVec2( 0, -GB_ONE ), CIwSVec2( -GB_ONE, -GB_ONE ),
	CIwSVec2( -GB_ONE, 0 ), CIwSVec2( -GB_ONE, GB_ONE ), CIwSVec2( 0, GB_ONE ),
	CIwSVec2( GB_ONE, GB_ONE ), CIwSVec2( GB_ONE, 0 ), CIwSVec2( GB_ONE, -GB_ONE ),
	CIwSVec2( 0, -GB_ONE )
};

void GUIBitmap::CreateVertexAndUVData()
{
	IW_CALLSTACK( "GUIBitmap::CreateVertexAndUVData" );

	// If no UVs specified, allocate a block of them and set up with
	// full texture defaults
	CIwFVec2* lpNewUV = NULL;
	if( !m_pUV )
	{
		m_pUV = new CIwFVec2[4];
		int32 lWidth = m_Material.GetTexture()->GetWidth();
		int32 lHeight = m_Material.GetTexture()->GetHeight();
		m_pUV[0].x = MakeUVValue( 0, lWidth );		m_pUV[0].y = MakeUVValue( 0, lHeight );
		m_pUV[1].x = MakeUVValue( 0, lWidth );		m_pUV[1].y = MakeUVValue( lHeight, lHeight );
		m_pUV[2].x = MakeUVValue( lWidth, lWidth );	m_pUV[2].y = MakeUVValue( lHeight, lHeight );
		m_pUV[3].x = MakeUVValue( lWidth, lWidth );	m_pUV[3].y = MakeUVValue( 0, lHeight );
	}

	switch( m_Mirror )
	{
		case GB_MIRROR_X:
		{
			m_VertexCount = 6;
			m_pVertRatios = s_VertRatios_MirrorX;

			lpNewUV = new CIwFVec2[6];
			lpNewUV[0].x = m_pUV[3].x;		lpNewUV[0].y = m_pUV[3].y;
			lpNewUV[1].x = m_pUV[0].x;		lpNewUV[1].y = m_pUV[0].y;
			lpNewUV[2].x = m_pUV[1].x;		lpNewUV[2].y = m_pUV[1].y;
			lpNewUV[3].x = m_pUV[2].x;		lpNewUV[3].y = m_pUV[2].y;
			lpNewUV[4].x = m_pUV[1].x;		lpNewUV[4].y = m_pUV[1].y;
			lpNewUV[5].x = m_pUV[0].x;		lpNewUV[5].y = m_pUV[0].y;
			break;
		}

		case GB_MIRROR_Y:
		{
			m_VertexCount = 6;
			m_pVertRatios = s_VertRatios_MirrorY;

			lpNewUV = new CIwFVec2[6];
			lpNewUV[0].x = m_pUV[2].x;		lpNewUV[0].y = m_pUV[2].y;
			lpNewUV[1].x = m_pUV[3].x;		lpNewUV[1].y = m_pUV[3].y;
			lpNewUV[2].x = m_pUV[0].x;		lpNewUV[2].y = m_pUV[0].y;
			lpNewUV[3].x = m_pUV[1].x;		lpNewUV[3].y = m_pUV[1].y;
			lpNewUV[4].x = m_pUV[0].x;		lpNewUV[4].y = m_pUV[0].y;
			lpNewUV[5].x = m_pUV[3].x;		lpNewUV[5].y = m_pUV[3].y;
			break;
		}

		case GB_MIRROR_X + GB_MIRROR_Y:
		{
			m_VertexCount = 10;
			m_pVertRatios = s_VertRatios_MirrorXY;

			lpNewUV = new CIwFVec2[10];
			lpNewUV[0].x = m_pUV[2].x;		lpNewUV[0].y = m_pUV[2].y;
			lpNewUV[1].x = m_pUV[3].x;		lpNewUV[1].y = m_pUV[3].y;
			lpNewUV[2].x = m_pUV[0].x;		lpNewUV[2].y = m_pUV[0].y;
			lpNewUV[3].x = m_pUV[1].x;		lpNewUV[3].y = m_pUV[1].y;
			lpNewUV[4].x = m_pUV[0].x;		lpNewUV[4].y = m_pUV[0].y;
			lpNewUV[5].x = m_pUV[3].x;		lpNewUV[5].y = m_pUV[3].y;
			lpNewUV[6].x = m_pUV[0].x;		lpNewUV[6].y = m_pUV[0].y;
			lpNewUV[7].x = m_pUV[1].x;		lpNewUV[7].y = m_pUV[1].y;
			lpNewUV[8].x = m_pUV[0].x;		lpNewUV[8].y = m_pUV[0].y;
			lpNewUV[9].x = m_pUV[3].x;		lpNewUV[9].y = m_pUV[3].y;
			break;
		}

		default:
		{
			// No mirroring
			m_VertexCount = 4;
			m_pVertRatios = s_VertRatios_MirrorNone;
			break;
		}
	}

	// If we are mirroring, set material to clamp
	if( m_Mirror )
		m_Material.SetClamping( true );

	// Replace m_pUV if necessary
	if( lpNewUV )
	{
		delete[] m_pUV;
		m_pUV = lpNewUV;
	}
}