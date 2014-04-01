#include "CStatic.h"
#include "WorldLoader.h"

#include "IwSerialise.h"
#include "IwTextParserITX.h"

//----------------------------------------------------------------------------------------------

IW_MANAGED_IMPLEMENT_FACTORY( CStatic );

//----------------------------------------------------------------------------------------------

CStatic::CStatic()
{
	IW_CALLSTACK( "CStatic::CStatic" );
}

//----------------------------------------------------------------------------------------------

CStatic::~CStatic()
{
	IW_CALLSTACK( "CStatic::~CStatic" );
}

//----------------------------------------------------------------------------------------------

bool CStatic::ParseAttribute( CIwTextParserITX* apParser, const char* apAttribute )
{
	IW_CALLSTACK( "CStatic::ParseAttribute" );

	if( !stricmp( apAttribute, "name" ) )
	{
		apParser->ReadString( m_name );
	}
	else if( !stricmp( apAttribute, "model" ) )
	{
		apParser->ReadString( m_modelName );
	}
	else if( !stricmp( apAttribute, "groupName" ) )
	{
		apParser->ReadString( m_groupName );
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
	else if( !stricmp( apAttribute, "entityType" ) )
	{
		apParser->ReadString( m_entityType );
	}
	else if( !stricmp( apAttribute, "collisionType" ) )
	{
		apParser->ReadString( m_collisionType );
	}
	else if( !stricmp( apAttribute, "mass" ) )
	{
		apParser->ReadFloat( &m_mass );
	}
	else if( !stricmp( apAttribute, "scale" ) )
	{
		apParser->ReadFloat( &m_scale );
	}
	else if( !stricmp( apAttribute, "onEnterScript" ) )
	{
		apParser->ReadString( m_onEnterScript );
	}
	else if( !stricmp( apAttribute, "onLeaveScript" ) )
	{
		apParser->ReadString( m_onLeaveScript );
	}
	else
	{
		return CIwManaged::ParseAttribute( apParser, apAttribute );
	}

	return true;
}

//----------------------------------------------------------------------------------------------

void CStatic::ParseClose( CIwTextParserITX* apParser )
{
	IW_CALLSTACK( "CStatic::ParseClose" );

	WorldLoader::AddObject( this );
}

//----------------------------------------------------------------------------------------------

void CStatic::Serialise()
{
	IW_CALLSTACK( "CStatic::Serialise" );

	CIwManaged::Serialise();

	m_name.Serialise();
	m_modelName.Serialise();
	m_groupName.Serialise();
	m_entityType.Serialise();
	m_collisionType.Serialise();
	m_position.Serialise();
	m_rotation.Serialise();
	IwSerialiseBool( m_isVisible );
	IwSerialiseBool( m_isEnabled );
	IwSerialiseFloat( m_mass );
	IwSerialiseFloat( m_scale );
	m_onEnterScript.Serialise();
	m_onLeaveScript.Serialise();
}

//----------------------------------------------------------------------------------------------

void CStatic::Resolve()
{
	IW_CALLSTACK( "CStatic::Resolve" );
}