/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	GluteusBoss class


*******************************************************************************
******************************************************************************/

#ifndef GLUTEUSBOSS_H
#define GLUTEUSBOSS_H

#include "Boss.h" // Base class: Boss
#include "Bezier.h"

#define GLUTEUS_MAX_SEGMENTS 8

class GluteusBoss : public Boss 
{

public:
	struct Part
	{
		s32 x;
		s32 y;
		int Width;
		int Height;
		s32 Angle;
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
	void LoadPartPath( Part &Prt, int _sx, int _sy, int _ex, int _ey, int CannonID );
	void AnimateLinks();
	void CloseLinks();
	
	Part Parts[3];
	Spline Path;
	
	int LinkFrame;
	
	
	
};

#endif // GLUTEUSBOSS_H
