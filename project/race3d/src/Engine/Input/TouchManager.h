#ifndef _TOUCHMANAGER_H_
#define _TOUCHMANAGER_H_

#include "s3ePointer.h"
#include "IwGeomFVec2.h"

#include "../../Common/Singleton.h"

//-------------------------------------------------------------------------------------------

#define MAX_TOUCHES S3E_POINTER_TOUCH_MAX

//-------------------------------------------------------------------------------------------
// Structure used for detecting a touch
//-------------------------------------------------------------------------------------------
struct Touch
{
public:
	int		x, y;		// Touch position
	bool	active;		// Touch active state
	int		id;			// ID of touch - The system tracks multiple touches by assigning each one a unique ID
};

//-------------------------------------------------------------------------------------------
// Structure used for detecting a swipe
//-------------------------------------------------------------------------------------------
struct Swipe
{
public:
	bool	m_tracking;
	uint32	m_touchID;
	CIwVec2	m_delta;
	uint32	m_startTime;
};

//-------------------------------------------------------------------------------------------
// Structure used for detecting a pinch
//-------------------------------------------------------------------------------------------
struct Pinch
{
public:
	bool	m_tracking;
	uint32	m_touchID1;
	uint32	m_touchID2;
	CIwVec2	m_delta;
	float	m_initialDistance;
	float	m_zoom;
};

//-------------------------------------------------------------------------------------------
// TouchManager - manager for detecting touches, swipes, pinches
//-------------------------------------------------------------------------------------------
class TouchManager : public Singleton<TouchManager>
{
public:
	static uint32	s_swipeTime;
	static uint32	s_swipeDistance;
	static int		s_swipeAccuracy;

	enum SwipeDirection
	{
		SD_NONE,
		SD_LEFT,
		SD_RIGHT,
		SD_UP,
		SD_DOWN
	};

public:
	// Initialization
	bool Init();

	// Update
	void Update();

	// Remove the instance
	void Remove();

	// Check for the beginning of a new gesture
	void DetectGestureStart( const uint32 touchID );

	// Check for the end of a gesture
	void DetectGestureEnd( const uint32 touchID );

	// Check for a gesture update
	void DetectGestureUpdate( const uint32 touchID );

	// Reset swipe direction
	void ResetSwipeDirection();

	// Increase touch count
	void IncreaseTouchCount();

	// Decrease touch count
	void DecreaseTouchCount();

	// Returns availability of the pointer
	bool IsAvailable() const;

	// Returns multitouch capability
	bool IsMultiTouch() const;

	// Gets a specific touch
	Touch* GetTouch( const int index );

	// Get number of touches this frame
	int GetTouchCount() const;

	// Get swipe direction
	uint16 GetSwipeDirection() const;

	// Get swipe instance
	Swipe* GetSwipe() const;

protected:
	TouchManager() {}
	friend class Singleton<TouchManager>;

private:
	bool		m_available;						// true if a pointer is present
	bool		m_isMultiTouch;						// true if multitouch is enabled
	Touch		m_touches[MAX_TOUCHES];				// List of potential touches
	uint32		m_touchCount;
	Swipe*		m_swipe;
	uint16		m_swipeDirection;
	Pinch*		m_pinch;
};

//-------------------------------------------------------------------------------------------

inline bool TouchManager::IsAvailable() const
{
	return m_available;
}

//-------------------------------------------------------------------------------------------

inline bool TouchManager::IsMultiTouch() const
{
	return m_isMultiTouch;
}

//-------------------------------------------------------------------------------------------

inline Touch* TouchManager::GetTouch( const int index )
{
	return &m_touches[index];
}

//-------------------------------------------------------------------------------------------

inline void TouchManager::IncreaseTouchCount()
{
	m_touchCount++;
}

//-------------------------------------------------------------------------------------------

inline void TouchManager::DecreaseTouchCount()
{
	m_touchCount--;
}

//-------------------------------------------------------------------------------------------

inline uint16 TouchManager::GetSwipeDirection() const
{
	return m_swipeDirection;
}

//-------------------------------------------------------------------------------------------

inline Swipe* TouchManager::GetSwipe() const
{
	return m_swipe;
}

//-------------------------------------------------------------------------------------------

inline void TouchManager::ResetSwipeDirection()
{
	m_swipeDirection = SD_NONE;
}

#endif	// _TOUCHMANAGER_H_