/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	LightBall class


*******************************************************************************
******************************************************************************/

#include "LightBall.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_LIGHTBALLS 4
#define LIGHTBALL_MAX_HP 512

//------------------------------------------------------------------------------
//	LightBall Enemy
//------------------------------------------------------------------------------
LightBall::LightBall()
{
	Frame = 0;
	Active = false;	
	Hp = LIGHTBALL_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 40;
	Height = 30;
	FlipMode = GL_FLIP_NONE;
	
}

LightBall::~LightBall()
{
}

//--------------------------------------
//	
//--------------------------------------
void LightBall::Update( PlayerShip &Vic )
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
		if( TurretPtr->GetShotCounter() == 2 )   // Change Pattern
			TurretPtr->NextPattern();
	}
	
	
	
	if( TurretPtr->GetShotCounter() == 40 )   // stop shooting
	{
		TurretPtr->Deactivate();
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
void LightBall::Destroy()
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
void LightBall::Kill()
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
//	LightBall Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
LightBallContainer::LightBallContainer()
{
}
	
LightBallContainer::~LightBallContainer()
{
	LightBalls.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void LightBallContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	LightBalls.resize( MAX_LIGHTBALLS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void LightBallContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<LightBall>::iterator iter;
	for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void LightBallContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<LightBall>::iterator iter;
	for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 20, (iter->y >> 12) - 15, 
					      iter->FlipMode, &Images[iter->Frame + 16] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 20, (iter->y >> 12) - 15, 
					      iter->FlipMode, &Images[iter->Frame + 16] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void LightBallContainer::DrawAABB( int color )
{
	
	std::vector<LightBall>::iterator iter;
	for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
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
EntityContainer::CollisionPacket LightBallContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 3;
	
	std::vector<LightBall>::iterator iter;
	for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
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
void LightBallContainer::KillAllEntities()
{
	std::vector<LightBall>::iterator iter;
	for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
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
void LightBallContainer::Spawn( bool Below, TurretArray &Turrets )
{
	
	std::vector<LightBall>::iterator iter;
	for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
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
				iter->y = inttof32(192-15);
			}
			else
			{
				iter->FlipMode = GL_FLIP_V;
				iter->y = inttof32(15);
			}
			
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = LIGHTBALL_MAX_HP;
			
			Turret::Params Tp;
	
			// 1
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI+BRAD_PI/2;					// StartAngle
			Tp.MinAngle			= BRAD_PI;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 3776;				// + rotation speed
			Tp.SpreadAngle		= 5967;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 180;			// 
			Tp.BulletsPerShot	= 6;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 16;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 1320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_WAVE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = floattof32(0.042);
			Tp.Bp.Speed = floattof32(0.75)*4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 16;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 7;
			Tp.Bp.Height = 7;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			if( !Below )
			{
				Tp.Angle			= BRAD_PI/2;					// StartAngle
				Tp.MinAngle			= 0;				// 
				Tp.MaxAngle			= BRAD_PI;
				Tp.Bp.Gy = -floattof32(0.042);
			}
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr
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
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 28;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
	
			iter->TurretPtr->AddPattern( Tp );
			
			// Deactivate
			iter->TurretPtr->Deactivate();
			break;
		
		}
	}
	
	
}

