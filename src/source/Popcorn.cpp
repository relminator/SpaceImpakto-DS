/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Popcorn class


*******************************************************************************
******************************************************************************/

#include "Popcorn.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_POPCORNS 16
#define POPCORN_MAX_HP 1

//------------------------------------------------------------------------------
//	Popcorn Enemy
//------------------------------------------------------------------------------
Popcorn::Popcorn()
{
	Frame = 0;
	Active = false;	
	Hp = POPCORN_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 20;
	Height = 20;
	FlipMode = GL_FLIP_NONE;
	
}

Popcorn::~Popcorn()
{
}

//--------------------------------------
//	
//--------------------------------------
void Popcorn::Update( PlayerShip &Vic )
{
	
	Counter++;

	//iprintf("\x1b[1;1HNewTarget = %i         ", NewTarget );
			
	
	
	
	if( (Counter == 30) )
	{
			TurretPtr->Activate();
			TurretPtr->DoPattern(0);
	}
	
	if( Counter < 100 )
	{
		if( y < Vic.GetY() )
		{
			Dy = floattof32(0.5);
		}
		else if( y > Vic.GetY() )
		{
			Dy = -floattof32(0.5);
		}
	}
	else
	{
		Dy = 0;
	}
	
	x += Dx;
	y = Sy + sinLerp(Counter * 256) * 32;
	
	if( TurretPtr->GetShotCounter() == 4 )   // Deactivate
		TurretPtr->Deactivate();

	
	if( x < MIN_X )
	{
		Kill();
		return;
	}
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPos.x = x;
	TurretPos.y = y;
	
	
	TurretTar.x = Vic.GetX();
	TurretTar.y = Vic.GetY();
	

	// animate
	if( ( Counter & 3 ) == 0 )
	{
		Frame = (Frame + 1) & 3;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Popcorn::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  Dx,
								  Dy,
								  Explode::SMALL_2 );			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
	
}

//--------------------------------------
//	
//--------------------------------------
void Popcorn::Kill()
{

	Active = false;
	x = 300 << 12;
	y = 96 << 12;
	TurretPtr->Kill();
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
}

//------------------------------------------------------------------------------
//	Popcorn Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
PopcornContainer::PopcornContainer()
{
}
	
PopcornContainer::~PopcornContainer()
{
	Popcorns.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void PopcornContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Popcorns.resize( MAX_POPCORNS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void PopcornContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Popcorn>::iterator iter;
	for( iter = Popcorns.begin(); iter != Popcorns.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void PopcornContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Popcorn>::iterator iter;
	for( iter = Popcorns.begin(); iter != Popcorns.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      iter->FlipMode, &Images[iter->Frame + 28] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      iter->FlipMode, &Images[iter->Frame + 28] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void PopcornContainer::DrawAABB( int color )
{
	
	std::vector<Popcorn>::iterator iter;
	for( iter = Popcorns.begin(); iter != Popcorns.end(); ++iter )
	{
		if( iter->Active )
		{
			AABB aabb = iter->Aabb.Get();
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			aabb.Resize(512*3);
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket PopcornContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 1;
	
	std::vector<Popcorn>::iterator iter;
	for( iter = Popcorns.begin(); iter != Popcorns.end(); ++iter )
	{
		if( iter->Active )
		{
			AABB aabb = iter->Aabb.Get();
			aabb.Resize(512*3);
			
			if( !Vic.IsInvincible() )
			{
				if( !BombIsActive )
				{
					if( Vic.GetAABB().Intersects(aabb) )
					{
						Ret.x = Vic.GetX() >> 12;
						Ret.y = Vic.GetY() >> 12;
						Ret.Collision = 2;
						Vic.Kill();
					}
				}
			}
			
			// Viper shots
			int Collisions = 0;
			if( iter->x < ( 256<<12 ) )
			{
				Collisions = Vic.CollideShots(aabb);
				if( Collisions )
				{
					iter->DrawMode = EN_WHITE;
					// Dec HP
					iter->Hp -= Collisions;
					if( iter->Hp < 0 )
					{
						Ret.x = iter->x >> 12;
						Ret.y = iter->y >> 12;
						iter->Destroy();
						Vic.AddToScore( iter->DeathScore );
						IsDestroyed = true;
					}
					
				}
				
				// Viper Missiles
				Collisions = Vic.CollideMissiles(aabb);
				if( Collisions )
				{
					iter->DrawMode = EN_WHITE;
					// Dec HP
					iter->Hp -= Collisions;
					if( iter->Hp < 0 )
					{
						Ret.x = iter->x >> 12;
						Ret.y = iter->y >> 12;
						iter->Destroy();
						Vic.AddToScore( iter->DeathScore );
						IsDestroyed = true;
					}
					
				}
			}    // end if
		}
	}
	
	return Ret;
}

//--------------------------------------
//	
//--------------------------------------
void PopcornContainer::KillAllEntities()
{
	std::vector<Popcorn>::iterator iter;
	for( iter = Popcorns.begin(); iter != Popcorns.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Kill();
		}
	}
	
}

//--------------------------------------
//	Non-Virtual Functions
//--------------------------------------
void PopcornContainer::Spawn( int sy, int ey, TurretArray &Turrets )
{
	
	std::vector<Popcorn>::iterator iter;
	for( iter = Popcorns.begin(); iter != Popcorns.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->x = 300 << 12;
			iter->Sy = sy << 12;
			iter->Dx = -floattof32(1.0);
			iter->Dy = 0;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = POPCORN_MAX_HP;
		
			
			Turret::Params Tp;
	
			// 1
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= 0;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 24;			// 
			Tp.BulletsPerShot	= 1;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SINGLE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(3);
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -20;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 20;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 12;
			Tp.Bp.Height = 12;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr
	  		             );
	
	
			// Deactivate
			iter->TurretPtr->Deactivate();
			break;
		
		}
	}
	
	
}

