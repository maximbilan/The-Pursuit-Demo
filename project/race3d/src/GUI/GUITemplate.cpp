//-----------------------------------------------------------------------------
// GUITemplate.cpp
// Helper class that allows a block of GUI elements to be easily cloned
// in other parts of the GUI - e.g. a button backing image
//-----------------------------------------------------------------------------

#include "GUITemplate.h"

// Library includes
#include "IwClassFactory.h"

// Project includes
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY( GUITemplate );

//-----------------------------------------------------------------------------
// Allows a GUIElement to initialise itself from a GUITemplate
//-----------------------------------------------------------------------------
void GUITemplate::CloneTemplate( GUIElement* apElement )
{
	// First, check that there is only one child element in template, otherwise
	// this will make no sense.
	IwAssertMsg( GUI, m_Children.GetSize() == 1,
		( "Attempt to use template %s in element %s, but this template contains more than one child",
		DebugGetName(), apElement->DebugGetName() ) );

	// Next, make sure that single child is of the same type as the apElement
	GUIElement* lpChild = reinterpret_cast<GUIElement*>( m_Children[0] );
	IwAssertMsg( GUI, !strcmp( lpChild->GetClassName(), apElement->GetClassName() ),
		( "Element %s attempting to use template %s, but they are of different types",
		apElement->DebugGetName(), DebugGetName() ) );

	apElement->SetName( lpChild->DebugGetName() );
	apElement->Clone( lpChild );
}

//-----------------------------------------------------------------------------
// Allows a GUIElement to add one or more children defined by a GUITemplate
//-----------------------------------------------------------------------------
void GUITemplate::CloneTemplateChildren( GUIElement* apElement )
{
	IwAssertMsg( GUI, TestFlags( GF_FRAME ),
		( "Attempt to add GUITemplate %s to non frame element %s", DebugGetName(), apElement->DebugGetName() ) );

	GUIFrame* lpFrame = reinterpret_cast<GUIFrame*>( apElement );
	lpFrame->CloneChildren( this );
}