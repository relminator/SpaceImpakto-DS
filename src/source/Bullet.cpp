/******************************************************************************
*******************************************************************************

	Bullet
	relminator
	http://rel.betterwebber.com


*******************************************************************************
******************************************************************************/


#include "Bullet.h"


static const s32 FRICTION = (0.0042) * (1 << 12);
static const s32 INVFRICTION = (0.99) * (1 << 12);

//------------------------------------------------------------------------------
//	Bullet 
//------------------------------------------------------------------------------
Bullet::Bullet()
{
	
	Active = false;
	Frame = 0;
	NumFrames = 1;
	Target = 0;
	Width = 2;
	Height = 2;
	
}

Bullet::~Bullet()
{
	
}


//------------------------------------------------------------------------------
//	Bullet Array
//------------------------------------------------------------------------------
BulletArray::BulletArray()
{
	
}

BulletArray::~BulletArray()
{
	Bullets.clear();
}

//--------------------------------------
//	
//--------------------------------------
void BulletArray::Init( int Palette, glImage* const Sprites, int NumBullets )
{
	
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Bullets.resize( NumBullets );	 // Init array to maxbullets   
	
}

//--------------------------------------
//	
//--------------------------------------
void BulletArray::UpdateEntities()
{
	
	std::vector<Bullet>::iterator iter;
	for( iter = Bullets.begin(); iter != Bullets.end(); ++iter )
	{
		if( iter->Active )
			iter->Update();
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BulletArray::DrawEntities()
{
	
	numEntities = 0;
	std::vector<Bullet>::iterator iter;
	
	
	for( iter = Bullets.begin(); iter != Bullets.end(); ++iter )
	{
		if( iter->Active )
		{
			numEntities++;
			if(iter->AniMode == Bullet::DEFAULT)
			{
				glSprite( (iter->x >> 12) - iter->Width/2,
						  (iter->y >> 12) - iter->Height/2, 
						  GL_FLIP_NONE, &Images[iter->DrawFrame] 
						);
			}
			else
			{
				glSpriteRotate( (iter->x >> 12),
								(iter->y >> 12),
								iter->Rotation,
								GL_FLIP_NONE, &Images[iter->DrawFrame] 
							  );
				
			}
					
			
		}
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
void BulletArray::KillEntities()
{

	std::vector<Bullet>::iterator iter;
	for( iter = Bullets.begin(); iter != Bullets.end(); ++iter )
	{
		if(iter->Active)
		{
			Explosion::Instance()->Spawn( iter->x >> 12,
								  iter->y >> 12,
								  iter->Dx/2,
								  iter->Dy/2,
								  Explode::SMALL_1
								);			
		
			iter->Kill();
		}
		
	}
	
}

//--------------------------------------
//	
//--------------------------------------
EntityContainer::CollisionPacket BulletArray::Collide(  PlayerShip &Vic  )
{
	EntityContainer::CollisionPacket Ret;
	
	Ret.Collision = 0;
	
	std::vector<Bullet>::iterator iter;
	for( iter = Bullets.begin(); iter != Bullets.end(); ++iter )
	{
		if( iter->Active )
		{
			AABB aabb = iter->Aabb.Get();
			
			if( !Vic.IsInvincible() )
			{
				if( Vic.GetAABB().Intersects(aabb) )
				{
					Ret.x = Vic.GetX() >> 12;
					Ret.y = Vic.GetY() >> 12;
					if( Vic.IsAutoBomb() )
					{
						if( Vic.GetBombs() > 0 )
						{
							 Vic.SetThrowBomb(true);	
						}
						else
						{
							Ret.Collision = 2;						
							Vic.Kill();
						}
					}
					else
					{
						Ret.Collision = 2;						
						Vic.Kill();
					}
				}
			}
			
		}
		
	}
	
	return Ret;
}


//--------------------------------------
//	
//--------------------------------------
void BulletArray::DrawAABB(int color)
{
	
	std::vector<Bullet>::iterator iter;
	
	
	for( iter = Bullets.begin(); iter != Bullets.end(); ++iter )
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
void BulletArray::SpawnBullets( Bullet::Params &Bp,
								s32 Px,
								s32 Py,
								int BulletsPerShot,
								s32 SpreadAngle,
								s32 BulletSpeedDelta,
								Vector2df32 *target )
{
	
	
	if( BulletsPerShot > 1)
	{
		s32 BpsDiv2 = ((BulletsPerShot-1) << 12) / 2;
		
		s32 BulletOffset = 1 << 12;
		
		s32 TargetAngle = atan2Lookup( target->x - Px, target->y - Py );
		
		int SpreadRot = (SpreadAngle * BulletOffset) >> 12;
		s32 angle = TargetAngle - mulf32((((BulletsPerShot + 1)<<12)/2), (SpreadRot));
	
		for( int i = 0; i < BulletsPerShot; i++ )
		{
			
			s32 CenterFactor = ( (abs(inttof32(i) - BpsDiv2) << 12) / BpsDiv2 ); //12
			s32 factor = Bp.Speed + ( (BulletSpeedDelta * CenterFactor) >> 12 );  // 12
			
			angle += (SpreadAngle);		// 12
			s32 CosA = cosLerp(angle);
			s32 SinA = sinLerp(angle);
			
			
			Bp.x = Px;
			Bp.y = Py;
		
			Bp.Dx = (CosA * factor) >> 12;
			Bp.Dy = (SinA * factor) >> 12;
			Bp.AccelX = Bp.Dx / 2;
			Bp.AccelY = Bp.Dy / 2;
			Bp.Angle = angle;
			Bp.Rotation = angle;
			
			Spawn( Bp, target );
		
		}
	}
	else
	{
		
		s32 TargetAngle = atan2Lookup( target->x - Px, target->y - Py );
		
		s32 CosA = cosLerp(TargetAngle);
		s32 SinA = sinLerp(TargetAngle);
		
		Bp.x = Px;
		Bp.y = Py;
		
		Bp.Dx = (CosA * Bp.Speed) >> 12;
		Bp.Dy = (SinA * Bp.Speed) >> 12;
	
		Bp.AccelX = Bp.Dx / 2;
		Bp.AccelY = Bp.Dy / 2;
		Bp.Angle = TargetAngle;
		Bp.Rotation = TargetAngle;
		
		Spawn( Bp, target );
		
	}
	
}

//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------
void StopAndGo( Bullet *B )
{
	
	if( B->Counter == B->StopDelay )		//stop
	{
		B->DxO = B->Dx;
		B->DyO = B->Dy;
		B->Dx = 0;
		B->Dy = 0;     	
	}

	if( (B->Counter > B->StopDelay ) && (B->Counter < B->StopDelay  * 3) )  // wait	  
	{
		B->Dx = 0;
		B->Dy = 0;     	
	}
	
	if( B->Counter == B->StopDelay * 3 )		//move again			
	{
		if( B->Smart )
		{
			sassert( B->Target, "Bullet Target Undefined!!!" );
			B->Angle = atan2Lookup( B->Target->x -  B->x, B->Target->y - B->y );
			
			if( B->Type == Bullet::ACCEL )
			{
				B->Dx = 0;
				B->Dy = 0;
				B->AccelX = ( cosLerp(B->Angle) * B->Speed ) >> 14;   // * 0.25
				B->AccelY = ( sinLerp(B->Angle) * B->Speed ) >> 14;					
			}
			else
			{
				B->Dx = ( cosLerp(B->Angle) * B->Speed ) >> 12;
				B->Dy = ( sinLerp(B->Angle) * B->Speed ) >> 12;			
			}
			
		}
		else
		{
			B->Dx = B->DxO;
			B->Dy = B->DyO;
			
			if( B->Type == Bullet::ACCEL )
			{
				B->AccelX /= 2;
				B->AccelY /= 2;
			}
			
		}
	}


}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void TypeNormal( Bullet *B )
{

	B->Dx += B->Gx; 
	B->Dy += B->Gy;					
	B->x += B->Dx;  
	B->y += B->Dy;	 
	
	if ( B->StopDelay >= 0 )
	{
		StopAndGo( B );
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void TypeAccel( Bullet *B )
{

	B->AccelX += B->Gx;
	B->AccelY += B->Gy;

	B->Dx += B->AccelX;
	B->Dy += B->AccelY;

	B->Dx = B->Dx - ( (B->Dx * FRICTION) >> 12 );   
	B->Dy = B->Dy - ( (B->Dy * FRICTION) >> 12 );

	B->x += B->Dx; 
	B->y += B->Dy;	
	
	if ( B->StopDelay >= 0 )
	{
		StopAndGo( B );
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void TypeDecel( Bullet *B )
{

	B->Dx += B->Gx; 
	B->Dy += B->Gy;		
	
	if( ((B->Dx * B->Dx + B->Dy * B->Dy) >> 12) > 4096 )  			 
	{
		B->Dx = (B->Dx * INVFRICTION) >> 12;
		B->Dy = (B->Dy * INVFRICTION) >> 12;
	}
	
	B->x += B->Dx;  
	B->y += B->Dy;	 
	
	if ( B->StopDelay >= 0 )
	{
		StopAndGo( B );
	}
	
}




