#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>

//------------------------------------------------------------
// Entity - base class for entities, contains simple data
//------------------------------------------------------------
class Entity
{
public:
	enum EntityType
	{
		ENTITY_NONE			= -1,
		ENTITY_TERRAIN,
		ENTITY_DECORATION,
		ENTITY_CAR,
		ENTITY_TRIGGER
	};

public:
	static unsigned int GetEntityType( const char* name );

public:
	Entity();
	virtual ~Entity();

	// Get id
	unsigned int GetID() const;

	// Get name
	const std::string& GetName() const;

	// Set name
	void SetName( const std::string name );

	// Get type
	unsigned int GetType() const;

	// Set type
	void SetType( const unsigned int type );

	// Check is enabled
	bool IsEnabled() const;

	// Enable or disable entity
	void SetEnabled( const bool enabled );

private:
	static unsigned int	s_idTotal;

protected:
	unsigned int	m_id;
	std::string		m_name;
	unsigned int	m_type;
	bool			m_isEnabled;
};

//------------------------------------------------------------

inline unsigned int Entity::GetID() const
{
	return m_id;
}

//------------------------------------------------------------

inline const std::string& Entity::GetName() const
{
	return m_name;
}

//------------------------------------------------------------

inline void Entity::SetName( const std::string name )
{
	m_name = name;
}

//------------------------------------------------------------

inline unsigned int Entity::GetType() const
{
	return m_type;
}

//------------------------------------------------------------

inline void Entity::SetType( const unsigned int type )
{
	m_type = type;
}

//------------------------------------------------------------

inline bool Entity::IsEnabled() const
{
	return m_isEnabled;
}

//------------------------------------------------------------

inline void Entity::SetEnabled( const bool enabled )
{
	m_isEnabled = enabled;
}

#endif	// _ENTITY_H_