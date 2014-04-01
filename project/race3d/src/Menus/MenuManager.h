#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

#include <IwArray.h>
#include "../Common/Singleton.h"
#include "../Menus/MenuElement.h"
#include "../Engine/PostProcessing/PostProcessing.h"

//-----------------------------------------------------------------------------------

typedef CIwArray<MenuElement*> MenuElementArray;

//-----------------------------------------------------------------------------------
// MenuManager
//-----------------------------------------------------------------------------------
class MenuManager : public Singleton<MenuManager>
{
public:
	static uint8 s_fadeTransitionSpeed;
	static uint8 s_fadeInTransitionSpeed;
	static uint8 s_fadeOutTransitionSpeed;
	static uint8 s_slideTransitionSpeed;

public:
	// Initialization
	void Init();

	// Render
	void Render();

	// Update
	void Update( const s3e_uint64_t dt );

	// Remove the instance
	void Remove();

	// Reset texts
	void ResetTexts();

	// Create menu
	void CreateMenu( const int id );

	// Push menu to array
	void PushMenu( const int id );

	// Replace menu
	void ReplaceMenu( const int id );

	// Pop menu
	void PopMenu();

	/// Fade effect
	void Fade();

	// Fadein effect
	void Fadein();

	// Fadeout effect
	void Fadeout();

	// Slide right effect
	void SildeRight();

	// Slide left effect
	void SlideLeft();

	// Slide up effect
	void SlideUp();

	// Slide down effect
	void SlideDown();

	// Get current menu id
	int GetCurrentMenuId() const;

	// Get current menu element
	MenuElement* GetCurrentMenuElement() const;

	// Get menu element by ID
	MenuElement* GetMenuElementById( const int id ) const;

private:
	int m_currentTypeId;
	int m_lastTypeId;

	MenuElementArray	m_menus;

	PostProcessing*		m_postProcessing;

protected:
	MenuManager() {}
	friend class Singleton<MenuManager>;
};

//-----------------------------------------------------------------------------------

inline int MenuManager::GetCurrentMenuId() const
{
	return m_currentTypeId;
}

//-----------------------------------------------------------------------------------

inline MenuElement* MenuManager::GetCurrentMenuElement() const
{
	return m_menus[m_currentTypeId];
}

//-----------------------------------------------------------------------------------

inline MenuElement* MenuManager::GetMenuElementById( const int id ) const
{
	MenuElement* element = NULL;

	if( id > MenuElement::MENUELEMENTTYPE_NONE && id < MenuElement::MENUELEMENTTYPE_COUNT )
	{
		element = m_menus[id];
	}

	return element;
}

#endif	// _MENUMANAGER_H_