#ifndef _SCRIPT_MANAGER_H_
#define _SCRIPT_MANAGER_H_

#include "../Common/Singleton.h"

class CLuaVirtualMachine;
class CLuaDebugger;

//-------------------------------------------------------------------------------------
// ScriptManager - manager fow working with scripts
//-------------------------------------------------------------------------------------
class ScriptManager : public Singleton<ScriptManager>
{
public:
	// Initialization
	void Init();

	// Remove the instance
	void Remove();

	// Returns lua virtual machine
	CLuaVirtualMachine& GetVirtualMachine() const;

protected:
	ScriptManager() {}
	friend class Singleton<ScriptManager>;

private:
	CLuaVirtualMachine*	m_luaVM;
	CLuaDebugger*		m_luaDebugger;
};

//-----------------------------------------------------------------------------------

inline CLuaVirtualMachine& ScriptManager::GetVirtualMachine() const
{
	return *m_luaVM;
}

#endif