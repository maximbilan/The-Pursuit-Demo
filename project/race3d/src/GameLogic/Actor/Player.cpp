#include "Player.h"
#include "Actor.h"
#include "../../GameObjects/Car.h"

//---------------------------------------------------------------------------------------

Player::Player( const unsigned int carColor ) :
	Actor( carColor )
{
}

//---------------------------------------------------------------------------------------

Player::~Player()
{
	for( ActorSpawnPointArray::const_iterator it = m_spawnPoints.begin(), end = m_spawnPoints.end(); it != end; ++it )
	{
		if( *it )
		{
			delete ( *it );
		}
	}
}

//---------------------------------------------------------------------------------------

void Player::Init()
{
	Actor::Init();
}

//---------------------------------------------------------------------------------------

void Player::Update( const s3e_uint64_t dt )
{
	Actor::Update( dt );
}

//---------------------------------------------------------------------------------------

void Player::Render()
{
	Actor::Render();
}

//---------------------------------------------------------------------------------------

void Player::DoLeft()
{
	m_car->Left();
}

//---------------------------------------------------------------------------------------

void Player::DoRight()
{
	m_car->Right();
}

//---------------------------------------------------------------------------------------

void Player::Gas()
{
	m_car->Force();
}

//---------------------------------------------------------------------------------------

void Player::Brake()
{
	m_car->BackDraft();
}

//---------------------------------------------------------------------------------------

void Player::AddSpawnPoint( ActorSpawnPoint* point )
{
	m_spawnPoints.push_back( point );
}

//---------------------------------------------------------------------------------------

void Player::Respawn()
{
	if( !m_spawnPoints.empty() )
	{
		double min = 0.0f;

		ActorSpawnPointArray::iterator minIt = m_spawnPoints.begin();
		for( ActorSpawnPointArray::iterator it = m_spawnPoints.begin(), end = m_spawnPoints.end(); it != end; ++it )
		{
			const CIwFVec3 distanceFromCar = ( *it )->GetDistance( m_car->GetPosition() );
			double distanceXZ = distanceFromCar.x + distanceFromCar.z;

			if( it == m_spawnPoints.begin() )
			{
				min = distanceXZ;
				minIt = it;
				continue;
			}
			
			if( distanceXZ < min )
			{
				min = distanceXZ;
				minIt = it;
			}
		}

		if( *minIt )
		{
			m_car->Reset();
			m_car->SetPosition( ( *minIt )->GetPosition() );
			m_car->SetRotation( ( *minIt )->GetRotation() );
		}
	}
}

//---------------------------------------------------------------------------------------

void Player::Respawn( const std::string& id )
{
	if( !m_spawnPoints.empty() )
	{
		for( ActorSpawnPointArray::iterator it = m_spawnPoints.begin(), end = m_spawnPoints.end(); it != end; ++it )
		{
			if( (*it)->GetName() == id )
			{
				m_car->Reset();
				m_car->SetPosition( (*it)->GetPosition() );
				m_car->SetRotation( (*it)->GetRotation() );
				break;
			}
		}
	}
}

//---------------------------------------------------------------------------------------

void Player::RespawnOnStartPosition()
{
	if( m_startPoint )
	{
		m_car->Reset();
		m_car->SetPosition( m_startPoint->GetPosition() );
		m_car->SetRotation( m_startPoint->GetRotation() );
	}
}