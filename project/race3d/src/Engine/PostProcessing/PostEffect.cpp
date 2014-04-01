#include "PostEffect.h"

//---------------------------------------------------------------------------------------

PostEffect::PostEffect( const unsigned int id ) :
	m_id( id )
{
	const int height = IwGxGetScreenHeight();
	const int width = IwGxGetScreenWidth();

	m_surface = new CIwGxSurface();
	m_surface->CreateSurface( NULL, width, height, CIwGxSurface::EXACT_MATCH_F );
}

//---------------------------------------------------------------------------------------

PostEffect::~PostEffect()
{
	delete m_surface;
}