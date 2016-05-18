/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Singleton Palettes class


*******************************************************************************
******************************************************************************/

#include "Palettes.h"

Palettes* Palettes::ms_instance = 0;

//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
Palettes::Palettes()
{
}

Palettes::~Palettes()
{
}

//--------------------------------------
//	
//--------------------------------------
Palettes* Palettes::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Palettes();
	}
	return ms_instance;
}

//--------------------------------------
//	
//--------------------------------------
void Palettes::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

//--------------------------------------
//	
//--------------------------------------
void Palettes::Init()
{
	
	// Black and White
	for( int i = 0; i < 256; i++ )
	{
		PalWhite[i] = (0xFF << 8 ) | 0xFF;
		PalBlack[i] = (0x00 << 8 ) | 0x00;
	}
	
//	WhitePal = gluTexLoadPal( PalWhite, 512, GL_RGB256 );
//	BlackPal = gluTexLoadPal( PalBlack, 512, GL_RGB256 );
//	
//	//red
//	for( int i = 0; i < 256; i++ )
//	{
//		Pal[i] = RGB15( 31, 0, 0 );
//	}
//	RedPal = gluTexLoadPal( Pal, 512, GL_RGB256 );
//	
//	//green
//	for( int i = 0; i < 256; i++ )
//	{
//		Pal[i] = RGB15( 0, 31, 0 );
//	}
//	GreenPal = gluTexLoadPal( Pal, 512, GL_RGB256 );
//	
//	//blue
//	for( int i = 0; i < 256; i++ )
//	{
//		Pal[i] = RGB15( 0, 0, 31 );
//	}
//	BluePal = gluTexLoadPal( Pal, 512, GL_RGB256 );
	
}
	
//--------------------------------------
//	
//--------------------------------------	
unsigned short* Palettes::LerpPal( const unsigned short* Pal1,  const unsigned short* Pal2, s32 t, int MaxColors )
{
	
	t = UTIL::Clamp( t, 0, 4096 );
	
	for( int i = 0; i < MaxColors; i++ )
	{
		int r1 = RGB15_R( Pal1[i] );
		int g1 = RGB15_G( Pal1[i] );
		int b1 = RGB15_B( Pal1[i] );
		
		int r2 = RGB15_R( Pal2[i] );
		int g2 = RGB15_G( Pal2[i] );
		int b2 = RGB15_B( Pal2[i] );
		
		s32 rn = UTIL::Lerpf32( r1 << 12, r2 << 12, t );
		s32 gn = UTIL::Lerpf32( g1 << 12, g2 << 12, t );
		s32 bn = UTIL::Lerpf32( b1 << 12, b2 << 12, t );
		
		Pal[i] = RGB15( rn >> 12, gn >> 12, bn >> 12 );
	}
	
	return Pal;
}
	