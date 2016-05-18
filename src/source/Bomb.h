/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Viper class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <gl2d.h>
#include "UTIL.h"
#include "Vector2df32.h"
#include "cearn_atan.h"
#include "GlobalManager.h"

#ifndef BOMB_H
#define BOMB_H

#define MAX_BOMBS 32
#define MAX_RAYS 16

#define MAX_COOLDOWN_COUNTER 60 * 6;

class Bomb 
{

public:
	struct Particle : public Vector2df32
	{
		
		s32 Angle;
		s32 Dx;
		s32 Dy;
		s32 Speed;
		int Frame;
		bool Active;
		
	};
	
	struct Ray
	{
		s32 Angle;
		s32 SpreadAngle;
		s32 AngleSpeed;
	};
	
	
	Bomb();
	~Bomb();

	void Init( int Palette, glImage* const Sprites, glImage* const Sprites2, glImage* const Sprites3 );
	void Update( int type );
	void Reset( int type );
	void Draw( int type ) const;
	void DrawAgilaBomb() const;
	void DoAgilaBomb();
	void DrawVicBomb() const;
	void DoVicBomb();
	
	void Activate()
	{
		Active = true;
		DamageValue = 1;
		Counter = 0;
		Ax = -100 << 12;
		CoolDownCounter = MAX_COOLDOWN_COUNTER;
	}
	
	void Deactivate()
	{
		Active = false;
	}
	
	bool IsActive() const
	{
		return Active;
	}
	
private:

	bool Active;
	int Counter;
	int CoolDownCounter;
	int DamageValue;
	glImage *Images;
	glImage *Images2;
	glImage *Images3;
	int ColorTable;
	
	s32 Angle;
	Particle Bombs[MAX_BOMBS];

	s32 Ax;
	s32 Ay;
	s32 Aspeed;
	Ray Rays[MAX_RAYS];
	
};

#endif // BOMB_H
