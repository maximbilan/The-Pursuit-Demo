#include "FontManager.h"
#include "Font.h"

#include "IwResManager.h"

#include "../../Common/Defines.h"

//-------------------------------------------------------------------------------------------

void FontManager::Init()
{
	IwGxFontInit();

	CIwResGroup* lpFontResGroup = NULL;
	
	const uint32 width = IwGxGetScreenWidth();
	const uint32 height = IwGxGetScreenHeight();

	// For resolutions 480x800, 480x854 and lesser
	if( width <= 854 && height <= 480 )
	{
		lpFontResGroup = IwGetResManager()->LoadGroup( FONTS_GROUP_PATH_LOW );
	}
	// For resolutions 1536x1152, 1920x1152, 1920x1200, 2048x1536, 2560x1536, 2560x1600
	else if( width >= 1536 && height >= 1152 )
	{
		lpFontResGroup = IwGetResManager()->LoadGroup( FONTS_GROUP_PATH_HIGH );
	}
	else
	{
		lpFontResGroup = IwGetResManager()->LoadGroup( FONTS_GROUP_PATH );
	}
	
	CIwGxFont* lpSmallFont = NULL;
	CIwGxFont* lpMediumFont = NULL;
	CIwGxFont* lpLargeFont = NULL;

	LoadFont( lpSmallFont, "tahoma_small" );
	LoadFont( lpMediumFont, "tahoma_medium" );
	LoadFont( lpLargeFont, "tahoma_large" );

	IwAssertMsg( APP, lpSmallFont, ( "Unable to find font" ) );
	IwAssertMsg( APP, lpMediumFont, ( "Unable to find font" ) );
	IwAssertMsg( APP, lpLargeFont, ( "Unable to find font" ) );
	
	m_fontsArray.push_back( new Font( Font::FN_TAHOMA, Font::FS_SMALL, lpSmallFont ) );
	m_fontsArray.push_back( new Font( Font::FN_TAHOMA, Font::FS_MEDIUM, lpMediumFont ) );
	m_fontsArray.push_back( new Font( Font::FN_TAHOMA, Font::FS_LARGE, lpLargeFont ) );
}

//-------------------------------------------------------------------------------------------

void FontManager::Remove()
{
	for( FontArray::const_iterator it = m_fontsArray.begin(), end = m_fontsArray.end(); it != end; ++it )
	{
		delete ( *it );
	}

	IwGxFontTerminate();
}

//-------------------------------------------------------------------------------------------

Font* FontManager::GetFont( const unsigned int index, const unsigned int size )
{
	Font* font = NULL;

	for( FontArray::const_iterator it = m_fontsArray.begin(), end = m_fontsArray.end(); it != end; ++it )
	{
		if( ( *it )->GetIndex() == index && ( *it )->GetSize() == size )
		{
			font = *it;
			break;
		}
	}

	return font;
}

//-------------------------------------------------------------------------------------------

void FontManager::LoadFont( CIwGxFont*& font, const char* resName )
{
	font = static_cast<CIwGxFont*>( IwGetResManager()->GetResNamed( resName, "CIwGxFont" ) );
}

//-------------------------------------------------------------------------------------------

void FontManager::DrawText( const char* text, const int x, const int y, const int width, const int height, const unsigned int fontIndex, const unsigned int fontSize, const CIwColour fontColor,
							const IwGxFontAlignHor horizAlign, const IwGxFontAlignVer vertAlign )
{
	Font* font = GetFont( fontIndex, fontSize );

	if( font )
	{
		IwGxLightingOn();
		IwGxFontSetCol( fontColor );
		IwGxFontSetRect( CIwRect( x, y, width, height ) );
		IwGxFontSetAlignmentHor( horizAlign );
		IwGxFontSetAlignmentVer( vertAlign );
		IwGxFontSetFont( font->GetFontPointer() );
		IwGxFontDrawText( text );
		IwGxLightingOff();
	}
}

//-------------------------------------------------------------------------------------------

void FontManager::PrepareText( CIwGxFontPreparedData& preparedText, const char* text, const int x, const int y, const int width, const int height, 
							   const IwGxFontAlignHor horizAlign, const IwGxFontAlignVer vertAlign )
{
	IwGxFontSetRect( CIwRect( x, y, width, height ) );
	IwGxFontSetAlignmentHor( horizAlign );
	IwGxFontSetAlignmentVer( vertAlign );
	IwGxFontPrepareText( preparedText, text );
}

//-------------------------------------------------------------------------------------------

void FontManager::DrawPreparedText( CIwGxFontPreparedData& preparedText, const int x, const int y, const int width, const int height, const unsigned int fontIndex, const unsigned int fontSize, 
									const CIwColour fontColor, const IwGxFontAlignHor horizAlign, const IwGxFontAlignVer vertAlign )
{
	Font* font = GetFont( fontIndex, fontSize );

	if( font )
	{
		IwGxLightingOn();
		IwGxFontSetCol( fontColor );
		IwGxFontSetRect( CIwRect( x, y, width, height ) );
		IwGxFontSetAlignmentHor( horizAlign );
		IwGxFontSetAlignmentVer( vertAlign );
		IwGxFontSetFont( font->GetFontPointer() );
		IwGxFontDrawText( preparedText );
		IwGxLightingOff();
	}
}