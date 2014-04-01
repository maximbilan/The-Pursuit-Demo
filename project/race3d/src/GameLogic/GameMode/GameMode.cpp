#include "GameMode.h"
#include "../Actor/Player.h"
#include "../../Engine/Record/RecordDB.h"

//-------------------------------------------------------------------------------------

GameMode::GameMode() :
	m_player( NULL ),
	m_state( 0 ),
	m_id( 0 )
{
}

//-------------------------------------------------------------------------------------

GameMode::~GameMode()
{
}

//-------------------------------------------------------------------------------------

void GameMode::Init( Player* player )
{
	m_player = player;
	m_state = GMS_SUSPENDED;
}