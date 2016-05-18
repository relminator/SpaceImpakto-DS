/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Phoenix class


*******************************************************************************
******************************************************************************/

#include "Phoenix.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_PHOENIXS 4
#define PHOENIX_MAX_HP 512

//------------------------------------------------------------------------------
//	Phoenix Enemy
//------------------------------------------------------------------------------
Phoenix::Phoenix()
{
	Frame = 0;
	Active = false;	
	Hp = PHOENIX_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 48;
	Height = 32;
	
}

Phoenix::~Phoenix()
{
}

//--------------------------------------
//	
//--------------------------------------
void Phoenix::Update( PlayerShip &Vic )
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
			if( !TurretPtr->IsActivated() ) TurretPtr->Activate();
			break;
		case 3:
			TurretPtr->DoPattern(1);
			break;
		case 5:
			TurretPtr->DoPattern(2);
			break;
		case 9:
			if( TurretPtr->IsActivated() ) TurretPtr->Deactivate();
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
	

	// animate
	if( ( Counter & 3 ) == 0 )
	{
		Frame++;
		if( Frame > 2) Frame = 0;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Phoenix::Destroy()
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
void Phoenix::Kill()
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
void Phoenix::LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate )
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
	
	p.x = 230;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //3
	
	p.x = 130;
	p.y = abs(neger - 20);
	Path.AddPoint( p ); //4
	
	p.x = 230;
	p.y = abs(neger - 180);
	Path.AddPoint( p ); //5
	
	p.x = 130;
	p.y = abs(neger - 20);
	Path.AddPoint( p ); //6
	
	p.x = 230;
	p.y = abs(neger - 180);
	Path.AddPoint( p ); //7
	
	p.x = 180;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //8
	
	p.x = 180;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //9
	
	p.x = 250;
	p.y = abs(neger - 70);
	Path.AddPoint( p ); //10
	
	p.x = 160;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //11
	
	p.x = 160;
	p.y = abs(neger - 40);
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
//	Phoenix Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
PhoenixContainer::PhoenixContainer()
{
}
	
PhoenixContainer::~PhoenixContainer()
{
	Phoenixs.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void PhoenixContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Phoenixs.resize( MAX_PHOENIXS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void PhoenixContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Phoenix>::iterator iter;
	for( iter = Phoenixs.begin(); iter != Phoenixs.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void PhoenixContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Phoenix>::iterator iter;
	for( iter = Phoenixs.begin(); iter != Phoenixs.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 24, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[iter->Frame + 56] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 24, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[iter->Frame + 56] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void PhoenixContainer::DrawAABB( int color )
{
	
	std::vector<Phoenix>::iterator iter;
	for( iter = Phoenixs.begin(); iter != Phoenixs.end(); ++iter )
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
EntityContainer::CollisionPacket PhoenixContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<Phoenix>::iterator iter;
	for( iter = Phoenixs.begin(); iter != Phoenixs.end(); ++iter )
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
void PhoenixContainer::KillAllEntities()
{
	std::vector<Phoenix>::iterator iter;
	for( iter = Phoenixs.begin(); iter != Phoenixs.end(); ++iter )
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
void PhoenixContainer::Spawn( int _x, int _y, bool Below, TurretArray &Turrets )
{
	
	int ey;
	std::vector<Phoenix>::iterator iter;
	for( iter = Phoenixs.begin(); iter != Phoenixs.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = PHOENIX_MAX_HP;
			
			if( !Below )
			{
				ey = 0;
			}
			else
			{
				ey = 192;
			}
			
			iter->LoadPath( _x, _y, 300, ey, Below );
			
			
			// 1st pattern
			Turret::Params Tp;
	
			Tp.ActiveTime		= 65535;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2 ;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= 3584;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 60;				// 
			Tp.MaxSpawnDelay	= 50;			// 
			Tp.BulletsPerShot	= 5;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 4;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.5);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_CENTERED;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 1584;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 9;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 31;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::ACCEL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPos, 
						   &iter->TurretTar,
						   iter->TurretPtr
	  		             );
	
			
			
	
			
			// 2nd
			Tp.ActiveTime		= 65535;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 15;				// 
			Tp.Yradius			= -24;				// 
			Tp.Angle			= 16384;					// StartAngle
			Tp.MinAngle			= 14144;				// 
			Tp.MaxAngle			= 18752;
			Tp.AngleDelta		= 2048;				// + rotation speed
			Tp.SpreadAngle		= 960;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 60;				// 
			Tp.MaxSpawnDelay	= 32;			// 
			Tp.BulletsPerShot	= 7;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_OFFSET;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 3072;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 0;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			if( Below ) Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			
			iter->TurretPtr->AddPattern( Tp );
			
			// 3rd
			Tp.ActiveTime		= 65535;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= -10;				// 
			Tp.Angle			= 8960;					// StartAngle
			Tp.MinAngle			= 8128;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 1024;				// + rotation speed
			Tp.SpreadAngle		= 8192;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 120;				// 
			Tp.MaxSpawnDelay	= 20;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 3;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 6;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_NORMAL;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 5352;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 21;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 5;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			if( Below ) Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			
			iter->TurretPtr->AddPattern( Tp );
			
			
			// Deactivate
			iter->TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}

