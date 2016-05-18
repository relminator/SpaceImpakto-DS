/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Singleton Explosion class


*******************************************************************************
******************************************************************************/

#include "Explosion.h"

Explosion* Explosion::ms_instance = 0;

//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
Explosion::Explosion()
{
}

Explosion::~Explosion()
{
}

Explosion* Explosion::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Explosion();
	}
	return ms_instance;
}

void Explosion::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

//--------------------------------------
//	
//--------------------------------------
void Explosion::Init( int Palette, glImage* const Sprites )
{
	
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Explosions.resize( 256 );
	
	CoolDownCounter = 0;
	Radius = 128;
	MinID = Explode::TINY;
	MaxID = Explode::MEDIUM_5;
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Explosion::Update()
{
	
	if( CoolDownCounter > 0 )
	{
		MegaExplode();
	}
	
	std::vector<Explode>::iterator iter;
	for( iter = Explosions.begin(); iter != Explosions.end(); ++iter )
	{
		if( iter->Active )
			iter->Update();
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Explosion::Draw()
{
	
	//glColorTable( GL_RGB256, ColorTable );
	numExplosions = 0;
	std::vector<Explode>::iterator iter;
	for( iter = Explosions.begin(); iter != Explosions.end(); ++iter )
	{
		if( iter->Active )
		{
			numExplosions++;
			
			int frame = iter->Frame + iter->BaseFrame;
			glSprite( (iter->x >> 12) - Images[frame].width/2, 
					  (iter->y >> 12) - Images[frame].height/2, 
					  GL_FLIP_NONE, &Images[frame] 
				    );
			
		}
	}
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Explosion::Spawn(  int _x, int _y, s32 _dx, s32 _dy, Explode::TYPE ID )
{
	
	int base = 6;
	int frames = 8;
	
	switch( ID )
	{
		case Explode::TINY:
			base = 42;
			frames = 3;
			break;
		case Explode::SMALL_1:
			base = 6;
			frames = 8;
			break;
		case Explode::SMALL_2:
			base = 19;
			frames = 5;
			break;
		case Explode::SMALL_3:
			base = 32;
			frames = 8;
			break;
		case Explode::SMALL_4:
			base = 40;
			frames = 5;
			break;
		case Explode::MEDIUM_1:
			base = 0;
			frames = 6;
			break;
		case Explode::MEDIUM_2:
			base = 14;
			frames = 5;
			break;
		case Explode::MEDIUM_3:
			base = 24;
			frames = 4;
			break;
		case Explode::MEDIUM_4:
			base = 28;
			frames = 4;
			break;
		case Explode::MEDIUM_5:
			base = 45;
			frames = 4;
			break;
		default:
			base = 6;
			frames = 8;
	}
	
	std::vector<Explode>::iterator iter;
	for( iter = Explosions.begin(); iter != Explosions.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Init( _x << 12 , _y << 12, _dx, _dy, base, frames );
			break;
		
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Explosion::Spawn(  int _x, int _y, s32 _dx, s32 _dy, Explode::TYPE ID_1, Explode::TYPE ID_2 )
{
	Spawn( _x,
		   _y,
		   0,
		   0,
		   ID_1
		 );

	for( int i = 0; i < 4; i++ )
	{	
		s32 dx = 2 * cosLerp(BRAD_PI/4 + i * BRAD_PI / 2);
		s32 dy = 2 * sinLerp(BRAD_PI/4 + i * BRAD_PI / 2);
		Spawn( _x,
			   _y,
			   dx,
			   dy,
			   ID_2
			 );
	}
					
}


//--------------------------------------
//	
//--------------------------------------
void Explosion::KillEntities()
{
	std::vector<Explode>::iterator iter;
	for( iter = Explosions.begin(); iter != Explosions.end(); ++iter )
	{
		if( iter->Active )
			iter->Active = false;
	}
}


//--------------------------------------
//	
//--------------------------------------
void Explosion::MegaSpawn( int cx, int cy, int radius, Explode::TYPE min, Explode::TYPE max, int cooldowncounter )
{
	CenterX = cx;
	CenterY = cy;
	Radius = radius;
	MinID = min;
	MaxID = max;
	Counter = 0;
	CoolDownCounter = cooldowncounter;
}


//--------------------------------------
//	
//--------------------------------------
void Explosion::MegaExplode()
{
	
	CoolDownCounter--;
	Counter++;
	if( (Counter & 7) == 0 )
	{
		for( int i = 1; i < 6; i++ )
		{
			int expid = qran_range(MinID, MaxID);
			s32 ex = cosLerp( Counter * 1200 * i) * qran_range(0,Radius);
			s32 ey = sinLerp( Counter * 1200 * i) * qran_range(0,Radius);
			Spawn(  CenterX + (ex >> 12), CenterY + (ey >> 12), 0, 0, (Explode::TYPE)expid );
		}
	}
	
	if( (Counter & 15) == 0 )
		Sound::Instance()->PlaySFX( E_SFX_EXP_MED1 );
	
	
	
}
	