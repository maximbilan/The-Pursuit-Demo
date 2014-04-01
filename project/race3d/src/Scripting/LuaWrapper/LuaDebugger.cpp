// ---------------------------------------------------------------------------
// FILE NAME            : LuaDebugger.cpp
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Simple debugging routines
// 
// ---------------------------------------------------------------------------
// VERSION              : 1.00
// DATE                 : 1-Sep-2005
// AUTHOR               : Richard Shephard
// ---------------------------------------------------------------------------
// MODIFIED BY          : Maxim Bilan
// ---------------------------------------------------------------------------

#include "luadebugger.h"

// -------------------------------------------------------------------------

// typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);
static void LuaHookCall( lua_State* lua )
{
	printf( "---- Call Stack ----\n" );
	//printf ("[Level] [Function] [# args] [@line] [src]\n");

	lua_Debug ar;

	// Look at call stack
	for( int iLevel = 0; lua_getstack( lua, iLevel, &ar ) != 0; ++iLevel )
	{
		if( lua_getinfo( lua, "Snlu", &ar ) != 0 )
		{
			printf( "%d %s %s %d @%d %s\n", iLevel, ar.namewhat, ar.name, ar.nups, ar.linedefined, ar.short_src );
		}
	}
}

// -------------------------------------------------------------------------

static void LuaHookRet( lua_State* lua )
{
}

// -------------------------------------------------------------------------

static void LuaHookLine( lua_State* lua )
{
	lua_Debug ar;
	lua_getstack( lua, 0, &ar );

	if( lua_getinfo( lua, "Sl", &ar ) != 0 )
	{
		printf( "exe %s on line %d\n", ar.short_src, ar.currentline );
	}
}

// -------------------------------------------------------------------------

static void LuaHookCount( lua_State* lua )
{
	LuaHookLine( lua );
}

// -------------------------------------------------------------------------

static void LuaHook( lua_State* lua, lua_Debug* ar )
{
	// Handover to the correct hook
	switch( ar->event )
	{
	case LUA_HOOKCALL:
		{
			LuaHookCall( lua );
		}
		break;
	/*case LUA_HOOKRET:
	case LUA_HOOKTAILRET:
		LuaHookRet( lua );
		break;*/
	case LUA_HOOKLINE:
		{
			LuaHookLine( lua );
		}
		break;
	case LUA_HOOKCOUNT:
		{
			LuaHookCount( lua );
		}
		break;
	}
}

// -------------------------------------------------------------------------

CLuaDebugger::CLuaDebugger( CLuaVirtualMachine& vm ) :
	m_iCountMask( 10 ),
	m_vm( vm )
{
	// Clear all current hooks
	if( vm.Ok() )
	{
		vm.AttachDebugger( this );
		lua_sethook( ( lua_State* ) vm, LuaHook, 0, m_iCountMask );
	}
}

// -------------------------------------------------------------------------

CLuaDebugger::~CLuaDebugger( void )
{
	// Clear all current hooks
	if( m_vm.Ok() )
	{
		lua_sethook( (lua_State*) m_vm, LuaHook, 0, m_iCountMask );
	}
}

// -------------------------------------------------------------------------

void CLuaDebugger::SetHooksFlag( const int iMask )
{
	// Set hooks
	lua_sethook( ( lua_State* ) m_vm, LuaHook, iMask, m_iCountMask );
}

// -------------------------------------------------------------------------

void CLuaDebugger::ErrorRun( const int iErrorCode )
{
	switch( iErrorCode )
	{
	case LUA_ERRRUN:
		{
			printf( "LUA_ERRRUN\n" );
		}
		break;
	case LUA_ERRMEM:
		{
			printf( "LUA_ERRMEM\n" );
		}
		break;
	case LUA_ERRERR:
		{
			printf( "LUA_ERRERR\n" );
		}
		break;
	}

	// Get the error string that appears on top of stack when a function
	// fails to run
	printf( "Error: %s\n", lua_tostring( (lua_State*) m_vm, -1 ) );
}