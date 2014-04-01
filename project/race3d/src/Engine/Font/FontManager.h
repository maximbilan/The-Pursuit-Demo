#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include "s3eTypes.h"

#include "IwGxFont.h"
#include "IwArray.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "IwColour.h"

#include "../../Common/Singleton.h"

class Font;

//-------------------------------------------------------------------------------------------

typedef CIwArray<Font*> FontArray;

//-------------------------------------------------------------------------------------------
// FontManager - font management
//-------------------------------------------------------------------------------------------

class FontManager : public Singleton<FontManager>
{
public:
	// Initialization
	void Init();

	// Remove the instance
	void Remove();

	// Prepare text
	void PrepareText( CIwGxFontPreparedData& preparedText, const char* text, const int x, const int y, const int width, const int height, 
					const IwGxFontAlignHor horizAlign = IW_GX_FONT_ALIGN_LEFT, const IwGxFontAlignVer vertAlign = IW_GX_FONT_ALIGN_TOP );

	// Render prepared text
	void DrawPreparedText( CIwGxFontPreparedData& preparedText, const int x, const int y, const int width, const int height, const unsigned int fontIndex, const unsigned int fontSize, 
						  const CIwColour fontColor = IwGxGetColFixed( IW_GX_COLOUR_WHITE ), const IwGxFontAlignHor horizAlign = IW_GX_FONT_ALIGN_LEFT,
						   const IwGxFontAlignVer vertAlign = IW_GX_FONT_ALIGN_TOP );

	// Render text
	void DrawText( const char* text, const int x, const int y, const int width, const int height, const unsigned int fontIndex, const unsigned int fontSize, 
				   const CIwColour fontColor = IwGxGetColFixed( IW_GX_COLOUR_WHITE ), const IwGxFontAlignHor horizAlign = IW_GX_FONT_ALIGN_LEFT,
				   const IwGxFontAlignVer vertAlign = IW_GX_FONT_ALIGN_TOP );

private:
	// Load font
	void LoadFont( CIwGxFont*& font, const char* resName );

	// Get font
	Font* GetFont( const unsigned int index, const unsigned int size );

protected:
	FontManager() {}
	friend class Singleton<FontManager>;

private:
	FontArray	m_fontsArray;
};

#endif	// _FONTMANAGER_H_