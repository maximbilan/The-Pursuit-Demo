#include "KeyManager.h"

#include "s3eKeyboard.h"

//-----------------------------------------------------------------------------------------

void KeyManager::Init()
{
	m_isLeftKey = false;
	m_isRightKey = false;
	m_isUpKey = false;
	m_isDownKey = false;
	m_isOkKey = false;

	for( int i = s3eKeyFirst; i < s3eKeyCount; ++i )
	{
		KeyStatus keyStatus;
		keyStatus.m_keyId = i;
		keyStatus.m_isDown = false;
		keyStatus.m_isPressed = false;
		keyStatus.m_isReleased = false;
		m_keyStatusArray.push_back( keyStatus );
	}
}

//-----------------------------------------------------------------------------------------

void KeyManager::Update()
{
	m_isLeftKey = ( s3eKeyboardGetState( s3eKeyLeft ) & S3E_KEY_STATE_DOWN ) != 0;
	m_isRightKey = ( s3eKeyboardGetState( s3eKeyRight ) & S3E_KEY_STATE_DOWN ) != 0;
	m_isUpKey = ( s3eKeyboardGetState( s3eKeyUp ) & S3E_KEY_STATE_PRESSED ) != 0;
	m_isDownKey = ( s3eKeyboardGetState( s3eKeyDown ) & S3E_KEY_STATE_PRESSED ) != 0;
	m_isOkKey = ( ( s3eKeyboardGetState( s3eKeyEnter ) & S3E_KEY_STATE_PRESSED ) != 0) |
				( ( s3eKeyboardGetState( s3eKeyOk ) & S3E_KEY_STATE_PRESSED) != 0 ) |
				( ( s3eKeyboardGetState( s3eKeySpace ) & S3E_KEY_STATE_PRESSED ) != 0);

	for( KeyStatusArray::iterator it = m_keyStatusArray.begin(), end = m_keyStatusArray.end(); it != end; ++it )
	{
		const s3eKey id = static_cast<s3eKey>( ( *it ).m_keyId );
		( *it ).m_isDown = ( s3eKeyboardGetState( id ) & S3E_KEY_STATE_DOWN ) != 0;
		( *it ).m_isPressed = ( s3eKeyboardGetState( id ) & S3E_KEY_STATE_PRESSED ) != 0;
		( *it ).m_isReleased = ( s3eKeyboardGetState( id ) & S3E_KEY_STATE_RELEASED ) != 0;
	}
}

//-----------------------------------------------------------------------------------------

void KeyManager::Remove()
{
	m_keyStatusArray.clear();
}