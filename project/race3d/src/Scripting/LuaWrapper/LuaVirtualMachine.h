// ---------------------------------------------------------------------------
// FILE NAME            : LuaVirtualMachine.h
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Lua virtual machine implementation
// 
// ---------------------------------------------------------------------------
// VERSION              : 1.00
// DATE                 : 1-Sep-2005
// AUTHOR               : Richard Shephard
// ---------------------------------------------------------------------------
// MODIFIED BY          : Maxim Bilan
// ---------------------------------------------------------------------------

#ifndef __LUA_VIRTUAL_MACHINE_H__
#define __LUA_VIRTUAL_MACHINE_H__

#include <string>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "luadebugger.h"

//-------------------------------------------------------------------------------------

#define USE_LUA_BINARY_CONVERTER

#ifdef USE_LUA_BINARY_CONVERTER

#define LUA_FILE_BUFFER 8136

enum
{
	LuaFileSuccess = 0,
	LuaFileFail
};

#endif

//-------------------------------------------------------------------------------------

class CLuaDebugger;

class CLuaVirtualMachine
{
public:
	static const std::string s_luaExt;
	static const std::string s_bluaExt;

public:
	CLuaVirtualMachine( void );
	virtual ~CLuaVirtualMachine( void );

	bool InitialiseVM( void );
	bool DestroyVM( void );

	// Load and run script elements
	bool RunFile( const char* strFilename );
	bool RunBuffer( const unsigned char* pbBuffer, size_t szLen, const char* strName = NULL );

	// C-Api into script
	bool CallFunction( const int nArgs, const int nReturns = 0 );

	// Get the state of the lua stack (use the cast operator)
	//lua_State* GetState (void) { return m_pState; }
	operator lua_State* ( void ) { return m_pState; }

	static void Panic( lua_State* lua );

	// Check if the VM is OK and can be used still
	virtual bool Ok( void ) { return m_fIsOk; }

	// For debugging
	void AttachDebugger( CLuaDebugger *dbg ) { m_pDbg = dbg; }

private:
#ifdef USE_LUA_BINARY_CONVERTER
	// Changes extention of file
	void ChangeExt( std::string& out, const std::string ext );
	
	// Read lua file to buffer
	int ReadLuaFile( const char* fileName );

	// Read binary file
	int ReadBinaryFile( const char* fileName );

	// Write blua binary file
	int WriteBinaryFile( const char* fileName );
#endif

private:
	std::string m_buffer;

protected:
	lua_State *m_pState;
	bool m_fIsOk;
	CLuaDebugger *m_pDbg;
};

#endif // __LUA_VIRTUAL_MACHINE_H__