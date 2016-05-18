/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Rotator class


*******************************************************************************
******************************************************************************/

#include "Rotator.h"

#define MIN_X ( -128 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_ROTATORS 4
#define ROTATOR_MAX_HP 800

//------------------------------------------------------------------------------
//	Rotator Enemy
//------------------------------------------------------------------------------
Rotator::Rotator()
{
	Frame = 0;
	Active = false;	
	Hp = ROTATOR_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;	
	Width = 32;
	Height = 32;
	Radius = 50;
	
}

Rotator::~Rotator()
{
	Cannons.clear();
}


//--------------------------------------
//	
//--------------------------------------
void Rotator::Init()
{
	x = 300 << 12;
	y = 0;
	Frame = 0;
	Counter = 0;
	Active = true;
	Hp = ROTATOR_MAX_HP;
	
	
	
	// set up coptions
	Cannon cannon;
	
	cannon.x = x;
	cannon.y = y;
	cannon.Width = 16;
	cannon.Height = 16;
	
	cannon.TurretPosition.x = x;
	cannon.TurretPosition.y = y;
	cannon.TurretTarget.x = 0;
	cannon.TurretTarget.y = 96 << 12;
	
	// be safe
	Cannons.clear();
	
	// add
	Cannons.push_back(cannon);    // 1
	Cannons.push_back(cannon);    // 2
	
	
}
	
	

//--------------------------------------
//	
//--------------------------------------
void Rotator::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	//iprintf( "\x1b[1;1HSplineIndex = %i         ", Path.GetIndex() );
	
	Path.Update();
	
	switch( Path.GetIndex() )
	{
		case 0:
			break;
		case 2:
			for( int i = 0; i < 2; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(0);			
			}
			break;
		case 4:
			for( int i = 0; i < 2; i++ )
			{
				Cannons[i].TurretPtr->DoPattern(1);
			}
			break;
		case 5:
			break;
		case 6:
			for( int i = 0; i < 2; i++ )
			{
				Cannons[i].TurretPtr->Deactivate();
			}
			break;
		case 7:
			for( int i = 0; i < 2; i++ )
			{
				Cannons[i].TurretPtr->Activate();
				Cannons[i].TurretPtr->DoPattern(0);
			}
			break;
		case 9:
			for( int i = 0; i < 2; i++ )
			{
				Cannons[i].TurretPtr->Deactivate();
			}
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
	
	// Update options
	int i = 0;
	std::vector<Cannon>::iterator iter;
	for( iter = Cannons.begin(); iter != Cannons.end(); ++iter )
	{
		// rotate
		s32 angle = Counter * 128 + i * BRAD_PI;
		int Rad = (sinLerp(Counter * 256) * Radius) >> 12;
		iter->x = x + cosLerp(angle)*Rad;
		iter->y = y + sinLerp(angle)*Rad;
		iter->TurretPosition.x = iter->x;
		iter->TurretPosition.y = iter->y;
		
		// move targets for kicks
		iter->TurretTarget.x = Vic.GetX();
		iter->TurretTarget.y = Vic.GetY();
		
		// Reset AABB
		iter->Aabb.Init( (iter->x >> 12) - (iter->Width/2), 
				   (iter->y >> 12) - (iter->Height/2), 
				   iter->Width, iter->Height 
				  );

		i++;
		//iter->TurretPtr->Deactivate();
	}
	
	
	// animate
	if( ( Counter & 3 ) == 0 )
	{
		Frame++;
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
void Rotator::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::SMALL_2
								);
	// explode cannons/options
	std::vector<Rotator::Cannon>::iterator cannoniter;
	for( cannoniter = Cannons.begin(); cannoniter != Cannons.end(); ++cannoniter )
	{
		Explosion::Instance()->Spawn( cannoniter->x >> 12,
							  cannoniter->y >> 12,
							  0,
							  0,
							  Explode::MEDIUM_4,
							  Explode::SMALL_2
							);
	
	}

	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_MED2 );
	
}

//--------------------------------------
//	
//--------------------------------------
void Rotator::Kill()
{

	Active = false;
	x = 300 << 12;
	y = 96 << 12;
	
	std::vector<Cannon>::iterator iter;
	for( iter = Cannons.begin(); iter != Cannons.end(); ++iter )
	{
		iter->x = x;
		iter->y = y;
		// Reset AABB
		iter->Aabb.Init( (iter->x >> 12) - (iter->Width >> 1), 
				   (iter->y >> 12) - (iter->Height >> 1), 
			       iter->Width, iter->Height 
			      );
	
		iter->TurretPtr->Kill();
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void Rotator::LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate )
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
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //5
	
	p.x = 180;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //6
	
	p.x = 200;
	p.y = abs(neger - 50);
	Path.AddPoint( p ); //7
	
	p.x = 200;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //8
	
	p.x = 180;
	p.y = abs(neger - 96);
	Path.AddPoint( p ); //9
	
	p.x = 200;
	p.y = abs(neger - 70);
	Path.AddPoint( p ); //10
	
	p.x = 200;
	p.y = abs(neger - 70);
	Path.AddPoint( p ); //11
	
	p.x = 220;
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
//	Rotator Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
RotatorContainer::RotatorContainer()
{
}
	
RotatorContainer::~RotatorContainer()
{
	Rotators.clear();				// clear the bullets
}


//--------------------------------------
//	
//--------------------------------------
void RotatorContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Rotators.resize( MAX_ROTATORS );	// (speed of iteration)
}


//--------------------------------------
//	Virtual Functions
//--------------------------------------
void RotatorContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Rotator>::iterator iter;
	for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Update( Vic );
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void RotatorContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Rotator>::iterator iter;
	for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			// cannons/options
			std::vector<Rotator::Cannon>::iterator cannoniter;
			for( cannoniter = iter->Cannons.begin(); cannoniter != iter->Cannons.end(); ++cannoniter )
			{
				glSprite( (cannoniter->x >> 12) - 8, (cannoniter->y >> 12) - 8, 
					  GL_FLIP_NONE, &Images[iter->Frame+36] 
					);
			
			}
			
			// Main
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 16, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[53] 
					    );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				glSprite( (iter->x >> 12) - 16, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[53] 
					    );
					
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void RotatorContainer::DrawAABB( int color )
{
	
	std::vector<Rotator>::iterator iter;
	for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
	{
		if( iter->Active )
		{
			// cannons
			std::vector<Rotator::Cannon>::iterator cannoniter;
			for( cannoniter = iter->Cannons.begin(); cannoniter != iter->Cannons.end(); ++cannoniter )
			{
				AABB caabb = cannoniter->Aabb.Get();
				glBox( caabb.x1, caabb.y1, caabb.x2, caabb.y2, color );
				caabb.Resize(512*3);
				glBox( caabb.x1, caabb.y1, caabb.x2, caabb.y2, color );
				
			}
			// main
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
EntityContainer::CollisionPacket RotatorContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 3;
	
	std::vector<Rotator>::iterator iter;
	for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
	{
		if( iter->Active )
		{
			// main
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
			
			// cannons/options
			std::vector<Rotator::Cannon>::iterator cannoniter;
			for( cannoniter = iter->Cannons.begin(); cannoniter != iter->Cannons.end(); ++cannoniter )
			{
				AABB caabb = cannoniter->Aabb.Get();
				caabb.Resize(512*3);
			
				// Viper
				if( !Vic.IsInvincible() )
				{
					if( !BombIsActive )
					{
						if( Vic.GetAABB().Intersects(caabb) )
						{
							Ret.x = Vic.GetX() >> 12;
							Ret.y = Vic.GetY() >> 12;
							Ret.Collision = 2;
							Vic.Kill();
						}
					}
				}
			
				// Viper shots
				// Do nothing as the cannons are indestructible
				Vic.CollideShots(caabb);
				Vic.CollideMissiles(caabb);
					
				
			}  // next cannoniter
	
		}  // end if iter->active
		
	} // next iter
	
	return Ret;
	
}

//--------------------------------------
//	
//--------------------------------------
void RotatorContainer::KillAllEntities()
{
	std::vector<Rotator>::iterator iter;
	for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
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
void RotatorContainer::Spawn( int _x, int _y, bool Below, TurretArray &Turrets )
{
	
	std::vector<Rotator>::iterator iter;
	for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			// init the rotator
			iter->Init();
			int ey = 0;
			// load spline path
			if( !Below )
			{
				ey = 0;
			}
			else
			{
				ey = 192;
			}
			
			iter->LoadPath( _x, _y, -300, ey, Below );
			
			// turret params
			Turret::Params Tp;
	
			Tp.ActiveTime		= 1<<24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 0;				// 
			Tp.Yradius			= 0;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= 0;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 2;			// whether back anf forth or not
			Tp.SpawnDelay		= 10;				// 
			Tp.MaxSpawnDelay	= 60;			// 
			Tp.BulletsPerShot	= 1;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 12;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 1;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 320;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SINGLE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(5);
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -20;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 3;
			Tp.Bp.BaseFrame = 28;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::ORIENTED;
			
			// load turret params to cannons
			std::vector<Rotator::Cannon>::iterator cannoniter;
			for( cannoniter = iter->Cannons.begin(); cannoniter != iter->Cannons.end(); ++cannoniter )
			{
			
				Turrets.Spawn( Tp, 
							   &cannoniter->TurretPosition, 
						       &cannoniter->TurretTarget,
						       cannoniter->TurretPtr
	  		                 );
			}
			
			
			// 2nd
			
			Tp.ActiveTime		= 1 << 24;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= false;					// if targeting or not
			Tp.Xradius			= -20;				// 
			Tp.Yradius			= -20;				// 
			Tp.Angle			= 0;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_2PI;
			Tp.AngleDelta		= 512;				// + rotation speed
			Tp.SpreadAngle		= BRAD_2PI/12;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 60;			// 
			Tp.BulletsPerShot	= 12;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 5;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 3;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= 0;		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = 1 << 12;		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPRAY_WAVE;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(0.75)*4;
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = -20;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 16;
			Tp.Bp.Smart = false;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::DECEL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			iter->Cannons[0].TurretPtr->AddPattern( Tp );
			
			Tp.AngleDirection	= -1;			// -1 or +1  determines rotation direction
			Tp.SpawnDelay		= 30;				// 
			Tp.Bp.BaseFrame = 0;
			
			iter->Cannons[1].TurretPtr->AddPattern( Tp );
			
			
			iter->Cannons[0].TurretPtr->Deactivate();
			iter->Cannons[1].TurretPtr->Deactivate();
			
			break;
		
		}
	}
	
	
}



