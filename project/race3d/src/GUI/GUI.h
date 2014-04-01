//-----------------------------------------------------------------------------
// GUI.h
// Set up/tear down for GUI module
//-----------------------------------------------------------------------------

#ifndef GUI_H
#define GUI_H

// Initialise GUI Module
void GUIInit();

// Release GUI Modules
void GUITerminate();

// Update GUI
void GUIUpdate( int32 aTimeMS );

// Render GUI
void GUIRender();

// Reset texts
void GUIResetTexts();

#endif