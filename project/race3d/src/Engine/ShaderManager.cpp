#include "ShaderManager.h"

//-------------------------------------------------------------------------------------

const char* ShaderManager::s_shaderNames[] = { "lambert", "cartoon", "sepia", "black_white" };
const char* ShaderManager::s_shaderTechiqueResType = "CIwGxShaderTechnique";
const char* ShaderManager::s_resGroupName = "shaders.group";

//-------------------------------------------------------------------------------------

void ShaderManager::Init()
{
}

//-------------------------------------------------------------------------------------

void ShaderManager::Load()
{
	m_resGroup = IwGetResManager()->LoadGroup( s_resGroupName );

	if( m_resGroup )
	{
		for( int i = 0; i < TECHNIQUE_COUNT; ++i )
		{
			CIwGxShaderTechnique* technique = static_cast<CIwGxShaderTechnique*>( m_resGroup->GetResNamed( s_shaderNames[i], s_shaderTechiqueResType ) );
			if( technique )
			{
				ShaderTechniquePair value( i, technique );
				m_techniques.insert( value );
			}
		}
	}
}

//-------------------------------------------------------------------------------------

void ShaderManager::Reload()
{
}

//-------------------------------------------------------------------------------------

void ShaderManager::Remove()
{
	IwGetResManager()->DestroyGroup( m_resGroup );
	m_techniques.clear();
}