#ifndef _GAMELEVEL_H_
#define _GAMELEVEL_H_

#include <set>
#include <string>

#include <IwArray.h>
#include <IwManagedList.h>
#include <IwGx.h>

class Camera;
class Frustum;
class Physics;
class LightManager;
class Decoration;
class Terrain;
class Car;
class Trigger;
class GameEntity;
class Actor;
class Player;
class WorldLoader;
class MenuManager;
class GameLevelScript;

//-----------------------------------------------------------------------------------------

typedef std::set<std::string> StringSet;
typedef CIwArray<GameEntity*> GameEntityArray;
typedef CIwArray<GameEntity*>::const_iterator GameEntityIterator;
typedef CIwArray<Trigger*> TriggerArray;
typedef CIwArray<Trigger*>::const_iterator TriggerIterator;

//-----------------------------------------------------------------------------------------
// GameLevel
//-----------------------------------------------------------------------------------------
class GameLevel
{
public:
	enum GameLevelID
	{
		GAMELEVEL_NONE = - 1,
		GAMELEVEL_TRAINING,
		GAMELEVEL_TIME_ATTACK,

		GAMELEVEL_COUNT
	};

private:
	static const char* s_levelNames[GAMELEVEL_COUNT];
	static const char* s_levelScripts[GAMELEVEL_COUNT];

public:
	GameLevel( const int modeID, const int levelID );
	~GameLevel();

	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Render
	void Render();

	// Get camera
	Camera* GetCamera() const;

	// Get player
	Player* GetPlayer() const;

	// Get script
	GameLevelScript* GetScript() const;

	// Returns level id
	int GetLevelID() const;

	// Returns game mode id
	int GetGameModeID() const;

public:
	// Get level name
	static const char* GetLevelName( const unsigned int id );

private:
	// Load level resources
	void Load();

	// Load data for each entity group
	void LoadData( StringSet& group );

	// Check player location
	void CheckPlayerLocation();

	// Checks any intersects the actor with triggers
	void CheckActorIntersectsWithTrigger( Actor* actor );

private:
	int					m_levelID;
	int					m_gameModeID;

	Camera*				m_camera;
	Frustum*			m_frustum;
	Physics*			m_physics;
	Terrain*			m_terrain;
	Player*				m_player;
	WorldLoader*		m_worldLoader;
	LightManager*		m_lightManager;

	GameLevelScript*	m_script;

	GameEntityArray		m_objects;
	TriggerArray		m_triggers;

	MenuManager*		m_menuManager;
};

//---------------------------------------------------------------------------------------

inline int GameLevel::GetLevelID() const
{
	return m_levelID;
}

//---------------------------------------------------------------------------------------

inline int GameLevel::GetGameModeID() const
{
	return m_gameModeID;
}

//---------------------------------------------------------------------------------------

inline Camera* GameLevel::GetCamera() const
{
	return m_camera;
}

//---------------------------------------------------------------------------------------

inline Player* GameLevel::GetPlayer() const
{
	return m_player;
}

//---------------------------------------------------------------------------------------

inline GameLevelScript* GameLevel::GetScript() const
{
	return m_script;
}

#endif	// _GAMELEVEL_H_