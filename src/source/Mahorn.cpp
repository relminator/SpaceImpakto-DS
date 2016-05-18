/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Mahorn class


*******************************************************************************
******************************************************************************/

#include "Mahorn.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_MAHORNS 4
#define MAHORN_MAX_HP 512

//------------------------------------------------------------------------------
//	Mahorn Enemy
//------------------------------------------------------------------------------
Mahorn::Mahorn()
{
	Frame = 0;
	Active = false;	
	Hp = MAHORN_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 40;
	Height = 60;
	
}

Mahorn::~Mahorn()
{
}

//--------------------------------------
//	
//--------------------------------------
void Mahorn::Update( PlayerShip &Vic )
{
	
	Counter++;

	
	if( (Counter >= 200) && (Counter <= 700) )
	{
		Dx = 0;
		Dy = 0;
		if( Counter == 210 )
		{
			for( int i = 0; i < 2; i++)
			{
				TurretPtr[i]->Activate();
			}	
		}
	
	}
	else
	{
		if( Counter == 710)
		Dx = -floattof32(1);
	}
	
	x += Dx;
	y += Dy;
	
	
	for( int i = 0; i < 2; i++)
	{
		if( TurretPtr[i]->GetIndex() == 0 )
		{
			if( TurretPtr[i]->GetShotCounter() == 4 )   // Change Pattern
				TurretPtr[i]->NextPattern();
		}
	}
		
	
	
	if( TurretPtr[0]->GetShotCounter() == 40 )   // stop shooting
	{
		TurretPtr[0]->Deactivate();
	}
	
	if( TurretPtr[1]->GetShotCounter() == 40 )   // stop shooting
	{
		TurretPtr[1]->Deactivate();
	}

	if( x < MIN_X )
	{
		Kill();
		return;
	}
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPos[0].x = x;
	TurretPos[0].y = y - inttof32(24);
	
	TurretPos[1].x = x;
	TurretPos[1].y = y + inttof32(24);
	
	TurretTar.x = Vic.GetX();
	TurretTar.y = Vic.GetY();
	

	// animate
	if( ( Counter & 3 ) == 0 )
	{
		Frame = (Frame + 1) & 1;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Mahorn::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::SMALL_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_MED2 );
	
}

//--------------------------------------
//	
//--------------------------------------
void Mahorn::Kill()
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
//	Mahorn Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
MahornContainer::MahornContainer()
{
}
	
MahornContainer::~MahornContainer()
{
	Mahorns.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void MahornContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Mahorns.resize( MAX_MAHORNS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void MahornContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Mahorn>::iterator iter;
	for( iter = Mahorns.begin(); iter != Mahorns.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void MahornContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Mahorn>::iterator iter;
	for( iter = Mahorns.begin(); iter != Mahorns.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 20, (iter->y >> 12) - 30, 
					      GL_FLIP_NONE, &Images[iter->Frame + 66] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 20, (iter->y >> 12) - 30, 
					      GL_FLIP_NONE, &Images[iter->Frame + 66] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void MahornContainer::DrawAABB( int color )
{
	
	std::vector<Mahorn>::iterator iter;
	for( iter = Mahorns.begin(); iter != Mahorns.end(); ++iter )
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
EntityContainer::CollisionPacket MahornContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 3;
	
	std::vector<Mahorn>::iterator iter;
	for( iter = Mahorns.begin(); iter != Mahorns.end(); ++iter )
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
void MahornContainer::KillAllEntities()
{
	std::vector<Mahorn>::iterator iter;
	for( iter = Mahorns.begin(); iter != Mahorns.end(); ++iter )
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
void MahornContainer::Spawn( bool Below, TurretArray &Turrets )
{
	
	std::vector<Mahorn>::iterator iter;
	for( iter = Mahorns.begin(); iter != Mahorns.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->x = 220 << 12;
			iter->Dx = 0;
			if( Below )
			{
				iter->y = inttof32(192+40);
				iter->Dy = -floattof32(0.5);
			}
			else
			{
				iter->y = inttof32(-40);
				iter->Dy = floattof32(0.5);
			}
			
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = MAHORN_MAX_HP;
			
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
			Tp.MaxSpawnDelay	= 16;			// 
			Tp.BulletsPerShot	= 1;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 4;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(3.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_OFFSET;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)*4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 11;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos[0], 
						   &iter->TurretTar,
						   iter->TurretPtr[0]
	  		             );
	
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos[1], 
						   &iter->TurretTar,
						   iter->TurretPtr[1]
	  		             );
	
	
			// 2nd wave
			
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 5;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 3776;				// + rotation speed
			Tp.SpreadAngle		= 6656;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 2;			// 
			Tp.BulletsPerShot	= 3;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_NORMAL;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 384/256;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 0;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
	
			iter->TurretPtr[0]->AddPattern( Tp );
			
			Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			iter->TurretPtr[1]->AddPattern( Tp );
	
			// Deactivate
			iter->TurretPtr[0]->Deactivate();
			iter->TurretPtr[1]->Deactivate();
	
			break;
		
		}
	}
	
	
}

