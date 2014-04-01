#ifndef _POST_EFFECT_H_
#define _POST_EFFECT_H_

#include "IwGraphics.h"

//---------------------------------------------------------------------------------------
// Post effect - post processing effect
//---------------------------------------------------------------------------------------
class PostEffect
{
public:
	PostEffect( const unsigned int id );
	~PostEffect();

	// Returns id
	unsigned int GetID() const;

	// Set technique
	void SetTechnique( CIwGxShaderTechnique* t );

	// Returns technique
	CIwGxShaderTechnique* GetTechnique() const;

	// Returns the surface
	CIwGxSurface* GetSurface() const;

private:
	unsigned int			m_id;

	CIwGxShaderTechnique*	m_technique;
	CIwGxSurface*			m_surface;
};

//---------------------------------------------------------------------------------------

inline unsigned int PostEffect::GetID() const
{
	return m_id;
}

//---------------------------------------------------------------------------------------

inline void PostEffect::SetTechnique( CIwGxShaderTechnique* t )
{
	m_technique = t;
}

//---------------------------------------------------------------------------------------

inline CIwGxShaderTechnique* PostEffect::GetTechnique() const
{
	return m_technique;
}

//---------------------------------------------------------------------------------------

inline CIwGxSurface* PostEffect::GetSurface() const
{
	return m_surface;
}

#endif	//	_POST_EFFECT_H_