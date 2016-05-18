/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	BonusCarrier class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include <gl2d.h>
#include "Vector2df32.h"

#include "Turret.h"
#include "cearn_atan.h"

#include "EntityContainer.h"

#include "PowerUp.h"

#ifndef BONUSCARRIER_H
#define BONUSCARRIER_H

//------------------------------------------------------------------------------
//	BonusCarrier Enemy
//------------------------------------------------------------------------------
class BonusCarrier 
{

public:
	friend class BonusCarrierContainer;
	
	BonusCarrier();
	~BonusCarrier();
	void Update( PlayerShip &Vic );
	
private:
	void Destroy();
	void Kill();
	
	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;
	int		Frame;
	int 	Counter;
	bool	Active;
	int 	Hp;
	int 	DeathScore;
	
	EN_DRAW_MODE DrawMode;
	
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
	
	
};

//------------------------------------------------------------------------------
//	BonusCarrier Container
//------------------------------------------------------------------------------
class BonusCarrierContainer : public EntityContainer
{	

public:

	BonusCarrierContainer();
    ~BonusCarrierContainer();
	void Init( int Palette, glImage* const Sprites );
    virtual void UpdateEntities( PlayerShip &Vic );
	virtual void DrawEntities();
	virtual void DrawAABB( int color );
	virtual void KillAllEntities();
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode );
	virtual EntityContainer::CollisionPacket BombEntities( PlayerShip &Vic, int BombDamageValue )
	{
		
		EntityContainer::CollisionPacket Ret;
		Ret.Collision = 0;
		
		std::vector<BonusCarrier>::iterator iter;
		for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
		{
			if( (iter->Active) && ( iter->x < (256 << 12) ) ) 
			{
				iter->Hp -= BombDamageValue;
				if( iter->Hp < 0 )
				{
					Ret.x = iter->x >> 12;
					Ret.y = iter->y >> 12;
					Ret.Collision = 1;
					iter->Destroy();
					Vic.AddToScore( iter->DeathScore );
				}
				
			}
		}
		
		return Ret;
	}
	
	virtual void SetDrawMode( EN_DRAW_MODE mode )
	{
		std::vector<BonusCarrier>::iterator iter;
		for( iter = BonusCarriers.begin(); iter != BonusCarriers.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
	
	void Spawn(  int _x, int _y );
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:
	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<BonusCarrier> BonusCarriers;
	
	
};

#endif // BONUSCARRIER_H
