/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	LightBall class


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

#ifndef LIGHTBALL_H
#define LIGHTBALL_H

//------------------------------------------------------------------------------
//	LightBall Enemy
//------------------------------------------------------------------------------
class LightBall 
{

public:
	friend class LightBallContainer;
	
	LightBall();
	~LightBall();
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
	int		DeathScore;
	int		FlipMode;
	
	EN_DRAW_MODE DrawMode;
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
		
	Vector2df32	TurretPos;
	Vector2df32	TurretTar;
	std::vector<Turret>::iterator TurretPtr;
	
};

//------------------------------------------------------------------------------
//	LightBall Container
//------------------------------------------------------------------------------
class LightBallContainer : public EntityContainer
{	

public:

	LightBallContainer();
    ~LightBallContainer();
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
		
		std::vector<LightBall>::iterator iter;
		for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
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
		std::vector<LightBall>::iterator iter;
		for( iter = LightBalls.begin(); iter != LightBalls.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
	
	void Spawn( bool Below, TurretArray &Turrets  );
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:
	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<LightBall> LightBalls;
	
	
};


#endif // LIGHTBALL_H
