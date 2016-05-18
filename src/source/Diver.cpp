/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Diver class


*******************************************************************************
******************************************************************************/

#include "Diver.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_DIVERS 16
#define DIVER_MAX_HP 1

//------------------------------------------------------------------------------
//	Diver Enemy
//------------------------------------------------------------------------------
Diver::Diver()
{
	Frame = 0;
	Active = false;	
	Hp = DIVER_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 20;
	Height = 20;
	
}

Diver::~Diver()
{
}

//--------------------------------------
//	
//--------------------------------------
void Diver::Update( PlayerShip &Vic )
{
	
	Counter++;
	if( (x >> 12) < 300 )
	{
		NewCounter++;
	}	
	if( NewCounter > 0 )
	{
		
		if( NewCounter < 60 )
		{
			Dx += 164;
			if( NewCounter == 20 )
			{
				if( !TurretPtr->IsActivated() ) TurretPtr->Activate();
			}
		
		}
		else if( NewCounter == 61 )
		{
			Dx = 0;
			s32 angle = atan2Lerp( Vic.GetX() - x, Vic.GetY() - y );
			Dx = -(abs(cosLerp(angle)));
			Dy = sinLerp(angle);
		}
		else if( NewCounter > 61 )
		{
			Dx = mulf32(Dx, 4200);
			Dy = mulf32(Dy, 4200);
		}
		
	}
		
	x += Dx;
	y += Dy;
	
	
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
		Frame = ( Frame + 1 ) & 7;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Diver::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  Dx,
								  Dy,
								  Explode::SMALL_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
	
}

//--------------------------------------
//	
//--------------------------------------
void Diver::Kill()
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
//	Diver Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
DiverContainer::DiverContainer()
{
}
	
DiverContainer::~DiverContainer()
{
	Divers.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void DiverContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Divers.resize( MAX_DIVERS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void DiverContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Diver>::iterator iter;
	for( iter = Divers.begin(); iter != Divers.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void DiverContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Diver>::iterator iter;
	for( iter = Divers.begin(); iter != Divers.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(0,31,0) );
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      GL_FLIP_NONE, &Images[iter->Frame + 19] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      GL_FLIP_NONE, &Images[iter->Frame + 19] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void DiverContainer::DrawAABB( int color )
{
	
	std::vector<Diver>::iterator iter;
	for( iter = Divers.begin(); iter != Divers.end(); ++iter )
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
EntityContainer::CollisionPacket DiverContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 1;
	
	std::vector<Diver>::iterator iter;
	for( iter = Divers.begin(); iter != Divers.end(); ++iter )
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
void DiverContainer::KillAllEntities()
{
	std::vector<Diver>::iterator iter;
	for( iter = Divers.begin(); iter != Divers.end(); ++iter )
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
void DiverContainer::Spawn( int _x, int _y, TurretArray &Turrets )
{
	
	std::vector<Diver>::iterator iter;
	for( iter = Divers.begin(); iter != Divers.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->x = _x << 12;
			iter->y = _y << 12;
			iter->Dx = -floattof32(3);
			iter->Dy = 0;
			
			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->NewCounter = 0;
			iter->Hp = DIVER_MAX_HP;
			
			// 1st pattern
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2 ;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= 256;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 1 << 12;			// 
			Tp.BulletsPerShot	= 3;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.5);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_CENTERED;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(3);
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 31;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::DECEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
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


