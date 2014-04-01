#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "s3eTypes.h"

//-------------------------------------------------------------------------------------
// GameState - base class for gamestates
//-------------------------------------------------------------------------------------
class GameState
{
public:
	static const int k_state_none		= 0;
	static const int k_state_gameplay	= 1;
	static const int k_state_loading	= 2;
	static const int k_state_menu		= 3;

public:	
	// game states stack
	static const int k_statesStackLen = 10;

public:
	GameState();
	virtual ~GameState();

	// Resume state
	virtual void ResumeState() = 0;

	// Update state
	virtual void UpdateState( const s3e_uint64_t frameTime ) = 0;

	// Render state
	virtual void RenderState() = 0;

	// Init states stack
	static void InitStatesStack();	

	// Release states stack
	static void ReleaseStatesStack();

	// Set state
	static void SetState( GameState* newState );

	// Set state and reset stack
	static void SetStateAndResetStack( GameState* newState );

	// Pop state
	static void PopState();

	// Update current state
	static void UpdateCurrentState( const s3e_uint64_t frameTime );

	// Render current state
	static void RenderCurrentState();

	// Set sub state
	static void SetSubState( const int state );

	// Get sub state ID
	static int GetSubStateID();

	// Get state ID
	static int GetStateID();

	// Returns current state
	static GameState* GetCurrentState();

	// Returns previous sub state id
	static int GetPrevSubStateID();

protected:
	// states management static vars
	static int			s_statesStackHead;
	static GameState*	s_statesStack[k_statesStackLen];
	static GameState*	s_currentState;
	static GameState*	s_stateToPop;
	static bool			s_resetStack;
	static int			s_stateID;
	static int			s_subState;
	static int			s_prevSubState;
};

#endif	// _GAMESTATE_H_