//-----------------------------------------------------------------------------
// Localization.cpp
// Localisation functions
//-----------------------------------------------------------------------------

#include "Localization.h"

#include "ctype.h"

#include "s3eFile.h"

#include "IwDebug.h"
#include "IwRuntime.h"

#include "../Common/Defines.h"

//-----------------------------------------------------------------------------
// Structure used internally to represent a single string entry
//-----------------------------------------------------------------------------
typedef struct _LocalisedString
{
	uint32	mHash;
	char	mStr[1];
} LocalisedString;

typedef struct _LocalisedString* LocalisedStringPtr;

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

// Pointer to start of currently loaded string table
char* gBuffer = NULL;

// Count of strings in table
uint32 gStringCount = 0;

// Array of Localised Strings
LocalisedStringPtr* gStringTable = NULL;

//-----------------------------------------------------------------------------
// Initialise Localisation Module
//-----------------------------------------------------------------------------
void LocaliseInit()
{
	IW_CALLSTACK( "LocaliseInit" );

	gBuffer = NULL;
	gStringCount = 0;
	gStringTable = 0;
}

//-----------------------------------------------------------------------------
// Terminate Localisation Module
//-----------------------------------------------------------------------------
void LocaliseTerminate()
{
	IW_CALLSTACK( "LocaliseTerminate" );
	LocaliseReleaseStrings();
}

//-----------------------------------------------------------------------------
// Loads a string database into memory
//-----------------------------------------------------------------------------
void LocaliseLoadStrings( const char* apFileName )
{
	IW_CALLSTACK( "LocaliseLoadStrings" );

	// Clear any existing strings
	LocaliseReleaseStrings();

	// Open file
	s3eFile* f = s3eFileOpen( apFileName, "rb" );
	IwAssertMsg( LOCALISE, f, ("Error opening string database file '%s'", apFileName ) );

	// Find length of file so we can allocate a big enough buffer to
	// load all the text into
	s3eFileSeek( f, 0, S3E_FILESEEK_END );
	uint32 lLength = s3eFileTell( f );
	s3eFileSeek( f, 0, S3E_FILESEEK_SET );
	gBuffer = new char[lLength - sizeof( uint32 )];

	// First four bytes are the string count, then it's all the string data
	s3eFileRead( &gStringCount, 1, sizeof( uint32 ), f );
	s3eFileRead( gBuffer, 1, lLength, f );
	s3eFileClose( f );

	// Allocate a big enough array of LocalisedString instances
	gStringTable = new LocalisedStringPtr[gStringCount];

	// Chop the text file up and populate the string table
	char* p = gBuffer;
	for( uint32 i = 0; i < gStringCount; i++ )
	{
		// Set the pointer
		gStringTable[ i ] = ( LocalisedStringPtr ) p;

		// Skip over the hash value, which might contain nulls
		p += 4;

		// Now find the end of the string
		while( *p++ != 0 )
		{
		}

		// Skip through nulls to next four byte boundary
		while( ( *p == 0 ) && ( ( ( uint32) p & 3 ) != 0 ) )
		{
			p++;
		}
	}
}

//-----------------------------------------------------------------------------
// Loads a string database into memory by langCode
//-----------------------------------------------------------------------------
void LocaliseLoadStrings( const int langCode )
{
	switch( langCode )
	{
		case S3E_DEVICE_LANGUAGE_ENGLISH:
		{
			LocaliseLoadStrings( LANGPACK_EN_PATH );
		}
		break;

		case S3E_DEVICE_LANGUAGE_FRENCH:
		{
			LocaliseLoadStrings( LANGPACK_FR_PATH );
		}
		break;

		case S3E_DEVICE_LANGUAGE_RUSSIAN:
		case S3E_DEVICE_LANGUAGE_UKRAINIAN:
		{
			LocaliseLoadStrings( LANGPACK_RU_PATH );
		}
		break;

		case S3E_DEVICE_LANGUAGE_GERMAN:
		{
			LocaliseLoadStrings( LANGPACK_GE_PATH );
		}
		break;

		case S3E_DEVICE_LANGUAGE_ITALIAN:
		{
			LocaliseLoadStrings( LANGPACK_IT_PATH );
		}
		break;

		case S3E_DEVICE_LANGUAGE_SPANISH:
		{
			LocaliseLoadStrings( LANGPACK_SP_PATH );
		}
		break;

		default:
		{
			LocaliseLoadStrings( LANGPACK_EN_PATH );
		}
	}
}

//-----------------------------------------------------------------------------
// Releases any currently loaded string table
//-----------------------------------------------------------------------------
void LocaliseReleaseStrings()
{
	IW_CALLSTACK( "LocaliseReleaseStrings" );

	if( gStringTable )
	{
		delete[] gStringTable;
		delete[] gBuffer;

		gBuffer = NULL;
		gStringCount = 0;
		gStringTable = NULL;
	}
}

//-----------------------------------------------------------------------------
// Look up a string by it's ID name
//-----------------------------------------------------------------------------
const char* LocaliseFindString( const char* apID )
{
	IW_CALLSTACK( "LocaliseFindString" );

	return LocaliseFindString( LocaliseCalculateIDHash( apID ) );
}

//-----------------------------------------------------------------------------
// Look up a string by it's hash value
//-----------------------------------------------------------------------------
const char* LocaliseFindString( uint32 aHash )
{
	IW_CALLSTACK( "LocaliseFindString" );

	IwAssertMsg( "LOCALISE", gStringCount, ( "No localised string table available" ) );

	// Do a binary search on the hash values (which are in sorted numerical order)
	// to find the string we're after
	uint32 lFirst = 0;
	uint32 lLast = gStringCount - 1;

	while( lLast >= lFirst )
	{
		uint32 lTest = ( lFirst + lLast ) / 2;
		uint32 lHash = gStringTable[lTest]->mHash;
		if( lHash == aHash )
		{
			return gStringTable[lTest]->mStr;
		}
		else if( aHash < lHash )
		{
			lLast = lTest - 1;
		}
		else
		{
			lFirst = lTest + 1;
		}
	}

	IwAssertMsg( LOCALISE, false, ( "Unable to find string with hash value %d", aHash ) );
	return "Missing String!";
}

//-----------------------------------------------------------------------------
// Calculate the hash value for a string ID
//-----------------------------------------------------------------------------
uint32 LocaliseCalculateIDHash( const char* apID )
{
	IW_CALLSTACK( "LocaliseCalculateIDHash" );

	uint32 lHash = 0;

	while( *apID )
	{
		lHash = ( lHash * 29 ) + toupper( *apID++ );
	}
	return lHash;
}