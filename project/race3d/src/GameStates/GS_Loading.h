#ifndef _GS_LOADING_H_
#define _GS_LOADING_H_

#include "IwGx.h"
#include "IwGxPrint.h"
#include "IwMaterial.h"
#include "IwTexture.h"

#include "GameState.h"

class LoadingScreen;

//-----------------------------------------------------------------------------------
// GS_Loading - gamestate for loading resources
//-----------------------------------------------------------------------------------
class GS_Loading : public GameState
{
private:
	static const int k_substate_none		= 0;
	static const int k_substate_start_init	= 1;
	static const int k_substate_level		= 2;

	static const uint32 s_renderCallbackPeriod;

public:
	GS_Loading();
	GS_Loading( const int modeID, const int levelID );
	virtual ~GS_Loading();

	// Resume state
	void ResumeState();

	// Update state
	void UpdateState( const s3e_uint64_t frameTime );

	// Render state
	void RenderState();

private:
	int				m_levelID;
	int				m_gameModeID;
	LoadingScreen*	m_loadingScreen;
};

#endif	// _GS_LOADING_H_