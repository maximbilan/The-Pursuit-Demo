#include "GlobalScript.h"
#include "ScriptFunctions.h"

#include "../../Utilities/Config/Config.h"

//-----------------------------------------------------------------------------------

GlobalScript::GlobalScript( CLuaVirtualMachine& vm ) :
	CLuaScript( vm )
{
	m_iMethodBase = RegisterFunction ( "ismusicenabled" );	// SF_IS_MUSIC_ENABLED
	RegisterFunction ( "issoundeffectsenabled" );			// SF_IS_SOUND_EFFECTS_ENABLED
}

//-----------------------------------------------------------------------------------

GlobalScript::~GlobalScript()
{
}

//-----------------------------------------------------------------------------------

int GlobalScript::IsMusicEnabled( CLuaVirtualMachine& vm )
{
	lua_State *state = static_cast<lua_State*>( vm );

	Config* config = Config::GetInstance();
	const int result = config->IsMusicEnabled() ? 1 : 0;
	lua_pushboolean( state, result );
	config->FreeInstance();

	return 1;
}

//-----------------------------------------------------------------------------------

int GlobalScript::IsSoundEffectsEnabled( CLuaVirtualMachine& vm )
{
	lua_State *state = static_cast<lua_State*>( vm );

	Config* config = Config::GetInstance();
	const int result = config->IsSoundEffectsEnabled() ? 1 : 0;
	lua_pushboolean( state, result );
	config->FreeInstance();

	return 1;
}

//-----------------------------------------------------------------------------------

int GlobalScript::ScriptCalling( CLuaVirtualMachine& vm, const int iFunctionNumber )
{
	switch( iFunctionNumber - m_iMethodBase )
	{
	case SF_IS_MUSIC_ENABLED:
		return IsMusicEnabled( vm );
	case SF_IS_SOUND_EFFECTS_ENABLED:
		return IsSoundEffectsEnabled( vm );
	}

	return 0;
}

//-----------------------------------------------------------------------------------

void GlobalScript::HandleReturns( CLuaVirtualMachine& vm, const char *strFunc )
{
}