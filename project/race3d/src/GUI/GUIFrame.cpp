//-----------------------------------------------------------------------------
// GUIFrame.cpp
// GUI element that can contain other GUI elements to ease screen layout
//-----------------------------------------------------------------------------

#include "GUIFrame.h"

// Project includes
#include "GUIManager.h"

IW_MANAGED_IMPLEMENT_FACTORY( GUIFrame );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GUIFrame::GUIFrame()
{
	IW_CALLSTACK( "GUIFrame::GUIFrame" );
	SetFlags( GUIElement::GF_FRAME );
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
GUIFrame::~GUIFrame()
{
	IW_CALLSTACK( "GUIFrame::~GUIFrame" );
	m_Children.Delete();
}

//-----------------------------------------------------------------------------
// Serialise to/from binary file
//-----------------------------------------------------------------------------
void GUIFrame::Serialise()
{
	IW_CALLSTACK( "GUIFrame::Serialise" );

	GUIElement::Serialise();

	// Serialise out the list of child elements
	m_Children.Serialise();
}

//-----------------------------------------------------------------------------
// Lookup resource references
//-----------------------------------------------------------------------------
void GUIFrame::Resolve()
{
	IW_CALLSTACK( "GUIFrame::Resolve" );

	GUIElement::Resolve();

	// Make sure all the children know who the parent is!
	IW_MANAGED_LIST_ITERATE( m_Children, itChild )
	{
		GUIElement* lpElement = reinterpret_cast<GUIElement*>( *itChild );
		lpElement->SetParent( this );
		lpElement->Resolve();
	}
}

//-----------------------------------------------------------------------------
// Clone this element
//-----------------------------------------------------------------------------
void GUIFrame::Clone( GUIElement* apElement )
{
	IW_CALLSTACK( "GUIFrame::Clone" );

	GUIElement::Clone( apElement );
	GUIFrame* lpFrame = reinterpret_cast<GUIFrame*>( apElement );
	CloneChildren( lpFrame );
}

//-----------------------------------------------------------------------------
// GUIFrame is responsible for updating all it's children
//-----------------------------------------------------------------------------
void GUIFrame::Update( int32 aTick )
{
	IW_CALLSTACK( "GUIFrame::Update" );

	GUIElement::Update( aTick );

	if( !IsEnabled() )
		return;

	IW_MANAGED_LIST_ITERATE( m_Children, itChild )
	{
		GUIElement* lpElement = reinterpret_cast<GUIElement*>( *itChild );
		lpElement->Update( aTick );
	}
}

//-----------------------------------------------------------------------------
// GUIFrame is responsible for rendering all it's children
//-----------------------------------------------------------------------------
void GUIFrame::Render()
{
	IW_CALLSTACK( "GUIFrame::Render" );

	GUIElement::Render();

	IW_MANAGED_LIST_ITERATE( m_Children, itChild )
	{
		GUIElement* lpElement = reinterpret_cast<GUIElement*>( *itChild );
		if( lpElement->IsVisible() )
			lpElement->Render();
	}
}

//-----------------------------------------------------------------------------
// Add child element to this frame
//-----------------------------------------------------------------------------
void GUIFrame::AddChild( GUIElement* apElement )
{
	IW_CALLSTACK( "GUIFrame::AddChild" );
	m_Children.Add( apElement );
}


//-----------------------------------------------------------------------------
// Find a child element by its name
//-----------------------------------------------------------------------------
GUIElement* GUIFrame::FindChildByName( const char* apName ) const
{
	GUIElement* lpElement = reinterpret_cast<GUIElement*>( m_Children.GetObjNamed( apName ) );
	return lpElement;
}

//-----------------------------------------------------------------------------
// Find a child element by its hash value
//-----------------------------------------------------------------------------
GUIElement* GUIFrame::FindChildByHash( uint32 aHash ) const
{
	GUIElement* lpElement = reinterpret_cast<GUIElement*>( m_Children.GetObjHashed( aHash ) );
	return lpElement;
}

//-----------------------------------------------------------------------------
// Clone the m_Children list of this element
//-----------------------------------------------------------------------------
void GUIFrame::CloneChildren( GUIFrame* apFrameSource )
{
	IW_CALLSTACK( "GUIFrame::CloneChildren" );

	IW_MANAGED_LIST_ITERATE( apFrameSource->m_Children, lpChildIt )
	{
		GUIElement* lpChild = reinterpret_cast<GUIElement*>( *lpChildIt );
		GUIElement* lpNewInstance = reinterpret_cast<GUIElement*>( IwClassFactoryCreate( lpChild->GetClassName() ) );
		lpNewInstance->SetName( lpChild->DebugGetName() );
		lpNewInstance->Clone( lpChild );
		AddChild( lpNewInstance );
	}
}