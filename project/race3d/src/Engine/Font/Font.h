#ifndef _FONT_H_
#define _FONT_H_

#include "IwGxFont.h"

//---------------------------------------------------------------------------------------------
// Font
//---------------------------------------------------------------------------------------------
class Font
{
public:
	enum FONT_INDEX
	{
		FN_TAHOMA
	};

	enum FONT_SIZE
	{
		FS_SMALL,
		FS_MEDIUM,
		FS_LARGE
	};

public:
	Font( const unsigned int index, const unsigned int size, CIwGxFont* font );
	~Font();

	// Get index
	unsigned int GetIndex() const;

	// Get size
	unsigned int GetSize() const;

	// Get font pointer
	CIwGxFont* GetFontPointer() const;

private:
	unsigned int	m_index;
	unsigned int	m_size;
	CIwGxFont*		m_pFont;
};

//-------------------------------------------------------------------------------------------

inline unsigned int Font::GetIndex() const
{
	return m_index;
}

//-------------------------------------------------------------------------------------------

inline unsigned int Font::GetSize() const
{
	return m_size;
}

//-------------------------------------------------------------------------------------------

inline CIwGxFont* Font::GetFontPointer() const
{
	return m_pFont;
}

#endif	// _FONT_H_