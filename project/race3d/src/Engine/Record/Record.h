#ifndef _RECORD_H_
#define _RECORD_H_
 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "s3eTypes.h"

//-----------------------------------------------------------------------------------------

#define RECORD_BUFFER 255

//-----------------------------------------------------------------------------------------
// Record - data used in the RecordDB
//-----------------------------------------------------------------------------------------
class Record
{
public:
	enum RECORD_TYPE
	{
		RT_STRING,
		RT_INT,
		RT_UINT64,
		RT_INT64,
		RT_FLOAT,
		RT_DOUBLE,
		RT_BOOL
	};

public:
	Record();
	~Record();

	// Set value with key and type
	void Set( const std::string key, unsigned int type, const std::string value );

	// Set value as string
	void SetAsString( const std::string key, const std::string value );
	
	// Set value as int
	void SetAsInt( const std::string key, const int value );
	
	// Set value as unsigned int64
	void SetAsUInt64( const std::string key, const uint64 value );
	
	// Set value as int64
	void SetAsInt64( const std::string key, const int64 value );
	
	// Set value as float
	void SetAsFloat( const std::string key, const float value );
	
	// Set value as double
	void SetAsDouble( const std::string key, const double value );

	// Set value as boolean
	void SetAsBool( const std::string key, const bool value );

	// Get value as string
	const std::string& GetValueAsString() const;

	// Get value as int
	int GetValueAsInt() const;

	// Get value as unsigned int64
	uint64 GetValueAsUInt64() const;

	// Get value as int64
	int64 GetValueAsInt64() const;

	// Get value as float
	float GetValueAsFloat() const;

	// Get value as double
	double GetValueAsDouble() const;

	// Get value as boolean
	bool GetValueAsBool() const;

	// Get key
	const std::string& GetKey() const;

	// Get type
	unsigned int GetType() const;

private:
	std::string		m_key;
	unsigned int	m_type;
	std::string		m_value;
};

//-----------------------------------------------------------------------------------------

inline void Record::Set( const std::string key, unsigned int type, const std::string value )
{
	m_key = key;
	m_type = type;
	m_value = value;
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsString( const std::string key, const std::string value )
{
	m_key = key;
	m_type = RT_STRING;
	m_value = value;
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsInt( const std::string key, const int value )
{
	m_key = key;
	m_type = RT_INT;
	char buffer[RECORD_BUFFER];
	sprintf( buffer, "%d", value );
	m_value = std::string( buffer );
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsUInt64( const std::string key, const uint64 value )
{
	m_key = key;
	m_type = RT_UINT64;
	char buffer[RECORD_BUFFER];
	sprintf( buffer, "%llu", value );
	m_value = std::string( buffer );
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsInt64( const std::string key, const int64 value )
{
	m_key = key;
	m_type = RT_INT64;
	char buffer[RECORD_BUFFER];
	sprintf( buffer, "%lld", value );
	m_value = std::string( buffer );
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsFloat( const std::string key, const float value )
{
	m_key = key;
	m_type = RT_FLOAT;
	char buffer[RECORD_BUFFER];
	sprintf( buffer, "%f", value );
	m_value = std::string( buffer );
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsDouble( const std::string key, const double value )
{
	m_key = key;
	m_type = RT_DOUBLE;
	char buffer[RECORD_BUFFER];
	sprintf( buffer, "%e", value );
	m_value = std::string( buffer );
}

//-----------------------------------------------------------------------------------------

inline void Record::SetAsBool( const std::string key, const bool value )
{
	m_key = key;
	m_type = RT_BOOL;
	m_value = value ? "true" : "false";
}

//-----------------------------------------------------------------------------------------

inline const std::string& Record::GetValueAsString() const
{
	return m_value;
}

//-----------------------------------------------------------------------------------------

inline int Record::GetValueAsInt() const
{
	int value = 0;
	value = atoi( m_value.c_str() );
	return value;
}

//-----------------------------------------------------------------------------------------

inline uint64 Record::GetValueAsUInt64() const
{
	uint64 value = 0;
	std::stringstream strm( m_value.c_str() );
	strm >> value;
	return value;
}

//-----------------------------------------------------------------------------------------

inline int64 Record::GetValueAsInt64() const
{
	int64 value = 0;
	std::stringstream strm( m_value.c_str() );
	strm >> value;
	return value;
}

//-----------------------------------------------------------------------------------------

inline float Record::GetValueAsFloat() const
{
	float value = 0.0f;
	value = static_cast<float>( atof( m_value.c_str() ) );
	return value;
}

//-----------------------------------------------------------------------------------------

inline double Record::GetValueAsDouble() const
{
	double value = 0;
	value = atof( m_value.c_str() );
	return value;
}

//-----------------------------------------------------------------------------------------

inline bool Record::GetValueAsBool() const
{
	bool value = false;
	
	if( !strcmp( m_value.c_str(), "true" ) )
	{
		value = true;
	}

	return value;
}

//-----------------------------------------------------------------------------------------

inline const std::string& Record::GetKey() const
{
	return m_key;
}

//-----------------------------------------------------------------------------------------

inline unsigned int Record::GetType() const
{
	return m_type;
}

#endif	// _RECORD_H_