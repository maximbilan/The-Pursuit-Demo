#include "PostProcessing.h"
#include "PostEffect.h"
#include "../ShaderManager.h"

//---------------------------------------------------------------------------------------

void PostProcessing::Init()
{
	m_state = PP_DISABLE;
	m_shaderManager = ShaderManager::GetInstance();

	const int height = IwGxGetScreenHeight();
	const int width = IwGxGetScreenWidth();

	m_offScreenSurface =  new CIwGxSurface();
	m_offScreenSurface->CreateSurface( NULL, width, height, CIwGxSurface::EXACT_MATCH_F );
}

//---------------------------------------------------------------------------------------

void PostProcessing::LoadShaders()
{
	CreatePostEffect( PP_SEPIA, ShaderManager::TECHNIQUE_PE_SEPIA );
	CreatePostEffect( PP_BLACK_WHITE, ShaderManager::TECHNIQUE_PE_BLACK_WHITE );
}

//---------------------------------------------------------------------------------------

void PostProcessing::PreRender()
{
	if( m_state != PP_DISABLE )
	{
		m_offScreenSurface->MakeCurrent();
	}
}

//---------------------------------------------------------------------------------------

void PostProcessing::PostRender()
{
	if( m_state != PP_DISABLE )
	{
		switch( m_state )
		{
		case PP_SEPIA:
		case PP_BLACK_WHITE:
			{
				PostEffect* pe = m_effects[m_state];
				RenderPass( m_offScreenSurface, pe->GetSurface(), pe->GetTechnique() );
				RenderOnDisplay( pe->GetSurface() );
			};
			break;
		}
	}
}

//---------------------------------------------------------------------------------------

void PostProcessing::RenderPass( CIwGxSurface* src, CIwGxSurface* dst, CIwGxShaderTechnique* shader )
{
	dst->MakeCurrent();

	IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetTexture( src->GetTexture() );
	pMat->SetShaderTechnique( shader );
	IwGxSetMaterial( pMat );

	const int16 width = static_cast<int16>( IwGxGetDisplayWidth() );
	const int16 height = static_cast<int16>( IwGxGetDisplayHeight() );

	CIwSVec2 wh( width, height );
	CIwSVec2 xy( 0, 0 );
	CIwFVec2 uvxy( 0, 0 );
	CIwFVec2 uvwh( dst->GetClientUVExtentFloat() );
	IwGxDrawRectScreenSpace( &xy, &wh, &uvxy, &uvwh );
}

//---------------------------------------------------------------------------------------

void PostProcessing::RenderOnDisplay( CIwGxSurface* src, CIwGxShaderTechnique* shader )
{
	CIwGxSurface::MakeDisplayCurrent();

	IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetTexture( src->GetTexture() );
	if( shader )
	{
		pMat->SetShaderTechnique( shader );
	}
	IwGxSetMaterial( pMat );
	
	const int16 width = static_cast<int16>( IwGxGetDisplayWidth() );
	const int16 height = static_cast<int16>( IwGxGetDisplayHeight() );

	CIwSVec2 wh( width, height );
	CIwSVec2 xy( 0, 0 );
	CIwFVec2 uvxy( 0, 0 );
	CIwFVec2 uvwh( src->GetClientUVExtentFloat() );
	IwGxDrawRectScreenSpace( &xy, &wh, &uvxy, &uvwh );
}

//---------------------------------------------------------------------------------------

void PostProcessing::Remove()
{
	delete m_offScreenSurface;

	for( PostEffectIterator it = m_effects.begin(), end = m_effects.end(); it != end; ++it )
	{
		delete ( *it );
	}
	m_shaderManager->FreeInstance();
}

//---------------------------------------------------------------------------------------

void PostProcessing::CreatePostEffect( const unsigned int id, const unsigned int shaderId )
{
	PostEffect* postEffect = new PostEffect( id );
	CIwGxShaderTechnique* technique = m_shaderManager->GetShaderTechnique( shaderId );
	if( technique )
	{
		postEffect->SetTechnique( technique );
	}
	m_effects.push_back( postEffect );
}