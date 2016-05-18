/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Global Manager Singleton class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#ifndef GLOBALMANAGER_H
#define GLOBALMANAGER_H

class GlobalManager
{

	static GlobalManager* ms_instance;

public:
	static GlobalManager* Instance();
	static void Release();

	void Init()
	{
		ResetAll();
	}
	
	void ResetAll()
	{
		FreeID = 0;
		FreeOAM = 0;
	}
	
	
	inline int GetFreePolyID()
	{
		FreeID++;
		sassert( FreeID < 64, "Poly ID Overflow" );
		return ( FreeID - 1 );
	}
	
	inline int GetFreeOAM()
	{
		FreeOAM++;
		sassert( FreeOAM < 128, "OAM ID Overflow" );
		return ( FreeOAM - 1 );
	}
	
private:
	GlobalManager();
	~GlobalManager();

	int FreeID;
	int FreeOAM;

};

#endif // GLOBALMANAGER_H
