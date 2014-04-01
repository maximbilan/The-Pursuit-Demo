#include "Renderable.h"

#include "../Engine/LightManager.h"
#include "../Engine/ShaderManager.h"

//------------------------------------------------------------

Renderable::Renderable() :
	m_isVisible( false )
{
	m_lightManager = LightManager::GetInstance();
	m_shaderManager = ShaderManager::GetInstance();
}

//------------------------------------------------------------

Renderable::~Renderable()
{
	m_shaderManager->FreeInstance();
	m_lightManager->FreeInstance();
}