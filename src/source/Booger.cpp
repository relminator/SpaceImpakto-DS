/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Booger class


*******************************************************************************
******************************************************************************/

#include "Booger.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_BOOGERS 4
#define BOOGER_MAX_HP 712

//------------------------------------------------------------------------------
//	Booger Enemy
//------------------------------------------------------------------------------
Booger::Booger()
{
	Frame = 0;
	Active = false;	
	Hp = BOOGER_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 32;
	Height = 32;
	
}

Booger::~Booger()
{
}

//--------------------------------------
//	
//--------------------------------------
void Booger::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	Path.Update();
	
	switch( Path.GetIndex() )
	{
		case 0:
			break;
		case 2:
			for( int i = 0; i < 2; i++ )
			{
				TurretPtr[i]->Activate();
				TurretPtr[i]->DoPattern(0);			
			}
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			for( int i = 0; i < 2; i++ )
			{
				TurretPtr[i]->Deactivate();
			}
			break;
		case 7:
			TurretPtr[0]->Activate();
			TurretPtr[0]->DoPattern(1);
		break;
		case 9:
			TurretPtr[0]->Deactivate();
			TurretPtr[1]->Deactivate();
			break;
	}
	
	x = Path.GetX();
	y = Path.GetY();
	
	if( x < MIN_X )
	{
		Kill();
		return;
	}
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPosition.x = x;
	TurretPosition.y = y;
	
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
void Booger::Destroy()
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
void Booger::Kill()
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

//--------------------------------------
//	
//--------------------------------------
void Booger::LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate )
{
	int neger = 0;
	if( negate )
	{
		neger = 192;
	}
	Path.Init( 16 );
	
	Spline::Coord p;
	
	// start
	p.x = _sx;
	p.y = _sy;
	
	// we should use 2 points
	Path.AddPoint( p ); //1
	Path.AddPoint( p ); //2
	
	p.x = 200;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //3
	
	p.x = 200;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //4
	
	p.x = 200;
	p.y = abs(neger - 150);
	Path.AddPoint( p ); //5
	
	p.x = 180;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //6
	
	p.x = 220;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //7
	
	p.x = 220;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //8
	
	p.x = 200;
	p.y = abs(neger - 75);
	Path.AddPoint( p ); //9
	
	p.x = 200;
	p.y = abs(neger - 120);
	Path.AddPoint( p ); //10
	
	p.x = 200;
	p.y = abs(neger - 170);
	Path.AddPoint( p ); //11
	
	p.x = 220;
	p.y = abs(neger - 170);
	Path.AddPoint( p ); //12
	
	
	// close last 2 coords
	p.x = _ex;
	p.y = _ey;
	
	// we should use 2 points
	Path.AddPoint( p );  //13
	Path.AddPoint( p );  //14
	
	Path.SetSpeed(32);
		
	Path.SetT(0);
	
}
	
//------------------------------------------------------------------------------
//	Booger Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BoogerContainer::BoogerContainer()
{
}
	
BoogerContainer::~BoogerContainer()
{
	Boogers.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void BoogerContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Boogers.resize( MAX_BOOGERS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BoogerContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Booger>::iterator iter;
	for( iter = Boogers.begin(); iter != Boogers.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BoogerContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Booger>::iterator iter;
	for( iter = Boogers.begin(); iter != Boogers.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 16, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[iter->Frame] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 16, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[iter->Frame] 
					    );
					
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void BoogerContainer::DrawAABB( int color )
{
	
	std::vector<Booger>::iterator iter;
	for( iter = Boogers.begin(); iter != Boogers.end(); ++iter )
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
EntityContainer::CollisionPacket BoogerContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 3;
	
	std::vector<Booger>::iterator iter;
	for( iter = Boogers.begin(); iter != Boogers.end(); ++iter )
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
			if( iter->x < ( 256 << 12 ) )
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
			}  // end if
		}
	}
	
	return Ret;
	
}

//--------------------------------------
//	
//--------------------------------------
void BoogerContainer::KillAllEntities()
{
	std::vector<Booger>::iterator iter;
	for( iter = Boogers.begin(); iter != Boogers.end(); ++iter )
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
void BoogerContainer::Spawn( int _x, int _y, bool Below, TurretArray &Turrets )
{
	
	
	std::vector<Booger>::iterator iter;
	for( iter = Boogers.begin(); iter != Boogers.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = BOOGER_MAX_HP;
			iter->LoadPath( _x, _y, -300, 96, Below );
			
			Turret::Params Tp;
	
			// 1st turret, 1st pattern
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/6;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 60;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 5;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.02150);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_OFFSET;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)/8;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 28;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPosition, 
						   &iter->TurretTarget,
						   iter->TurretPtr[0]
	  		             );
	
			// 1st turret, 2nd pattern
			
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= BRAD_PI / 2;				// 
			Tp.MaxAngle			= BRAD_PI * 2 - BRAD_PI / 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;			// 
			Tp.MaxSpawnDelay	= 180;			// 
			Tp.BulletsPerShot	= 9;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 5;			// maximum bullets per wave
			Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(1.2);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.5);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_OFFSET;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.35)*2;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 1;
			Tp.Bp.BaseFrame = 4;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 16;
			Tp.Bp.Height = 16;
			Tp.Bp.Type = Bullet::DECEL;
			Tp.Bp.AniMode = Bullet::ROTATED;
			
			// add pattern
			iter->TurretPtr[0]->AddPattern( Tp );
			
			
			// 2nd turret
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 16;				// 
			Tp.Yradius			= 16;				// 
			Tp.Angle			= 0;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI*2/8;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 128;				// 
			Tp.MaxSpawnDelay	= 180;			// 
			Tp.BulletsPerShot	= 8;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 2;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 30;			// maximum delay for next shot in a wave
			
			
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
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPosition, 
						   &iter->TurretTarget,
						   iter->TurretPtr[1]
	  		             );
						 
			iter->TurretPtr[0]->Deactivate();
			iter->TurretPtr[1]->Deactivate();
			
	
			break;
		
		}
	}
	
	
}

