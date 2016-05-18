/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Predator Boss class


*******************************************************************************
******************************************************************************/

#include "PredatorBoss.h"
namespace PredatorBarrage
{
void Load2Straight( TurretArray &Turrets,
			        Vector2df32 *Position,
			        Vector2df32 *Target,
			        std::vector<Turret>::iterator &TurretPtr );

void Add3Way( Vector2df32 *Position,
			  Vector2df32 *Target,
			  std::vector<Turret>::iterator &TurretPtr );

void Add4Spray( Vector2df32 *Position,
			    Vector2df32 *Target,
			    std::vector<Turret>::iterator &TurretPtr );


void AddWasAG4WaySpread(  Vector2df32 *Position,
						  Vector2df32 *Target,
					      std::vector<Turret>::iterator &TurretPtr );
void AddWasAG3Spread6Way(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr );

//  ====== 2nd cannon =======
void Load5WaySmart( TurretArray &Turrets,
					 Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction );

void Add4WayRotate(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction );
void AddRotoSmart(  Vector2df32 *Position,
					Vector2df32 *Target,
					std::vector<Turret>::iterator &TurretPtr,
					int direction );
void AddStraightSpread(  Vector2df32 *Position,
						 Vector2df32 *Target,
						 std::vector<Turret>::iterator &TurretPtr );
void AddStraight3x(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr );
void AddSpear(  Vector2df32 *Position,
			    Vector2df32 *Target,
			    std::vector<Turret>::iterator &TurretPtr );
void AddBlank(  Vector2df32 *Position,
			    Vector2df32 *Target,
			    std::vector<Turret>::iterator &TurretPtr );
}   // PredatorBarrage end namespace

//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::Spawn( TurretArray &Turrets )
{

	x = 400 << 12;
	y = 96 << 12;
	Dx = -1024;
	Dy = 96 << 12;
	
	Frame = 0;
	Counter = 0;
	Active = true;
	Alive = true;
	Hp = 800 * 8;
	MaxHp = Hp;
	DeathScore = Hp * 8;
	

	Width = 64;
	Height = 64;
	
	DestroyedCounter = 60 * 5;
	
	LoadPath();
	
	// set up options
	Cannon cannon;
	
	cannon.x = x;
	cannon.y = y;
	cannon.Width = 32;
	cannon.Height = 32;
	
	cannon.TurretPosition.x = x;
	cannon.TurretPosition.y = y;
	cannon.TurretTarget.x = 0;
	cannon.TurretTarget.y = 96 << 12;
	
	// be safe
	Cannons.clear();
	
	// add
	Cannons.push_back(cannon);    // 0
	Cannons.push_back(cannon);    // 1
	
	
	// 1st
	
	PredatorBarrage::Load2Straight( Turrets,
			       &(Cannons[0].TurretPosition),
			       &(Cannons[0].TurretTarget),
			       Cannons[0].TurretPtr );
	// 2
	PredatorBarrage::Add4WayRotate( &(Cannons[0].TurretPosition),
				   &(Cannons[0].TurretTarget),
				   Cannons[0].TurretPtr,
				   -1 );
	// 3
	PredatorBarrage::Add3Way( &(Cannons[0].TurretPosition),
			 &(Cannons[0].TurretTarget),
			 Cannons[0].TurretPtr );
	// 4
	PredatorBarrage::Add4Spray( &(Cannons[0].TurretPosition),
			   &(Cannons[0].TurretTarget),
			   Cannons[0].TurretPtr );
	// 5
	PredatorBarrage::AddWasAG4WaySpread( &(Cannons[0].TurretPosition),
						&(Cannons[0].TurretTarget),
						Cannons[0].TurretPtr );
	// 6
	PredatorBarrage::AddSpear( &(Cannons[0].TurretPosition),
			  &(Cannons[0].TurretTarget),
			  Cannons[0].TurretPtr );
	
	// 7
	PredatorBarrage::AddBlank( &(Cannons[0].TurretPosition),
			  &(Cannons[0].TurretTarget),
			  Cannons[0].TurretPtr );
	
	// 2nd
	PredatorBarrage::Load5WaySmart( Turrets,
				   &(Cannons[1].TurretPosition),
				   &(Cannons[1].TurretTarget),
				   Cannons[1].TurretPtr,
				   1 );
	// 2
	PredatorBarrage::Add4WayRotate( &(Cannons[1].TurretPosition),
				   &(Cannons[1].TurretTarget),
				   Cannons[1].TurretPtr,
				   1 );
	// 3
	PredatorBarrage::AddRotoSmart( &(Cannons[1].TurretPosition),
				   &(Cannons[1].TurretTarget),
				   Cannons[1].TurretPtr,
				   1 );
	// 4
	PredatorBarrage::AddStraightSpread( &(Cannons[1].TurretPosition),
				       &(Cannons[1].TurretTarget),
				       Cannons[1].TurretPtr );
	// 5
	PredatorBarrage::AddBlank( &(Cannons[1].TurretPosition),
			  &(Cannons[1].TurretTarget),
			  Cannons[1].TurretPtr );
	// 6
	PredatorBarrage::AddStraight3x( &(Cannons[1].TurretPosition),
				   &(Cannons[1].TurretTarget),
				   Cannons[1].TurretPtr );
	// 7
	PredatorBarrage::AddWasAG3Spread6Way( &(Cannons[1].TurretPosition),
						 &(Cannons[1].TurretTarget),
						 Cannons[1].TurretPtr );
	
	// Deactivate
	Cannons[0].TurretPtr->Deactivate();
	Cannons[1].TurretPtr->Deactivate();

	
}


//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	//iprintf( "\x1b[1;1HSplineIndex = %i         ", Path.GetIndex() );
	//iprintf( "\x1b[2;1HTurretIndex = %i         ", Cannons[0].TurretPtr->GetIndex() );
	
	switch( Path.GetIndex() )
	{
		case 2:
			if( !Cannons[0].TurretPtr->IsActivated() )
			{
				Cannons[0].TurretPtr->Activate();
				Cannons[0].TurretPtr->DoPattern(0);
			}
			if( !Cannons[1].TurretPtr->IsActivated() )
			{
				Cannons[1].TurretPtr->Activate();
				Cannons[1].TurretPtr->DoPattern(0);
			}
			break;
		case 4:
			Cannons[0].TurretPtr->DoPattern(1);
			Cannons[1].TurretPtr->DoPattern(1);
			break;
		case 9:
			Cannons[0].TurretPtr->DoPattern(2);
			Cannons[1].TurretPtr->DoPattern(2);
			break;
		case 12:
			Cannons[0].TurretPtr->DoPattern(3);
			Cannons[1].TurretPtr->DoPattern(3);
			break;
		case 17:
			Cannons[0].TurretPtr->DoPattern(4);
			Cannons[1].TurretPtr->DoPattern(4);
			break;
		case 20:
			Cannons[0].TurretPtr->DoPattern(5);
			Cannons[1].TurretPtr->DoPattern(5);
			break;
		case 24:
			Cannons[0].TurretPtr->DoPattern(6);
			Cannons[1].TurretPtr->DoPattern(6);
			break;
		case 29:
			if( Cannons[0].TurretPtr->IsActivated() ) Cannons[0].TurretPtr->Deactivate();
			if( Cannons[1].TurretPtr->IsActivated() ) Cannons[1].TurretPtr->Deactivate();
			break;
	}
	
	if( Path.GetIndex() == 7 )
	{
		Path.SetPoint(0, 128, 96);
		Path.SetPoint(1, 128, 96);
		Path.SetPoint(3, 235, 40);
		Path.SetPoint(4, 235, 192-40);
		Path.SetPoint(5, 235, 30);
		Path.SetPoint(6, 235, 192-30);
	}
	
	if( Counter == (256))
	{
		//Cannons[0].TurretPtr->Activate();
		//Cannons[1].TurretPtr->Activate();
	}
	
	Path.Update();
	
	x = Path.GetX();
	y = Path.GetY();
	
	
	Targets::Instance()->AddCoord( x, y );
	
	// Update options
	
	std::vector<Cannon>::iterator iter;
	for( iter = Cannons.begin(); iter != Cannons.end(); ++iter )
	{
		// rotate
		//s32 angle = Counter * 128 + i * BRAD_PI;
		iter->x = x - inttof32(12);
		iter->y = y - inttof32(8);
		iter->TurretPosition.x = iter->x;
		iter->TurretPosition.y = iter->y;
		
		// move targets for kicks
		iter->TurretTarget.x = Vic.GetX();
		iter->TurretTarget.y = Vic.GetY();
		
		// Reset AABB
		iter->Aabb.Init( (iter->x >> 12) - (iter->Width >> 1), 
				   (iter->y >> 12) - (iter->Height >> 1), 
				   Width, Height 
				  );
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
void PredatorBoss::Draw()
{
	int DrawFrame = 50+Frame;
	
	if( (DrawMode == EN_WHITE) )
	{
		DrawMode = EN_NORMAL;
		glColor( RGB15(31,0,0) );
		glSprite( (x >> 12) - Width/2, 
				  (y >> 12) - Height/2, 
				  GL_FLIP_NONE, &Images[DrawFrame] 
				);
		glColor( RGB15(31,31,31) );
	}
	else
	{
		glSprite( (x >> 12) - Width/2, 
				  (y >> 12) - Height/2, 
				  GL_FLIP_NONE, &Images[DrawFrame] 
				);	
	}
	
	DrawHp( Counter );
	
	
}


//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::DrawAABB( int color ) const 
{

	AABB aabb = Aabb.Get();
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
	aabb.Resize(512*3);
	glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket PredatorBoss::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed )
{
	EntityContainer::CollisionPacket Ret;
	Ret.Collision = 0;
	
	if( Active )
	{
			
		AABB aabb = Aabb.Get();
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
		if( x < ( 256 << 12 ) )
		{
			Collisions = Vic.CollideShots(aabb);
			if( Collisions )
			{
				DrawMode = EN_WHITE;
				// Dec HP
				Hp -= Collisions;
				if( Hp < 0 )
				{
					Destroy();
					Vic.AddToScore( DeathScore );
					IsDestroyed = true;
				}
				
			}
			
			// Viper Missiles
			Collisions = Vic.CollideMissiles(aabb);
			if( Collisions )
			{
				DrawMode = EN_WHITE;
				// Dec HP
				Hp -= Collisions;
				if( Hp < 0 )
				{
					Destroy();
					Vic.AddToScore( DeathScore );
					IsDestroyed = true;
				}
				
			}
		}  // end if
	}

	return Ret;
	
}


//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::ForceDestruction()
{
	Destroy();
}
	
//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::LoadPath()
{
	Path.Init( 32 );
	
	Spline::Coord p;
	
	// start
	p.x = 300;
	p.y = 96;
	
	// we should use 2 points
	Path.AddPoint( p );  // 1
	Path.AddPoint( p );  // 2
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 3
 	
	p.x = 210;
	p.y = 96;
	Path.AddPoint( p );  // 4
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 5
	
	p.x = 230;
	p.y = 96;
	Path.AddPoint( p );  // 6
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 7
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 8
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 9
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 10
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 11
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 12
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 13
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 14
	
	p.x = 170;
	p.y = 40;
	Path.AddPoint( p );  // 15
	
	p.x = 170;
	p.y = 50;
	Path.AddPoint( p );  // 16
	
	p.x = 220;
	p.y = 120;
	Path.AddPoint( p );  // 17
	
	p.x = 220;
	p.y = 70;
	Path.AddPoint( p );  // 18
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 19
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 20
	
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 21
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 22
	
	p.x = 230;
	p.y = 192-40;
	Path.AddPoint( p );  // 23
	
	p.x = 230;
	p.y = 40;
	Path.AddPoint( p );  // 24
	
	p.x = 170;
	p.y = 96;
	Path.AddPoint( p );  // 25
	
	p.x = 170;
	p.y = 96;
	Path.AddPoint( p );  // 26
	
	p.x = 170;
	p.y = 96;
	Path.AddPoint( p );  // 27
	
	p.x = 230;
	p.y = 50;
	Path.AddPoint( p );  // 28
	
	p.x = 230;
	p.y = 100;
	Path.AddPoint( p );  // 29
	
	p.x = 128;
	p.y = 96;
	Path.AddPoint( p );  // 30
	
	Path.SetSpeed(32);
		
	Path.SetT(0);
	
}

//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::MEDIUM_2
								);
	
	Explosion::Instance()->MegaSpawn(x >> 12,y >> 12,80,Explode::TINY,Explode::MEDIUM_5, 60*4 );
	
	Alive = false;
	
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_BIG );
	
}


//--------------------------------------
//	
//--------------------------------------
void PredatorBoss::BombMe( PlayerShip &Vic, int BombDamageValue )
{
	
	if( Active ) 
	{
		Hp -= BombDamageValue;
		if( Hp < 0 )
		{
			Destroy();
			Vic.AddToScore( DeathScore );
		}
		
	}
	
}



namespace PredatorBarrage
{
//------------------------------------------------------------------------------
//	Private funks
//------------------------------------------------------------------------------

// ------------- 1st cannon -------------------------
void Load2Straight( TurretArray &Turrets,
			        Vector2df32 *Position,
			        Vector2df32 *Target,
			        std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= BRAD_PI / 2 + BRAD_PI / 4;				// 
	Tp.MaxAngle			= BRAD_PI * 2 - BRAD_PI / 2 - BRAD_PI / 4;
	Tp.AngleDelta		= 1024;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/16;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 0;				// 
	Tp.MaxSpawnDelay	= 4;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 1;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75) * 6;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 0;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 8;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 20;
	Tp.Bp.Height = 20;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr
				 );


}

void Add3Way( Vector2df32 *Position,
			  Vector2df32 *Target,
			  std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= BRAD_PI / 2;				// 
	Tp.MaxAngle			= BRAD_PI * 2 - BRAD_PI / 2;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/8;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 60;				// 
	Tp.MaxSpawnDelay	= 1 << 12;   //60;			// 
	Tp.BulletsPerShot	= 3;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 2;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 4;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.352);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPREAD_OFFSET;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 0;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 0;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	
}

void Add4Spray( Vector2df32 *Position,
			    Vector2df32 *Target,
			    std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	
	Tp.Angle			= BRAD_PI + BRAD_PI / 4;					// StartAngle
	Tp.MinAngle			= BRAD_PI / 2 + BRAD_PI / 4;				// 
	Tp.MaxAngle			= BRAD_PI * 2 - BRAD_PI / 2 - BRAD_PI / 4;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 256;				// 
	Tp.MaxSpawnDelay	= 4;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75) * 2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 10;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 31;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	
}


void AddWasAG4WaySpread(  Vector2df32 *Position,
						  Vector2df32 *Target,
					      std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
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
	
	
	Tp.BulletSpeedDelta	= 120;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 84;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 16;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 7;
	Tp.Bp.Height = 7;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	
}

void AddWasAG3Spread6Way(  Vector2df32 *Position,
						   Vector2df32 *Target,
					       std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;

	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= BRAD_PI - BRAD_PI/8;				// 
	Tp.MaxAngle			= BRAD_PI + BRAD_PI/8;
	Tp.AngleDelta		= 128;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 1;			// whether back anf forth or not
	Tp.SpawnDelay		= 120;				// 
	Tp.MaxSpawnDelay	= 1;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(4);
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 0;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 7;
	Tp.Bp.Height = 7;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;

	TurretPtr->AddPattern( Tp );
	
}



// ------------- 2nd cannon -------------------------
void Load5WaySmart( TurretArray &Turrets,
					 Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_PI * 2;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 60;				// 
	Tp.MaxSpawnDelay	= 96;			// 
	Tp.BulletsPerShot	= 5;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 3;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 2;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75)/2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 15;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 5;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	Turrets.Spawn( Tp, 
				   Position, 
				   Target,
				   TurretPtr
				 );


}


// 2
void Add4WayRotate(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr,
					 int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_PI * 2;
	Tp.AngleDelta		= 512;				// + rotation speed
	Tp.SpreadAngle		= BRAD_2PI/4;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 60;				// 
	Tp.MaxSpawnDelay	= 12;			// 
	Tp.BulletsPerShot	= 4;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75)*2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 0;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void AddRotoSmart(  Vector2df32 *Position,
					Vector2df32 *Target,
					std::vector<Turret>::iterator &TurretPtr,
					int direction )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= 0;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_PI * 2;
	Tp.AngleDelta		= 1024;				// + rotation speed
	Tp.SpreadAngle		= 0;			// distance of each bullet on a spread
	Tp.AngleDirection	= direction;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 60;				// 
	Tp.MaxSpawnDelay	= 2;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_NORMAL;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = floattof32(0.75)/3;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = 30;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 28;
	Tp.Bp.Smart = true;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	
	TurretPtr->AddPattern( Tp );
	

}



void AddStraightSpread(  Vector2df32 *Position,
						 Vector2df32 *Target,
						 std::vector<Turret>::iterator &TurretPtr )
{
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= 10;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_PI * 2;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/10;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 0;			// whether back anf forth or not
	Tp.SpawnDelay		= 256;				// 
	Tp.MaxSpawnDelay	= 60;			// 
	Tp.BulletsPerShot	= 7;
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
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 20;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 12;
	Tp.Bp.Height = 12;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	
	TurretPtr->AddPattern( Tp );
	

}


void AddStraight3x(  Vector2df32 *Position,
					 Vector2df32 *Target,
					 std::vector<Turret>::iterator &TurretPtr )
{

	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= false;					// if targeting or not
	Tp.Xradius			= 10;				// 
	Tp.Yradius			= -12;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_PI * 2 ;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= BRAD_PI/2;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 60;				// 
	Tp.MaxSpawnDelay	= 8;			// 
	Tp.BulletsPerShot	= 2;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 0;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 0;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 17072;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 4;
	Tp.Bp.BaseFrame = 11;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::NORMAL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	
}

void AddSpear(  Vector2df32 *Position,
			    Vector2df32 *Target,
			    std::vector<Turret>::iterator &TurretPtr )
{
	
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 1024;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 60;				// 
	Tp.MaxSpawnDelay	= 85;			// 
	Tp.BulletsPerShot	= 6;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 4;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 256;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SPRAY_CENTERED;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 384;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::ORIENTED;
	
	TurretPtr->AddPattern( Tp );
	
}


void AddBlank(  Vector2df32 *Position,
			    Vector2df32 *Target,
			    std::vector<Turret>::iterator &TurretPtr )
{
	
	Turret::Params Tp;
	
	Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
	Tp.Active			= true;					// 
	Tp.Smart			= true;					// if targeting or not
	Tp.Xradius			= 0;				// 
	Tp.Yradius			= 0;				// 
	Tp.Angle			= BRAD_PI;					// StartAngle
	Tp.MinAngle			= 0;				// 
	Tp.MaxAngle			= BRAD_2PI;
	Tp.AngleDelta		= 0;				// + rotation speed
	Tp.SpreadAngle		= 1024;			// distance of each bullet on a spread
	Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
	Tp.RotationType		= 2;			// whether back anf forth or not
	Tp.SpawnDelay		= 1 << 24;				// 
	Tp.MaxSpawnDelay	= 85;			// 
	Tp.BulletsPerShot	= 1;
	Tp.WaveCounter	 	= 0;			// 
	Tp.MaxWave			= 4;				// maximum bullets per wave
	Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
	
	
	Tp.BulletSpeedDelta	= 256;		// for variable bullet speeds in a single wave
	Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
	
	Tp.Mode				= Turret::SINGLE;
	
	Tp.Bp.Gx = 0;
	Tp.Bp.Gy = 0;
	Tp.Bp.Speed = 384/2;
	Tp.Bp.RotationDelta = 256;
	Tp.Bp.StopDelay = -1;
	Tp.Bp.Frame = 0;
	Tp.Bp.NumFrames = 3;
	Tp.Bp.BaseFrame = 34;
	Tp.Bp.Smart = false;
	Tp.Bp.Width = 8;
	Tp.Bp.Height = 8;
	Tp.Bp.Type = Bullet::ACCEL;
	Tp.Bp.AniMode = Bullet::DEFAULT;
	
	TurretPtr->AddPattern( Tp );
	
}


}   // PredatorBarrage