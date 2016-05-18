/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	TranspormaBoss class


*******************************************************************************
******************************************************************************/

#ifndef TRANSPORMABOSS_H
#define TRANSPORMABOSS_H

#include "Boss.h" // Base class: Boss
#include "Bezier.h"

#define TRANSPORMA_MAX_SEGMENTS 8

class TranspormaBoss : public Boss 
{

public:
	struct Part
	{
		s32 x;
		s32 y;
		int Width;
		int Height;
		Spline Path;
		AABB Aabb;
	};
	
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
	void LoadPartPath( Part &Prt, int _sx, int _sy, int _ex, int _ey );
	void DrawLinks( s32 sx, s32 sy, s32 ex, s32 ey, int Frame, glImage *Sprite );
	void ExplodeLinks( s32 sx, s32 sy, s32 ex, s32 ey, int Frame );	
	void AnimateLinks();
	void CloseLinks();
	
	Part Parts[3];
	Spline Path;
	
	int LinkFrame;
	
	
	
};

#endif // TRANSPORMABOSS_H
