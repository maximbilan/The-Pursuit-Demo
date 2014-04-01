#include "WorldLoader.h"

#include "IwUtil.h"
#include "IwResManager.h"

//----------------------------------------------------------------------------------------------

CIwManagedList* WorldLoader::s_objectList = new CIwManagedList;

//----------------------------------------------------------------------------------------------

WorldLoader::WorldLoader( const char* worldName )
{
	m_worldName = worldName;
}

//----------------------------------------------------------------------------------------------

WorldLoader::~WorldLoader()
{
}

//----------------------------------------------------------------------------------------------

void WorldLoader::AddObject( CIwManaged* pObject, bool allowDups )
{
	s_objectList->Add( pObject );
}

//----------------------------------------------------------------------------------------------

void WorldLoader::Delete()
{
	s_objectList->Delete();
}

//----------------------------------------------------------------------------------------------

void WorldLoader::ClearAndFree()
{
	s_objectList->ClearAndFree();
}

//----------------------------------------------------------------------------------------------

void WorldLoader::LoadWorld()
{
	const std::string rawWorldName = m_worldName + ".world";
	const std::string binWorldName = m_worldName + ".bin";

#ifdef IW_BUILD_RESOURCES
	IwGetTextParserITX()->ParseFile( rawWorldName.c_str() );
	IwSerialiseOpen( binWorldName.c_str(), false );
	s_objectList->SerialiseHeader();
	s_objectList->Serialise();
	IwSerialiseClose();

	s_objectList->Delete();
#endif

	IwSerialiseOpen( binWorldName.c_str(), true );
	s_objectList->SerialiseHeader();
	s_objectList->Serialise();
	s_objectList->Resolve();
	IwSerialiseClose();
}

//----------------------------------------------------------------------------------------------

CIwManagedList* WorldLoader::GetObjects()
{
	return s_objectList;
}