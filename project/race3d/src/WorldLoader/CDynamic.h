#ifndef _CDYNAMIC_H_
#define _CDYNAMIC_H_

#include "IObject.h"
#include "IwResManager.h"

//------------------------------------------------------------------------------------------------
// CDynamic - interface for parsing CDynamic objects from .world files
// CDynamic - this is used for objects that are not static within the scene and allows the object
// to be accessed via the application code. An example of this is a car which is animated but 
// not a Character, therefore it would be exported with the scene and within the WORLD file.
//------------------------------------------------------------------------------------------------
class CDynamic : public CIwResource, public IObject
{
public:
	IW_MANAGED_DECLARE( CDynamic );

	CDynamic();
	~CDynamic();

	// This method parses attributes as they are read from the ITX file
	virtual bool ParseAttribute( CIwTextParserITX* apParser, const char* apAttribute );

	// This method is called when the text parser finds the end of the class definition
	virtual void ParseClose( CIwTextParserITX* apParser );

	// This function serializes the contents of our class to or from a file
	virtual void Serialise();

	// This function resolves any data that could not be serialized directly
	virtual void Resolve();
};

#endif	// _CDYNAMIC_H_