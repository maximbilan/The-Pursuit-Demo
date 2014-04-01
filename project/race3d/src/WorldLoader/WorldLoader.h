#ifndef _WORLDLOADER_H_
#define _WORLDLOADER_H_

#include <string>

#include "IwManaged.h"
#include "IwManagedList.h"

//------------------------------------------------------------------------------------------------
// WorldLoader - loader of the WORLD file. Contains list of loaded objects
//------------------------------------------------------------------------------------------------
class WorldLoader
{
public:
	WorldLoader( const char* worldName );
	~WorldLoader();

public:
	// Add loaded object to the list
	static void AddObject( CIwManaged* pObject, bool allowDups = false );
	
	// Delete objects. Frees all managed objects referenced by the list
	static void Delete();

	// Clear and free objects. Performs an optimised clear on the list and frees all pointers, but does not free any managed objects referenced by the list
	static void ClearAndFree();

	// Get objects
	static CIwManagedList* GetObjects();

public:
	// Load world file
	void LoadWorld();

private:
	static CIwManagedList* s_objectList;

private:
	std::string m_worldName;
};

#endif	// _WORLDLOADER_H_