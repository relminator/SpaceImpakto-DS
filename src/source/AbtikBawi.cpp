/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	AbtikBawi class


*******************************************************************************
******************************************************************************/

#include "AbtikBawi.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_ABTIKBAWIS 8
#define ABTIKBAWI_MAX_HP 128

//------------------------------------------------------------------------------
//	AbtikBawi Enemy
//------------------------------------------------------------------------------
AbtikBawi::AbtikBawi()
{
	Frame = 0;
	Active = false;	
	Hp = ABTIKBAWI_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 32;
	Height = 32;
	Angle = 0;
	
}

AbtikBawi::~AbtikBawi()
{
	
}

//--------------------------------------
//	
//--------------------------------------
void AbtikBawi::Update( PlayerShip &Vic )
{
	bool KillMe = false;
	
	Counter++;
	
	//iprintf("\x1b[1;1HTime = %i         ", Path.GetTime() );
	//iprintf("\x1b[2;1HDistance = %i         ", Path.GetLinearDistance()>>12 );
			
	if( Counter > Delay )
	{
		if( (Path.GetLinearDistance() >> 12) == 202 )
		{
			Path.SetRate( 0 );
			if( !TurretPtr->IsActivated() ) TurretPtr->Activate();
		}
		
		if( TurretPtr->GetIndex() < 1 )
		{
			if( TurretPtr->GetShotCounter() == 3 )
			{
				Path.SetRate( inttof32(2) );
				TurretPtr->NextPattern();
			}
		}
		else
		{
			if( TurretPtr->GetShotCounter() == 2 )
			{
				TurretPtr->Deactivate();
			}
		}
		
		KillMe = Path.Update();
		x = Path.GetX();
		y = Path.GetY();
	}
	else
	{
		x = 300 << 12;
		y = 96 << 12;		
	}
	
	y += OffsetY;
	
	if( TurretPtr->GetShotCounter() == 8 )   // stop shooting
	{
		TurretPtr->Deactivate();
	}
	
	if( KillMe )
	{
		Kill();
		return;
	}
	
	if( Path.GetRate() == 0 )
	{
		s32 Dx = cosLerp(Angle);
		s32 Dy = sinLerp(Angle);
		s32 dot = (mulf32(Dy ,  x - (Vic.GetX())) - mulf32(Dx, (y - Vic.GetY())));
	
		if( abs(dot) > (4096*2) )
		{
			if( dot > 0)
				Angle = (Angle - 128);
			else
				Angle = (Angle + 128);
		}
		else
		{
			Angle = BRAD_PI + atan2Lerp( Vic.GetX() - x, Vic.GetY()-y );
		}
	}				
	else
	{
		Angle = BRAD_PI + atan2Lerp( Path.GetDx(), Path.GetDy() );
	}				
	
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPosition.x = x;
	TurretPosition.y = y;
	
	// Target where head is facing
	TurretTarget.x = Vic.GetX();
	TurretTarget.y = Vic.GetY();
	
	// animate
	//if( ( Counter & 3 ) == 0 )
	//{
		Frame = 0; //(Frame + 1) & 3;
	//}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void AbtikBawi::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
	
}

//--------------------------------------
//	
//--------------------------------------
void AbtikBawi::Kill()
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
void AbtikBawi::LoadPath( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist )
{
	
	Path.Init( _points, _lut, LutSize, rate, totaldist );

	Path.SetRate( floattof32(1.50) );
	
}
	
//------------------------------------------------------------------------------
//	AbtikBawi Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
AbtikBawiContainer::AbtikBawiContainer()
{
}
	
AbtikBawiContainer::~AbtikBawiContainer()
{
	AbtikBawis.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void AbtikBawiContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	AbtikBawis.resize( MAX_ABTIKBAWIS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void AbtikBawiContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<AbtikBawi>::iterator iter;
	for( iter = AbtikBawis.begin(); iter != AbtikBawis.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void AbtikBawiContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<AbtikBawi>::iterator iter;
	for( iter = AbtikBawis.begin(); iter != AbtikBawis.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSpriteRotate( (iter->x >> 12), (iter->y >> 12),
						  iter->Angle,
					      GL_FLIP_NONE, &Images[65+iter->Frame] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSpriteRotate( (iter->x >> 12), (iter->y >> 12),
						  iter->Angle,
					      GL_FLIP_NONE, &Images[65+iter->Frame] 
					    );
					
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void AbtikBawiContainer::DrawAABB( int color )
{
	
	std::vector<AbtikBawi>::iterator iter;
	for( iter = AbtikBawis.begin(); iter != AbtikBawis.end(); ++iter )
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
EntityContainer::CollisionPacket AbtikBawiContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<AbtikBawi>::iterator iter;
	for( iter = AbtikBawis.begin(); iter != AbtikBawis.end(); ++iter )
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
void AbtikBawiContainer::KillAllEntities()
{
	std::vector<AbtikBawi>::iterator iter;
	for( iter = AbtikBawis.begin(); iter != AbtikBawis.end(); ++iter )
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
void AbtikBawiContainer::Spawn( Bezier::Coord* _points, 
								Bezier::Table* _lut, 
								int LutSize, 
								s32 rate, s32 
								totaldist, 
								int delay, 
								int _yoffset, 
								TurretArray &Turrets  )
{
	
	std::vector<AbtikBawi>::iterator iter;
	for( iter = AbtikBawis.begin(); iter != AbtikBawis.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Delay = delay;
			iter->Hp = ABTIKBAWI_MAX_HP;
			iter->LoadPath( _points, _lut, LutSize, rate, totaldist );
			iter->OffsetY = _yoffset << 12;
			
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 16;				// 
			Tp.Yradius			= 16;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI*2/8;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 30;			// 
			Tp.BulletsPerShot	= 1;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 3;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SINGLE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)/2;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 25;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPosition, 
						   &iter->TurretTarget,
						   iter->TurretPtr
	  		             );
	
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 16;				// 
			Tp.Yradius			= 16;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI*2/8;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 20;			// 
			Tp.BulletsPerShot	= 8;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_RINGS;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)/2;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 10;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 16;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			iter->TurretPtr->AddPattern( Tp );
			
			// Deactivate
			iter->TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}


