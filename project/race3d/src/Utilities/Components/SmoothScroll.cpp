#include "SmoothScroll.h"

#include "s3eTimer.h"

#include "../../Engine/Input/TouchManager.h"

//---------------------------------------------------------------------------------------------

double SmoothScroll::s_lengthFactor = 1.5f;
double SmoothScroll::s_durationFactor = 0.12f;
double SmoothScroll::s_distanceDecrease = 0.001f;
double SmoothScroll::s_timeCoef = 0.1f;
double SmoothScroll::s_percentOfSmoothingStart = 0.4f;

//---------------------------------------------------------------------------------------------

SmoothScroll::SmoothScroll() :
	m_duration( 0 ),
	m_startTime( 0 ),
	m_stepX( 0 ),
	m_stepY( 0 ),
	m_direction( 0 ),
	m_distance( 0.0f )
{
	m_touchManager = TouchManager::GetInstance();
}

//---------------------------------------------------------------------------------------------

SmoothScroll::~SmoothScroll()
{
	m_touchManager->FreeInstance();
}

//---------------------------------------------------------------------------------------------

void SmoothScroll::Update()
{
	if( m_touchManager->GetSwipeDirection() != TouchManager::SD_NONE )
	{
		const double distance = m_touchManager->GetSwipe()->m_delta.GetLength() * s_lengthFactor;
		const uint64 duration = static_cast< uint64 >( distance * s_durationFactor );

		StartScroll( m_touchManager->GetSwipeDirection(), distance * s_distanceDecrease * s_timeCoef, duration ); 
		
		m_touchManager->ResetSwipeDirection();
	}

	if( m_isActive )
	{
		const uint64 currentTime = s3eTimerGetMs() - m_startTime;

		if( currentTime <= m_duration )
		{
			if( currentTime >= m_duration * s_percentOfSmoothingStart )
			{
				m_distance = ( m_duration - currentTime ) * s_distanceDecrease;
			}

			switch( m_direction )
			{
			case TouchManager::SD_LEFT:
				{
					m_stepX = m_distance;
				}	
				break;
			case TouchManager::SD_RIGHT:
				{
					m_stepX = -m_distance;
				}	
				break;
			case TouchManager::SD_UP:
				{
					m_stepY = m_distance;
				}
				break;
			case TouchManager::SD_DOWN:
				{
					m_stepY = -m_distance;
				}
				break;
			}
		}
		else
		{
			StopScroll();
		}
	}
}

//---------------------------------------------------------------------------------------------

void SmoothScroll::StartScroll( const unsigned int direction, const double distance, const uint64 duration )
{
	m_direction = direction;
	m_duration = duration;
	m_isActive = true;
	m_distance = distance;
	m_startTime = s3eTimerGetMs();
}

//---------------------------------------------------------------------------------------------

void SmoothScroll::StopScroll()
{
	m_isActive = false;
	m_direction = 0;
	m_distance = 0.0f;
	m_duration = 0;
	m_startTime = 0;
}