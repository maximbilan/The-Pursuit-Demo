// ---------------------------------------------------------------------------
// FILE NAME            : LuaScript.cpp
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

#include <assert.h>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "luascript.h"
#include "luarestorestack.h"
#include "luathis.h"

// ---------------------------------------------------------------------------

#define BEGIN_LUA_CHECK( vm )	lua_State* state = ( lua_State* ) vm; \
								if( vm.Ok() ) {
#define END_LUA_CHECK			}


// --------------------------------------------------------------------------
// int LuaCallback
//---------------------------------------------------------------------------
// Lua C-API calling that figures out which object to hand over to
//
// Parameter   Dir      Description
// ---------   ---      -----------
// lua         IN       State variable
//
// Return
// ------
// Number of return varaibles on the stack
//
// Comments
// --------
// This is the function lua calls for each C-Function that is
// registered with lua. At the time of registering the function
// with lua, we make lua record the method "number" so we can
// know what method was actually called. The lua stack is the
// following structure:
// 0: 'this' (table)
// 1 - ...: parameters passed in
//
// --------------------------------------------------------------------------
static int LuaCallback( lua_State* lua )
{
	// Locate the psudo-index for the function number
	int iNumberIdx = lua_upvalueindex( 1 );
	int nRetsOnStack = 0;

	bool fSuccess = false;

	// Check for the "this" table
	if( lua_istable( lua, 1 ) )
	{
		// Found the "this" table. The object pointer is at the index 0
		lua_rawgeti( lua, 1, 0 );

		if( lua_islightuserdata( lua, -1 ) )
		{
			// Found the pointer, need to cast it
			CLuaScript* pThis = static_cast<CLuaScript* >( lua_touserdata( lua, -1 ) );

			// Get the method index
			const int iMethodIdx = static_cast<int>( lua_tonumber( lua, iNumberIdx ) );

			// Check that the method is correct index
			assert( !( iMethodIdx > pThis->methods() ) );

			// Reformat the stack so our parameters are correct
			// Clean up the "this" table
			lua_remove( lua, 1 );
			// Clean up the pThis pointer
			lua_remove( lua, -1 );

			// Call the class
			nRetsOnStack = pThis->ScriptCalling( pThis->vm(), iMethodIdx );

			fSuccess = true;
		}
	}

	if( fSuccess == false )
	{
		lua_pushstring( lua, "LuaCallback -> Failed to call the class function" );
		lua_error( lua );
	}

	// Number of return variables
	return nRetsOnStack;
}

// --------------------------------------------------------------------------
// CLuaScript::CLuaScript
//---------------------------------------------------------------------------
// Constructor. Sets up the lua stack and the "this" table
//
// Parameter            Dir      Description
// ---------            ---      -----------
// CLuaVirtualMachine   IN       VM to run on
//
// Return
// ------
// None.
//
// --------------------------------------------------------------------------
CLuaScript::CLuaScript( CLuaVirtualMachine& vm ) :
	m_vm( vm ),
	m_nMethods( 0 ),
	m_iThisRef( 0 ),
	m_nArgs( 0 )
{
	BEGIN_LUA_CHECK( vm )
		// Create a reference to the "this" table. Each reference is unique
		lua_newtable( state );
		m_iThisRef = luaL_ref( state, LUA_REGISTRYINDEX );

		// Save the "this" table to index 0 of the "this" table
		CLuaRestoreStack rs( vm );
		lua_rawgeti( state, LUA_REGISTRYINDEX, m_iThisRef );
		lua_pushlightuserdata( state, ( void* ) this );
		lua_rawseti( state, -2, 0 );
	END_LUA_CHECK
}

// --------------------------------------------------------------------------
// CLuaScript::~CLuaScript
//---------------------------------------------------------------------------
// Destructor
//
// Parameter   Dir      Description
// ---------   ---      -----------
// None.
//
// Return
// ------
// None.
//
// --------------------------------------------------------------------------
CLuaScript::~CLuaScript( void )
{
	CLuaRestoreStack rs( m_vm );

	BEGIN_LUA_CHECK( m_vm )
		// Get the reference "this" table
		lua_rawgeti( state, LUA_REGISTRYINDEX, m_iThisRef );

		// Clear index 0
		lua_pushnil( state );
		lua_rawseti( state, -2, 0 );
	END_LUA_CHECK
}

// --------------------------------------------------------------------------
// bool CLuaScript::CompileBuffer
//---------------------------------------------------------------------------
// Compiles a given buffer
//
// Parameter   Dir      Description
// ---------   ---      -----------
// pbBuffer    IN       Data buffer
// szLen       IN       Length of buffer
//
// Return
// ------
// Success
//
// --------------------------------------------------------------------------
bool CLuaScript::CompileBuffer( unsigned char *pbBuffer, size_t szLen )
{
	assert( pbBuffer != NULL && "CLuaScript::CompileBuffer ->  pbBuffer == NULL" );
	assert( szLen != 0 && "CLuaScript::CompileBuffer -> szLen == 0" );
	assert( m_vm.Ok() && "VM Not OK" );

	// Make sure we have the correct "this" table
	CLuaThis luaThis( m_vm, m_iThisRef );

	return m_vm.RunBuffer( pbBuffer, szLen );
}

// --------------------------------------------------------------------------
// bool CLuaScript::CompileBuffer
//---------------------------------------------------------------------------
// Compiles a given file
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFilename IN       File name to compile
//
// Return
// ------
// Success
//
// --------------------------------------------------------------------------
bool CLuaScript::CompileFile( const char* strFilename )
{
	assert( strFilename != NULL && "CLuaScript::CompileFile -> strFilename == NULL" );
	assert( m_vm.Ok() && "VM Not OK" );

	// Make sure we have the correct "this" table
	CLuaThis luaThis( m_vm, m_iThisRef );

	return m_vm.RunFile( strFilename );
}

// --------------------------------------------------------------------------
// int CLuaScript::RegisterFunction
//---------------------------------------------------------------------------
// Registers a function with Lua
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFuncName IN       Function name
//
// Return
// ------
// Success
//
// --------------------------------------------------------------------------
int CLuaScript::RegisterFunction( const char* strFuncName )
{
	assert( strFuncName != NULL && "CLuaScript::RegisterFunction -> strFuncName == NULL" );
	assert( m_vm.Ok() && "VM Not OK" );

	int iMethodIdx = -1;

	CLuaRestoreStack rs( m_vm );

	BEGIN_LUA_CHECK( m_vm )
		iMethodIdx = ++m_nMethods;

		// Register a function with the lua script. Added it to the "this" table
		lua_rawgeti( state, LUA_REGISTRYINDEX, m_iThisRef );

		// Push the function and parameters
		lua_pushstring( state, strFuncName );
		lua_pushnumber( state, ( lua_Number ) iMethodIdx );
		lua_pushcclosure( state, LuaCallback, 1 );
		lua_settable( state, -3 );

	END_LUA_CHECK

	return iMethodIdx;
}

// --------------------------------------------------------------------------
// bool CLuaScript::SelectScriptFunction
//---------------------------------------------------------------------------
// Selects a script function to run
//
// Parameter   Dir      Description
// ---------   ---      -----------
// strFuncName IN       Function name
//
// Return
// ------
// Success
//
// --------------------------------------------------------------------------
bool CLuaScript::SelectScriptFunction( const char* strFuncName )
{
	assert( strFuncName != NULL && "CLuaScript::SelectScriptFunction -> strFuncName == NULL" );
	assert( m_vm.Ok () && "VM Not OK" );

	bool fSuccess = true;

	BEGIN_LUA_CHECK( m_vm )
		// Look up function name
		lua_rawgeti( state, LUA_REGISTRYINDEX, m_iThisRef );
		lua_pushstring( state, strFuncName );
		lua_rawget( state, -2 );
		lua_remove( state, -2 );

		// Put the "this" table back
		lua_rawgeti( state, LUA_REGISTRYINDEX, m_iThisRef );

		// Check that we have a valid function
		if( !lua_isfunction( state, -2 ) )
		{
			fSuccess = false;
			lua_pop( state, 2 );
		}
		else
		{
			m_nArgs = 0;
			m_strFunctionName = strFuncName;
		}
	END_LUA_CHECK

	return fSuccess;
}

// --------------------------------------------------------------------------
// bool CLuaScript::ScriptHasFunction
//---------------------------------------------------------------------------
// Checks to see if a function exists
//
// Parameter      Dir      Description
// ---------      ---      -----------
// strScriptName  IN       Function name
//
// Return
// ------
// Success
//
// --------------------------------------------------------------------------
bool CLuaScript::ScriptHasFunction( const char* strScriptName )
{
	assert( strScriptName != NULL && "CLuaScript::ScriptHasFunction -> strScriptName == NULL" );
	assert( m_vm.Ok() && "VM Not OK" );

	CLuaRestoreStack rs( m_vm );

	bool fFoundFunc = false;

	BEGIN_LUA_CHECK( m_vm )
		lua_rawgeti( state, LUA_REGISTRYINDEX, m_iThisRef );
		lua_pushstring( state, strScriptName );
		lua_rawget( state, -2 );
		lua_remove( state, -2 );

		if( lua_isfunction( state, -1 ) )
		{
			fFoundFunc = true;
		}
	END_LUA_CHECK

	return fFoundFunc;
}

// --------------------------------------------------------------------------
// void CLuaScript::AddParam
//---------------------------------------------------------------------------
// Adds a parameter to the parameter list
//
// Parameter   Dir      Description
// ---------   ---      -----------
// string      IN       string param
//
// Return
// ------
// None.
//
// --------------------------------------------------------------------------
void CLuaScript::AddParam( const char* string )
{
	assert( string != NULL && "CLuaScript::AddParam -> string == NULL" );
	assert( m_vm.Ok() && "VM Not OK" );

	BEGIN_LUA_CHECK( m_vm )
		lua_pushstring( state, string );
		++m_nArgs;
	END_LUA_CHECK
}

// --------------------------------------------------------------------------
// void CLuaScript::AddParam
//---------------------------------------------------------------------------
// Adds a parameter to the parameter list
//
// Parameter   Dir      Description
// ---------   ---      -----------
// iInt        IN       int param
//
// Return
// ------
// None.
//
// --------------------------------------------------------------------------
void CLuaScript::AddParam( const int iInt )
{
	assert( m_vm.Ok() && "VM Not OK" );

	BEGIN_LUA_CHECK( m_vm )
		lua_pushnumber( state, ( lua_Number ) iInt );
		++m_nArgs;
	END_LUA_CHECK
}

// --------------------------------------------------------------------------
// void CLuaScript::AddParam
//---------------------------------------------------------------------------
// Adds a parameter to the parameter list
//
// Parameter   Dir      Description
// ---------   ---      -----------
// fFloat      IN       float param
//
// Return
// ------
// None.
//
// --------------------------------------------------------------------------
void CLuaScript::AddParam( const float fFloat )
{
	assert( m_vm.Ok() && "VM Not OK" );

	BEGIN_LUA_CHECK( m_vm )
		lua_pushnumber( state, ( lua_Number ) fFloat );
		++m_nArgs;
	END_LUA_CHECK
}

// --------------------------------------------------------------------------
// bool CLuaScript::Go
//---------------------------------------------------------------------------
// Runs the selected script function
//
// Parameter   Dir      Description
// ---------   ---      -----------
// nReturns    IN       Number of expected returns
//
// Return
// ------
// None.
//
// --------------------------------------------------------------------------
bool CLuaScript::Go( const int nReturns /* = 0 */ )
{
	assert( m_vm.Ok() && "VM Not OK" );

	// At this point there should be a parameters and a function on the
	// Lua stack. Each function get a "this" parameter as default and is
	// pushed onto the stack when the method is selected

	bool fSuccess = m_vm.CallFunction( m_nArgs + 1, nReturns );

	if( fSuccess == true && nReturns > 0 )
	{
		// Check for returns
		HandleReturns( m_vm, m_strFunctionName );
		lua_pop( ( lua_State* ) m_vm, nReturns );
	}

	return fSuccess;
}