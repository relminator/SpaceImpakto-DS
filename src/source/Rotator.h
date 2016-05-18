/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Rotator class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include <gl2d.h>

#include "Vector2df32.h"

#include "Turret.h"
#include "Spline.h"
#include "cearn_atan.h"

#include "EntityContainer.h"

#include "PowerUp.h"

#ifndef ROTATOR_H
#define ROTATOR_H

//------------------------------------------------------------------------------
//	Rotator Enemy
//------------------------------------------------------------------------------
class Rotator 
{

public:
	friend class RotatorContainer;
	
	Rotator();
	~Rotator();
	void Init();
	void Update( PlayerShip &Vic );


private:
	
	// Our enemy option
	struct Cannon
	{
		s32		x;
		s32		y;
		
		int		Width;
		int		Height;
	
		AABB	Aabb;
	
		Vector2df32	TurretPosition;
		Vector2df32	TurretTarget;
		std::vector<Turret>::iterator TurretPtr;
	};
	
	void Destroy();
	void Kill();
	void LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate );
	
	s32		x;
	s32		y;
	int		Frame;
	int 	Counter;
	bool	Active;
	int 	Hp;
	int 	DeathScore;
	int 	Radius;
	
	EN_DRAW_MODE DrawMode;
	
	Spline Path;
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
	
	std::vector<Cannon> Cannons;
	
};

//------------------------------------------------------------------------------
//	Rotator Container
//------------------------------------------------------------------------------
class RotatorContainer : public EntityContainer
{	

public:

	RotatorContainer();
    ~RotatorContainer();
	virtual void UpdateEntities( PlayerShip &Vic );
	virtual void DrawEntities();
	virtual void DrawAABB( int color );
	virtual void KillAllEntities();
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode );
	virtual EntityContainer::CollisionPacket BombEntities( PlayerShip &Vic, int BombDamageValue )
	{
		EntityContainer::CollisionPacket Ret;
		Ret.Collision = 0;
		
		std::vector<Rotator>::iterator iter;
		for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
		{
			if( (iter->Active) && ( iter->x < (256 << 12) ) ) 
			{
				iter->Hp -= BombDamageValue;
				if( iter->Hp < 0 )
				{
					iter->Destroy();
					Vic.AddToScore( iter->DeathScore );
				}
				
			}
		}
		
		return Ret;
		
	}
	
	
	virtual void SetDrawMode( EN_DRAW_MODE mode )
	{
		std::vector<Rotator>::iterator iter;
		for( iter = Rotators.begin(); iter != Rotators.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
	
	void Init(int Palette, glImage* const Sprites );
    void Spawn(  int _x, int _y, bool Below, TurretArray &Turrets  );
	
	
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:

	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<Rotator> Rotators;
	
	
};

#endif // ROTATOR_H
