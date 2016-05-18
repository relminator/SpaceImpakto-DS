/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	PowerUp class


*******************************************************************************
******************************************************************************/

#include "PowerUp.h"

#define MIN_X ( -8 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_X ( (256 + 8) << 12 )
#define MAX_Y ( (196 + 18) << 12 )

#define MAX_POWERUPS 3

//------------------------------------------------------------------------------
//	PowerUp Enemy
//------------------------------------------------------------------------------
PowerUp::PowerUp()
{
	Frame = 0;
	Active = false;	
	Width = 20;
	Height = 20;
	
}

PowerUp::~PowerUp()
{
}

//--------------------------------------
//	
//--------------------------------------
void PowerUp::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	x += Dx;
	y += Dy;
	
	if(Counter < (60 * 15))
	{
		if( x < 0 )
		{
			Dx = -Dx;
			x = 0;
			x += Dx;
		}
		
		if( x > (255 << 12) )
		{
			Dx = -Dx;
			x = 255 << 12;
			x += Dx;
		}
		
		if( y < 0 )
		{
			Dy = -Dy;
			y = 0;
			y += Dy;
		}
		
		if( y > (192 << 12) )
		{
			Dy = -Dy;
			y = 192 << 12;
			y += Dy;
		}
	}
	
	
	if( (x < MIN_X ) || (x > MAX_X ) || (y < MIN_Y ) || (y > MAX_Y ) ) 
	{
		Kill();
		return;
	}
	
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
void PowerUp::Kill()
{

	Active = false;
	x = 300 << 12;
	y = 96 << 12;

	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	
}

//------------------------------------------------------------------------------
//	PowerUp Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
PowerUpContainer::PowerUpContainer()
{
}
	
PowerUpContainer::~PowerUpContainer()
{
	PowerUps.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void PowerUpContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	PowerUps.resize( MAX_POWERUPS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void PowerUpContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<PowerUp>::iterator iter;
	for( iter = PowerUps.begin(); iter != PowerUps.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void PowerUpContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<PowerUp>::iterator iter;
	for( iter = PowerUps.begin(); iter != PowerUps.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			glSprite( (iter->x >> 12) - 8, (iter->y >> 12) - 8, 
					  GL_FLIP_NONE, &Images[iter->Frame] 
					);
				
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void PowerUpContainer::DrawAABB( int color )
{
	
	std::vector<PowerUp>::iterator iter;
	for( iter = PowerUps.begin(); iter != PowerUps.end(); ++iter )
	{
		if( iter->Active )
		{
			AABB aabb = iter->Aabb.Get();
			glBox( aabb.x1, aabb.y1, aabb.x2, aabb.y2, color );
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket PowerUpContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 0;
	
	std::vector<PowerUp>::iterator iter;
	for( iter = PowerUps.begin(); iter != PowerUps.end(); ++iter )
	{
		if( iter->Active )
		{
				
			AABB aabb = iter->Aabb.Get();
			
			if( Vic.GetAABB().Intersects(aabb) )
			{
				if( Vic.GetLevel() < 4 )
				{
					Vic.AddToScore( 2000 );
				}
				else
				{
					Vic.AddToScore( 10000 );
				}
				
				Vic.IncLevel();
				Sound::Instance()->PlaySFX( E_SFX_POWERUP );
				iter->Kill();
			}
		}
	}
	
	return Ret;
	
}

//--------------------------------------
//	
//--------------------------------------
void PowerUpContainer::KillAllEntities()
{
	std::vector<PowerUp>::iterator iter;
	for( iter = PowerUps.begin(); iter != PowerUps.end(); ++iter )
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
void PowerUpContainer::Spawn( int _x, int _y )
{
	
	
	std::vector<PowerUp>::iterator iter;
	for( iter = PowerUps.begin(); iter != PowerUps.end(); ++iter )
	{
		if( !iter->Active )		// insert a powerup on a free slot
		{

			iter->x = _x << 12;
			iter->y = _y << 12;
			iter->Dx = UTIL::RND_RANGE(0,inttof32(4)) - inttof32(2);
			iter->Dy = UTIL::RND_RANGE(0,inttof32(4)) - inttof32(2);
			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			break;
		
		}
	}
	
	
}


