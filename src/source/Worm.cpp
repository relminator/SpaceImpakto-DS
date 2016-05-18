/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Worm class


*******************************************************************************
******************************************************************************/

#include "Worm.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_WORMS 8
#define WORM_MAX_HP 128

//------------------------------------------------------------------------------
//	Worm Enemy
//------------------------------------------------------------------------------
Worm::Worm()
{
	Frame = 0;
	Active = false;	
	Hp = WORM_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 60;
	Height = 20;
	
}

Worm::~Worm()
{
}

//--------------------------------------
//	
//--------------------------------------
void Worm::Update( PlayerShip &Vic )
{
	
	Counter++;
	Angle += 512;
	
	
	
	if( (Counter >= 100) && (Counter <= 512) )
	{
		x = Sx + sinLerp( Angle ) * 8;
		if( Counter == 110 )
		{
			for( int i = 0; i < 2; i++)
			{
				TurretPtr[i]->Activate();
			}	
		}
	
	}
	else
	{
		Sx += Dx;
		x = Sx + sinLerp( Angle ) * 8;
	}
	
	
	
	if( TurretPtr[0]->GetShotCounter() == 3 )   // stop shooting
	{
		TurretPtr[0]->Deactivate();
	}
	
	if( TurretPtr[1]->GetShotCounter() == 12 )   // stop shooting
	{
		TurretPtr[1]->Deactivate();
	}

	if( x < MIN_X )
	{
		Kill();
		return;
	}
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPos.x = x-(20<<12);
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
void Worm::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::SMALL_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_MED1 );
	
}

//--------------------------------------
//	
//--------------------------------------
void Worm::Kill()
{

	Active = false;
	x = 300 << 12;
	y = 96 << 12;
	TurretPtr[0]->Kill();
	TurretPtr[1]->Kill();
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
}

//------------------------------------------------------------------------------
//	Worm Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
WormContainer::WormContainer()
{
}
	
WormContainer::~WormContainer()
{
	Worms.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void WormContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Worms.resize( MAX_WORMS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void WormContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Worm>::iterator iter;
	for( iter = Worms.begin(); iter != Worms.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void WormContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Worm>::iterator iter;
	for( iter = Worms.begin(); iter != Worms.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 30, (iter->y >> 12) - 10, 
					      GL_FLIP_NONE, &Images[iter->Frame + 6] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 30, (iter->y >> 12) - 10, 
					      GL_FLIP_NONE, &Images[iter->Frame + 6] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void WormContainer::DrawAABB( int color )
{
	
	std::vector<Worm>::iterator iter;
	for( iter = Worms.begin(); iter != Worms.end(); ++iter )
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
EntityContainer::CollisionPacket WormContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<Worm>::iterator iter;
	for( iter = Worms.begin(); iter != Worms.end(); ++iter )
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
void WormContainer::KillAllEntities()
{
	std::vector<Worm>::iterator iter;
	for( iter = Worms.begin(); iter != Worms.end(); ++iter )
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
void WormContainer::Spawn( int _y, s32 angle, TurretArray &Turrets )
{
	
	std::vector<Worm>::iterator iter;
	for( iter = Worms.begin(); iter != Worms.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Sx = 300 << 12;
			iter->Dx = -floattof32(1.00);
			iter->y = _y << 12;
			iter->Angle = angle;
			
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = WORM_MAX_HP;
			
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= 1024;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 10;			// 
			Tp.BulletsPerShot	= 3;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 1;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(3.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_OFFSET;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)/8;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 16;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr[0]
	  		             );
	
			
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= BRAD_PI/12;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/2;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 96;				// 
			Tp.MaxSpawnDelay	= 0;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 2;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 6;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_NORMAL;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)*4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 28;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr[1]
	  		             );
	
			// Deactivate
			iter->TurretPtr[0]->Deactivate();
			iter->TurretPtr[1]->Deactivate();
	
			break;
		
		}
	}
	
	
}

