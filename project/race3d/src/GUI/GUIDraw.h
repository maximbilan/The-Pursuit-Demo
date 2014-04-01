//-----------------------------------------------------------------------------
// GUIDraw.h
// Simple drawing functions (e.g. rectangles)
//-----------------------------------------------------------------------------

#ifndef GUIDRAW_H
#define GUIDRAW_H

// Forward declarations
class CIwMaterial;

// Some handy colour definitions
#define COL_WHITE			0xFFFFFFFF
#define COL_BLACK			0xFF000000
#define COL_RED				0xFF0000FF
#define COL_GREEN			0xFF00FF00
#define COL_BLUE			0xFFFF0000
#define COL_YELLOW			0xFF00FFFF
#define COL_MAGENTA			0xFFFF00FF
#define COL_CYAN			0xFFFFFF00
#define COL_GREY			0xFF808080
#define COL_ORANGE			0xFF0080FF
#define COL_DARK_RED		0xFF000080
#define COL_DARK_GREEN		0xFF008000
#define COL_DARK_BLUE		0xFF800000
#define COL_DARK_YELLOW		0xFF008080
#define COL_DARK_MAGENTA	0xFF800080
#define COL_DARK_CYAN		0xFF808000
#define COL_DARK_GREY		0xFF404040
#define COL_DARK_ORANGE		0xFF004080

//-----------------------------------------------------------------------------
// Drawing functions
//-----------------------------------------------------------------------------
void DrawRectangle( int32 aX, int32 aY, int32 aWidth, int32 aHeight, uint32 aColour );
void DrawRectangleOutline( int32 aX, int32 aY, int32 aWidth, int32 aHeight, uint32 aColour );
void DrawRectangleTextured( int32 aX, int32 aY, int32 aWidth, int32 aHeight, CIwMaterial* apMaterial, CIwFVec2* apUV = NULL );
void DrawRectangleTexturedRotated( int32 aCentreX, int32 aCentreY, int32 aWidth, int32 aHeight, iwangle aAngle, CIwMaterial* apMaterial, CIwFVec2* apUV = NULL );
void DrawCircle( int32 aCentreX, int32 aCentreY, int32 aRadius, uint32 aColour );

//-----------------------------------------------------------------------------
// UV helpers
//-----------------------------------------------------------------------------
float MakeUVValue( int16 aPixel, int16 aFullSize );


#endif