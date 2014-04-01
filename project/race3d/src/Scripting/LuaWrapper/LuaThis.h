// ---------------------------------------------------------------------------
// FILE NAME            : LuaThis.h
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Controls the "this" table
// 
// ---------------------------------------------------------------------------
// VERSION              : 1.01
// DATE                 : 1-Sep-2005
// AUTHOR               : Richard Shephard
// ---------------------------------------------------------------------------
// MODIFIED BY          : Maxim Bilan
// ---------------------------------------------------------------------------

#ifndef __LUA_THIS_H__
#define __LUA_THIS_H__

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "luavirtualmachine.h"

// ---------------------------------------------------------------------------
// Sets the "this" global table that scripts use
// ---------------------------------------------------------------------------
class CLuaThis
{
public:
	CLuaThis( CLuaVirtualMachine& vm, int iRef ) :
		m_iOldRef( 0 ),
		m_vm( vm )
	{
		lua_State* state = static_cast<lua_State*>( vm );
		if( vm.Ok() )
		{
			// Save the old "this" table
			lua_getglobal( state, "this" );
			m_iOldRef = luaL_ref( state, LUA_REGISTRYINDEX );

			// replace it with our new one
			lua_rawgeti( state, LUA_REGISTRYINDEX, iRef );
			lua_setglobal( state, "this" );
		}
	}

	virtual ~CLuaThis( void )
	{
		lua_State* state = static_cast<lua_State*>( m_vm );
		if( m_iOldRef > 0 && m_vm.Ok() )
		{
			// Replace the old "this" table
			lua_rawgeti( state, LUA_REGISTRYINDEX, m_iOldRef );
			lua_setglobal( state, "this" );
			luaL_unref( state, LUA_REGISTRYINDEX, m_iOldRef );
		}
	}

protected:
	int m_iOldRef;
	CLuaVirtualMachine& m_vm;
};

#endif // __LUA_THIS_H__