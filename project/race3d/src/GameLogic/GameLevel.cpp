#include "GameLevel.h"
#include "Actor/Actor.h"
#include "Actor/ActorSpawnPoint.h"
#include "Actor/Player.h"

#include "../Engine/Camera.h"
#include "../Engine/Frustum.h"
#include "../Engine/Physics.h"
#include "../Engine/LightManager.h"

#include "../GameObjects/Entity.h"
#include "../GameObjects/GameEntity.h"
#include "../GameObjects/Physical.h"
#include "../GameObjects/Decoration.h"
#include "../GameObjects/Terrain.h"
#include "../GameObjects/Car.h"
#include "../GameObjects/Trigger.h"

#include "GameMode/GameMode.h"

#include "../WorldLoader/WorldLoader.h"
#include "../WorldLoader/CStatic.h"
#include "../WorldLoader/CDynamic.h"
#include "../WorldLoader/CLocator.h"

#include "../Menus/MenuElement.h"
#include "../Menus/MenuManager.h"
#include "../Menus/MenuGameMode.h"

#include "IwGraphics.h"
#include "IwManaged.h"
#include "IwManagedList.h"

#include "btBulletDynamicsCommon.h"

#include "../Scripting/GameLevelScript.h"
#include "../Scripting/ScriptManager.h"

//-------------------------------------------------------------------------------------

const char* GameLevel::s_levelNames[] =	{
											"level_training2_world",
											"level_time_attack_world"
										};
const char* GameLevel::s_levelScripts[] ={
											"scripts/level_training.lua",
											"scripts/level_time_attack.lua"
										 };

//-------------------------------------------------------------------------------------

GameLevel::GameLevel( const int modeID, const int levelID ) :
	m_levelID( levelID ),
	m_gameModeID( modeID )
{
	if( m_levelID <= GAMELEVEL_NONE || m_levelID >= GAMELEVEL_COUNT )
	{
		IwAssert( APP, "levelID is incorrect!!!" );
	}

	if( m_gameModeID <= GameMode::GM_ID_NONE || m_gameModeID >= GameMode::GM_ID_COUNT )
	{
		IwAssert( APP, "modeID is incorrect!!!" );
	}

	const char* scriptName = s_levelScripts[m_levelID];
	const char* levelName = s_levelNames[m_levelID];

	ScriptManager* scriptManager = ScriptManager::GetInstance();
	m_script = new GameLevelScript( scriptManager->GetVirtualMachine() );
	m_script->CompileFile( scriptName );
	scriptManager->FreeInstance();

	m_camera = new Camera();
	m_frustum = new Frustum();

	m_physics = Physics::GetInstance();

	m_lightManager = LightManager::GetInstance();

	m_menuManager = MenuManager::GetInstance();
	MenuGameMode* menuGameMode = static_cast<MenuGameMode*>( m_menuManager->GetMenuElementById( MenuElement::MENUELEMENTTYPE_GAME_MODE ) );
	
	const unsigned int carColor = menuGameMode->GetCurrentCarFrame();
	Car::LoadData( Car::s_carGroupNames[carColor] );
	m_player = new Player( carColor );

	m_worldLoader = new WorldLoader( levelName );
	m_worldLoader->LoadWorld();
	Load();
}

//-------------------------------------------------------------------------------------

GameLevel::~GameLevel()
{
	m_worldLoader->Delete();
	delete m_worldLoader;

	m_menuManager->FreeInstance();

	for( GameEntityIterator it = m_objects.begin(), end = m_objects.end(); it != end; ++it )
	{
		delete ( *it );
	}

	delete m_player;

	Car::DestroyData();
	Terrain::DestroyData();
	Decoration::DestroyData();
	
	m_lightManager->FreeInstance();

	m_physics->FreeInstance();

	delete m_frustum;
	delete m_camera;
	delete m_script;
}

//-------------------------------------------------------------------------------------

void GameLevel::Load()
{
	StringSet terrainGroups;
	StringSet decoGroups;
	StringSet carGroups;

	IW_MANAGED_LIST_ITERATE( *( WorldLoader::GetObjects() ), it )
	{
		if( !strcmp( ( *it )->GetClassName(), "CStatic" ) )
		{
			CStatic* objStatic = static_cast<CStatic*>( *it );

			if( objStatic )
			{
				const char* entityName = objStatic->GetEntityType().c_str();
				const unsigned int entityType = Entity::GetEntityType( entityName );

				switch( entityType )
				{
					case Entity::ENTITY_TERRAIN:
					{
						const std::string name = objStatic->GetName().c_str();
						const std::string groupName = objStatic->GetGroupName().c_str();
						const std::string modelName = objStatic->GetModelName().c_str();
						const CIwFVec3 pos = objStatic->GetPosition();
						const CIwFQuat rot = objStatic->GetRotation();

						Terrain* terrain = new Terrain( groupName, modelName );
						terrain->SetName( name );
						terrain->SetPosition( pos );
						terrain->SetRotation( rot );
						m_objects.push_back( terrain );

						terrainGroups.insert( groupName );
						m_terrain = terrain;
					}
					break;

					case Entity::ENTITY_DECORATION:
					{
						const std::string name = objStatic->GetName().c_str();
						const std::string groupName = objStatic->GetGroupName().c_str();
						const std::string modelName = objStatic->GetModelName().c_str();
						const CIwFVec3 pos = objStatic->GetPosition();
						const CIwFQuat rot = objStatic->GetRotation();
						const float mass = objStatic->GetMass();

						Decoration* decoration = new Decoration( groupName, modelName, Physical::PHYSICS_COLLISION_MESH );
						decoration->SetName( name );
						decoration->SetMass( mass );
						decoration->SetPosition( pos );
						decoration->SetRotation( rot );
						m_objects.push_back( decoration );

						decoGroups.insert( std::string( objStatic->GetGroupName().c_str() ) );
					}
					break;

					case Entity::ENTITY_TRIGGER:
						{
							const std::string name = objStatic->GetName().c_str();
							const CIwFVec3 pos = objStatic->GetPosition();
							const CIwFQuat rot = objStatic->GetRotation();
							const float scale = objStatic->GetScale();
							float mass = objStatic->GetMass();
							const std::string onEnterScript = objStatic->GetOnEnterScript().c_str();
							const std::string onLeaveScript = objStatic->GetOnLeaveScript().c_str();

							Trigger* trigger = new Trigger();
							trigger->SetName( name );
							trigger->SetMass( mass );
							trigger->SetPosition( pos );
							trigger->SetRotation( rot );
							trigger->SetScale( CIwFVec3( scale, scale, scale ) );
							trigger->SetOnEnterScriptName( onEnterScript.c_str() );
							trigger->SetOnLeaveScriptName( onLeaveScript.c_str() );
							m_triggers.push_back( trigger );
							m_objects.push_back( trigger );
						}
						break;
				}
			}
		}

		if( !strcmp( ( *it )->GetClassName(), "CDynamic" ) )
		{
			CDynamic* objDynamic = static_cast<CDynamic*>( *it );

			if( objDynamic )
			{
				const char* entityName = objDynamic->GetEntityType().c_str();
				const unsigned int entityType = Entity::GetEntityType( entityName );

				switch( entityType )
				{
					case Entity::ENTITY_DECORATION:
					{
						const std::string name = objDynamic->GetName().c_str();
						const std::string groupName = objDynamic->GetGroupName().c_str();
						const std::string modelName = objDynamic->GetModelName().c_str();
						const CIwFVec3 pos = objDynamic->GetPosition();
						const CIwFQuat rot = objDynamic->GetRotation();
						float mass = objDynamic->GetMass();

						Decoration* decoration = new Decoration( groupName, modelName, Physical::PHYSICS_COLLISION_MESH );
						decoration->SetName( name );
						decoration->SetMass( mass );
						decoration->SetPosition( pos );
						decoration->SetRotation( rot );
						m_objects.push_back( decoration );

						decoGroups.insert( std::string( objDynamic->GetGroupName().c_str() ) );
					}
					break;

					case Entity::ENTITY_CAR:
					{
						carGroups.insert( std::string( objDynamic->GetGroupName().c_str() ) );
					}
					break;
				}
			}
		}

		if( !strcmp( ( *it )->GetClassName(), "CLocator" ) )
		{
			CLocator* objLocator = static_cast<CLocator*>( *it );

			if( objLocator )
			{
				const char* className = objLocator->GetActorName().c_str();
				const unsigned int classNameInt = Actor::GetActorType( className );
				const std::string name = objLocator->GetName().c_str();

				switch( classNameInt )
				{
					case Actor::ACTOR_TYPE_PLAYER:
					{
						const char* typeName = objLocator->GetTypeName().c_str();
						const unsigned int typeNameInt = ActorSpawnPoint::GetActorSpawnPointType( typeName );

						const CIwFVec3 pos = objLocator->GetPosition();
						const CIwFQuat rotQuat = objLocator->GetRotation();
						CIwFVec3 rot = CIwFVec3( 0.0f, 0.0f, 0.0f );

						m_physics->QuaternionToEuler( rotQuat, rot );
						rot *= static_cast<float>( m_physics->BPU_180_PI );

						switch( typeNameInt )
						{
							case ActorSpawnPoint::ASPT_START_POINT:
							{
								ActorSpawnPoint* point = new ActorSpawnPoint();
								point->SetName( name );
								point->SetPosition( pos );
								point->SetRotation( rot );
								m_player->SetStartPosition( point );
							}
							break;

							case ActorSpawnPoint::ASPT_SPAWN_POINT:
							{
								ActorSpawnPoint* point = new ActorSpawnPoint();
								point->SetName( name );
								point->SetPosition( pos );
								point->SetRotation( rot );
								m_player->AddSpawnPoint( point );
							}
							break;
						}
					}
					break;

					case Actor::ACTOR_TYPE_NPC:
					{
					}
					break;
				}
			}
		}
	}

	LoadData( terrainGroups );
	LoadData( decoGroups );
	LoadData( carGroups );
}

//-------------------------------------------------------------------------------------

void GameLevel::LoadData( StringSet& group )
{
	if( !group.empty() )
	{
		for( StringSet::const_iterator it = group.begin(), end = group.end(); it != end; ++it )
		{
			GameEntity::LoadData( ( *it ).c_str() );
		}
	}
}

//-------------------------------------------------------------------------------------

void GameLevel::Init()
{
	for( GameEntityIterator it = m_objects.begin(), end = m_objects.end(); it != end; ++it )
	{
		( *it )->Init();
	}

	const float fov = static_cast<float>( IwGxGetDeviceWidth() / IwGxGetDeviceHeight() );
	m_camera->Init( CIwFVec3( 0.0f, 128.0f, -512.0f ), fov, 16384.0f, 1024.0f );
	m_camera->RotateZ( 180.0f );
	m_camera->RotateY( 180.0f );
	m_camera->RotateX( 0.0f );
	m_camera->Apply();

	m_player->Init();

	if( !m_triggers.empty() )
	{
		for( TriggerIterator it = m_triggers.begin(), end = m_triggers.end(); it != end; ++it )
		{
			m_player->AddTrigger( (*it)->GetID() );
		}
	}

	m_camera->FollowGameObject( m_player->GetCar() );

	CIwColour ambientColor;
	ambientColor.Set( 0x40, 0x40, 0x40 );
	m_lightManager->SetAmbient( ambientColor );

	CIwColour diffuseColor;
	CIwColour specularColor;
	CIwFVec3 lightDirection( 0.577f, 0.577f, 0.577f );
	diffuseColor.Set( 0, 0, 0 );
	specularColor.SetGrey( 100 );
	CIwFVec3 lightPosition( 0, 600, 0 );
	m_lightManager->SetDiffuse( diffuseColor, specularColor, lightPosition, lightDirection );
}

//-------------------------------------------------------------------------------------

void GameLevel::Update( const s3e_uint64_t dt )
{
	for( GameEntityIterator it = m_objects.begin(), end = m_objects.end(); it != end; ++it )
	{
		( *it )->Update( dt );
	}

	m_player->Update( dt );

	CheckPlayerLocation();
	CheckActorIntersectsWithTrigger( m_player );
}

//-------------------------------------------------------------------------------------

void GameLevel::Render()
{
	for( GameEntityIterator it = m_objects.begin(), end = m_objects.end(); it != end; ++it )
	{
		bool visible = m_frustum->ObjectInFrustum( *it );

		if( visible || ( *it )->GetType() == Entity::ENTITY_TERRAIN )
		{
			( *it )->Render();
		}
	}

	m_player->Render();

	m_camera->Apply();
}

//-------------------------------------------------------------------------------------

void GameLevel::CheckPlayerLocation()
{
	const CIwFVec3 playerPos = m_player->GetCar()->GetPosition();
	const CIwFBBox terrainBBox = m_terrain->GetBBox();

	if( playerPos.x > terrainBBox.maxV.x || playerPos.x < terrainBBox.minV.x || playerPos.z > terrainBBox.maxV.z || playerPos.z < terrainBBox.minV.z )
	{
		m_script->SelectScriptFunction( "OnRespawnPlayer" );
		m_script->Go();

		if( m_gameModeID == GameMode::GM_ID_TRAINING )
		{
			m_player->Respawn();
		}
	}
}

//-------------------------------------------------------------------------------------

void GameLevel::CheckActorIntersectsWithTrigger( Actor* actor )
{
	for( TriggerIterator it = m_triggers.begin(), end = m_triggers.end(); it != end; ++it )
	{
		( *it )->CheckIntersect( actor );
	}
}

//-------------------------------------------------------------------------------------

const char* GameLevel::GetLevelName( const unsigned int id )
{
	const char* levelName = "";

	if( id >= 0 && id < GAMELEVEL_COUNT )
	{
		levelName = s_levelNames[id];
	}

	return levelName;
}