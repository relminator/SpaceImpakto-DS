/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Boss class


*******************************************************************************
******************************************************************************/

#include "Boss.h"

//--------------------------------------
//	
//--------------------------------------
Boss::Boss()
{
	
	ID = 0;

	x = 0;
	y = 0;
	Dx = 0;
	Dy = 0;

	Frame = 0;
	Counter = 0;
	Hp = 1024;
	MaxHp = Hp;
	DeathScore = Hp * 8;
	
	Alive = false;
	Active = false;


	DestroyedCounter = 0;


	DrawMode = EN_NORMAL;

	Width = 32;
	Height = 32;

	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
}

//--------------------------------------
//	
//--------------------------------------
Boss::~Boss()
{
	Cannons.clear();
}


//--------------------------------------
//	
//--------------------------------------
void Boss::Init( int Palette, glImage* const Sprites, int _id )
{
	Cannons.clear();
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	ID = _id;
	
}

//--------------------------------------
//	
//--------------------------------------
void Boss::Kill()
{

	Active = false;
	Alive = false;
	
	x = 400 << 12;
	y = 96 << 12;
	
	std::vector<Cannon>::iterator iter;
	for( iter = Cannons.begin(); iter != Cannons.end(); ++iter )
	{
		iter->x = x;
		iter->y = y;
		// Reset AABB
		Aabb.Init( (iter->x >> 12) - (iter->Width >> 1), 
				   (iter->y >> 12) - (iter->Height >> 1), 
			       iter->Width, iter->Height 
			      );
	
		iter->TurretPtr->Deactivate();
		iter->TurretPtr->Kill();
	}
	
	DestroyedCounter = 60 * 5;
	
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Boss::DrawHp( int Frame )
{

	const int LEN = 255;
	s32 Percentagef32 = (Hp << 12) / MaxHp;
	s32 HpLeft = UTIL::Lerpf32( 0, LEN, Percentagef32 );
	int Color = Frame & 31 ;
	
	glPolyFmt( POLY_ALPHA(18) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	glBoxFilled(0,189,HpLeft,190, RGB15(31,31,0) );
	
	glBox(0,188,255,190, RGB15(31-Color,Color,Color*2) );
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );	
	
}
