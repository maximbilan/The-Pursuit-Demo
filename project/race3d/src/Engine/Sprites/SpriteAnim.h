#ifndef _SPRITEANIM_H_
#define _SPRITEANIM_H_

#include "Sprite.h"

//---------------------------------------------------------------------------------------
// SpriteAnim - sprite with animation
//---------------------------------------------------------------------------------------
class SpriteAnim : public Sprite
{
public:
	SpriteAnim( const char* texAtlasFileName, const int x, const int y, const int widthFrame, const int heightFrame, const int columnsCount, const int rowsCount, const uint8 celPeriod );
	~SpriteAnim();

	// Initialization
	void Init();

	// render
	void Render();

private:
	// Compute animation positions
	void ComputeAnimPos();

private:
	int		m_columnsCount;
	int		m_rowsCount;

	uint8	m_celPeriod;
};

#endif	// _SPRITEANIM_H_