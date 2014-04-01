#ifndef _GLOBAL_SCRIPT_H_
#define _GLOBAL_SCRIPT_H_

#include "LuaWrapper/LuaScript.h"
#include "LuaWrapper/LuaVirtualMachine.h"

class GamePlayLog;
class Config;

//-----------------------------------------------------------------------------------
// GlobalScript - base script, which has base functions
//-----------------------------------------------------------------------------------
class GlobalScript : public CLuaScript
{
public:
	GlobalScript( CLuaVirtualMachine& vm );
	virtual ~GlobalScript();

	// Check is music enabled
	int IsMusicEnabled( CLuaVirtualMachine& vm );
	
	// Check is sound effects enabled
	int IsSoundEffectsEnabled( CLuaVirtualMachine& vm );

	// When the script calls a class method, this is called
	virtual int ScriptCalling( CLuaVirtualMachine& vm, const int iFunctionNumber );

	// When the script function has returns
	virtual void HandleReturns( CLuaVirtualMachine& vm, const char *strFunc );

protected:
	int m_iMethodBase;
};

#endif	// _GLOBAL_SCRIPT_H_