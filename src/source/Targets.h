/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Targets class


*******************************************************************************
******************************************************************************/

#include "TargetEntity.h"

#ifndef TARGETS_H
#define TARGETS_H

// NEEDS to be 2^n!!!!
#define TARGETS_MAX_TARGETS	 8

class Targets 
{

	static Targets* ms_instance;

public:
	static Targets* Instance();
	static void Release();
	
	inline void Init()
	{
		Numtargets = 0;
		for( int i = 0; i < TARGETS_MAX_TARGETS; i++ )
		{
			Coords[i].Kill();
		}
	}
	
	void Reset()
	{
		Numtargets = 0;
		for( int i = 0; i < TARGETS_MAX_TARGETS; i++ )
		{
			Coords[i].Kill();
		}
	}
	
	void AddCoord( s32 x, s32 y )
	{
		for( int i = 0; i < TARGETS_MAX_TARGETS; i++ )
		{
			if( !Coords[i].IsActive() )
			{
				Coords[i].Init( x, y );
				//if( Numtargets < ( TARGETS_MAX_TARGETS ) ) 
					Numtargets++;
				break;
			}
		}
	}


	inline TargetEntity GetTarget( int index )
	{
		return Coords[index & (TARGETS_MAX_TARGETS-1)];
	}
	
	inline int GetNumTargets() const
	{
		return Numtargets;
	}
	
	
	
private:
	Targets();
	~Targets();
	
	int Numtargets;
	
	TargetEntity Coords[TARGETS_MAX_TARGETS];
	

};

#endif // TARGETS_H
