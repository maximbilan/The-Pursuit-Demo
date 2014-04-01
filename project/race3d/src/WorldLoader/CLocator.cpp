#include "CLocator.h"
#include "WorldLoader.h"

#include "IwSerialise.h"
#include "IwTextParserITX.h"

//----------------------------------------------------------------------------------------------

IW_MANAGED_IMPLEMENT_FACTORY( CLocator );

//----------------------------------------------------------------------------------------------

CLocator::CLocator()
{
	IW_CALLSTACK( "CLocator::CLocator" );
}

//----------------------------------------------------------------------------------------------

CLocator::~CLocator()
{
	IW_CALLSTACK( "CLocator::~CLocator" );
}

//----------------------------------------------------------------------------------------------

bool CLocator::ParseAttribute( CIwTextParserITX* apParser, const char* apAttribute )
{
	IW_CALLSTACK( "CLocator::ParseAttribute" );

	if( !stricmp( apAttribute, "name" ) )
	{
		apParser->ReadString( m_name );
	}
	else if( !stricmp( apAttribute, "pos" ) )
	{
		float f[3];
		apParser->ReadFloatArray( f, 3 );

		m_position.x = f[0];
		m_position.y = f[1];
		m_position.z = f[2];
	}
	else if( !stricmp( apAttribute, "rot" ) )
	{
		float f[4];
		apParser->ReadFloatArray( f, 4 );
		m_rotation.s = static_cast<float>( f[0] );
		m_rotation.x = static_cast<float>( f[1] );
		m_rotation.y = static_cast<float>( f[2] );
		m_rotation.z = static_cast<float>( f[3] );
	}
	else if( !stricmp( apAttribute, "actorName" ) )
	{
		apParser->ReadString( m_actorName );
	}
	else if( !stricmp( apAttribute, "typeName" ) )
	{
		apParser->ReadString( m_typeName );
	}
	else
	{
		return CIwManaged::ParseAttribute( apParser, apAttribute );
	}

	return true;
}

//----------------------------------------------------------------------------------------------

void CLocator::ParseClose( CIwTextParserITX* apParser )
{
	IW_CALLSTACK( "CLocator::ParseClose" );

	WorldLoader::AddObject( this );
}

//----------------------------------------------------------------------------------------------

void CLocator::Serialise()
{
	IW_CALLSTACK( "CLocator::Serialise" );

	CIwManaged::Serialise();

	m_name.Serialise();
	m_actorName.Serialise();
	m_typeName.Serialise();
	m_position.Serialise();
	m_rotation.Serialise();
}

//----------------------------------------------------------------------------------------------

void CLocator::Resolve()
{
	IW_CALLSTACK( "CLocator::Resolve" );
}