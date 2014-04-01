//-----------------------------------------------------------------------------
// GUIDraw.cpp
// Simple drawing functions (e.g. rectangles)
//-----------------------------------------------------------------------------

// Library includes
#include "IwGx.h"
#include "IwGeomSVec2.h"
#include "IwGeomFVec2.h"

// Project includes
#include "GUIManager.h"

static uint16	g_Indices[5] = { 0, 1, 2, 3, 0 };
static CIwFVec2	g_FullTextureUV[4] =
{
	CIwFVec2( 0.0f, 1.0f ), CIwFVec2( 1.0f, 1.0f ),
	CIwFVec2( 0.0f, 0.0f ), CIwFVec2( 1.0f, 0.0f )
};

//-----------------------------------------------------------------------------
// Draw a rectangle at the given screen positions in the given colour
//-----------------------------------------------------------------------------
void DrawRectangle( int32 aX, int32 aY, int32 aWidth, int32 aHeight, uint32 aColour )
{
	CIwMaterial* lpMaterial = IW_GX_ALLOC_MATERIAL();
	lpMaterial->Init();
	lpMaterial->SetColEmissive( aColour );
	lpMaterial->SetColAmbient( aColour );
	IwGxSetMaterial( lpMaterial );

	CIwSVec2* lPts = IW_GX_ALLOC( CIwSVec2, 4 );
	lPts[0].x = aX << SUBPIXEL_SHIFT;				lPts[0].y = ( aY + aHeight ) << SUBPIXEL_SHIFT;
	lPts[1].x = ( aX + aWidth )  << SUBPIXEL_SHIFT;	lPts[1].y = ( aY + aHeight ) << SUBPIXEL_SHIFT;
	lPts[2].x = aX << SUBPIXEL_SHIFT;				lPts[2].y = aY << SUBPIXEL_SHIFT;
	lPts[3].x = ( aX + aWidth ) << SUBPIXEL_SHIFT;	lPts[3].y = aY << SUBPIXEL_SHIFT;
	IwGxSetVertStreamScreenSpaceSubPixel( lPts, 4 );
	IwGxSetColStream( NULL );
	IwGxSetUVStream( NULL );

	IwGxDrawPrims( IW_GX_TRI_STRIP, g_Indices, 4 );
}

//-----------------------------------------------------------------------------
// Draw a rectanglular outline at the given screen positions
//-----------------------------------------------------------------------------
void DrawRectangleOutline( int32 aX, int32 aY, int32 aWidth, int32 aHeight, uint32 aColour )
{
	CIwMaterial* lpMaterial = IW_GX_ALLOC_MATERIAL();
	lpMaterial->Init();
	lpMaterial->SetColEmissive( aColour );
	lpMaterial->SetColAmbient( aColour );
	IwGxSetMaterial( lpMaterial );

	CIwSVec2* lPts = IW_GX_ALLOC( CIwSVec2, 4 );
	lPts[0].x = aX << SUBPIXEL_SHIFT;				lPts[0].y = aY << SUBPIXEL_SHIFT;
	lPts[1].x = ( aX + aWidth ) << SUBPIXEL_SHIFT;	lPts[1].y = aY << SUBPIXEL_SHIFT;
	lPts[2].x = ( aX + aWidth ) << SUBPIXEL_SHIFT;	lPts[2].y = ( aY + aHeight ) << SUBPIXEL_SHIFT;
	lPts[3].x = aX << SUBPIXEL_SHIFT;				lPts[3].y = ( aY + aHeight ) << SUBPIXEL_SHIFT;
	IwGxSetVertStreamScreenSpaceSubPixel( lPts, 4 );
	IwGxSetColStream( NULL );
	IwGxSetUVStream( NULL );

	IwGxDrawPrims( IW_GX_LINE_STRIP, g_Indices, 5 );
}

//-----------------------------------------------------------------------------
// Draw a textured rectangle (using entire texture or uisng specified UVs)
// at the given screen positions
//-----------------------------------------------------------------------------
void DrawRectangleTextured( int32 aX, int32 aY, int32 aWidth, int32 aHeight, CIwMaterial* apMaterial, CIwFVec2* apUV )
{
	IwGxSetMaterial( apMaterial );

	CIwSVec2* lPts = IW_GX_ALLOC( CIwSVec2, 4 );
	lPts[0].x = aX << SUBPIXEL_SHIFT;				lPts[0].y = ( aY + aHeight ) << SUBPIXEL_SHIFT;
	lPts[1].x = ( aX + aWidth ) << SUBPIXEL_SHIFT;	lPts[1].y = ( aY + aHeight ) << SUBPIXEL_SHIFT;
	lPts[2].x = aX << SUBPIXEL_SHIFT;				lPts[2].y = aY << SUBPIXEL_SHIFT;
	lPts[3].x = ( aX + aWidth ) << SUBPIXEL_SHIFT;	lPts[3].y = aY << SUBPIXEL_SHIFT;
	IwGxSetVertStreamScreenSpaceSubPixel( lPts, 4 );
	IwGxSetColStream( NULL );
	if( apUV )
		IwGxSetUVStream( apUV );
	else
		IwGxSetUVStream( g_FullTextureUV );

	IwGxDrawPrims( IW_GX_TRI_STRIP, g_Indices, 4 );
}

//-----------------------------------------------------------------------------
// Draw a rotated textured rectangle (using entire texture or uisng specified
// UVs) at the given centre point with the given width and height
//-----------------------------------------------------------------------------
void DrawRectangleTexturedRotated( int32 aCentreX, int32 aCentreY, int32 aWidth, int32 aHeight, iwangle aAngle, CIwMaterial* apMaterial, CIwFVec2* apUV )
{
	IwGxSetMaterial( apMaterial );

	aWidth >>= 1;
	aHeight >>= 1;

	CIwMat lRotMatrix;
	lRotMatrix.SetRotZ( aAngle );

	CIwSVec2* lPts = IW_GX_ALLOC( CIwSVec2, 4 );

	iwfixed lSine = IwGeomSin( aAngle );
	iwfixed lCosine = IwGeomCos( aAngle );

	lPts[0].x = IW_FIXED_MUL( -aWidth, lCosine ) - IW_FIXED_MUL( aHeight, lSine );
	lPts[0].y = IW_FIXED_MUL( -aWidth, lSine ) + IW_FIXED_MUL( aHeight, lCosine );
	lPts[1].x = IW_FIXED_MUL( aWidth, lCosine ) - IW_FIXED_MUL( aHeight, lSine );
	lPts[1].y = IW_FIXED_MUL( aWidth, lSine ) + IW_FIXED_MUL( aHeight, lCosine );
	lPts[2].x = aCentreX - lPts[1].x;
	lPts[2].y = aCentreY - lPts[1].y;
	lPts[3].x = aCentreX - lPts[0].x;
	lPts[3].y = aCentreY - lPts[0].y;
	lPts[0].x += aCentreX;
	lPts[0].y += aCentreY;
	lPts[1].x += aCentreX;
	lPts[1].y += aCentreY;

	for( uint32 i = 0; i < 4; i++ )
	{
		lPts[i].x <<= SUBPIXEL_SHIFT;
		lPts[i].y <<= SUBPIXEL_SHIFT;
	}

	IwGxSetVertStreamScreenSpaceSubPixel( lPts, 4 );
	IwGxSetColStream( NULL );
	if( apUV )
		IwGxSetUVStream( apUV );
	else
		IwGxSetUVStream( g_FullTextureUV );

	IwGxDrawPrims( IW_GX_TRI_STRIP, g_Indices, 4 );
}

//-----------------------------------------------------------------------------
// Draw a 2D circle centred at given position with specified radius and colour
//-----------------------------------------------------------------------------
#define MAX_CIRCLE_PTS	20

static uint16	g_CircleIndices[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0 };

void DrawCircle( int32 aCentreX, int32 aCentreY, int32 aRadius, uint32 aColour )
{
	CIwMaterial* lpMaterial = IW_GX_ALLOC_MATERIAL();
	lpMaterial->Init();
	lpMaterial->SetColEmissive( aColour );
	lpMaterial->SetColAmbient( aColour );
	IwGxSetMaterial( lpMaterial );

	CIwSVec2* lPts = IW_GX_ALLOC( CIwSVec2, MAX_CIRCLE_PTS );
	for( uint32 i = 0; i < MAX_CIRCLE_PTS; i++ )
	{
		iwangle lAngle = ( 4096 * i ) / MAX_CIRCLE_PTS;
		lPts[i].x = aCentreX + IW_FIXED_MUL( IwGeomSin( lAngle ), aRadius );
		lPts[i].y = aCentreY + IW_FIXED_MUL( IwGeomCos( lAngle ), aRadius );

		lPts[i].x <<= SUBPIXEL_SHIFT;
		lPts[i].y <<= SUBPIXEL_SHIFT;
	}

	IwGxSetVertStreamScreenSpaceSubPixel( lPts, MAX_CIRCLE_PTS );
	IwGxSetColStream( NULL );
	IwGxSetUVStream( NULL );

	IwGxDrawPrims( IW_GX_LINE_STRIP, g_CircleIndices, MAX_CIRCLE_PTS + 1 );
}

//-----------------------------------------------------------------------------
// UV helpers
//-----------------------------------------------------------------------------
float MakeUVValue( int16 aPixel, int16 aFullSize )
{
	// Note this doesn't support texture wrapping!!
	float lHalfPixel = 1.0f / ( float ) ( aFullSize * 2 );
	float lMaxUV = 1.0f - lHalfPixel;
	float lUV = ( float ) aPixel / ( float ) aFullSize;
	lUV = MIN( lUV + lHalfPixel, lMaxUV );
	return lUV;
}
