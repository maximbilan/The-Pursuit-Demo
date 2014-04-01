#ifndef _POST_PROCESSING_H_
#define _POST_PROCESSING_H_

#include "../../Common/Singleton.h"
#include "IwGraphics.h"
#include <vector>

class PostEffect;
class ShaderManager;

//---------------------------------------------------------------------------------------

typedef std::vector<PostEffect*> PostEffectArray;
typedef std::vector<PostEffect*>::const_iterator PostEffectIterator;

//---------------------------------------------------------------------------------------
// Post Processing manager
//---------------------------------------------------------------------------------------
class PostProcessing : public Singleton<PostProcessing>
{
public:
	enum POST_PROCESSING_STATE
	{
		PP_DISABLE = -1,
		PP_SEPIA,
		PP_BLACK_WHITE,

		PP_COUNT
	};

public:
	// Initialization
	void Init();

	// Load shaders
	void LoadShaders();

	// Pre render function
	void PreRender();

	// Post render function
	void PostRender();

	// Remove the instance
	void Remove();

	// Set state
	void SetState( const int id );

private:
	// Creates post effect
	void CreatePostEffect( const unsigned int id, const unsigned int shaderId );

	// Render pass
	void RenderPass( CIwGxSurface* src, CIwGxSurface* dst, CIwGxShaderTechnique* shader );

	// Render on display
	void RenderOnDisplay( CIwGxSurface* src, CIwGxShaderTechnique* shader = NULL );

protected:
	PostProcessing() {}
	friend class Singleton<PostProcessing>;

private:
	int				m_state;
	PostEffectArray m_effects;
	CIwGxSurface*	m_offScreenSurface;

	ShaderManager*	m_shaderManager;
};

//---------------------------------------------------------------------------------------

inline void PostProcessing::SetState( const int id )
{
	m_state = id;
}

#endif	//	_POST_PROCESSING_H_