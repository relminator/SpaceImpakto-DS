/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	PowerUp class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include <gl2d.h>
#include "Vector2df32.h"

#include "EntityContainer.h"

#ifndef POWERUP_H
#define POWERUP_H

//------------------------------------------------------------------------------
//	PowerUp Enemy
//------------------------------------------------------------------------------
class PowerUp 
{

public:
	friend class PowerUpContainer;
	
	PowerUp();
	~PowerUp();
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
//	PowerUp Container
//------------------------------------------------------------------------------
class PowerUpContainer : public EntityContainer
{	

public:

	PowerUpContainer();
    ~PowerUpContainer();
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
	
	std::vector<PowerUp> PowerUps;
	
	
};

#endif // POWERUP_H
