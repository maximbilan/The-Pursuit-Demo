//-----------------------------------------------------------------------------
// GUI.cpp
// Set up/tear down for GUI module
//-----------------------------------------------------------------------------

// Marmalade includes
#include "s3ePointer.h"

// Project includes
#include "GUIEventManager.h"
#include "GUIManager.h"

//-----------------------------------------------------------------------------
// Initialise GUI Module
//-----------------------------------------------------------------------------
void GUIInit()
{
	IW_CLASS_REGISTER( GUIBorder );
	IW_CLASS_REGISTER( GUIBitmap );
	IW_CLASS_REGISTER( GUIButton );
	IW_CLASS_REGISTER( GUIColour );
	IW_CLASS_REGISTER( GUIFrame );
	IW_CLASS_REGISTER( GUILabel );
	IW_CLASS_REGISTER( GUITemplate );

	new GUIEventManager();
	new GUIManager();
}

//-----------------------------------------------------------------------------
// Release GUI Modules
//-----------------------------------------------------------------------------
void GUITerminate()
{
	delete GetGUIEventManager();
	delete GetGUIManager();
}

//-----------------------------------------------------------------------------
// Update GUI
//-----------------------------------------------------------------------------
void GUIUpdate( int32 aTimeMS )
{
	GetGUIManager()->Update( aTimeMS );
}

//-----------------------------------------------------------------------------
// Render GUI
//-----------------------------------------------------------------------------
void GUIRender()
{
	GetGUIManager()->Render();
}

//-----------------------------------------------------------------------------
// Reset texts
//-----------------------------------------------------------------------------
void GUIResetTexts()
{
	GetGUIManager()->ResetLabelStrings();
}
