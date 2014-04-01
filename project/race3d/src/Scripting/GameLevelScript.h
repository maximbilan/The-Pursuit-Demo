#ifndef _GAME_LEVEL_SCRIPT_H_
#define _GAME_LEVEL_SCRIPT_H_

#include "GlobalScript.h"
#include "LuaWrapper/LuaVirtualMachine.h"

//---------------------------------------------------------------------------------
// GameLevelScript - script for gamelevel
//---------------------------------------------------------------------------------
class GameLevelScript : public GlobalScript
{
public:
	GameLevelScript( CLuaVirtualMachine& vm );
	virtual ~GameLevelScript();

	// Print into gameplay log
	int GamePlayLog( CLuaVirtualMachine& vm );

	// Add lap to time attack mode
	int TimeAttackAddLap( CLuaVirtualMachine& vm );

	// Finish the lap in the Time Attack mode
	int TimeAttackFinishLap( CLuaVirtualMachine& vm );

	// Respawn player
	int RespawnPlayer( CLuaVirtualMachine& vm );

	// When the script calls a class method, this is called
	virtual int ScriptCalling( CLuaVirtualMachine& vm, const int iFunctionNumber );

	// When the script function has returns
	virtual void HandleReturns( CLuaVirtualMachine& vm, const char *strFunc );
};

#endif	// _GAME_LEVEL_SCRIPT_H_