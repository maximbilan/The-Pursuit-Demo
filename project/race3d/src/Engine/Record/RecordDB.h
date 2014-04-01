#ifndef _RECORDDB_H_
#define _RECORDDB_H_

#include "IwArray.h"

#include <string>

class Record;

//---------------------------------------------------------------------------------------

typedef CIwArray<Record*> RecordArray;

//---------------------------------------------------------------------------------------
// RecordDB - class for storage data
//---------------------------------------------------------------------------------------
class RecordDB
{
public:
	RecordDB( const std::string fileName );
	~RecordDB();

	// Save data to file
	void Save();

	// Load data from file
	void Load();

	// Reset
	void Reset();

	// Returns true if file is exist
	bool IsExist() const;

	// Add record as string
	void AddRecord( const std::string key, const std::string value );

	// Add record as int
	void AddRecord( const std::string key, const int value );

	// Add record as unsigned int64
	void AddRecord( const std::string key, const uint64 value );

	// Add record as int64
	void AddRecord( const std::string key, const int64 value );

	// Add record as float
	void AddRecord( const std::string key, const float value );

	// Add record as double
	void AddRecord( const std::string key, const double value );

	// Add record as boolean
	void AddRecord( const std::string key, const bool value );

	// Set record as string
	void SetRecord( const std::string key, const std::string value );

	// Set record 
	void SetRecord( const std::string key, const int value );

	// Set record as unsigned int64
	void SetRecord( const std::string key, const uint64 value );

	// Set record as int64
	void SetRecord( const std::string key, const int64 value );

	// Set record as float
	void SetRecord( const std::string key, const float value );

	// Set record as double
	void SetRecord( const std::string key, const double value );

	// Set record as boolean
	void SetRecord( const std::string key, const bool value );

	// Get record by key
	Record* GetRecord( const std::string key ) const;

	// Get record as string
	std::string GetRecordAsString( const std::string key ) const;

	// Get record as int
	int GetRecordAsInt( const std::string key ) const;

	// Get record as unsigned int64
	uint64 GetRecordAsUInt64( const std::string key ) const;

	// Get record as int64
	int64 GetRecordAsInt64( const std::string key ) const;

	// Get record as float
	float GetRecordAsFloat( const std::string key ) const;

	// Get record as double
	double GetRecordAsDouble( const std::string key ) const;

	// Get record as boolean
	bool GetRecordAsBool( const std::string key ) const;

private:
	std::string	m_fileName;

	RecordArray	m_records;
};

//-------------------------------------------------------------------------------------

inline bool RecordDB::IsExist() const
{
	return s3eFileCheckExists( m_fileName.c_str() ) ? true : false;
}

#endif	// _RECORDDB_H_