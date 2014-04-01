#include "LightManager.h"

//-----------------------------------------------------------------------------------------

void LightManager::SetAmbient( const CIwColour color )
{
	IwGxSetLightType( LS_ID_AMBIENT, IW_GX_LIGHT_AMBIENT );
	IwGxSetLightCol( LS_ID_AMBIENT, &color );
}

//-----------------------------------------------------------------------------------------

void LightManager::SetDiffuse( const CIwColour color, const CIwColour specularColor, const CIwFVec3 position, const CIwFVec3 direction )
{
	IwGxSetLightType( LS_ID_DIFFUSE, IW_GX_LIGHT_DIFFUSE );
	IwGxSetLightCol( LS_ID_DIFFUSE, &color );
	IwGxSetLightDirn( LS_ID_DIFFUSE, &direction );
	IwGxSetLightPos( LS_ID_DIFFUSE, &position );
	IwGxSetLightSpecularCol( LS_ID_DIFFUSE, &specularColor );
}