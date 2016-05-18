/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Boss class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <gl2d.h>


#include "Vector2df32.h"
#include "cearn_atan.h"
#include "UTIL.h"

#include "Sound.h"
#include "Palettes.h"
#include "Explosion.h"


#include "PlayerShip.h"
#include "EntityContainer.h"

#include "Bullet.h"
#include "Turret.h"
#include "Spline.h"

#ifndef BOSS_H
#define BOSS_H

class Boss
{

public:
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

	Boss();
	~Boss();

	virtual void Spawn( TurretArray &Turrets ) = 0;
	virtual void Update( PlayerShip &Vic ) = 0;
	virtual void Draw() = 0;
	virtual void DrawAABB( int color ) const = 0;
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed ) = 0;
	virtual void BombMe( PlayerShip &Vic, int BombDamageValue ) = 0;
	virtual void ForceDestruction() = 0;
	
	void Init( int Palette, glImage* const Sprites, int _id );
	void Kill();
	void DrawHp( int Frame );
	
	
	// --------- Setters --------------
	void SetActive( bool active )
	{
		Active = active;
	}

	void SetAlive( bool alive )
	{
		Alive = alive;
	}

	void SetX( const s32& _x )
	{
		x = _x;
	}

	void SetY( const s32& _y )
	{
		y = _y;
	}

	const AABB& GetAabb() const
	{
		return Aabb;
	}

	void SetID( int _ID )
	{
		ID = _ID;
	}

	void SetDestroyedCounter( int _DestroyedCounter )
	{
		DestroyedCounter = _DestroyedCounter;
	}

	void DecDestroyedCounter()
	{
		DestroyedCounter--;
	}
	
	void SetDrawMode( const EN_DRAW_MODE& _DrawMode )
	{
		DrawMode = _DrawMode;
	}
	
	// --------- Getters --------------
	bool IsActive() const
	{
		return Active;
	}

	bool IsAlive() const
	{
		return Alive;
	}

	int GetHp() const
	{
		return Hp;
	}

	const s32& GetX() const
	{
		return x;
	}

	const s32& GetY() const
	{
		return y;
	}

	int GetID() const
	{
		return ID;
	}

	int GetDestroyedCounter() const
	{
		return DestroyedCounter;
	}

	int GetNumCannons() const
	{
		return Cannons.size();
	}

	
	void SetHp( int value )
	{
		Hp = value;
	}
	
protected:

	int 	ID;

	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;

	int		Frame;
	int 	Counter;
	int 	Hp;
	int 	MaxHp;
	int 	DeathScore;
	
	bool	Alive;
	bool	Active;


	int		DestroyedCounter;


	EN_DRAW_MODE DrawMode;

	int ColorTable;		// init needed data for drawing
	glImage *Images;

	int		Width;
	int		Height;

	AABB	Aabb;

	std::vector<Cannon> Cannons;

};

#endif // BOSS_H
