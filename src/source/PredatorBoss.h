/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Predator Boss class


*******************************************************************************
******************************************************************************/

#ifndef PREDATORBOSS_H
#define PREDATORBOSS_H

#include "Boss.h" // Base class: Boss

class PredatorBoss : public Boss 
{

public:
	virtual void Spawn( TurretArray &Turrets );
	virtual void Update( PlayerShip &Vic );
	virtual void Draw();
	virtual void DrawAABB( int color ) const;
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed );
	virtual void BombMe( PlayerShip &Vic, int BombDamageValue );
	virtual void ForceDestruction();
	
private:

	void LoadPath();
	void Destroy();
	
	Spline Path;
	
	
};

#endif // PREDATORBOSS_H
