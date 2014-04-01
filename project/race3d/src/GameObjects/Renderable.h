#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

class LightManager;
class ShaderManager;

//------------------------------------------------------------
// Renderable - interface for renderable entities
//------------------------------------------------------------
class Renderable
{
public:
	Renderable();
	virtual ~Renderable();

	// Render function
	virtual void Render() = 0;
		
	// Get amount of polygons
	virtual unsigned int GetPolyCount() = 0;
		
	// Check is visible
	bool IsVisible() const;
		
	// Set visibility
	void SetVisible( const bool visible );

protected:
	bool			m_isVisible;
	LightManager*	m_lightManager;
	ShaderManager*	m_shaderManager;
};

//-----------------------------------------------------------

inline bool Renderable::IsVisible() const
{
	return m_isVisible;
}

//-----------------------------------------------------------

inline void Renderable::SetVisible( const bool visible )
{
	m_isVisible = visible;
}

#endif	//	_RENDERABLE_H_