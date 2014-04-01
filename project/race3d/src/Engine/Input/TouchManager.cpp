#include "TouchManager.h"

//-------------------------------------------------------------------------------------------

uint32 TouchManager::s_swipeTime = 300;
uint32 TouchManager::s_swipeDistance = 100;
int TouchManager::s_swipeAccuracy = 200;

//-------------------------------------------------------------------------------------------

void HandleMultiTouchButtonCB( s3ePointerTouchEvent* );
void HandleMultiTouchMotionCB( s3ePointerTouchMotionEvent* );
void HandleSingleTouchButtonCB( s3ePointerEvent* );
void HandleSingleTouchMotionCB( s3ePointerMotionEvent* );

//-------------------------------------------------------------------------------------------

bool TouchManager::Init()
{
	// Check to see if the device that we are running on supports the pointer
	m_available = s3ePointerGetInt( S3E_POINTER_AVAILABLE ) ? true : false;
	if( !m_available )
	{
		return false;	// No pointer support
	}

	// Clear out the touches array
	for( int t = 0; t < MAX_TOUCHES; ++t )
	{
		m_touches[t].active = false;
		m_touches[t].id = 0;
	}

	m_swipe = new Swipe();
	m_pinch = new Pinch();

	m_touchCount = 0;
	m_swipe->m_tracking = false;
	m_swipeDirection = SD_NONE;
	m_pinch->m_tracking = false;
	m_pinch->m_zoom = 0.0f;

	// Determine if the device supports multi-touch
	m_isMultiTouch = s3ePointerGetInt( S3E_POINTER_MULTI_TOUCH_AVAILABLE ) ? true : false;

	// For multi-touch devices we handle touch and motion events using different callbacks
	if( m_isMultiTouch )
	{
		s3ePointerRegister( S3E_POINTER_TOUCH_EVENT, (s3eCallback)HandleMultiTouchButtonCB, NULL );
		s3ePointerRegister( S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)HandleMultiTouchMotionCB, NULL );
	}
	else
	{
		s3ePointerRegister( S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleSingleTouchButtonCB, NULL );
		s3ePointerRegister( S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotionCB, NULL );
	}

	return true;
}

//-------------------------------------------------------------------------------------------

void TouchManager::Update()
{
	// Update the pointer if it is available
	if( m_available )
	{
		s3ePointerUpdate();
	}
}

//-------------------------------------------------------------------------------------------

void TouchManager::Remove()
{
	if( m_available )
	{
		// Unregister the pointer system callbacks
		if( m_isMultiTouch )
		{
			s3ePointerUnRegister( S3E_POINTER_TOUCH_EVENT, (s3eCallback)HandleMultiTouchButtonCB );
			s3ePointerUnRegister( S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)HandleMultiTouchMotionCB );
		}
		else
		{
			s3ePointerUnRegister( S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleSingleTouchButtonCB );
			s3ePointerUnRegister( S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotionCB );
		}
	}

	delete m_swipe;
	delete m_pinch;
}

//-------------------------------------------------------------------------------------------

int TouchManager::GetTouchCount() const
{
	if( !m_available )
	{
		return 0;
	}

	// Return the total number of active touches
	int count = 0;
	for( int t = 0; t < MAX_TOUCHES; ++t )
	{
		if( m_touches[t].active )
		{
			++count;
		}
	}

	return count;
}

//-------------------------------------------------------------------------------------------

void TouchManager::DetectGestureStart( const uint32 touchID )
{
	if( m_touchCount == 1 )
	{
		// First touch on screen. Might be the start of a swipe.
		m_swipe->m_tracking = true;
		m_swipe->m_touchID = touchID;
		m_swipe->m_startTime = static_cast<uint32>( s3eTimerGetMs() );
		m_swipe->m_delta.x = m_touches[touchID].x;
		m_swipe->m_delta.y = m_touches[touchID].y;
		m_swipeDirection = SD_NONE;

		// Could also be start of a pinch
		m_pinch->m_tracking = true;
		m_pinch->m_touchID1 = touchID;
		m_pinch->m_touchID2 = MAX_TOUCHES;
		m_pinch->m_initialDistance = 0.0f;
		m_pinch->m_zoom = 0.0f;
		m_pinch->m_delta.x = m_touches[touchID].x;
		m_pinch->m_delta.y = m_touches[touchID].y;
	}
	else if( m_touchCount == 2 )
	{
		// Second touch detected.  This will abandon the swipe.
		m_swipe->m_tracking = false;

		// This touch lets us start tracking a pinch, so long
		// as it is still active
		if( m_pinch->m_tracking )
		{
			m_pinch->m_touchID2 = touchID;
			m_pinch->m_delta.x -= m_touches[touchID].x;
			m_pinch->m_delta.y -= m_touches[touchID].y;
			m_pinch->m_initialDistance = static_cast<float>( m_pinch->m_delta.GetLength() );
			m_pinch->m_zoom = 1.0f;
		}
	}
	else if( m_touchCount == 3 )
	{
		// A third concurrent touch cancels the pinch too
		m_pinch->m_tracking = false;
	}
}

//-------------------------------------------------------------------------------------------

void TouchManager::DetectGestureEnd( const uint32 touchID )
{
	// Does this complete a swipe?
	if( m_swipe->m_tracking && ( m_swipe->m_touchID == touchID ) )
	{
		// Check time swipe took to make
		const uint32 lTime = static_cast<uint32>( s3eTimerGetMs() - m_swipe->m_startTime );
		if( lTime <= s_swipeTime )
		{
			// Quick enough to be a swipe - is it long enough?
			m_swipe->m_delta.x -= m_touches[touchID].x;
			m_swipe->m_delta.y -= m_touches[touchID].y;
			const uint32 lDistance = m_swipe->m_delta.GetLength();
			if( lDistance > s_swipeDistance )
			{
				// Normalise the delta
				m_swipe->m_delta.Normalise();

				// Dot product it with positive X axis and Y axis
				const int32 lDotProductX = m_swipe->m_delta.Dot( CIwVec2::g_AxisX );
				const int32 lDotProductY = m_swipe->m_delta.Dot( CIwVec2::g_AxisY );
#ifndef _DEBUG
				IwTrace( APP, ( "DPX = %d, DPY = %d", lDotProductX, lDotProductY ) );
#endif

				// Assume no swipe detected to start with
				m_swipeDirection = SD_NONE;

				// Compare the dot products to see if our direction vector
				// is close to either the X or Y axis.  If it is we've
				// found a swipe!
				if( lDotProductX > ( IW_GEOM_ONE - s_swipeAccuracy ) )
				{
					m_swipeDirection = SD_LEFT;
				}
				else if( lDotProductX < -( IW_GEOM_ONE - s_swipeAccuracy ) )
				{
					m_swipeDirection = SD_RIGHT;
				}

				if( lDotProductY > ( IW_GEOM_ONE - s_swipeAccuracy ) )
				{
					m_swipeDirection = SD_UP;
				}
				else if( lDotProductY < -( IW_GEOM_ONE - s_swipeAccuracy ) )
				{
					m_swipeDirection = SD_DOWN;
				}
			}
		}

		// Cancel the swipe tracking status
		m_swipe->m_tracking = false;
	}

	// Does it complete a pinch?
	if( m_pinch->m_tracking && ( ( m_pinch->m_touchID1 == touchID ) || ( m_pinch->m_touchID2 = touchID ) ) )
	{
		m_pinch->m_tracking = false;
	}
}

//-------------------------------------------------------------------------------------------

void TouchManager::DetectGestureUpdate( const uint32 touchID )
{
	// Are we updating a current swipe tracking attempt
	if( m_swipe->m_tracking && ( m_swipe->m_touchID == touchID ) )
	{
		// Has this swipe been tracked for too long?
		const uint32 lTime = static_cast< uint32 >( s3eTimerGetMs() - m_swipe->m_startTime );
		if( lTime > s_swipeTime )
		{
			m_swipe->m_tracking = false;
		}
	}

	// Try updating pinch
	if( m_pinch->m_tracking && ( ( m_pinch->m_touchID1 == touchID ) || ( m_pinch->m_touchID2 == touchID ) ) )
	{
		if( m_pinch->m_touchID2 != MAX_TOUCHES )
		{
			m_pinch->m_delta.x = m_touches[m_pinch->m_touchID1].x - m_touches[m_pinch->m_touchID2].x;
			m_pinch->m_delta.y = m_touches[m_pinch->m_touchID1].y - m_touches[m_pinch->m_touchID1].y;
			const uint32 lNewDistance = m_pinch->m_delta.GetLength();
			m_pinch->m_zoom = static_cast<float>( lNewDistance / m_pinch->m_initialDistance );
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HandleMultiTouchButtonCB - For multitouch devices the system will call this callback when the user touches the screen. This callback is called once for each screen touch
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HandleMultiTouchButtonCB( s3ePointerTouchEvent* event )
{
	TouchManager* touchManager = TouchManager::GetInstance();

	if (event->m_Pressed )
	{
		// Left mouse button or touch screen pressed
		Touch* touch = touchManager->GetTouch( event->m_TouchID );
		touch->active = true;
		touch->x = event->m_x;
		touch->y = event->m_y;
		touchManager->IncreaseTouchCount();

		// See if this might indicate the beginnings of a gesture
		touchManager->DetectGestureStart( event->m_TouchID );
	}
	else
	{
		// Left mouse button or touch screen released
		Touch* touch = touchManager->GetTouch( event->m_TouchID );
		touch->active = false;
		touchManager->DecreaseTouchCount();

		// See if this might indicate the end of a gesture
		touchManager->DetectGestureEnd( event->m_TouchID );
	}

	touchManager->FreeInstance();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HandleMultiTouchMotionCB - For multitouch devices the system will call this callback when the user moves their finger on the screen. This callback is called once for each screen touch
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HandleMultiTouchMotionCB( s3ePointerTouchMotionEvent* event )
{
	TouchManager* touchManager = TouchManager::GetInstance();

	// Check to see if the touch already exists
	Touch* touch = touchManager->GetTouch( event->m_TouchID );
	if( touch != NULL )
	{
		// Updates the touches positional information
		touch->x = event->m_x;
		touch->y = event->m_y;

		// See if this might be updating a current gesture
		touchManager->DetectGestureUpdate( event->m_TouchID );
	}

	touchManager->FreeInstance();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HandleSingleTouchButtonCB - The system will call this callback when the user touches the screen
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HandleSingleTouchButtonCB( s3ePointerEvent* event )
{
	if( event->m_Button == S3E_POINTER_BUTTON_SELECT )
	{
		TouchManager* touchManager = TouchManager::GetInstance();

		if( event->m_Pressed )
		{
			// Left mouse button or touch screen pressed
			Touch* touch = touchManager->GetTouch( 0 );
			touch->active = true;
			touch->x = event->m_x;
			touch->y = event->m_y;
			touchManager->IncreaseTouchCount();

			// See if this might indicate the beginnings of a gesture
			touchManager->DetectGestureStart( 0 );
		}
		else
		{
			// Left mouse button or touch screen released
			Touch* touch = touchManager->GetTouch( 0 );
			touch->active = false;
			touchManager->DecreaseTouchCount();

			// See if this might indicate the end of a gesture
			touchManager->DetectGestureEnd( 0 );
		}
	
		touchManager->FreeInstance();
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HandleSingleTouchMotionCB - The system will call this callback when the user moves their finger on the screen
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HandleSingleTouchMotionCB( s3ePointerMotionEvent* event )
{
	TouchManager* touchManager = TouchManager::GetInstance();

	Touch* touch = touchManager->GetTouch( 0 );
	touch->x = event->m_x;
	touch->y = event->m_y;

	// See if this might be updating a current gesture
	touchManager->DetectGestureUpdate( 0 );

	touchManager->FreeInstance();
}