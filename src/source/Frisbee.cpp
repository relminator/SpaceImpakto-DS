/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	Frisbee class


*******************************************************************************
******************************************************************************/

#include "Frisbee.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_FRISBEES 16
#define FRISBEE_MAX_HP 2

//------------------------------------------------------------------------------
//	Frisbee Enemy
//------------------------------------------------------------------------------
Frisbee::Frisbee()
{
	Frame = 0;
	Active = false;	
	Hp = FRISBEE_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 20;
	Height = 20;

}

Frisbee::~Frisbee()
{
	
}

//--------------------------------------
//	
//--------------------------------------
void Frisbee::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	bool KillMe = false;
		
	if( Counter > Delay )
	{
		NewCounter++;
		KillMe = Path.Update();
		x = Path.GetX();
		y = Path.GetY();
	}
	else
	{
		x = 300 << 12;
		y = 96 << 12;		
	}
	
	if( NewCounter ==  60 )
	{
		if( !TurretPtr->IsActivated() ) TurretPtr->Activate();
	}
	
	if( TurretPtr->GetShotCounter() == 4 )   // stop shooting
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
void Frisbee::Destroy()
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
void Frisbee::Kill()
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
void Frisbee::LoadPath( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist )
{
	
	Path.Init( _points, _lut, LutSize, rate, totaldist );

	Path.SetRate( floattof32(3) );
	
}
	
//------------------------------------------------------------------------------
//	Frisbee Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
FrisbeeContainer::FrisbeeContainer()
{
}
	
FrisbeeContainer::~FrisbeeContainer()
{
	Frisbees.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void FrisbeeContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Frisbees.resize( MAX_FRISBEES );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void FrisbeeContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Frisbee>::iterator iter;
	for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void FrisbeeContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Frisbee>::iterator iter;
	for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12)-10, (iter->y >> 12)-10,
						  GL_FLIP_NONE, &Images[32+iter->Frame] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12)-10, (iter->y >> 12)-10,
						  GL_FLIP_NONE, &Images[32+iter->Frame] 
					    );
					
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void FrisbeeContainer::DrawAABB( int color )
{
	
	std::vector<Frisbee>::iterator iter;
	for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
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
EntityContainer::CollisionPacket FrisbeeContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 1;
	
	std::vector<Frisbee>::iterator iter;
	for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
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
void FrisbeeContainer::KillAllEntities()
{
	std::vector<Frisbee>::iterator iter;
	for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
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
void FrisbeeContainer::Spawn( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist, int delay, int frame, TurretArray &Turrets )
{
	
	std::vector<Frisbee>::iterator iter;
	for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = frame;
			iter->Counter = 0;
			iter->NewCounter = 0;
			iter->Delay = delay;
			iter->Hp = FRISBEE_MAX_HP;
			iter->LoadPath( _points, _lut, LutSize, rate, totaldist );
		
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/16;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 50;			// 
			Tp.BulletsPerShot	= 1;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 8;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SINGLE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(3);
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 0;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 7;
			Tp.Bp.Height = 7;
			Tp.Bp.Type = Bullet::DECEL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPosition, 
						   &iter->TurretTarget,
						   iter->TurretPtr
	  		             );
	
			iter->TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}

