/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	Centipede class


*******************************************************************************
******************************************************************************/

#include "Centipede.h"

#define MIN_X ( -300 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_CENTIPEDES 2
#define CENTIPEDE_MAX_HP 512

//------------------------------------------------------------------------------
//	Centipede Enemy
//------------------------------------------------------------------------------
Centipede::Centipede()
{
	Frame = 0;
	Active = false;	
	Hp = CENTIPEDE_MAX_HP; 
	DeathScore = Hp * 8;
	ShotsFired = 0;
	DrawMode = EN_NORMAL;
	Width = 32;
	Height = 32;
	
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i++ )
	{
		Segments[i].x = floattof32(300);
		Segments[i].y = floattof32(96);
		Segments[i].Aabb.Init( (Segments[i].x >> 12) - (Width >> 1), 
							   (Segments[i].y >> 12) - (Height >> 1), 
							   Width, Height );
		
	}
	
	
}

Centipede::~Centipede()
{
}

//--------------------------------------
//	
//--------------------------------------
void Centipede::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	if (Counter == 64 * 32)
	{
		Target.x = -400 << 12;
		Target.y = UTIL::RND_RANGE(0, 191) << 12;
		NewTarget = false;
	}
	
	if (NewTarget)
	{
		Target.x = UTIL::RND_RANGE(0, 255) << 12;
		Target.y = UTIL::RND_RANGE(0, 191) << 12;
		NewTarget = false;
	}
	
	Dx = mulf32(cosLerp(Angle), Speed);
	Dy = mulf32(sinLerp(Angle), Speed);
	
	x += Dx;
	y += Dy;
	
	if( x < MIN_X )
	{
		Kill();
		return;
	}
	
	
	// set up our centipede
	Segments[CENTIPEDE_MAX_SEGMENTS - 1].x = x;
	Segments[CENTIPEDE_MAX_SEGMENTS - 1].y = y;

	for( int i = 0; i < (CENTIPEDE_MAX_SEGMENTS - 1);  i++ )
	{
		Segments[i].x = Segments[i+1].x;
		Segments[i].y = Segments[i+1].y;
	}
	
	// distance
	if ( (abs(x - Target.x) + abs(y - Target.y)) < (32 << 12) )
            NewTarget = true;
	
	// Project normal
	s32 dot = (mulf32(Dy , (Target.x - x)) - mulf32(Dx, (Target.y - y)));
	
	if( dot > 0)
		Angle = (Angle - Delta);
	else
		Angle = (Angle + Delta);
	
		
	Targets::Instance()->AddCoord( x, y );
	
	TurretPosition.x = x;
	TurretPosition.y = y;
	
	// Target where head is facing
	TurretTarget.x = x + cosLerp(Angle) * 20;
	TurretTarget.y = y + sinLerp(Angle) * 20;

	
	// animate
	if( ( Counter & 3 ) == 0 )
	{
		Frame++;
		if( Frame > 5) Frame = 0;
		
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
	const int STEP_SIZE = 16;
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i+= STEP_SIZE )
	{
		int sx = Segments[i].x;
		int sy = Segments[i].y;
		
		// Reset AABB
		Segments[i].Aabb.Init( (sx >> 12) - (Width >> 1), 
							   (sy >> 12) - (Height >> 1), 
							   Width, Height );
	
	}

}


//--------------------------------------
//	
//--------------------------------------
void Centipede::Destroy()
{
	ExplodeSegments();
	Explosion::Instance()->Spawn( x >> 12,
							  y >> 12,
							  Dx*2,
							  Dy*2,
							  Explode::MEDIUM_4,
							  Explode::SMALL_2
							);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_MED2 );
}

//--------------------------------------
//	
//--------------------------------------
void Centipede::Kill()
{

	Active = false;
	x = 300 << 12;
	y = 96 << 12;
	TurretPtr->Kill();
	TurretPtr->Kill();
	
	ResetSegments();
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
			 
	const int STEP_SIZE = 16;
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i+= STEP_SIZE )
	{
		Segments[i].x = x;
		Segments[i].y = y;
		// Reset AABB
		Segments[i].Aabb.Init( (x >> 12) - (Width >> 1), 
							   (y >> 12) - (Height >> 1), 
							   Width, Height );
	
	}
	
}



//--------------------------------------
//	
//--------------------------------------
void Centipede::Draw( glImage* const Images, int ColorTable, 
					  glImage* const Images2, int ColorTable2 ) const
{
	int BaseFrame = 0;
	int frame = Frame;
	const int STEP_SIZE = 8;
	
	
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i+= STEP_SIZE )
	{
		
		
		s32 tx = Segments[i].x;
		s32 ty = Segments[i].y;
		
		s32 angle;
		if( i == 0 )    //tail
		{
			angle = BRAD_PI + atan2Lerp( tx - Segments[i+(STEP_SIZE-1)].x, 
										 ty - Segments[i+(STEP_SIZE-1)].y 
									   );				
			BaseFrame = 4;					
		}	
		else if( i == (CENTIPEDE_MAX_SEGMENTS-STEP_SIZE) )	// head
		{
			angle = BRAD_PI + atan2Lerp( Segments[i-(STEP_SIZE-1)].x - tx, 
										 Segments[i-(STEP_SIZE-1)].y - ty 
									   );
			BaseFrame = 0;
			glSpriteRotate( tx >> 12,
						ty >> 12,
						angle,
						GL_FLIP_NONE, &Images[BaseFrame + frame ] 
					  );
			
			continue;
		
		}
		else	// segments
		{
			angle = BRAD_PI + atan2Lerp( Segments[i-(STEP_SIZE-1)].x - tx, 
										 Segments[i-(STEP_SIZE-1)].y - ty 
									   );
			BaseFrame = 0;
		}
		
		
		glSpriteRotate( tx >> 12,
						ty >> 12,
						angle,
						GL_FLIP_NONE, &Images2[BaseFrame + frame] 
					  );
		
		frame++;
		if( frame > 5) frame = 0;
				
	}  //next
					
}
	


//--------------------------------------
//	
//--------------------------------------
void Centipede::DrawAABB( int color ) const
{
	const int STEP_SIZE = 16;
	
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i+= STEP_SIZE )
	{
		
		AABB box = Segments[i].Aabb.Get();
		glBox( box.x1, box.y1, box.x2, box.y2, color );
		box.Resize(512*3);
		glBox( box.x1, box.y1, box.x2, box.y2, color );
			
		
				
	}  //next
					
}

//--------------------------------------
//	
//--------------------------------------
void Centipede::CollideSegments(  PlayerShip &Vic, bool BombIsActive, EntityContainer::CollisionPacket &Ret )
{
	const int STEP_SIZE = 16;
	
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i+= STEP_SIZE )
	{
		
		AABB aabb = Segments[i].Aabb.Get();
		
		// Viper shots
		Vic.CollideShots(aabb);
		// Viper Missiles
		Vic.CollideMissiles(aabb);
		
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
				
	}  //next
					
}

//--------------------------------------
//	
//--------------------------------------
void Centipede::ExplodeSegments() const
{
	const int STEP_SIZE = 16;
	
	for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i+= STEP_SIZE )
	{
		
		s32 dx = cosLerp(i * 128) * 2;
		s32 dy = sinLerp(i * 256) * 2;
								
		Explosion::Instance()->Spawn( Segments[i].x >> 12,
									  Segments[i].y >> 12,
									  dx,
									  dy,
									  Explode::MEDIUM_2
									);			
			
				
	}  //next
					
}

//------------------------------------------------------------------------------
//	Centipede Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
CentipedeContainer::CentipedeContainer()
{
}
	
CentipedeContainer::~CentipedeContainer()
{
	Centipedes.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void CentipedeContainer::Init( int Palette, glImage* const Sprites,
							   int Palette2, glImage* const Sprites2 )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	
	ColorTableSegments = Palette2;		// init needed data for drawing
	ImagesSegments = Sprites2;
	
	Centipedes.resize( MAX_CENTIPEDES );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void CentipedeContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<Centipede>::iterator iter;
	for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void CentipedeContainer::DrawEntities()
{

	numEntities = 0;
	std::vector<Centipede>::iterator iter;
	for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				iter->Draw( Images, Palettes::Instance()->Red(), ImagesSegments, Palettes::Instance()->White() );
				glColor( RGB15(31,31,31) );
			}
			else
			{
				iter->Draw( Images, ColorTable, ImagesSegments, ColorTableSegments );	
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void CentipedeContainer::DrawAABB( int color )
{
	
	std::vector<Centipede>::iterator iter;
	for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
	{
		if( iter->Active )
		{
			AABB aabb = iter->Aabb.Get();
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			aabb.Resize(512*3);
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
			
			iter->DrawAABB( RGB15(31,0,31) );
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket CentipedeContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 3;
	
	std::vector<Centipede>::iterator iter;
	for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
	{
		if( iter->Active )
		{
				
			AABB aabb = iter->Aabb.Get();
			
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
			}  // endif
				
			
			iter->CollideSegments( Vic, BombIsActive, Ret );
			
			
		}  // end if iter-active
	}
	
	return Ret;
	
}

//--------------------------------------
//	
//--------------------------------------
void CentipedeContainer::KillAllEntities()
{
	std::vector<Centipede>::iterator iter;
	for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
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
void CentipedeContainer::Spawn( int _x, int _y, TurretArray &Turrets )
{
	
	
	std::vector<Centipede>::iterator iter;
	for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->x = _x << 12;
			iter->y = _y << 12;
			iter->Speed = floattof32(1.5);
			iter->Angle = 0;
			iter->Delta = 256;
			iter->Target.x = UTIL::RND_RANGE(0,255) << 12;
			iter->Target.y = UTIL::RND_RANGE(0,191) << 12;
			iter->NewTarget = true;
		
			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = CENTIPEDE_MAX_HP;
			iter->ShotsFired = 0;
			
			Turret::Params Tp;
	
			Tp.ActiveTime		= 512;				// Frames current AI is active
			Tp.Active			= true;					// 
			Tp.Smart			= true;					// if targeting or not
			Tp.Xradius			= 10;				// 
			Tp.Yradius			= 10;				// 
			Tp.Angle			= BRAD_PI;					// StartAngle
			Tp.MinAngle			= 0;				// 
			Tp.MaxAngle			= BRAD_PI * 2;
			Tp.AngleDelta		= 0;				// + rotation speed
			Tp.SpreadAngle		= BRAD_PI/16;			// distance of each bullet on a spread
			Tp.AngleDirection	= 1;			// -1 or +1  determines rotation direction
			Tp.RotationType		= 0;			// whether back anf forth or not
			Tp.SpawnDelay		= 0;				// 
			Tp.MaxSpawnDelay	= 64;			// 
			Tp.BulletsPerShot	= 3;
			Tp.WaveCounter	 	= 0;			// 
			Tp.MaxWave			= 3;				// maximum bullets per wave
			Tp.MaxWaveDelay		= 8;			// maximum delay for next shot in a wave
			
			
			Tp.BulletSpeedDelta	= floattof32(0.0);		// for variable bullet speeds in a single wave
			Tp.BulletSpreadOffset = floattof32(1.0);		// for offsetted bullet placement in a single wave
			
			Tp.Mode				= Turret::SPREAD_RINGS;
			
			Tp.Bp.Gx = 0;
			Tp.Bp.Gy = 0;
			Tp.Bp.Speed = floattof32(3);
			Tp.Bp.RotationDelta = 256;
			Tp.Bp.StopDelay = 1;
			Tp.Bp.Frame = 0;
			Tp.Bp.NumFrames = 4;
			Tp.Bp.BaseFrame = 20;
			Tp.Bp.Smart = true;
			Tp.Bp.Width = 8;
			Tp.Bp.Height = 8;
			Tp.Bp.Type = Bullet::NORMAL;
			Tp.Bp.AniMode = Bullet::DEFAULT;
			
			
			Turrets.Spawn( Tp, 
						   &iter->TurretPosition, 
						   &iter->TurretTarget,
						   iter->TurretPtr
	  		             );
	
			
			break;
		
		}
	}
	
	
}

