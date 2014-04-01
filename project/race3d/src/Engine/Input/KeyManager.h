#ifndef _KEYMANAGER_H_
#define _KEYMANAGER_H_

#include <map>
#include <vector>

#include "s3ePointer.h"

#include "../../Common/Singleton.h"

//-----------------------------------------------------------------------------------------

typedef struct
{
	int		m_keyId;
	bool	m_isDown;		// Indicates a key is currently pressed down
	bool	m_isPressed;	// Indicates a key's state has changed from up to down in the last update
	bool	m_isReleased;	// Indicates a key's state has changed from down to up in the last update
} KeyStatus;

//-----------------------------------------------------------------------------------------

typedef std::vector<KeyStatus> KeyStatusArray;

//-------------------------------------------------------------------------------------------
// KeyManager - manager for working with keyboard
//-------------------------------------------------------------------------------------------
class KeyManager : public Singleton<KeyManager>
{
public:
	// Initialization
	void Init();

	// Update
	void Update();

	// Remove the instance
	void Remove();

	// Check is pressed left key
	bool IsPressedLeftKey() const;

	// Check is pressed right key
	bool IsPressedRightKey() const;

	// Check is pressed up key
	bool IsPressedUpKey() const;

	// Check is pressed down key
	bool IsPressedDownKey() const;

	// Check is pressed ok/space/enter key
	bool IsPressedOkKey() const;

	// Check is currently pressed down a key by id
	bool IsDown( const int id ) const;

	// Checks is a key's state has changed from up to down in the last update
	bool IsPressed( const int id ) const;

	// Check is a key's state has changed from down to up in the last update
	bool IsReleased( const int id ) const;

protected:
	KeyManager() {}
	friend class Singleton<KeyManager>;

private:
	bool m_isLeftKey;
	bool m_isRightKey;
	bool m_isUpKey;
	bool m_isDownKey;
	bool m_isOkKey;

	KeyStatusArray	m_keyStatusArray;
};

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsPressedLeftKey() const
{
	return m_isLeftKey;
}

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsPressedRightKey() const
{
	return m_isRightKey;
}

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsPressedUpKey() const
{
	return m_isUpKey;
}

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsPressedDownKey() const
{
	return m_isDownKey;
}

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsPressedOkKey() const
{
	return m_isOkKey;
}

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsDown( const int id ) const
{
	bool isDown = false;

	for( KeyStatusArray::const_iterator it = m_keyStatusArray.begin(), end = m_keyStatusArray.end(); it != end; ++it )
	{
		if( ( *it ).m_keyId == id && ( *it ).m_isDown )
		{
			isDown = true;
			break;
		}
	}

	return isDown;
}

//-----------------------------------------------------------------------------------------

inline bool KeyManager::IsPressed( const int id ) const
{
	bool isPressed = false;

	for( KeyStatusArray::const_iterator it = m_keyStatusArray.begin(), end = m_keyStatusArray.end(); it != end; ++it )
	{
		if( ( *it ).m_keyId == id && ( *it ).m_isPressed )
		{
			isPressed = true;
			break;
		}
	}

	return isPressed;
}

//---------------------------------------------------------------------------------------

inline bool KeyManager::IsReleased( const int id ) const
{
	bool isReleased = false;

	for( KeyStatusArray::const_iterator it = m_keyStatusArray.begin(), end = m_keyStatusArray.end(); it != end; ++it )
	{
		if( ( *it ).m_keyId == id && ( *it ).m_isReleased )
		{
			isReleased = true;
			break;
		}
	}

	return isReleased;
}

#endif	// _KEYMANAGER_H_