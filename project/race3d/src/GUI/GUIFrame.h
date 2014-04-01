//-----------------------------------------------------------------------------
// GUIFrame.h
// GUI element that can contain other GUI elements to ease screen layout
//-----------------------------------------------------------------------------

#ifndef GUIFRAME_H
#define GUIFRAME_H

// Library includes
#include "IwManagedList.h"

// Project includes
#include "GUIElement.h"

class GUIFrame : public GUIElement
{
public:
	IW_MANAGED_DECLARE( GUIFrame );
	GUIFrame();
	virtual ~GUIFrame();

	//-------------------------------------------------------------------------
	// Virtual overrides from CIwManaged
	//-------------------------------------------------------------------------
	virtual void Serialise();
	virtual void Resolve();

	//-------------------------------------------------------------------------
	// Virtual overrides from GUIElement
	//-------------------------------------------------------------------------
	virtual void Clone( GUIElement* apElement );
	virtual void Update( int32 aTick );
	virtual void Render();

	//-------------------------------------------------------------------------
	// Public methods
	//-------------------------------------------------------------------------
	// Add a child element to the frame
	void AddChild( GUIElement* apElement );

	// Find a child element by its name
	GUIElement* FindChildByName( const char* apName ) const;

	// Find a child element by its hash value
	GUIElement* FindChildByHash( uint32 aHash ) const;

	// Clone the m_Children list of this element
	void CloneChildren( GUIFrame* apFrameSource );

protected:
	//-------------------------------------------------------------------------
	// Protected members
	//-------------------------------------------------------------------------
	// The child elements of this frame
	CIwManagedList m_Children;
};

#endif
