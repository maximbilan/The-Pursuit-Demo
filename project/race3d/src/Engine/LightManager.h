#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

#include <IwColour.h>
#include <IwGeomFVec3.h>
#include <IwGx.h>
#include "../Common/Singleton.h"

//-----------------------------------------------------------------------------------------
// Light Manager - class for managment of the light sources
//-----------------------------------------------------------------------------------------
class LightManager : public Singleton<LightManager>
{
private:
	enum ELightSourceID
	{
		LS_ID_AMBIENT,
		LS_ID_DIFFUSE
	};

public:
	// Sets ambient lighting
	void SetAmbient( const CIwColour color );

	// Returns ambient color
	CIwColour& GetAmbientColor() const;

	// Sets diffuse lighting
	void SetDiffuse( const CIwColour color, const CIwColour specularColor, const CIwFVec3 position, const CIwFVec3 direction );

	// Returns diffuse color
	CIwColour& GetDiffuseColor() const;

	// Returns diffuse position
	CIwFVec3& GetDiffusePosition() const;

	// Returns diffuse direction
	CIwFVec3& GetDiffuseDirection() const;

protected:
	LightManager() {}
	friend class Singleton<LightManager>;
};

//-----------------------------------------------------------------------------------------

inline CIwColour& LightManager::GetAmbientColor() const
{
	return IwGxGetLightCol( LS_ID_AMBIENT );
}

//-----------------------------------------------------------------------------------------

inline CIwColour& LightManager::GetDiffuseColor() const
{
	return IwGxGetLightCol( LS_ID_DIFFUSE );
}

//-----------------------------------------------------------------------------------------

inline CIwFVec3& LightManager::GetDiffusePosition() const
{
	return IwGxGetLightPos( LS_ID_DIFFUSE );
}

//-----------------------------------------------------------------------------------------

inline CIwFVec3& LightManager::GetDiffuseDirection() const
{
	return IwGxGetLightDirn( LS_ID_DIFFUSE );
}

#endif	//	_LIGHT_MANAGER_H_