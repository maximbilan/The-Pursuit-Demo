//-----------------------------------------------------------------------------
// Localization.h
// Localisation functions
//-----------------------------------------------------------------------------

#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "s3eTypes.h"

// Initialise Localisation Module
void LocaliseInit();

// Terminate Localisation Module
void LocaliseTerminate();

// Loads a string database into memory
void LocaliseLoadStrings( const char* apFileName );

// Loads a string database into memory by langCode
void LocaliseLoadStrings( const int langCode );

// Releases any currently loaded string table
void LocaliseReleaseStrings();

// Look up a string by it's ID name
const char* LocaliseFindString( const char* apID );

// Look up a string by it's hash value
const char* LocaliseFindString( uint32 aHash );

// Calculate the hash value for a string ID
uint32 LocaliseCalculateIDHash( const char* apID );

#endif	// _LOCALIZATION_H_
