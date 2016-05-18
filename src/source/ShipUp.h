/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	ShipUp class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include <gl2d.h>
#include "Vector2df32.h"

#include "EntityContainer.h"

#ifndef SHIPUP_H
#define SHIPUP_H

//------------------------------------------------------------------------------
//	ShipUp Enemy
//------------------------------------------------------------------------------
class ShipUp 
{

public:
	friend class ShipUpContainer;
	
	ShipUp();
	~ShipUp();
	void Update( PlayerShip &Vic );
	
private:
	void Kill();
	
	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;
	int		Frame;
	int 	Counter;
	bool	Active;
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
	
	
};

//------------------------------------------------------------------------------
//	ShipUp Container
//------------------------------------------------------------------------------
class ShipUpContainer : public EntityContainer
{	

public:

	ShipUpContainer();
    ~ShipUpContainer();
	void Init( int Palette, glImage* const Sprites );
    virtual void UpdateEntities( PlayerShip &Vic );
	virtual void DrawEntities();
	virtual void DrawAABB( int color );
	virtual void KillAllEntities();
	virtual CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode );
	virtual EntityContainer::CollisionPacket BombEntities( PlayerShip &Vic, int BombDamageValue )
	{
		EntityContainer::CollisionPacket Ret;
		Ret.Collision = 0;
		
		return Ret;
	}
	
	virtual void SetDrawMode( EN_DRAW_MODE mode )
	{
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
	
	std::vector<ShipUp> ShipUps;
	
	
};


#endif // SHIPUP_H
