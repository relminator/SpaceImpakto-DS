/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.phatcode.net

	Singleton Fx3D class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <math.h>
#include <gl2d.h>
#include <cstdio>

#include "UTIL.h"
#include "cearn_atan.h"
#include "Palettes.h"
#include "Vector2df32.h"

#ifndef FX2D_H
#define FX2D_H

class Fx2D {

	static Fx2D* ms_instance;

public:
	static Fx2D* Instance();
	static void Release();

	void Init();
	
	void Fade( unsigned short* Target,    // sub or main bg/oam 
			   const unsigned short* StartPal,  // pal to lerp from 
			   const unsigned short* EndPal,    // pal to lerp to 
			   s32 &Interp,
			   s32 Speed,
		       int MaxColors = 512
			 );
	
	u8* PlasmaPolar1();
	u8* PlasmaPolar2();
	u8* PlasmaPolar3();
	u8* PlasmaPolar4();
	u8* PlasmaPolar5();
	u8* PlasmaPolar6();
	u8* PlasmaStencil( const unsigned int *image );
	
	void CyclePlasmaPal();
	void CyclePlasmaPal( int start, int end );
	
	u16* GetPlasmaPal()
	{
		return PlasmaPal;
	}
	
	void LoadPal( const unsigned short *pal );
	void LoadPal( int start, int end, const unsigned short *pal );
	
private:
	Fx2D();
	~Fx2D();
	
	u8 Vpage[SCREEN_WIDTH*SCREEN_HEIGHT];

	// RGB palette
	u16 PlasmaPal[256];    
	
};

#endif // FX2D_H
