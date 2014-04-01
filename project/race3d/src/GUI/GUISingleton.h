//-----------------------------------------------------------------------------
// GUISingleton.h
// Helper class for creating singleton instances
//-----------------------------------------------------------------------------

#ifndef GUISINGLETON_H
#define GUISINGLETON_H

// Library includes
#include "IwDebug.h"

//-----------------------------------------------------------------------------
// GUISINGLETON_GET macro
// Use in class declaration to create a static GetClass method for accessing the
// singleton instance
//-----------------------------------------------------------------------------
#define GUISINGLETON_GET( aClass )																\
	inline aClass* Get##aClass()																\
	{																							\
		IwAssertMsg( GUI, aClass::Get() != NULL, ( "%s singleton doesn't exist", #aClass ) );	\
		return aClass::Get();																	\
	}

//-----------------------------------------------------------------------------
// Singleton class template
//-----------------------------------------------------------------------------
template<class T> class GUISingleton
{
public:
	GUISingleton()
	{
		IwAssertMsg( GUI, sp_Singleton == NULL, ( "Singleton already exists" ) );
		sp_Singleton = static_cast<T*>( this );
	}

	virtual ~GUISingleton()
	{
		IwAssertMsg( GUI, sp_Singleton != NULL, ( "Singleton doesn't exist" ) );
		sp_Singleton = NULL;
	}

	static T* Get()
	{
		return sp_Singleton;
	}

private:
	static T*	sp_Singleton;
};

template<typename T> T* GUISingleton<T>::sp_Singleton = NULL;

#endif