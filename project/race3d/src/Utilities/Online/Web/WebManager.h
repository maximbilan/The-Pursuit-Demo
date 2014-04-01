#ifndef _WEBMANAGER_H_
#define _WEBMANAGER_H_

#include "s3eOSExec.h"

#include "../../Common/Singleton.h"

//------------------------------------------------------------------------------------------------
// WebManager - singleton with simple web functions
//------------------------------------------------------------------------------------------------
class WebManager : public Singleton<WebManager>
{
private:
	static const char* s_twitterShareLink;
	static const char* s_facebookShareLink;
	static const char* s_BBXVendorLink;

public:
	// Initialization
	void Init();

	// Update
	void Update();

	// Open link in web browser
	void OpenLink( const char* url );

	// Share on facebook
	void ShareFacebook( const char* title, const char* summary );

	// Share on twitter
	void ShareTwitter( const char* text );

protected:
	WebManager() {}
	friend class Singleton< WebManager >;

private:
	bool	m_available;
	int		m_requestResult;
};

#endif	// _WEBMANAGER_H_