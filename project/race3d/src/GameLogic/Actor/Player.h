#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <s3eTypes.h>
#include <IwArray.h>

#include "Actor.h"
#include "ActorSpawnPoint.h"

typedef CIwArray<ActorSpawnPoint*> ActorSpawnPointArray;

//---------------------------------------------------------------------------------------
// Player - controller of the car
//---------------------------------------------------------------------------------------
class Player : public Actor
{
public:
	Player( const unsigned int carColor = 0 );
	virtual ~Player();

	// Initialization
	void Init();

	// Update
	void Update( const s3e_uint64_t dt );

	// Render
	void Render();

	// Move left
	void DoLeft();

	// Move right
	void DoRight();

	// Move forward
	void Gas();

	// Brake
	void Brake();

	// Respawn
	void Respawn();

	// Respawn by id
	void Respawn( const std::string& id );

	// Respawn on start position
	void RespawnOnStartPosition();

	// Add spawn point
	void AddSpawnPoint( ActorSpawnPoint* point );

private:
	ActorSpawnPointArray	m_spawnPoints;
};

#endif	// _PLAYER_H_