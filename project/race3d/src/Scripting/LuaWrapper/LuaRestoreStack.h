// ---------------------------------------------------------------------------
// FILE NAME            : LuaRestoreStack.h
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Restores the Lua stack to the way we found it :)
// 
// ---------------------------------------------------------------------------
// VERSION              : 1.00
// DATE                 : 1-Sep-2005
// AUTHOR               : Richard Shephard
// ---------------------------------------------------------------------------
// MODIFIED BY          : Maxim Bilan
// ---------------------------------------------------------------------------

#ifndef __LUA_RESTORE_STACK_H__
#define __LUA_RESTORE_STACK_H__

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

// ----------------------------------------------------------------------------

class CLuaRestoreStack
{
public:
	CLuaRestoreStack( CLuaVirtualMachine& vm ) : 
		m_pState( NULL ),
		m_iTop( 0 )
	{
		m_pState = static_cast<lua_State*>( vm );
		if( vm.Ok() )
		{
			m_iTop = lua_gettop( m_pState );
		}
	}

	virtual ~CLuaRestoreStack( void )
	{
		lua_settop( m_pState, m_iTop );
	}

protected:
	lua_State* m_pState;
	int m_iTop;
};

#endif // __LUA_RESTORE_STACK_H__