/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Alienator class


*******************************************************************************
******************************************************************************/

#include "Alienator.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_ALIENATORS 4
#define ALIENATOR_MAX_HP 1000

//------------------------------------------------------------------------------
//	Alienator Enemy
//------------------------------------------------------------------------------
Alienator::Alienator()
{
	Frame = 0;
	Active = false;	
	Hp = ALIENATOR_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 64;
	Height = 32;
	
}

Alienator::~Alienator()
{
}

//--------------------------------------
//	
//--------------------------------------
void Alienator::Update( PlayerShip &Vic )
{
	
	Counter++;

	
	Path.Update();
	
	//iprintf( "\x1b[1;1HSplineIndex = %i         ", Path.GetIndex() );
	//iprintf( "\x1b[2;1HSplineMAXIndex = %i         ", Path.GetMaxPoints() );
	//iprintf( "\x1b[2;1HSplineTime = %i         ", Path.GetTime() );
	//iprintf( "\x1b[3;1HCounter = %i         ", Counter );
	//iprintf( "\x1b[5;1HShotCounter = %i         ", TurretPtr->GetShotCounter() );
	//iprintf( "\x1b[6;1HShotPattern = %i         ", TurretPtr->GetIndex() );
	//iprintf( "\x1b[7;1HShotNumPatterns = %i         ", TurretPtr->GetNumPatterns() );
	
	switch( Path.GetIndex() )
	{
		case 0:
			break;
		case 2:
			TurretPtr->Activate();
			TurretPtr->DoPattern(0);
			break;
		case 4:
			TurretPtr->DoPattern(1);
			break;
		case 5:
			break;
		case 6:
			TurretPtr->DoPattern(2);
			break;
		case 8:
			TurretPtr->DoPattern(0);
			break;
		case 9:
			TurretPtr->Deactivate();
			break;
	}
	
	x = Path.GetX();
	y = Path.GetY();
	
	
	if( Path.GetIndex() == (Path.GetMaxPoints()-2) )
	{
		Kill();
		return;
	}
	
	
	
	Targets::Instance()->AddCoord( x, y );
	
	TurretPos.x = x;
	TurretPos.y = y;
	
	
	TurretTar.x = Vic.GetX();
	TurretTar.y = Vic.GetY();
	
	if( (Path.GetIndex() >= 6) )
	{
		TurretTar.x = TurretPos.x;
		TurretTar.y = TurretPos.y;
	}
	// animate
	Frame = 0;
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Alienator::Destroy()
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
void Alienator::Kill()
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
void Alienator::LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate )
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
	p.y = abs(neger - 30);
	Path.AddPoint( p ); //3
	
	p.x = 200;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //4
	
	p.x = 200;
	p.y = abs(neger - 30);
	Path.AddPoint( p ); //5
	
	p.x = 180;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //6
	
	p.x = 200;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //7
	
	p.x = 200;
	p.y = abs(neger - 126);
	Path.AddPoint( p ); //8
	
	p.x = 180;
	p.y = abs(neger - 126);
	Path.AddPoint( p ); //9
	
	p.x = 200;
	p.y = abs(neger - 70);
	Path.AddPoint( p ); //10
	
	p.x = 200;
	p.y = abs(neger - 70);
	Path.AddPoint( p ); //11
	
	p.x = 150;
	p.y = abs(neger - 70);
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
//	Alienator Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
AlienatorContainer::AlienatorContainer()
{
}
	
AlienatorContainer::~AlienatorContainer()
{
	Alienators.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void AlienatorContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Alienators.resize( MAX_ALIENATORS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void AlienatorContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Alienator>::iterator iter;
	for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void AlienatorContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Alienator>::iterator iter;
	for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 32, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[iter->Frame + 51] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 32, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[iter->Frame + 51] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void AlienatorContainer::DrawAABB( int color )
{
	
	std::vector<Alienator>::iterator iter;
	for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
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
EntityContainer::CollisionPacket AlienatorContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 3;
	
	std::vector<Alienator>::iterator iter;
	for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
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
						IsDestroyed = true;
						Vic.AddToScore( iter->DeathScore );
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
void AlienatorContainer::KillAllEntities()
{
	std::vector<Alienator>::iterator iter;
	for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
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
void AlienatorContainer::Spawn( int _x, int _y, bool Below, TurretArray &Turrets )
{
	
	int ey;
	std::vector<Alienator>::iterator iter;
	for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = ALIENATOR_MAX_HP;
			
			if( !Below )
			{
				ey = 0;
			}
			else
			{
				ey = 192;
			}
			
			iter->LoadPath( _x, _y, -30, ey, Below );
			
			
			// 1st pattern
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= -5;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= BRAD_PI/2;				// 
			Tp.MaxAngle			= BRAD_2PI - BRAD_PI/2;
			Tp.AngleDelta		= 1024;				// + rotation speed
			Tp.SpreadAngle		= 8192;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 180;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 24;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 6;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 150;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_WAVE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.15);
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 25;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 0;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 7;
			Tp.Bp.Height = 7;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			if( Below ) Tp.AngleDirection	= -1;
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr
	  		             );
	
			
			
	
			
			// 2nd
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 5;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 1536;				// + rotation speed
			Tp.SpreadAngle		= BRAD_2PI/8;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 160;			// 
			Tp.BulletsPerShot	= 8;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 2;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 20;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 160;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_RINGS;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 224;
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
			
			// 3rd
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= 0;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 1200;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/12;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 180;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 20;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.25);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_WAVE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = inttof32(1)*2;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 12;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 5;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 12;
			Tp.Bp.Height = 12;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			if( Below ) Tp.AngleDirection	= -1;
			iter->TurretPtr->AddPattern( Tp );
			
			
			// Deactivate
			iter->TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}

