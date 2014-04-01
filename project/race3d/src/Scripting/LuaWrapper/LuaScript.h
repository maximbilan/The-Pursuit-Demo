// ---------------------------------------------------------------------------
// FILE NAME            : LuaScript.h
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Scripting base class
// 
// ---------------------------------------------------------------------------
// VERSION              : 1.00
// DATE                 : 1-Sep-2005
// AUTHOR               : Richard Shephard
// ---------------------------------------------------------------------------
// MODIFIED BY          : Maxim Bilan
// ---------------------------------------------------------------------------

#ifndef __LUA_SCRIPT_BASE_H__
#define __LUA_SCRIPT_BASE_H__

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "luavirtualmachine.h"

// --------------------------------------------------------------------------

class CLuaScript
{
public:
	CLuaScript( CLuaVirtualMachine& vm );
	virtual ~CLuaScript( void );

	// Compile script into Virtual Machine
	bool CompileFile( const char* strFilename );
	bool CompileBuffer( unsigned char* pbBuffer, size_t szLen );

	// Register function with Lua
	int RegisterFunction( const char* strFuncName );

	// Selects a Lua Script function to call
	bool SelectScriptFunction( const char* strFuncName );
	void AddParam( const int iInt );
	void AddParam( const float fFloat );
	void AddParam( const char* string );

	// Runs the loaded script
	bool Go( const int nReturns = 0 );

	// Checks on Virtual Machine script
	bool ScriptHasFunction( const char *strScriptName );

	// Method indexing check
	int methods( void );

	// When the script calls a class method, this is called
	virtual int ScriptCalling( CLuaVirtualMachine& vm, const int iFunctionNumber ) = 0;

	// When the script function has returns
	virtual void HandleReturns( CLuaVirtualMachine& vm, const char* strFunc ) = 0;

	CLuaVirtualMachine& vm( void );

protected:
	int m_nMethods;
	CLuaVirtualMachine& m_vm;
	int m_iThisRef;
	int m_nArgs;
	const char* m_strFunctionName;
};

// ---------------------------------------------------------------------------------

inline int CLuaScript::methods( void )
{
	return m_nMethods;
}

// ---------------------------------------------------------------------------------

inline CLuaVirtualMachine& CLuaScript::vm( void )
{
	return m_vm;
}

#endif // __LUA_SCRIPT_BASE_H__