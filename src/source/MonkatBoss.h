/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Predator Boss class


*******************************************************************************
******************************************************************************/

#ifndef MONKATBOSS_H
#define MONKATBOSS_H

#include "Boss.h" // Base class: Boss

class MonkatBoss : public Boss 
{

public:
	virtual void Spawn( TurretArray &Turrets );
	virtual void Update( PlayerShip &Vic );
	virtual void Draw();
	virtual void DrawAABB( int color ) const;
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed );
	virtual void BombMe( PlayerShip &Vic, int BombDamageValue );
	virtual void ForceDestruction();
	
	void Init( int Palette, glImage* const Sprites,
			   int Palette2, glImage* const Sprites2,
			   int _id );

private:

	void LoadPath();
	void Destroy();
	
	Spline Path;
	
	s32 Angle;
	int ColorTable2;		// init needed data for drawing
	glImage *Images2;
	
};


#endif // MONKATBOSS_H
