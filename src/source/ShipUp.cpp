/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	ShipUp class


*******************************************************************************
******************************************************************************/

#include "ShipUp.h"

#define MIN_X ( -8 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_X ( (256 + 8) << 12 )
#define MAX_Y ( (196 + 18) << 12 )

#define MAX_SHIPUPS 3

//------------------------------------------------------------------------------
//	ShipUp Enemy
//------------------------------------------------------------------------------
ShipUp::ShipUp()
{
	Frame = 0;
	Active = false;	
	Width = 20;
	Height = 20;
	
}

ShipUp::~ShipUp()
{
}

//--------------------------------------
//	
//--------------------------------------
void ShipUp::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	x += Dx;
	y += Dy;
	
	
	
	if( (x < MIN_X ) ) 
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
void ShipUp::Kill()
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
//	ShipUp Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
ShipUpContainer::ShipUpContainer()
{
}
	
ShipUpContainer::~ShipUpContainer()
{
	ShipUps.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void ShipUpContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	ShipUps.resize( MAX_SHIPUPS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void ShipUpContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<ShipUp>::iterator iter;
	for( iter = ShipUps.begin(); iter != ShipUps.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void ShipUpContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<ShipUp>::iterator iter;
	for( iter = ShipUps.begin(); iter != ShipUps.end(); ++iter )
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
void ShipUpContainer::DrawAABB( int color )
{
	
	std::vector<ShipUp>::iterator iter;
	for( iter = ShipUps.begin(); iter != ShipUps.end(); ++iter )
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
EntityContainer::CollisionPacket ShipUpContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 0;
	
	std::vector<ShipUp>::iterator iter;
	for( iter = ShipUps.begin(); iter != ShipUps.end(); ++iter )
	{
		if( iter->Active )
		{
				
			AABB aabb = iter->Aabb.Get();
			
			if( Vic.GetAABB().Intersects(aabb) )
			{
				Vic.AddToScore( 10000 );
				Vic.IncLives();
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
void ShipUpContainer::KillAllEntities()
{
	std::vector<ShipUp>::iterator iter;
	for( iter = ShipUps.begin(); iter != ShipUps.end(); ++iter )
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
void ShipUpContainer::Spawn( int _x, int _y )
{
	
	
	std::vector<ShipUp>::iterator iter;
	for( iter = ShipUps.begin(); iter != ShipUps.end(); ++iter )
	{
		if( !iter->Active )		// insert a powerup on a free slot
		{

			iter->x = _x << 12;
			iter->y = _y << 12;
			iter->Dx = -floattof32(0.35);
			iter->Dy = 0;
			iter->Active = true;
			iter->Frame = 0;
			iter->Counter = 0;
			break;
		
		}
	}
	
	
}


