/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	Frisbee class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include <gl2d.h>
#include "Vector2df32.h"

#include "Turret.h"
#include "Bezier.h"
#include "cearn_atan.h"

#include "EntityContainer.h"

#include "PowerUp.h"

#ifndef FRISBEE_H
#define FRISBEE_H

//------------------------------------------------------------------------------
//	Frisbee Enemy
//------------------------------------------------------------------------------
class Frisbee 
{

public:
	friend class FrisbeeContainer;
	
	Frisbee();
	~Frisbee();
	void Update( PlayerShip &Vic );

	
private:
	void LoadPath( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist );
	void Destroy();
	void Kill();
	
	s32		x;
	s32		y;
	
	int		Frame;
	int 	Counter;
	int		NewCounter;
	bool	Active;
	int 	Hp;
	int		DeathScore;
	int 	Delay;
	
	EN_DRAW_MODE DrawMode;
	
	Bezier Path;
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
	
	Vector2df32	TurretPosition;		
	Vector2df32	TurretTarget;	
	std::vector<Turret>::iterator TurretPtr;
		
};

//------------------------------------------------------------------------------
//	Frisbee Container
//------------------------------------------------------------------------------
class FrisbeeContainer : public EntityContainer
{	

public:

	FrisbeeContainer();
    ~FrisbeeContainer();
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
		
		std::vector<Frisbee>::iterator iter;
		for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
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
		std::vector<Frisbee>::iterator iter;
		for( iter = Frisbees.begin(); iter != Frisbees.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
	
	void Spawn( Bezier::Coord* _points, Bezier::Table* _lut, int LutSize, s32 rate, s32 totaldist, int delay, int frame, TurretArray &Turrets  );
	
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:
	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<Frisbee> Frisbees;
	
	
};


#endif // FRISBEE_H
