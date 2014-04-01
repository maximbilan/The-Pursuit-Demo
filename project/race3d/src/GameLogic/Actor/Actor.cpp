#include "Actor.h"
#include "ActorSpawnPoint.h"
#include "../../GameObjects/Car.h"
#include "../../GameObjects/Trigger.h"

//---------------------------------------------------------------------------------------

const std::string Actor::s_actorTypes[] = { "Player", "NPC" };

//---------------------------------------------------------------------------------------

Actor::Actor( const unsigned int carColor ) :
	m_type( ACTOR_TYPE_NONE ),
	m_startPoint( NULL )
{
	m_car = new Car( carColor );
}

//---------------------------------------------------------------------------------------

Actor::~Actor()
{
	delete m_car;

	if( m_startPoint )
	{
		delete m_startPoint;
	}
}

//---------------------------------------------------------------------------------------

unsigned int Actor::GetActorType( const char* type )
{
	if( !strcmp( type, "Player" ) )
	{
		return ACTOR_TYPE_PLAYER;
	}
	else if( !strcmp( type, "NPC" ) )
	{
		return ACTOR_TYPE_NPC;
	}
	else
	{
		return ACTOR_TYPE_NONE;
	}
}

//---------------------------------------------------------------------------------------

void Actor::Init()
{
	m_car->Init();

	if( m_startPoint )
	{
		m_car->SetPosition( m_startPoint->GetPosition() );
		m_car->SetRotation( m_startPoint->GetRotation() );
	}
}

//---------------------------------------------------------------------------------------

void Actor::Update( const s3e_uint64_t dt )
{
	m_car->Update( dt );
}

//---------------------------------------------------------------------------------------

void Actor::Render()
{
	m_car->Render();
}