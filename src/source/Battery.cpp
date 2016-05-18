/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Battery class


*******************************************************************************
******************************************************************************/

#include "Battery.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_BATTERYS 4
#define BATTERY_MAX_HP 512

//------------------------------------------------------------------------------
//	Battery Enemy
//------------------------------------------------------------------------------
Battery::Battery()
{
	Frame = 0;
	Active = false;	
	Hp = BATTERY_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 48;
	Height = 24;
	FlipMode = GL_FLIP_NONE;
	
}

Battery::~Battery()
{
}

//--------------------------------------
//	
//--------------------------------------
void Battery::Update( PlayerShip &Vic )
{
	
	Counter++;

	//iprintf("\x1b[1;1HShotCounter = %i         ", TurretPtr->GetShotCounter() );
			
	
	if( (Counter >= 200) && (Counter <= 700) )
	{
		Dx = 0;
		Dy = 0;
		if( Counter == 210 )
		{
			TurretPtr->Activate();
			TurretPtr->DoPattern(0);
		}
		
		
	}
	else
	{
		if( Counter == 710)
		Dx = -floattof32(1);
	}
	
	x += Dx;
	y += Dy;
	
	
	if( TurretPtr->GetIndex() == 0 )
	{
		if( TurretPtr->GetShotCounter() == 4 )   // Change Pattern
			TurretPtr->NextPattern();
	}
	else
	{
		if( TurretPtr->GetShotCounter() == 3 )   // stop shooting
		{
			TurretPtr->Deactivate();
		}
		
	}
	
	
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
		Frame = Frame + 1;
		if( Frame > 2 ) Frame = 0;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Battery::Destroy()
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
void Battery::Kill()
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
//	Battery Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BatteryContainer::BatteryContainer()
{
}
	
BatteryContainer::~BatteryContainer()
{
	Batterys.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void BatteryContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Batterys.resize( MAX_BATTERYS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BatteryContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Battery>::iterator iter;
	for( iter = Batterys.begin(); iter != Batterys.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BatteryContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Battery>::iterator iter;
	for( iter = Batterys.begin(); iter != Batterys.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 24, (iter->y >> 12) - 12, 
					      iter->FlipMode, &Images[iter->Frame + 59] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 24, (iter->y >> 12) - 12, 
					      iter->FlipMode, &Images[iter->Frame + 59] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BatteryContainer::DrawAABB( int color )
{
	
	std::vector<Battery>::iterator iter;
	for( iter = Batterys.begin(); iter != Batterys.end(); ++iter )
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
EntityContainer::CollisionPacket BatteryContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<Battery>::iterator iter;
	for( iter = Batterys.begin(); iter != Batterys.end(); ++iter )
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
void BatteryContainer::KillAllEntities()
{
	std::vector<Battery>::iterator iter;
	for( iter = Batterys.begin(); iter != Batterys.end(); ++iter )
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
void BatteryContainer::Spawn( bool Below, TurretArray &Turrets )
{
	
	std::vector<Battery>::iterator iter;
	for( iter = Batterys.begin(); iter != Batterys.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->x = 300 << 12;
			iter->Dx = -floattof32(0.5);
			iter->Dy = 0;
			if( Below )
			{
				iter->FlipMode = GL_FLIP_NONE;
				iter->y = inttof32(192-12);
			}
			else
			{
				iter->FlipMode = GL_FLIP_V;
				iter->y = inttof32(12);
			}
			
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = BATTERY_MAX_HP;
			
			Turret::Params Tp;
	
			// 1
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 3776;				// + rotation speed
			Tp.SpreadAngle		= 6656;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 30;			// 
			Tp.BulletsPerShot	= 24;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 2;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_NORMAL;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 384;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 20;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 31;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			if( !Below )
			{
				Tp.AngleDirection	= -1;
			}
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr
	  		             );
	
	
			// 2nd wave
			
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI+BRAD_PI/2;					// StartAngle
			Tp.MinAngle			= BRAD_PI;				// 
			Tp.MaxAngle			= BRAD_2PI-BRAD_PI/2;
			Tp.AngleDelta		= 4096;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/10;			// distance of each bullet on a spread
			Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 180;				// 
			Tp.MaxSpawnDelay	= 30;			// 
			Tp.BulletsPerShot	= 5;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 26;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 1;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_WAVE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.5)*12;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 5;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 8;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 20;
			Tp.Bp.Height = 20;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
	
			iter->TurretPtr->AddPattern( Tp );
			
			// Deactivate
			iter->TurretPtr->Deactivate();
			break;
		
		}
	}
	
	
}

