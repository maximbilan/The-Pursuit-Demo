#ifndef _LOADINGSCREEN_H_
#define _LOADINGSCREEN_H_

#include "IwTexture.h"
#include "IwMaterial.h"
#include "IwGeomSVec2.h"

class Sprite;
class SpriteAnim;

//-----------------------------------------------------------------------------------
// Loading screen - render loading screen
//-----------------------------------------------------------------------------------
class LoadingScreen
{
private:
	static const char*	s_backgroundTexture;
	static const char*	s_animationLoadingTexture;

public:
	LoadingScreen();
	~LoadingScreen();

	// Render
	void Render();

private:
	Sprite*		m_background;
	SpriteAnim*	m_circleAnim;
};

#endif	// _LOADINGSCREEN_H_