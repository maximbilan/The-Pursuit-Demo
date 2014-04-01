#include "RecordDB.h"
#include "Record.h"

//---------------------------------------------------------------------------------------

RecordDB::RecordDB( const std::string fileName ) :
	m_fileName( fileName )
{
}

//---------------------------------------------------------------------------------------

RecordDB::~RecordDB()
{
	Reset();
}

//---------------------------------------------------------------------------------------

void RecordDB::Save()
{
	s3eFile* file = s3eFileOpen( m_fileName.c_str(), "wb" );
	if( file != NULL )
	{
		for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
		{
			const std::string key = ( *it )->GetKey();
			const unsigned int type = ( *it )->GetType();
			const std::string value = ( *it )->GetValueAsString().c_str();

			char out[RECORD_BUFFER];
			sprintf( out, "%s$%u$%s|", key.c_str(), type, value.c_str() );

			s3eFileWrite( out, sizeof( out ), 1, file );
		}

		s3eFileClose( file );
	}
	else
	{
		s3eFileGetError();
		s3eDebugOutputString( s3eFileGetErrorString() );
	}
}

//---------------------------------------------------------------------------------------

void RecordDB::Load()
{
	s3eFile* file = s3eFileOpen( m_fileName.c_str(), "rb" );
	if( file != NULL )
	{
		char in[RECORD_BUFFER];

		m_records.clear();
		while( !s3eFileEOF( file ) )
		{
			s3eFileRead( in, sizeof( in ), 1, file );
			Record* r = new Record();

			std::string temp( in );
			std::size_t posFirst = temp.find( "$" );
			std::size_t posSecond = temp.find( "$", posFirst + 1 );
			std::size_t posThird = temp.find( "|" );

			const std::string key = temp.substr( 0, posFirst );
			const std::string value = temp.substr( posSecond + 1, posThird - posSecond - 1 );

			const unsigned int type = strtoul( temp.substr( posFirst + 1, posSecond - posFirst - 1 ).c_str(), NULL, 0 );

			r->Set( key, type, value );
			m_records.push_back( r );
		}

		s3eFileClose( file );
	}
	else
	{
		s3eFileGetError();
		s3eDebugOutputString( s3eFileGetErrorString() );
	}
}

//---------------------------------------------------------------------------------------

void RecordDB::Reset()
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		delete ( *it );
	}
	m_records.clear();
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const std::string value )
{
	Record* r = new Record();
	r->SetAsString( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const int value )
{
	Record* r = new Record();
	r->SetAsInt( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const uint64 value )
{
	Record* r = new Record();
	r->SetAsUInt64( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const int64 value )
{
	Record* r = new Record();
	r->SetAsInt64( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const float value )
{
	Record* r = new Record();
	r->SetAsFloat( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const double value )
{
	Record* r = new Record();
	r->SetAsDouble( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::AddRecord( const std::string key, const bool value )
{
	Record* r = new Record();
	r->SetAsBool( key, value );
	m_records.push_back( r );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const std::string value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsString( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const int value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsInt( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const uint64 value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsUInt64( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const int64 value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsInt64( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const float value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsFloat( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const double value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsDouble( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

void RecordDB::SetRecord( const std::string key, const bool value )
{
	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			( *it )->SetAsBool( key, value );
			return;
		}
	}

	AddRecord( key, value );
}

//---------------------------------------------------------------------------------------

Record* RecordDB::GetRecord( const std::string key ) const
{
	Record* r = NULL;

	for( RecordArray::const_iterator it = m_records.begin(), end = m_records.end(); it != end; ++it )
	{
		if( ( *it )->GetKey() == key )
		{
			r = ( *it );
			break;
		}
	}

	return r;
}

//---------------------------------------------------------------------------------------

std::string RecordDB::GetRecordAsString( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsString() : 0;
}

//---------------------------------------------------------------------------------------

int RecordDB::GetRecordAsInt( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsInt() : 0;
}

//---------------------------------------------------------------------------------------

uint64 RecordDB::GetRecordAsUInt64( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsUInt64() : 0;
}

//---------------------------------------------------------------------------------------

int64 RecordDB::GetRecordAsInt64( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsInt64() : 0;
}

//---------------------------------------------------------------------------------------

float RecordDB::GetRecordAsFloat( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsFloat() : 0.0f;
}

//---------------------------------------------------------------------------------------

double RecordDB::GetRecordAsDouble( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsDouble() : 0.0f;
}

//---------------------------------------------------------------------------------------

bool RecordDB::GetRecordAsBool( const std::string key ) const
{
	Record* r = GetRecord( key );
	return r ? r->GetValueAsBool() : false;
}