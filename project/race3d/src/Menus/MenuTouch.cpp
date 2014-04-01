#include "MenuTouch.h"
#include "../Engine/Input/TouchManager.h"

//---------------------------------------------------------------------------------

MenuTouch::MenuTouch()
{
	m_touchManager = TouchManager::GetInstance();
}

//---------------------------------------------------------------------------------

MenuTouch::~MenuTouch()
{
	m_touchManager->FreeInstance();
}

//---------------------------------------------------------------------------------

bool MenuTouch::UpdateTouches()
{
	bool wasTouches = false;

	if( m_touchManager->GetTouchCount() != 0 )
	{
		if( m_touchManager->IsMultiTouch() )
		{
			for( int i = 0; i < m_touchManager->GetTouchCount(); ++i )
			{
				Touch* touch = m_touchManager->GetTouch( i );
				if( touch != NULL )
				{
					HandleTouches( touch );
					wasTouches = true;
				}
			}
		}
		else
		{
			Touch* touch = m_touchManager->GetTouch( 0 );
			if( touch != NULL )
			{
				HandleTouches( touch );
				wasTouches = true;
			}
		}
	}

	return wasTouches;
}

//---------------------------------------------------------------------------------

void MenuTouch::HandleTouches( const Touch* touch )
{
}