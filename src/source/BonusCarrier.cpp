/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	BonusCarrier class


*******************************************************************************
******************************************************************************/

#include "BonusCarrier.h"

#define MIN_X ( -20 << 12 )
#define MIN_Y ( -18 << 12 )

#define MAX_BONUSCARRRIERS 3
#define BONUSCARRRIER_MAX_HP 1

//------------------------------------------------------------------------------
//	BonusCarrier Enemy
//------------------------------------------------------------------------------
BonusCarrier::BonusCarrier()
{
	Frame = 0;
	Active = false;	
	Hp = BONUSCARRRIER_MAX_HP; 
	DeathScore = Hp * 8;
	DrawMode = EN_NORMAL;
	Width = 32;
	Height = 32;
	
}

BonusCarrier::~BonusCarrier()
{
}

//--------------------------------------
//	
//--------------------------------------
void BonusCarrier::Update( PlayerShip &Vic )
{
	
	Counter++;
	
	
	x += Dx;
	y += Dy;
	
	if( x < MIN_X )
	{
		Kill();
		return;
	}
	
	Targets::Instance()->AddCoord( x, y );
	
	// animate
	if( ( Counter & 3 ) == 0 )
	{
		Frame++;
		if( Frame > 2 ) Frame = 0;
	}
	
	// Reset AABB
	Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
			   Width, Height 
			 );
	

}

//--------------------------------------
//	
//--------------------------------------
void BonusCarrier::Destroy()
{
	Explosion::Instance()->Spawn( x >> 12,
								  y >> 12,
								  0,
								  0,
								  Explode::MEDIUM_4,
								  Explode::SMALL_2
								);			
	Kill();
	Sound::Instance()->PlaySFX( E_SFX_EXP_SMALL );
}


//--------------------------------------
//	
//--------------------------------------
void BonusCarrier::Kill()
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
//	BonusCarrier Container
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BonusCarrierContainer::BonusCarrierContainer()
{
}
	
BonusCarrierContainer::~BonusCarrierContainer()
{
	BonusCarriers.clear();				// clear the bullets
}

//--------------------------------------
//	
//--------------------------------------
void BonusCarrierContainer::Init( int Palette, glImage* const Sprites )
{
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	BonusCarriers.resize( MAX_BONUSCARRRIERS );			// speed of iteration)
}

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BonusCarrierContainer::UpdateEntities( PlayerShip &Vic )
{
	
	std::vector<BonusCarrier>::iterator iter;
	for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
	{
		if( iter->Active )
			iter->Update( Vic );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BonusCarrierContainer::DrawEntities()
{
	
	numEntities = 0;
	std::vector<BonusCarrier>::iterator iter;
	for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if( (iter->DrawMode == EN_WHITE) )
			{
				iter->DrawMode = EN_NORMAL;
				glColor( RGB15(31,0,0) );
				glSprite( (iter->x >> 12) - 16, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[62+iter->Frame] 
					    );
				glColor( RGB15(31,31,31) );
	
			}
			else
			{
				glSprite( (iter->x >> 12) - 16, (iter->y >> 12) - 16, 
					      GL_FLIP_NONE, &Images[62+iter->Frame] 
					    );
					
			}
			
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void BonusCarrierContainer::DrawAABB( int color )
{
	
	std::vector<BonusCarrier>::iterator iter;
	for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
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
EntityContainer::CollisionPacket BonusCarrierContainer::Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	IsDestroyed = false;
	BulletSpawnMode = 0;
	
	std::vector<BonusCarrier>::iterator iter;
	for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
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
						Ret.Collision = 1;
						iter->Destroy();
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
						Ret.Collision = 1;
						iter->Destroy();
						Vic.AddToScore( iter->DeathScore );
					}
					
				}
			
			}  // end if (x < 256)
		}
	}
	
	return Ret;
	
}

//--------------------------------------
//	
//--------------------------------------
void BonusCarrierContainer::KillAllEntities()
{
	std::vector<BonusCarrier>::iterator iter;
	for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
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
void BonusCarrierContainer::Spawn( int _x, int _y )
{
	
	
	std::vector<BonusCarrier>::iterator iter;
	for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Active = true;
			iter->x = _x << 12;
			iter->y = _y << 12;
			iter->Dx = -2048;
			iter->Dy = 0;
			iter->Frame = 0;
			iter->Counter = 0;
			iter->Hp = BONUSCARRRIER_MAX_HP;

			
			break;
		
		}
	}
	
	
}

