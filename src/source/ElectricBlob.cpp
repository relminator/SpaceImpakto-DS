/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	ElectricBlob class


*******************************************************************************
******************************************************************************/

#include "ElectricBlob.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_ELECTRICBLOBS 8
#define ELECTRICBLOB_MAX_HP 16

//------------------------------------------------------------------------------
//	ElectricBlob Enemy
//------------------------------------------------------------------------------
ElectricBlob::ElectricBlob()
{
	Frame = 0;
	Active = false;	
	Hp = ELECTRICBLOB_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 20;
	Height = 20;
	
}

ElectricBlob::~ElectricBlob()
{
}

//--------------------------------------
//	
//--------------------------------------
void ElectricBlob::Update( PlayerShip &Vic )
{
	
	Counter++;
	Angle += 512;
	
	x += Dx;
	y = Sy + sinLerp( Angle ) * 16;
	
	if( Counter == 256 )
	{
		TurretPtr[0]->Activate();
		TurretPtr[1]->Activate();
	}
	
	for( int i = 0; i < 2; i++)
	{
		if( TurretPtr[i]->GetShotCounter() == 3 )   // stop shooting
		{
			TurretPtr[i]->Deactivate();
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
void ElectricBlob::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::SMALL_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
	
}

//--------------------------------------
//	
//--------------------------------------
void ElectricBlob::Kill()
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
//	ElectricBlob Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
ElectricBlobContainer::ElectricBlobContainer()
{
}
	
ElectricBlobContainer::~ElectricBlobContainer()
{
	ElectricBlobs.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void ElectricBlobContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	ElectricBlobs.resize( MAX_ELECTRICBLOBS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void ElectricBlobContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<ElectricBlob>::iterator iter;
	for( iter = ElectricBlobs.begin(); iter != ElectricBlobs.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void ElectricBlobContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<ElectricBlob>::iterator iter;
	for( iter = ElectricBlobs.begin(); iter != ElectricBlobs.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      GL_FLIP_NONE, &Images[iter->Frame + 32] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      GL_FLIP_NONE, &Images[iter->Frame + 32] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void ElectricBlobContainer::DrawAABB( int color )
{
	
	std::vector<ElectricBlob>::iterator iter;
	for( iter = ElectricBlobs.begin(); iter != ElectricBlobs.end(); ++iter )
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
EntityContainer::CollisionPacket ElectricBlobContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<ElectricBlob>::iterator iter;
	for( iter = ElectricBlobs.begin(); iter != ElectricBlobs.end(); ++iter )
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
void ElectricBlobContainer::KillAllEntities()
{
	std::vector<ElectricBlob>::iterator iter;
	for( iter = ElectricBlobs.begin(); iter != ElectricBlobs.end(); ++iter )
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
void ElectricBlobContainer::Spawn( int _y, s32 angle, TurretArray &Turrets )
{
	
	std::vector<ElectricBlob>::iterator iter;
	for( iter = ElectricBlobs.begin(); iter != ElectricBlobs.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->x = 300 << 12;
			iter->Dx = -floattof32(0.25);
			iter->Sy = _y << 12;
			iter->Angle = angle;
			
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = ELECTRICBLOB_MAX_HP;
			
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
			Tp.MaxSpawnDelay	= 120;			// 
			Tp.BulletsPerShot	= 6;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_CENTERED;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)/4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 34;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr[0]
	  		             );
	
			
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
			Tp.SpawnDelay		= 60;				// 
			Tp.MaxSpawnDelay	= 120;			// 
			Tp.BulletsPerShot	= 2;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_CENTERED;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)/4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 1;
			Tp.Bp.BaseFrame = 24;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
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

