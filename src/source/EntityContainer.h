/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	EntityContainer class


*******************************************************************************
******************************************************************************/


#include "PlayerShip.h"
#include "Targets.h"

#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H

enum EN_DRAW_MODE
{
	EN_NORMAL = 0,
	EN_COLOR,
	EN_WHITE
};


class PowerUpContainer;

class EntityContainer 
{

public:

	struct CollisionPacket
	{
		int x;
		int y;
		int Collision;
	};
	
    virtual void UpdateEntities( PlayerShip &Vic ) = 0;
	virtual void DrawEntities() = 0;
	virtual void DrawAABB( int color ) = 0;
	virtual void KillAllEntities() = 0;
	virtual CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode ) = 0;
	virtual EntityContainer::CollisionPacket BombEntities( PlayerShip &Vic, int BombDamageValue ) = 0;
	virtual int NumEntities() = 0;
	
	virtual void SetDrawMode( EN_DRAW_MODE mode ) = 0;
};



#endif // ENTITYCONTAINER_H
