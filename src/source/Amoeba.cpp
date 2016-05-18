/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Amoeba class


*******************************************************************************
******************************************************************************/

#include "Amoeba.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_AMOEBAS 32
#define AMOEBA_MAX_HP 2

//------------------------------------------------------------------------------
//	Amoeba Enemy
//------------------------------------------------------------------------------
Amoeba::Amoeba()
{
	Frame = 0;
	Active = false;	
	Hp = AMOEBA_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 20;
	Height = 20;
	FlipMode = GL_FLIP_NONE;
	
}

Amoeba::~Amoeba()
{
}

//--------------------------------------
//	
//--------------------------------------
void Amoeba::Update( PlayerShip &Vic )
{
	
	Counter++;

	//iprintf("\x1b[1;1HNewTarget = %i         ", NewTarget );
			
	
	
	
	if( (Counter == 100) )
	{
			TurretPtr->Activate();
			TurretPtr->DoPattern(0);
	}
	
	if( Counter >= (64 * 3) )
	{
		Target.x = -400 << 12;
		Target.y = Ey;
		NewTarget = false;
	}
	else
	{
		if( (Counter & 31) == 0 )
		{
			NewTarget = true;
		}
		
	}
	
		
	if(NewTarget)
	{
		Target.x = Vic.GetX();
		Target.y = Vic.GetY();
		NewTarget = false;
	}
	
	Dx = mulf32(cosLerp(Angle), Speed);
	Dy = mulf32(sinLerp(Angle), Speed);
	
	x += Dx;
	y += Dy;
	
	
	
	// Project normal
	s32 dot = (mulf32(Dy , (Target.x - x)) - mulf32(Dx, (Target.y - y)));
	
	if( dot > 0)
		Angle = (Angle - Delta);
	else
		Angle = (Angle + Delta);
	
	
	if( TurretPtr->GetShotCounter() == 1 )   // Deactivate
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
		Frame = Frame + 1;
		if( Frame > 5 ) Frame = 0;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Amoeba::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  Dx,
								  Dy,
								  Explode::MEDIUM_3 );			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
	
}

//--------------------------------------
//	
//--------------------------------------
void Amoeba::Kill()
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
//	Amoeba Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
AmoebaContainer::AmoebaContainer()
{
}
	
AmoebaContainer::~AmoebaContainer()
{
	Amoebas.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void AmoebaContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Amoebas.resize( MAX_AMOEBAS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void AmoebaContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Amoeba>::iterator iter;
	for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void AmoebaContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Amoeba>::iterator iter;
	for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      iter->FlipMode, &Images[iter->Frame + 10] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 10, (iter->y >> 12) - 10, 
					      iter->FlipMode, &Images[iter->Frame + 10] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void AmoebaContainer::DrawAABB( int color )
{
	
	std::vector<Amoeba>::iterator iter;
	for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
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
EntityContainer::CollisionPacket AmoebaContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<Amoeba>::iterator iter;
	for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
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
void AmoebaContainer::KillAllEntities()
{
	std::vector<Amoeba>::iterator iter;
	for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
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
void AmoebaContainer::Spawn( int sy, int ey, TurretArray &Turrets )
{
	
	std::vector<Amoeba>::iterator iter;
	for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->x = 300 << 12;
			iter->y = sy << 12;
			iter->Ey = ey << 12;
			iter->Dx = 0;
			iter->Dy = 0;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = AMOEBA_MAX_HP;
		
			iter->Speed = floattof32(1.5);
			iter->Angle = 0;
			iter->Delta = 256;
			iter->Target.x = UTIL::RND_RANGE(0,255) << 12;
			iter->Target.y = UTIL::RND_RANGE(0,191) << 12;
			iter->NewTarget = true;
			
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
			Tp.MaxSpawnDelay	= 60;			// 
			Tp.BulletsPerShot	= 1;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 2;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SINGLE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 384;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -20;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 28;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
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

