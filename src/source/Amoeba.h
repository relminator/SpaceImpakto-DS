/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Amoeba class


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

#ifndef AMOEBA_H
#define AMOEBA_H

//------------------------------------------------------------------------------
//	Amoeba Enemy
//------------------------------------------------------------------------------
class Amoeba 
{

public:
	friend class AmoebaContainer;
	
	Amoeba();
	~Amoeba();
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
	
	
	s32		Speed;
	s32		Angle;
	s32		Delta;
	s32		Ey;
	bool	NewTarget;
	
	Vector2df32 Target;
	
	
	EN_DRAW_MODE DrawMode;
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
		
	Vector2df32	TurretPos;
	Vector2df32	TurretTar;
	std::vector<Turret>::iterator TurretPtr;
	
};

//------------------------------------------------------------------------------
//	Amoeba Container
//------------------------------------------------------------------------------
class AmoebaContainer : public EntityContainer
{	

public:

	AmoebaContainer();
    ~AmoebaContainer();
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
		
		std::vector<Amoeba>::iterator iter;
		for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
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
		std::vector<Amoeba>::iterator iter;
		for( iter = Amoebas.begin(); iter != Amoebas.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
	
	void Spawn(  int sy, int ey,  TurretArray &Turrets  );
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:
	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<Amoeba> Amoebas;
	
	
};



#endif // AMOEBA_H
