/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	Centipede class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include <gl2d.h>
#include "Vector2df32.h"

#include "Turret.h"
#include "cearn_atan.h"

#include "EntityContainer.h"

#include "PowerUp.h"

#ifndef CENTIPEDE_H
#define CENTIPEDE_H

#define CENTIPEDE_MAX_SEGMENTS 192

//------------------------------------------------------------------------------
//	Centipede Enemy
//------------------------------------------------------------------------------
class Centipede 
{

public:
	friend class CentipedeContainer;
	
	Centipede();
	~Centipede();
	void Update( PlayerShip &Vic );

private:

	struct Segment
	{
		s32 x;
		s32 y;
		AABB Aabb;
	};
	
	void Destroy();
	void Kill();
	void Draw( glImage* const Images, int ColorTable, 
			   glImage* const Images2, int ColorTable2 ) const;
	void DrawAABB( int color ) const;
	void CollideSegments(  PlayerShip &Vic, bool BombIsActive, EntityContainer::CollisionPacket &Ret );
	void ExplodeSegments() const;
	
	void ResetSegments()
	{
		for( int i = 0; i < CENTIPEDE_MAX_SEGMENTS; i++ )
		{
			Segments[i].x = inttof32(300);
			Segments[i].y = inttof32(96);
		}
	}
	
	s32		x;
	s32		y;

	s32		Dx;
	s32		Dy;

	s32		Speed;
	s32		Angle;
	s32		Delta;
	bool	NewTarget;
	
	int		Frame;
	int 	Counter;
	bool	Active;
	int 	Hp;
	int		DeathScore;
	
	unsigned int ShotsFired;
	
	Vector2df32 Target;
	
	EN_DRAW_MODE DrawMode;
	
	
	int		Width;
	int		Height;
	
	AABB	Aabb;
	
	
	Segment  Segments[CENTIPEDE_MAX_SEGMENTS];
	
	// only need one of these as one of the turrets
	// is non-targeting
	// all positions are in center
	Vector2df32	TurretPosition;		
	Vector2df32	TurretTarget;	
	std::vector<Turret>::iterator TurretPtr;
	
};

//------------------------------------------------------------------------------
//	Centipede Container
//------------------------------------------------------------------------------
class CentipedeContainer : public EntityContainer
{	

public:

	CentipedeContainer();
    ~CentipedeContainer();
	void Init( int Palette, glImage* const Sprites,
			   int Palette2, glImage* const Sprites2 );
    virtual void UpdateEntities( PlayerShip &Vic );
	virtual void DrawEntities();
	virtual void DrawAABB( int color );
	virtual void KillAllEntities();
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed, int &BulletSpawnMode );
	virtual EntityContainer::CollisionPacket BombEntities( PlayerShip &Vic, int BombDamageValue )
	{
		EntityContainer::CollisionPacket Ret;
		Ret.Collision = 0;
		
		std::vector<Centipede>::iterator iter;
		for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
		{
			if( iter->Active ) 
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
		std::vector<Centipede>::iterator iter;
		for( iter = Centipedes.begin(); iter != Centipedes.end(); ++iter )
		{
			if( iter->Active ) iter->DrawMode = mode;
		}
	}
		
	
	void Spawn(  int _x, int _y, TurretArray &Turrets  );
	virtual int NumEntities() 
	{
		return numEntities;
	}
	
private:
	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	glImage *ImagesSegments;
	int ColorTableSegments;
	
	std::vector<Centipede> Centipedes;
	
	
};


#endif // CENTIPEDE_H
