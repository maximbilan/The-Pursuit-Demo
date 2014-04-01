#ifndef _CLOCATOR_H_
#define _CLOCATOR_H_

#include "IwGeomFVec3.h"
#include "IwGeomFQuat.h"
#include "IwString.h"
#include "IwResManager.h"

//----------------------------------------------------------------------------------------------
// CLocator - interface for parsing CLocator objects from .world files
// CLocator - this is a point marker, which holds a location that can be accessed and used from 
// within the application. For example, the CLocator point can be used for the entry point of 
// a character into a game, or the end of a level.
//----------------------------------------------------------------------------------------------
class CLocator : public CIwResource
{
public:
	IW_MANAGED_DECLARE( CLocator );

	CLocator();
	~CLocator();

	// This method parses attributes as they are read from the ITX file
	virtual bool ParseAttribute( CIwTextParserITX* apParser, const char* apAttribute );

	// This method is called when the text parser finds the end of the class definition
	virtual void ParseClose( CIwTextParserITX* apParser );

	// This function serializes the contents of our class to or from a file
	virtual void Serialise();

	// This function resolves any data that could not be serialized directly
	virtual void Resolve();

	// Get position
	const CIwFVec3& GetPosition() const;

	// Get rotation
	const CIwFQuat& GetRotation() const;

	// Get object name
	const CIwStringL& GetName() const;

	// Get actor name
	const CIwStringL& GetActorName() const;

	// Get type name
	const CIwStringL& GetTypeName() const;

private:
	CIwFVec3	m_position;
	CIwFQuat	m_rotation;

	CIwStringL	m_name;
	CIwStringL	m_actorName;
	CIwStringL	m_typeName;
};

//----------------------------------------------------------------------------------------------

inline const CIwFVec3& CLocator::GetPosition() const
{
	return m_position;
}

//----------------------------------------------------------------------------------------------

inline const CIwFQuat& CLocator::GetRotation() const
{
	return m_rotation;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& CLocator::GetName() const
{
	return m_name;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& CLocator::GetActorName() const
{
	return m_actorName;
}

//----------------------------------------------------------------------------------------------

inline const CIwStringL& CLocator::GetTypeName() const
{
	return m_typeName;
}

#endif	// _CLOCATOR_H_