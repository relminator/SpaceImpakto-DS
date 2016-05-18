/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	BumbleBee class


*******************************************************************************
******************************************************************************/

#include "BumbleBee.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_BUBMBLEBEES 4
#define BUBMBLEBEE_MAX_HP 512

//------------------------------------------------------------------------------
//	BumbleBee Enemy
//------------------------------------------------------------------------------
BumbleBee::BumbleBee()
{
	Frame = 0;
	Active = false;	
	Hp = BUBMBLEBEE_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 30;
	Height = 40;
	
}

BumbleBee::~BumbleBee()
{
}

//--------------------------------------
//	
//--------------------------------------
void BumbleBee::Update( PlayerShip &Vic )
{
	
	Counter++;

	
	Path.Update();
	
	//iprintf( "\x1b[1;1HSplineIndex = %i         ", Path.GetIndex() );
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
		case 6:
			TurretPtr->DoPattern(2);
			break;
		case 9:
			if( TurretPtr->IsActivated() ) TurretPtr->Deactivate();
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
	
	TurretPos.x = x;
	TurretPos.y = y;
	
	
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
void BumbleBee::Destroy()
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
void BumbleBee::Kill()
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
void BumbleBee::LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate )
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
	
	p.x = 128;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //3
	
	p.x = 230;
	p.y = abs(neger - 20);
	Path.AddPoint( p ); //4
	
	p.x = 230;
	p.y = abs(neger - 180);
	Path.AddPoint( p ); //5
	
	p.x = 230;
	p.y = abs(neger - 20);
	Path.AddPoint( p ); //6
	
	p.x = 230;
	p.y = abs(neger - 180);
	Path.AddPoint( p ); //7
	
	p.x = 200;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //8
	
	p.x = 200;
	p.y = abs(neger - 120);
	Path.AddPoint( p ); //9
	
	p.x = 250;
	p.y = abs(neger - 70);
	Path.AddPoint( p ); //10
	
	p.x = 200;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //11
	
	p.x = 100;
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
//	BumbleBee Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BumbleBeeContainer::BumbleBeeContainer()
{
}
	
BumbleBeeContainer::~BumbleBeeContainer()
{
	BumbleBees.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void BumbleBeeContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	BumbleBees.resize( MAX_BUBMBLEBEES );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BumbleBeeContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<BumbleBee>::iterator iter;
	for( iter = BumbleBees.begin(); iter != BumbleBees.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BumbleBeeContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<BumbleBee>::iterator iter;
	for( iter = BumbleBees.begin(); iter != BumbleBees.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			//0, 128, 44, 40,				// 68
			//0, 200, 30, 54,				// 69
			int w = Images[iter->Frame + 68].width/2;
			int h = Images[iter->Frame + 68].height/2;
			if( iter->Frame == 1 ) 
				h += 6;
			else
				w -= 6;
			
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - w, (iter->y >> 12) - h, 
					      GL_FLIP_NONE, &Images[iter->Frame + 68] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - w, (iter->y >> 12) - h, 
					      GL_FLIP_NONE, &Images[iter->Frame + 68] 
					    );
					
			}
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BumbleBeeContainer::DrawAABB( int color )
{
	
	std::vector<BumbleBee>::iterator iter;
	for( iter = BumbleBees.begin(); iter != BumbleBees.end(); ++iter )
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
EntityContainer::CollisionPacket BumbleBeeContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 2;
	
	std::vector<BumbleBee>::iterator iter;
	for( iter = BumbleBees.begin(); iter != BumbleBees.end(); ++iter )
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
void BumbleBeeContainer::KillAllEntities()
{
	std::vector<BumbleBee>::iterator iter;
	for( iter = BumbleBees.begin(); iter != BumbleBees.end(); ++iter )
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
void BumbleBeeContainer::Spawn( int _x, int _y, bool Below, TurretArray &Turrets )
{
	
	std::vector<BumbleBee>::iterator iter;
	for( iter = BumbleBees.begin(); iter != BumbleBees.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = BUBMBLEBEE_MAX_HP;
			iter->LoadPath( _x, _y, -300, 96, Below );
			
			
			
			Turret::Params Tp;
	
			Tp.ActiveTime		= 65535;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= 400;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 60;				// 
			Tp.MaxSpawnDelay	= 85;			// 
			Tp.BulletsPerShot	= 7;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 4;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= -80;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_CENTERED;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = 384;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 31;
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
	
			
			
	
			// 2nd wave
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 1024;				// + rotation speed
			Tp.SpreadAngle		= BRAD_2PI/4;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 60;				// 
			Tp.MaxSpawnDelay	= 8;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 1;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 8;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_NORMAL;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)*4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 1;
			Tp.Bp.BaseFrame = 15;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			if( Below ) Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			iter->TurretPtr->AddPattern( Tp );
			
			
			// 3rd/4th/5th wave
			Tp.ActiveTime		= 60*2;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 1024;				// + rotation speed
			Tp.SpreadAngle		= BRAD_2PI/4;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 1;			// whether back anf forth or not
			Tp.SpawnDelay		= 60;				// 
			Tp.MaxSpawnDelay	= 4;			// 
			Tp.BulletsPerShot	= 4;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 0;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SINGLE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)*8;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 0;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 16;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			iter->TurretPtr->AddPattern( Tp );
			iter->TurretPtr->AddPattern( Tp );
			iter->TurretPtr->AddPattern( Tp );
			
			// Deactivate
			iter->TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}

