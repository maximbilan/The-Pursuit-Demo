#ifndef _CSTATIC_H_
#define _CSTATIC_H_

#include "IObject.h"
#include "IwResManager.h"

//------------------------------------------------------------------------------------------------
// CStatic - interface for parsing CStatic objects from .world files
// CStatic - this is the most common and is used for most mesh objects such as walls, floors, 
// trees, and any static model that is to appear in the application as it is seen in the
// 3D Modelling Package.
//------------------------------------------------------------------------------------------------
class CStatic : public CIwResource, public IObject
{
public:
	IW_MANAGED_DECLARE( CStatic );

	CStatic();
	~CStatic();

	// This method parses attributes as they are read from the ITX file
	virtual bool ParseAttribute( CIwTextParserITX* apParser, const char* apAttribute );

	// This method is called when the text parser finds the end of the class definition
	virtual void ParseClose( CIwTextParserITX* apParser );

	// This function serializes the contents of our class to or from a file
	virtual void Serialise();

	// This function resolves any data that could not be serialized directly
	virtual void Resolve();

	// Returns OnEnter script
	const CIwStringL& GetOnEnterScript() const;

	// Returns OnLeave script
	const CIwStringL& GetOnLeaveScript() const;

private:
	CIwStringL	m_onEnterScript;
	CIwStringL	m_onLeaveScript;
};

//------------------------------------------------------------------------------------------------

inline const CIwStringL& CStatic::GetOnEnterScript() const
{
	return m_onEnterScript;
}

//------------------------------------------------------------------------------------------------

inline const CIwStringL& CStatic::GetOnLeaveScript() const
{
	return m_onLeaveScript;
}

#endif	// _CSTATIC_H_