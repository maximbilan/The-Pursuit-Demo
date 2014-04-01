#include "Sprite.h"

//---------------------------------------------------------------------------------------

Sprite::Sprite( const char* textureFileName, const int x, const int y, const int width, const int height ) :
	m_textureFileName( textureFileName ),
	m_x( x ),
	m_y( y ),
	m_width( width ),
	m_height( height ),
	m_texture( 0 )
{
}

//---------------------------------------------------------------------------------------

Sprite::~Sprite()
{
	if( m_texture )
	{
		delete m_texture;
		m_texture = NULL;
	}
}

//---------------------------------------------------------------------------------------

void Sprite::Init()
{
	LoadTexture();
	ComputePos();
}

//---------------------------------------------------------------------------------------

void Sprite::LoadTexture()
{
	m_texture = new CIwTexture;
	m_texture->LoadFromFile( m_textureFileName );
	m_texture->Upload();
}

//---------------------------------------------------------------------------------------

void Sprite::ComputePos()
{
	if( m_texture )
	{
		const int x1 = m_x;
		const int x2 = m_x + m_width;
		const int y1 = m_y;
		const int y2 = m_y + m_height;

		m_xy3[ 0 ].x = x1;
		m_xy3[ 1 ].x = x1;
		m_xy3[ 2 ].x = x2;
		m_xy3[ 3 ].x = x2;

		m_xy3[ 0 ].y = y1;
		m_xy3[ 1 ].y = y2;
		m_xy3[ 2 ].y = y2;
		m_xy3[ 3 ].y = y1;
		 
		m_uvs[ 0 ] = CIwFVec2( 0, 0 );
		m_uvs[ 1 ] = CIwFVec2( 0, 1 );
		m_uvs[ 2 ] = CIwFVec2( 1, 1 );
		m_uvs[ 3 ] = CIwFVec2( 1, 0 );
	}
}

//---------------------------------------------------------------------------------------

void Sprite::Render()
{
	if( m_texture )
	{
		m_material.SetModulateMode( CIwMaterial::MODULATE_NONE );
		m_material.SetTexture( m_texture );
		IwGxSetMaterial( &m_material );

		IwGxSetVertStreamScreenSpace( m_xy3, 4 );
		IwGxSetUVStream( m_uvs );

		IwGxDrawPrims( IW_GX_QUAD_LIST, NULL, 4 );
	}
}