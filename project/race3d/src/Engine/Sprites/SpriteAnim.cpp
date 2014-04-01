#include "SpriteAnim.h"

//---------------------------------------------------------------------------------------

SpriteAnim::SpriteAnim( const char* texAtlasFileName, const int x, const int y, const int widthFrame, const int heightFrame, const int columnsCount, const int rowsCount, const uint8 celPeriod ) :
	Sprite( texAtlasFileName, x, y, widthFrame, heightFrame ),
	m_columnsCount( columnsCount ),
	m_rowsCount( rowsCount ),
	m_celPeriod( celPeriod )
{
}

//---------------------------------------------------------------------------------------

SpriteAnim::~SpriteAnim()
{
}

//---------------------------------------------------------------------------------------

void SpriteAnim::Init()
{
	Sprite::LoadTexture();

	ComputeAnimPos();
}

//---------------------------------------------------------------------------------------

void SpriteAnim::ComputeAnimPos()
{
	if( m_texture )
	{
		const int16 midx = static_cast<int16>( m_x );
		const int16 midy = static_cast<int16>( m_y );
	
		const int16 animXLeft = midx - m_width;
		const int16 animXRight = midx + m_width;
		const int16 animYTop = midy - m_height;
		const int16 animYBottom = midy + m_height;

		m_xy3[0] = CIwSVec2( animXLeft, animYTop );
		m_xy3[1] = CIwSVec2( animXLeft, animYBottom );
		m_xy3[2] = CIwSVec2( animXRight, animYTop );
		m_xy3[3] = CIwSVec2( animXRight, animYBottom );

		const float c = 1.0f / m_columnsCount;
		const float r = 1.0f / m_rowsCount;

		m_uvs[0] = CIwFVec2( 0, 0 );
		m_uvs[1] = CIwFVec2( 0, r );
		m_uvs[2] = CIwFVec2( c, 0 );
		m_uvs[3] = CIwFVec2( c, r );

		m_material.SetTexture( m_texture );
		m_material.CreateAnim();
		m_material.SetAnimCelW( static_cast<float>( m_width ) );
		m_material.SetAnimCelH( static_cast<float>( m_height ) );
		m_material.SetAnimCelPeriod( m_celPeriod );
	}
}

//---------------------------------------------------------------------------------------

void SpriteAnim::Render()
{
	if( m_texture )
	{
		IwGxSetMaterial( &m_material );
		IwGxSetUVStream( m_uvs );
		IwGxSetColStream( NULL );
		IwGxSetVertStreamScreenSpace( m_xy3, 4 );
		IwGxDrawPrims( IW_GX_QUAD_STRIP, NULL, 4 );
	}
}