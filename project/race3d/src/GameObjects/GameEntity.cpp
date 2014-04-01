#include "GameEntity.h"
#include "IwGraphics.h"

//-------------------------------------------------------------------------------------

CIwArray<std::string> GameEntity::s_loadedGroups = CIwArray<std::string>();

//-------------------------------------------------------------------------------------

GameEntity::GameEntity() :
	m_position( CIwFVec3( 0.0f, 0.0f, 0.0f ) ),
	m_rotation( CIwFVec3( 0.0f, 0.0f, 0.0f ) ),
	m_isInited( false ),
	m_scale( CIwFVec3( 0.0f, 0.0f, 0.0f ) )
{
	m_isDynamic = true;
}

//-------------------------------------------------------------------------------------

GameEntity::~GameEntity()
{
}

//-------------------------------------------------------------------------------------

void GameEntity::BuildResources()
{
#if defined IW_DEBUG && (defined IW_BUILD_RESOURCES)
	if( !IwGetResManager()->BuildResources() ) 
	{
		// Register the model builder callbacks
		IwGetModelBuilder()->SetPostBuildFn( &Physics::BuildCollision );
	}
#endif
}

//-------------------------------------------------------------------------------------

void GameEntity::LoadGroup( const std::string& groupName )
{
	const int result = s_loadedGroups.find( groupName );

	if( result == -1 )
	{
		s_loadedGroups.push_back( groupName );

		std::string g;
		g.clear();
		g = groupName + ".group";
		IwGetResManager()->LoadGroup( g.c_str() );
	}
}

//-------------------------------------------------------------------------------------

void GameEntity::LoadData( const char* groupName )
{
	BuildResources();

	LoadGroup( groupName );
}

//-------------------------------------------------------------------------------------

void GameEntity::DestroyData()
{
	for( CIwArray<std::string>::const_iterator it = s_loadedGroups.begin(), end = s_loadedGroups.end(); it != end; ++it )
	{
		IwGetResManager()->DestroyGroup( ( *it ).c_str() );
	}

	s_loadedGroups.clear_optimised();
}