/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Plato class


*******************************************************************************
******************************************************************************/

#include "Plato.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_PLATOS 16
#define PLATO_MAX_HP 1

//------------------------------------------------------------------------------
//	Plato Enemy
//------------------------------------------------------------------------------
Plato::Plato()
{
	Frame = 0;
	Active = false;	
	Hp = PLATO_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 16;
	Height = 16;
	
}

Plato::~Plato()
{
	
}

//--------------------------------------
//	
//--------------------------------------
void Plato::Update( PlayerShip &Vic )
{
	bool KillMe = false;
	
	Counter++;
	if( Counter > Delay )
	{
		KillMe = Path.Update();
		x = Path.GetX();
		y = Path.GetY();
	}
	else
	{
		x = 300 << 12;
		y = 96 << 12;		
	}
	
	if( Counter == 90 )
	{
		TurretPtr->Activate();
	}
	
	if( TurretPtr->GetShotCounter() == 2 )
	{
		TurretPtr->Deactivate();
	}
	
	if( KillMe )
	{
		Kill();
		return;
	}
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPosition.x = x;
	TurretPosition.y = y;
	
	// Target where head is facing
	TurretTarget.x = Vic.GetX();
	TurretTarget.y = Vic.GetY();
	
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
void Plato::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::SMALL_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
	
}

//--------------------------------------
//	
//--------------------------------------
void Plato::Kill()
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

//--------------------------------------
//	
//--------------------------------------
void Plato::LoadPath( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist )
{
	
	Path.Init( _points, _lut, LutSize, rate, totaldist );

	Path.SetRate( floattof32(1.75) );
	
}
	
//------------------------------------------------------------------------------
//	Plato Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
PlatoContainer::PlatoContainer()
{
}
	
PlatoContainer::~PlatoContainer()
{
	Platos.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void PlatoContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Platos.resize( MAX_PLATOS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void PlatoContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Plato>::iterator iter;
	for( iter = Platos.begin(); iter != Platos.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void PlatoContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Plato>::iterator iter;
	for( iter = Platos.begin(); iter != Platos.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 8, (iter->y >> 12) - 8, 
					      GL_FLIP_NONE, &Images[iter->Frame] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 8, (iter->y >> 12) - 8, 
					      GL_FLIP_NONE, &Images[iter->Frame] 
					    );
					
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void PlatoContainer::DrawAABB( int color )
{
	
	std::vector<Plato>::iterator iter;
	for( iter = Platos.begin(); iter != Platos.end(); ++iter )
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
EntityContainer::CollisionPacket PlatoContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 1;
	
	std::vector<Plato>::iterator iter;
	for( iter = Platos.begin(); iter != Platos.end(); ++iter )
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
			}   // end if
		}
	}
	
	return Ret;
	
}

//--------------------------------------
//	
//--------------------------------------
void PlatoContainer::KillAllEntities()
{
	std::vector<Plato>::iterator iter;
	for( iter = Platos.begin(); iter != Platos.end(); ++iter )
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
void PlatoContainer::Spawn( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist, int delay, int frame, TurretArray &Turrets )
{
	
	std::vector<Plato>::iterator iter;
	for( iter = Platos.begin(); iter != Platos.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = frame;
			iter->Counter = 0;
			iter->Delay = delay;
			iter->Hp = PLATO_MAX_HP;
			iter->LoadPath( _points, _lut, LutSize, rate, totaldist );
			
			
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 512;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI*2/4;			// distance of each bullet on a spread
			Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 60;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_NORMAL;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)*4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 3;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 31;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPosition, 
						   &iter->TurretTarget,
						   iter->TurretPtr
	  		             );
			// Deactivate
			iter->TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}

