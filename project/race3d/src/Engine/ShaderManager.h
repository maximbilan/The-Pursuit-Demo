#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <map>
#include "IwGraphics.h"
#include "../Common/Singleton.h"

//-----------------------------------------------------------------------------------------

typedef std::map<unsigned int, CIwGxShaderTechnique*> ShaderTechniquesMap;
typedef std::pair<unsigned int, CIwGxShaderTechnique*> ShaderTechniquePair;

//-----------------------------------------------------------------------------------------
// ShaderManager - class for managment of the shaders
//-----------------------------------------------------------------------------------------
class ShaderManager : public Singleton<ShaderManager>
{
public:
	enum
	{
		TECHNIQUE_LIGHTING_LAMBERT,
		TECHNIQUE_TOON_SHADING,
		TECHNIQUE_PE_SEPIA,
		TECHNIQUE_PE_BLACK_WHITE,

		TECHNIQUE_COUNT
	};

private:
	static const char* s_shaderNames[TECHNIQUE_COUNT];
	static const char* s_shaderTechiqueResType;
	static const char* s_resGroupName;

public:
	// Initialization
	void Init();

	// Loads shaders
	void Load();

	// Reload shaders
	void Reload();

	// Remove the instance
	void Remove();

	CIwGxShaderTechnique* GetShaderTechnique( const unsigned int id ) const;

protected:
	ShaderManager() {}
	friend class Singleton<ShaderManager>;

private:
	ShaderTechniquesMap	m_techniques;

	CIwResGroup*		m_resGroup;
};

//-----------------------------------------------------------------------------------------

inline CIwGxShaderTechnique* ShaderManager::GetShaderTechnique( const unsigned int id ) const
{
	if( id >= 0 && id < TECHNIQUE_COUNT )
	{
		ShaderTechniquesMap::const_iterator findIt = m_techniques.find( id );
		if( findIt != m_techniques.end() )
		{
			return ( *findIt ).second;
		}
	}

	return NULL;
}

#endif	// _SHADER_MANAGER_H_