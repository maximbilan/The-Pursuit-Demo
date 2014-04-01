// ---------------------------------------------------------------------------
// FILE NAME            : LuaDebugger.h
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Debugging interface
// 
// ---------------------------------------------------------------------------
// VERSION              : 1.00
// DATE                 : 1-Sep-2005
// AUTHOR               : Richard Shephard
// ---------------------------------------------------------------------------
// MODIFIED BY          : Maxim Bilan
// ---------------------------------------------------------------------------

#ifndef __LUA_DEBUGGER_H__
#define __LUA_DEBUGGER_H__

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "luavirtualmachine.h"

// -------------------------------------------------------------------------

enum
{
	DBG_MASK_CALL	= LUA_MASKCALL,
	DBG_MASK_RET	= LUA_MASKRET,
	DBG_MASK_LINE	= LUA_MASKLINE,
	DBG_MASK_COUNT	= LUA_MASKCOUNT
};

// -------------------------------------------------------------------------

class CLuaVirtualMachine;

class CLuaDebugger
{
public:
	CLuaDebugger( CLuaVirtualMachine& vm );
	virtual ~CLuaDebugger( void );

	void SetHooksFlag( const int iMask );
	void SetCount( const int iCount );

	void ErrorRun( const int iErrorCode );

protected:
	int m_iCountMask;
	CLuaVirtualMachine& m_vm;
};

// -------------------------------------------------------------------------

inline void CLuaDebugger::SetCount( const int iCount )
{
	m_iCountMask = iCount;
}

#endif // __LUA_DEBUGGER_H__