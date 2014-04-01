#ifndef _MENUTOUCH_H_
#define _MENUTOUCH_H_

class TouchManager;
struct Touch;

//---------------------------------------------------------------------------------
// MenuTouch - interface for menus, which uses TouchManager for some computing
//---------------------------------------------------------------------------------
class MenuTouch
{
public:
	MenuTouch();
	virtual ~MenuTouch();

protected:
	// Update the touches
	virtual bool UpdateTouches();

	// Handle the touches
	virtual void HandleTouches( const Touch* touch );

protected:
	TouchManager*	m_touchManager;
};

#endif	// _MENUTOUCH_H_