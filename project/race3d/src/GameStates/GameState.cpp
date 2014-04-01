#include "GameState.h"
#include "s3e.h"

//-----------------------------------------------------------------------------------

int			GameState::s_statesStackHead = 0;
GameState*	GameState::s_statesStack[k_statesStackLen];
GameState*	GameState::s_currentState = NULL;
GameState*	GameState::s_stateToPop = NULL;
bool		GameState::s_resetStack = false;
int			GameState::s_stateID = 0;
int			GameState::s_subState = 0;
int			GameState::s_prevSubState = 0;

//-----------------------------------------------------------------------------------

GameState::GameState()
{
}

//-----------------------------------------------------------------------------------

GameState::~GameState()
{

}

//-----------------------------------------------------------------------------------

void GameState::InitStatesStack()
{
	s_currentState = NULL;
	s_statesStackHead = 0;
	s_resetStack = false;
	for( int iState = 0; iState < k_statesStackLen; iState++ )
	{
		s_statesStack[iState] = NULL;
	}
}

//-----------------------------------------------------------------------------------

void GameState::ReleaseStatesStack()
{
	// release current state
	if( s_currentState )
	{
		delete s_currentState;
	}

	// release state to pop
	if( s_stateToPop )
	{
		delete s_stateToPop;
	}
	
	// release stack
	for( int iState = 0; iState < k_statesStackLen; iState++ )
	{
		if( s_statesStack[iState] )
		{
			delete s_statesStack[iState];
		}
	}	
}

//-----------------------------------------------------------------------------------

void GameState::SetState( GameState* newState )
{
	if( s_currentState )
	{
		if( s_statesStack[s_statesStackHead] )
		{
			// delete old state if the stack was full
			delete s_statesStack[s_statesStackHead];
			s_statesStack[s_statesStackHead] = NULL;
		}
		
		s_statesStack[s_statesStackHead] = s_currentState;

		if( s_statesStackHead < k_statesStackLen )
		{
			s_statesStackHead++;		
		}
		else
		{
			s_statesStackHead = 0;
		}
	}	
		
	s_currentState = newState;
}

//-----------------------------------------------------------------------------------

void GameState::SetStateAndResetStack( GameState* newState )
{
	SetState( newState );
	s_resetStack = true;
}

//-----------------------------------------------------------------------------------

void GameState::PopState()
{
	int prevState = s_statesStackHead - 1;
	if( prevState < 0 )
	{
		prevState = k_statesStackLen - 1;
	}

	if( s_statesStack[prevState] && ( s_stateToPop == NULL ) )
	{
		s_statesStackHead = prevState;
		s_stateToPop = s_currentState;
		s_currentState = s_statesStack[s_statesStackHead];
		s_statesStack[s_statesStackHead] = NULL;

		s_currentState->ResumeState();
	}
}

//-----------------------------------------------------------------------------------

void GameState::UpdateCurrentState( const s3e_uint64_t frameTime )
{
	if( s_currentState )
	{
		// update state
		s_currentState->UpdateState( frameTime );

		// destroy last state pop from the stack
		if( s_stateToPop )
		{
			delete s_stateToPop;
			s_stateToPop = NULL;
		}

		// destroy all the stack states
		if( s_resetStack )
		{
			s_statesStackHead--;
			if( s_statesStackHead < 0 )
			{
				s_statesStackHead = k_statesStackLen - 1;
			}

			while( s_statesStack[s_statesStackHead] )
			{
				delete s_statesStack[s_statesStackHead];
				s_statesStack[s_statesStackHead] = NULL;

				s_statesStackHead--;
				if( s_statesStackHead < 0 )
				{
					s_statesStackHead = k_statesStackLen - 1;
				}
			}

			s_statesStackHead = 0;
			s_resetStack = false;
		}
	}
}

//-----------------------------------------------------------------------------------

void GameState::RenderCurrentState()
{
	if( s_currentState )
	{
		s_currentState->RenderState();
	}
}

//-----------------------------------------------------------------------------------

GameState * GameState::GetCurrentState()
{
	return s_currentState;
}

//-----------------------------------------------------------------------------------

int  GameState::GetStateID()
{
	return s_stateID;
}

//-----------------------------------------------------------------------------------

void GameState::SetSubState( const int state )
{
	s_prevSubState = s_subState;
	s_subState = state;
}

//-----------------------------------------------------------------------------------

int GameState::GetSubStateID()
{
	return s_subState;
}

//-----------------------------------------------------------------------------------

int GameState::GetPrevSubStateID()
{
	return s_prevSubState;
}