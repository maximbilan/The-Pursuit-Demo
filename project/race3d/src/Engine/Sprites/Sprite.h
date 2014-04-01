#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "IwTexture.h"
#include "IwMaterial.h"
#include "IwGeomSVec2.h"

//-----------------------------------------------------------------------------------------
// Sprite
//-----------------------------------------------------------------------------------------
class Sprite
{
public:
	Sprite( const char* textureFileName, const int x, const int y, const int width, const int height );
	virtual ~Sprite();

	// Initialization
	virtual void Init();
	
	// Render
	virtual void Render();

protected:
	// Loading the texture
	void LoadTexture();
	
	// Compute positions
	void ComputePos();

protected:
	const char*	m_textureFileName;
	int			m_x;
	int			m_y;
	int			m_width;
	int			m_height;

	CIwTexture*	m_texture;
	CIwMaterial	m_material;
	CIwSVec2	m_xy3[4];
	CIwFVec2	m_uvs[4];
};

#endif	// _SPRITE_H_