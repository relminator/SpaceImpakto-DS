/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Singleton Palettes class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <math.h>
#include <gl2d.h>

#include "UTIL.h"

#ifndef PALETTES_H
#define PALETTES_H

class Palettes {

	static Palettes* ms_instance;

public:
	static Palettes* Instance();
	static void Release();
	
	void Init();
	
	int White() const
		{
			return WhitePal;
		}
		
	int Black() const
		{
			return BlackPal;
		}	
	
	int Red() const
		{
			return RedPal;
		}
	
	int Green() const 
		{
			return GreenPal;
		}
	
	int Blue() const 
		{
			return BluePal;
		}
	
	
	unsigned short* GetWhite() 
		{
			return PalWhite;
		}
		
	unsigned short* GetBlack() 
		{
			return PalBlack;
		}	
	
	unsigned short* LerpPal( const unsigned short* Pal1,  const unsigned short* Pal2, s32 t, int MaxColors = 256 );
	
	
private:
	Palettes();
	~Palettes();
	
	int WhitePal;
	int BlackPal;
	int RedPal;
	int GreenPal;
	int BluePal;
	
	unsigned short Pal[256];
	unsigned short PalBlack[256];
	unsigned short PalWhite[256];
	

};

#endif // PALETTES_H
