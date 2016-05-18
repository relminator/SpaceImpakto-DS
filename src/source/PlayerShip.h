/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	PlayerShip class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <cstdio>
#include <gl2d.h>
#include "Vector2df32.h"
#include "cearn_atan.h"
#include "UTIL.h"
#include "Button.h"
#include "Shot.h"
#include "HomingMissile.h"
#include "Targets.h"
#include "Bomb.h"

#include "Sound.h"
#include "Palettes.h"
#include "Explosion.h"
#include "Button.h"

#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H


#define  MAX_PLAYERSHIP_BULLETS 32
#define MAX_PLAYERSHIP_MISSILES 4



class PlayerShip
{
public:
	enum SHIP_ID
	{
		VIPER = 0,
		R9,
		VIC
	};

	enum DIRECTION
	{
		NEUTRAL = 0,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum STATE
	{
		E_INVINCIBLE 	= (1 << 0),				// after death or stage start and end
		E_LOCKED 		= (1 << 1)				// stage start/end
	};

	enum E_MODE
	{
		E_ENTER     	= 0,				// stage start
		E_EXIT  		= 1 				// stage end
	};


	PlayerShip();
	~PlayerShip();

	virtual void Init( int Palette, glImage* const Sprites ) = 0;
	virtual bool Update( int KeyH, int KeyD, Bomb &Bomber ) = 0;
	virtual bool Replay( int Frame, Bomb &Bomber, int Stage ) = 0;
	virtual void Draw() const  = 0;
	virtual void DrawShots() const  = 0;
	virtual void DrawAABB( int color ) const = 0;

	void SetTargets();
	void NewStage();
	void NewLife();
	void NewGame();
	int LoadReplay( const Button::ReplayType &Replay );
	void Kill();
	void KillArmament();

	inline int CollideShots(AABB &aabb)
	{
		int Collisions = 0;
		for( int i = 0; i < MAX_PLAYERSHIP_BULLETS; i++)
		{
			if( ( Shots[i].IsActive() ) )
			{

				AABB Box = Shots[i].GetAABB();
				if( Box.Intersects(aabb) )
				{
					Explosion::Instance()->Spawn( Shots[i].GetX() >> 12,
					                              Shots[i].GetY() >> 12,
					                              1 << 12,
					                              0,
					                              Explode::TINY
					                            );

					Shots[i].Kill();
					Collisions++;
					Sound::Instance()->PlaySFX( E_SFX_ENEMY_HIT );
				}
			}
		}  // next

		return Collisions * DamageMultiplier;

	}


	inline int CollideMissiles(AABB &aabb)
	{
		int Collisions = 0;
		for( int i = 0; i < MAX_PLAYERSHIP_MISSILES; i++)
		{
			if( Missiles[i].IsActive() )
			{

				AABB Box = Missiles[i].GetAABB();
				if( Box.Intersects(aabb) )
				{
					Explosion::Instance()->Spawn( Missiles[i].GetX() >> 12,
					                              Missiles[i].GetY() >> 12,
					                              0,
					                              0,
					                              Explode::SMALL_1
					                            );

					Collisions += Missiles[i].GetEnergy();
					Missiles[i].Kill();
					Sound::Instance()->PlaySFX( E_SFX_ENEMY_HIT );
				}
			}
		}  // next

		return Collisions;

	}

	inline bool	IsInvincible() const
	{
		return State & E_INVINCIBLE;
	}

	inline bool	IsLocked() const
	{
		return State & E_LOCKED;
	}

	inline bool	IsAlive() const
	{
		return Alive;
	}

	inline bool	IsActive() const
	{
		return Active;
	}
	
	inline bool	IsAutoBomb() const
	{
		return AutoBomb;
	}
	
	inline E_MODE GetMode() const
	{
		return Mode;
	}
	
	inline AABB GetAABB() const
	{
		return Aabb;
	}

	inline Shot &GetShot( int i )
	{
		return Shots[i];
	}

	inline s32 GetX() const
	{
		return x;
	}

	inline s32 GetY() const
	{
		return y;
	}

	inline int GetID() const
	{
		return ID;
	}

	inline int GetLives() const
	{
		return Lives;
	}

	inline int GetBombs() const
	{
		return Bombs;
	}

	inline int GetScore() const
	{
		return Score;
	}

	inline int GetNewLifeCounter() const
	{
		return NewLifeCounter;
	}

	inline int GetLevel() const
	{
		return Level;
	}

	inline int GetNumShots() const
	{
		return NumShots;
	}

	inline void DecNewLifeCounter()
	{
		NewLifeCounter--;
	}

	inline void IncLevel()
	{
		if( Level < 4 ) Level++;
	}

	inline void IncBombs()
	{
		Bombs++;
	}

	inline void DecBombs()
	{
		if( Bombs>0 ) Bombs--;
	}

	inline void IncLives()
	{
		Lives++;
	}


	inline Vector2df32 *GetPosition()
	{
		return &Position;
	}
	
	inline int GetControlType()
	{
		return ControlType;
	}

	inline void SetActive( bool v)
	{
		Active = v;
	}

	inline void SetMode(const E_MODE& _Mode)
	{
		Mode = _Mode;
	}

	inline void SetState(int _State)
	{
		State |= _State;
	}

	inline void SetControlType( int value )
	{
		ControlType = value;
	}

	inline void AddToScore( int value)
	{
		Score += value;
	}


	inline void SetID( int value )
	{
		ID = (SHIP_ID)value;
	}

	inline void SetLevel( int value )
	{
		Level = value;
	}

	inline void SetLives( int value )
	{
		Lives = value;
	}

	inline void SetBombs( int value )
	{
		Bombs = value;
	}

	inline void SetScore( int value )
	{
		Score = value;
	}

	inline void SetThrowBomb( bool value )
	{
		ThrowBomb = value;
	}
	
	inline void SetAutoBomb( bool value)
	{
		AutoBomb = value;
	}
	
protected:

	SHIP_ID ID;
	int		Level;
	int		Score;
	int 	Lives;
	int 	Bombs;
	int 	DamageMultiplier;
	E_MODE 	Mode;		// Enter Exit
	int     ControlType;   // 0 = Normal, 1 = Replay
	bool	Alive;
	int		NewLifeCounter;	// when to spawn another star
	int		StateCounter;	// invincible

	int			State;		// Auto or manual
	DIRECTION 	Direction;

	bool	Active;
	s32		x;
	s32		y;

	s32		Dx;
	s32		Dy;
	s32		AnimationSpeed;

	int 	Counter;
	int 	ShotSpawnDelay;
	int 	MissileSpawnDelay;
	int 	ShotFrameOffset;
	int 	WaveShotFrameOffset;
	s32 	ShotAngleOffset;
	int 	MissileFrameOffset;
	int 	MissileRotation;



	int		Frame;
	int 	BaseFrame;

	int		NumShots;
	int 	NumMissiles;
	
	bool 	ThrowBomb;
	bool 	AutoBomb;
	int		JetFrame;

	int		Width;
	int		Height;

	AABB	Aabb;

	Shot 	Shots[MAX_PLAYERSHIP_BULLETS];

	HomingMissile Missiles[MAX_PLAYERSHIP_MISSILES];


	Vector2df32	Position;


	glImage *Images;
	int ColorTable;


};


#endif // PLAYERSHIP_H
