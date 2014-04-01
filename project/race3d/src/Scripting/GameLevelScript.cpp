#include "GameLevelScript.h"
#include "ScriptFunctions.h"

#include "../Common/Defines.h"

#include "../../Menus/GamePlayLog.h"
#include "../../GameStates/GameState.h"
#include "../../GameStates/GS_GamePlay.h"
#include "../../GameLogic/GameMode/GameMode.h"
#include "../../GameLogic/GameMode/GM_TimeAttack.h"
#include "../../GameLogic/GameLevel.h"
#include "../../GameLogic/Actor/Player.h"

//---------------------------------------------------------------------------------

GameLevelScript::GameLevelScript( CLuaVirtualMachine& vm ) :
	GlobalScript( vm )
{
	RegisterFunction( "gameplay_log" );				// SF_GAMEPLAY_LOG
	RegisterFunction( "time_attack_add_lap" );		// SF_TIME_ATTACK_ADD_LAP
	RegisterFunction( "time_attack_finish_lap" );	// SF_TIME_ATTACK_FINISH_LAP
	RegisterFunction( "respawn_player" );			// SF_RESPAWN_PLAYER
}

//---------------------------------------------------------------------------------

GameLevelScript::~GameLevelScript()
{
}

//---------------------------------------------------------------------------------

int GameLevelScript::GamePlayLog( CLuaVirtualMachine& vm )
{
	lua_State* state = static_cast<lua_State*>( vm );
	const char* str = lua_tostring( state, -1 );

#ifdef ENABLE_SCRIPT_MSG_LOG
	const int currentStateId = GameState::GetStateID();
	if( currentStateId == GameState::k_state_gameplay )
	{
		GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
		if( gameplay )
		{
			gameplay->GetLog()->AddMessage( std::string( str ) );
		}
	}
#endif

	return 0;
}

//---------------------------------------------------------------------------------

int GameLevelScript::TimeAttackAddLap( CLuaVirtualMachine& vm )
{
	lua_State* state = static_cast<lua_State*>( vm );

	const int currentStateId = GameState::GetStateID();
	if( currentStateId == GameState::k_state_gameplay )
	{
		GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
		if( gameplay )
		{
			GM_TimeAttack* timeAttack = static_cast<GM_TimeAttack*>( gameplay->GetGameMode() );
			if( timeAttack )
			{
				// Add passed lap
				timeAttack->AddLap();
				
				// Print message about passed laps
				const int laps = timeAttack->GetLapsAmount();
				char out[32];
				sprintf( out, "Current lap: %d", laps );
				gameplay->GetLog()->AddMessage( std::string( out ) );
			}
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------

int GameLevelScript::TimeAttackFinishLap( CLuaVirtualMachine& vm )
{
	lua_State* state = static_cast<lua_State*>( vm );

	const int currentStateId = GameState::GetStateID();
	if( currentStateId == GameState::k_state_gameplay )
	{
		GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
		if( gameplay )
		{
			GM_TimeAttack* timeAttack = static_cast<GM_TimeAttack*>( gameplay->GetGameMode() );
			if( timeAttack )
			{
				timeAttack->FinishLap();
			}
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------

int GameLevelScript::RespawnPlayer( CLuaVirtualMachine& vm )
{
	lua_State* state = static_cast<lua_State*>( vm );
	const std::string respawnId = lua_tostring( state, -1 );

	const int currentStateId = GameState::GetStateID();
	if( currentStateId == GameState::k_state_gameplay )
	{
		GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
		if( gameplay )
		{
			GameLevel* gamelevel = gameplay->GetGameLevel();
			if( gamelevel )
			{
				Player* player = gamelevel->GetPlayer();
				if( player )
				{
					player->Respawn( respawnId );
				}
			}
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------

int GameLevelScript::ScriptCalling( CLuaVirtualMachine& vm, const int iFunctionNumber )
{
	int result = 0;
	result = GlobalScript::ScriptCalling( vm, iFunctionNumber );

	switch( iFunctionNumber - m_iMethodBase )
	{
	case SF_GAMEPLAY_LOG:
		return GamePlayLog( vm );
	case SF_TIME_ATTACK_ADD_LAP:
		return TimeAttackAddLap( vm );
	case SF_TIME_ATTACK_FINISH_LAP:
		return TimeAttackFinishLap( vm );
	case SF_RESPAWN_PLAYER:
		return RespawnPlayer( vm );
	}

	return result;
}

//---------------------------------------------------------------------------------

void GameLevelScript::HandleReturns( CLuaVirtualMachine& vm, const char *strFunc )
{
	GlobalScript::HandleReturns( vm, strFunc );
}