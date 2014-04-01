#include "WebManager.h"
#include "s3eDebug.h"
#include <string>

//----------------------------------------------------------------------------------------------

const char* WebManager::s_twitterShareLink = "http://twitter.com/share?text=";
const char* WebManager::s_facebookShareLink = "http://www.facebook.com/sharer.php?";
const char* WebManager::s_BBXVendorLink = "https://appworld.blackberry.com/webstore/vendor/26396/";

//----------------------------------------------------------------------------------------------

void WebManager::Init()
{
	if( s3eOSExecAvailable() )
	{
		m_available = true;
	}
	else
	{
		m_available = false;
	}

	m_requestResult = -1;
}

//----------------------------------------------------------------------------------------------

void WebManager::OpenLink( const char* url )
{
	if( m_available )
	{
		m_requestResult = s3eOSExecExecute( url, S3E_FALSE );
	}
}

//----------------------------------------------------------------------------------------------

void WebManager::Update()
{
	if( m_requestResult != -1 )
	{
		switch( m_requestResult )
		{
			case S3E_RESULT_SUCCESS:
			{
				s3eDebugOutputString( "Link was successfully opened." );
			}
			break;

			case S3E_RESULT_ERROR:
			{
				s3eDebugOutputString( "Executing host OS commands API isn't available on this device." );
			}
			break;

		}
	}
}

//----------------------------------------------------------------------------------------------

void WebManager::ShareFacebook( const char* title, const char* summary )
{
	if( m_available )
	{
		std::string url = s_facebookShareLink;
		url += "s=100";
		url += "&p[url]=";
		url += s_BBXVendorLink;
		url += "&p[title]=";
		url += title;
		url += "&p[summary]=";
		url += summary;
		m_requestResult = s3eOSExecExecute( url.c_str(), S3E_FALSE );
	}
}

//----------------------------------------------------------------------------------------------

void WebManager::ShareTwitter( const char* text )
{
	if( m_available )
	{
		std::string url = s_twitterShareLink;
		url += text;
		url += " ";
		url += s_BBXVendorLink;
		m_requestResult = s3eOSExecExecute( url.c_str(), S3E_FALSE );
	}
}