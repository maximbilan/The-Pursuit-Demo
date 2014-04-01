//-----------------------------------------------------------------------------
// GUITemplate.h
// Helper class that allows a block of GUI elements to be easily cloned
// in other parts of the GUI - e.g. a button backing image
//-----------------------------------------------------------------------------

#ifndef GUITEMPLATE_H
#define GUITEMPLATE_H

#include "GUIFrame.h"

class GUITemplate : public GUIFrame
{
public:
	IW_MANAGED_DECLARE( GUITemplate );

	// Allows a GUIElement to initialise itself from a GUITemplate
	void CloneTemplate( GUIElement* apElement );

	// Allows a GUIElement to add one or more children defined by a GUITemplate
	void CloneTemplateChildren( GUIElement* apElement );
};

#endif // GUITEMPLATE_H