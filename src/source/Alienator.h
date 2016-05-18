/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Alienator class


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

#ifndef ALIENATOR_H
#define ALIENATOR_H

//------------------------------------------------------------------------------
//	Alienator Enemy
//------------------------------------------------------------------------------
class Alienator 
{

public:
	friend class AlienatorContainer;
	
	Alienator();
	~Alienator();
	void Update( PlayerShip &Vic );


private:
	
	void LoadPath( int _sx, int _sy, int _ex, int _ey, bool negate );
	void Destroy();
	void Kill();
	
	s32		x;
	s32		y;
	int		Frame;
	int 	Counter;
	bool	Active;
	int 	Hp;
	int		DeathScore;
	
	Spline Path;
	
	EN_DRAW_MODE DrawMode;
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
		
	Vector2df32	TurretPos;
	Vector2df32	TurretTar;
	std::vector<Turret>::iterator TurretPtr;
	
};

//------------------------------------------------------------------------------
//	Alienator Container
//------------------------------------------------------------------------------
class AlienatorContainer : public EntityContainer
{	

public:

	AlienatorContainer();
    ~AlienatorContainer();
	void Init(int Palette, glImage* const Sprites );
    virtual void UpdateEntities( PlayerShip &Vic );
	virtual void DrawEntities();
	virtual void DrawAABB( int color );
	virtual void KillAllEntities();
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode );
	virtual EntityContainer::CollisionPacket BombEntities( PlayerShip &Vic, int BombDamageValue )
	{
		EntityContainer::CollisionPacket Ret;
		Ret.Collision = 0;
		
		std::vector<Alienator>::iterator iter;
		for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
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
		std::vector<Alienator>::iterator iter;
		for( iter = Alienators.begin(); iter != Alienators.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
	
	void Spawn( int _x, int _y, bool Below, TurretArray &Turrets  );
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:
	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<Alienator> Alienators;
	
	
};



#endif // ALIENATOR_H
