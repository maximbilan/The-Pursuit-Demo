#include "Font.h"

//-------------------------------------------------------------------------------------------

Font::Font( const unsigned int index, const unsigned int size, CIwGxFont* font ) :
	m_index( index ),
	m_size( size ),
	m_pFont( font )
{
}

//-------------------------------------------------------------------------------------------

Font::~Font()
{
}