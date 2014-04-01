#include "LoadingScreen.h"

#include "IwGx.h"

#include "../Engine/Sprites/Sprite.h"
#include "../Engine/Sprites/SpriteAnim.h"

//-----------------------------------------------------------------------------------

const char* LoadingScreen::s_backgroundTexture			= "ui/textures/background.png";
const char* LoadingScreen::s_animationLoadingTexture	= "ui/textures/loading_anim_32.png";

//-----------------------------------------------------------------------------------

LoadingScreen::LoadingScreen()
{
	m_background = new Sprite( s_backgroundTexture, 0, 0, IwGxGetScreenWidth(), IwGxGetScreenHeight() );
	m_background->Init();

	const int16 midx = static_cast<int16>( IwGxGetScreenWidth() * 0.5 );
	const int16 midy = static_cast<int16>( IwGxGetScreenHeight() - 75 );
	m_circleAnim = new SpriteAnim( s_animationLoadingTexture, midx, midy, 32, 32, 4, 2, 3 );
	m_circleAnim->Init();
}

//-----------------------------------------------------------------------------------

LoadingScreen::~LoadingScreen()
{
	delete m_background;
	delete m_circleAnim;
}

//-----------------------------------------------------------------------------------

void LoadingScreen::Render()
{
	m_background->Render();
	m_circleAnim->Render();
}