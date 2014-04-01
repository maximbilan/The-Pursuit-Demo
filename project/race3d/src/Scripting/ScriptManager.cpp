#include "ScriptManager.h"

#include "LuaWrapper/LuaVirtualMachine.h"
#include "LuaWrapper/LuaDebugger.h"

//-----------------------------------------------------------------------------------

void ScriptManager::Init()
{
	m_luaVM = new CLuaVirtualMachine();
	m_luaVM->InitialiseVM();

	m_luaDebugger = new CLuaDebugger( *m_luaVM );
	m_luaDebugger->SetCount( 10 );
}

//-----------------------------------------------------------------------------------

void ScriptManager::Remove()
{
	delete m_luaDebugger;
	delete m_luaVM;
}