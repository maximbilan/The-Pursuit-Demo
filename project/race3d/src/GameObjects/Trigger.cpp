#include "Trigger.h"
#include "Car.h"

#include "IwGeomIntersect.h"

#include "../Common/Defines.h"

#include "../GameStates/GameState.h"
#include "../GameStates/GS_GamePlay.h"
#include "../GameLogic/Actor/Actor.h"

//------------------------------------------------------------------------------------------------

Trigger::Trigger()
{
	m_collisionType = PHYSICS_COLLISION_NONE;
	m_mass = 0.0f;
	m_type = ENTITY_NONE;
}

//------------------------------------------------------------------------------------------------

Trigger::~Trigger()
{
}

//------------------------------------------------------------------------------------------------

void Trigger::Init()
{
	const CIwFVec3 min = m_position;
	const CIwFVec3 max = min * 5.0f;
	m_bbox = CIwFBBox( min, max );

	m_isInited = true;
}

//------------------------------------------------------------------------------------------------

void Trigger::Update( const s3e_uint64_t dt )
{
}

//------------------------------------------------------------------------------------------------

void Trigger::UpdatePhysics( const s3e_uint64_t dt )
{
}

//------------------------------------------------------------------------------------------------

void Trigger::Render()
{
	const CIwFSphere sphere = CIwFSphere( m_position, m_scale.z );
#ifdef ENABLE_RENDER_BSPHERE
	IwGxDebugPrimSphere( sphere );
#endif
}

//------------------------------------------------------------------------------------------------

unsigned int Trigger::GetPolyCount()
{
	return 0;
}

//------------------------------------------------------------------------------------------------

bool Trigger::IsIntersect( GameEntity* obj )
{
	m_bsphere = CIwFSphere( m_position, m_scale.z );

	// Trigger sphere
	const int32 s1x = static_cast<int32>( m_bsphere.GetTrans().x );
	const int32 s1y = static_cast<int32>( m_bsphere.GetTrans().y );
	const int32 s1z = static_cast<int32>( m_bsphere.GetTrans().z );
	const int32 s1r = static_cast<int32>( m_bsphere.GetRadius() );
	const CIwSphere s1( CIwVec3( s1x, s1y, s1z ), s1r );

	// Game entity sphere
	CIwFSphere objSphere = obj->GetBSphere();
	const int32 s2x = static_cast<int32>( objSphere.GetTrans().x );
	const int32 s2y = static_cast<int32>( objSphere.GetTrans().y );
	const int32 s2z = static_cast<int32>( objSphere.GetTrans().z );
	const int32 s2r = static_cast<int32>( objSphere.GetRadius() );
	const CIwSphere s2( CIwVec3( s2x, s2y, s2z ), s2r );

	// Comparison
	return IwIntersectSphereSphere(s1, s2 );
	// return IwIntersectSphereSphere( m_bsphere, obj->GetBSphere() );
}

//------------------------------------------------------------------------------------------------

void Trigger::OnEnter()
{
	if( !m_onEnterScriptName.empty() )
	{
		RunLevelScript( m_onEnterScriptName );
	}
}

//------------------------------------------------------------------------------------------------

void Trigger::OnLeave()
{
	if( !m_onLeaveScriptName.empty() )
	{
		RunLevelScript( m_onLeaveScriptName );
	}
}

//------------------------------------------------------------------------------------------------

void Trigger::RunLevelScript( const std::string functionName )
{
	const int currentStateId = GameState::GetStateID();

	if( currentStateId == GameState::k_state_gameplay )
	{
		GS_GamePlay* gameplay = static_cast<GS_GamePlay*>( GameState::GetCurrentState() );
		if( gameplay )
		{
			gameplay->RunLevelScript( functionName.c_str() );
		}
	}
}

//------------------------------------------------------------------------------------------------

void Trigger::CheckIntersect( Actor* actor )
{
	if( actor )
	{
		Car* car = actor->GetCar();
		if( car )
		{
			const bool isIntersect = IsIntersect( car );
			int triggerState = actor->GetTriggerState( m_id );

			switch( triggerState )
			{
			case OBJECT_OUT:
				{
					triggerState = isIntersect ? OBJECT_ENTER : OBJECT_OUT;
				}
				break;
			case OBJECT_ENTER:
				{
					if( isIntersect )
					{
						OnEnter();
						triggerState = OBJECT_IN;
					}
					else
					{
						triggerState = OBJECT_ENTER;
					}
				}
				break;
			case OBJECT_IN:
				{
					triggerState = isIntersect ? OBJECT_IN : OBJECT_LEAVE;
				}
				break;
			case OBJECT_LEAVE:
				{
					if( isIntersect )
					{
						triggerState = OBJECT_LEAVE;
					}
					else
					{
						OnLeave();
						triggerState = OBJECT_OUT;
					}
				}
				break;
			}

			actor->SetTriggerState( m_id, triggerState );
		}
	}
}