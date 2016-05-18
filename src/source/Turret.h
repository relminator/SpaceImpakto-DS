/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.phatcode.net

	Turret class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>

#include "UTIL.h"
#include "Vector2df32.h"
#include "cearn_atan.h"
#include "Bullet.h"

#ifndef TURRET_H
#define TURRET_H

class Turret 
{

public:
	
	
	enum MODE
	{
		SINGLE = 0,
		SPRAY_NORMAL,
		SPRAY_CENTERED,
		SPRAY_WAVE,
		SPREAD_OFFSET,
		SPREAD_RINGS,
	};
	
	struct Params
	{

		int				ActiveTime;				// Frames current AI is active
		bool			Active;					// 
		bool			Smart;					// if targeting or not
		int				Xradius;				// 
		int				Yradius;				// 
		s32				Angle;					// StartAngle
		s32				MinAngle;				// 
		s32				MaxAngle;
		s32				AngleDelta;				// + rotation speed
		s32				SpreadAngle;			// distance of each bullet on a spread
		int				AngleDirection;			// -1 or +1  determines rotation direction
		int 			RotationType;			// whether back anf forth or not
		int				SpawnDelay;				// 
		int				MaxSpawnDelay;			// 
		int				BulletsPerShot;
		
		int				WaveCounter;			// 
		int 			MaxWave;				// maximum bullets per wave
		int 			MaxWaveDelay;			// maximum delay for next shot in a wave
		
		s32				BulletSpeedDelta;		// for variable bullet speeds in a single wave
		s32				BulletSpreadOffset;		// for offsetted bullet placement in a single wave
		
		MODE			Mode;
		
		Bullet::Params	Bp;	
	};
	
	
	Turret();
	~Turret();
	
	void Update( BulletArray &Bullets );
	void FirstPattern();
	void NextPattern();
	void DoPattern( unsigned int i );
	void Init( Params const &P, Vector2df32 *position, Vector2df32 *target );
	void Init( Params const &P, s32 px, s32 py, s32 tx, s32 ty );
	void AddPattern( Params const &P );
	void Kill();
	
	unsigned int GetIndex() const
	{
		return Index;
	}
	
	unsigned int GetShotCounter() const
	{
		return ShotCounter;
	}
	
	unsigned int GetNumPatterns() const
	{
		return Patterns.size();
	}
	
	bool IsActivated() const
	{
		return Activated;
	}
	
	void SetTarget( Vector2df32 *target )
	{
		Target = target;
	}
	
	void SetTarget( s32 tx, s32 ty )
	{
		DefaultTarget.x = tx;
		DefaultTarget.y = ty;
		Target = &DefaultTarget;
	}	
	
	
	void SetSpawnDelay( int value )
	{
		SpawnDelay = value;
	}
	
	void SetShotCounter( int value )
	{
		ShotCounter = value;
	}
	
	void SetAngle( s32 value )
	{
		Angle = value;
	}
	
	void Activate()
	{
		Activated = true;
	}
	
	void Deactivate()
	{
		Activated = false;
	}
	
	void ResetShotCounter()
	{
		ShotCounter = 0;
	}
	
private:

	friend class TurretArray;
	
	friend void Rotate1(Turret *t);
	friend void Rotate2(Turret *t);
	friend void Rotate3(Turret *t);
	
	friend void ModeSingle( Turret *t, BulletArray &Bullets );
	friend void ModeSprayNormal( Turret *t, BulletArray &Bullets );
	friend void ModeSprayCentered( Turret *t, BulletArray &Bullets );
	friend void ModeSpreadOffset( Turret *t, BulletArray &Bullets );
	friend void ModeSpreadRings( Turret *t, BulletArray &Bullets );
	
	void (*DoMode)( Turret *t, BulletArray &Bullets );
	void (*Rotate)( Turret *t );
	
	void SetMode( MODE mode );
	void SetRotation( int type );
	
	void ResetPattern( Params const &P );
	void Shoot( BulletArray &Bullets );
	
	Vector2df32		*Target;
	Vector2df32		*Position;
	Vector2df32		DefaultPosition;
	Vector2df32		DefaultTarget;
	
	int				ActiveTime;				// Frames current AI is active
	bool			Active;					// Always true as long as turret is in play
	bool 			Activated;				// switch to updata or not			
	bool			Smart;					// if targeting or not
	int				Xradius;				// 
	int				Yradius;				// 
	s32				Angle;					// StartAngle
	s32				MinAngle;				// 
	s32				MaxAngle;
	s32				AngleDelta;				// + rotation speed
	s32				SpreadAngle;			// distance of each bullet on a spread
	int				AngleDirection;			// -1 or +1  determines rotation direction
	int 			RotationType;			// whether back anf forth or not
	int				SpawnDelay;				// 
	int				MaxSpawnDelay;			// 
	int				BulletsPerShot;
	
	int				WaveCounter;			// 
	int 			MaxWave;				// maximum bullets per wave
	int 			MaxWaveDelay;			// maximum delay for next shot in a wave
	
	s32				BulletSpeedDelta;		// for variable bullet speeds in a single wave
	s32				BulletSpreadOffset;		// for offsetted bullet placement in a single wave
		
	MODE			Mode;
	
	
	Bullet::Params	Bp;
	
	std::vector<Params> Patterns;
	unsigned int Index;
	unsigned int ShotCounter;
	
};



//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
class TurretArray
{

public:

	TurretArray();
	~TurretArray();
	
	void Init( int Palette, glImage* const Sprites, int NumTurrets );
	void UpdateEntities( BulletArray &Bullets );
	void KillEntities();
	
	void Spawn( Turret::Params const &P, 
			    Vector2df32 *position, 
			    Vector2df32 *target
	  		  );
	
	void Spawn( Turret::Params const &P, 
			    s32 px, s32 py,
				s32 tx, s32 ty 
	  		  );
	
	void Spawn( Turret::Params const &P, 
			    Vector2df32 *position, 
			    Vector2df32 *target,
				std::vector<Turret>::iterator &TurretPtr
	  		  );
	
	void Spawn( Turret::Params const &P, 
			    s32 px, s32 py,
				s32 tx, s32 ty, 
				std::vector<Turret>::iterator &TurretPtr
	  		  );
	
	int NumEntities() const
	{
		return numEntities;
	}
	
private:

	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<Turret> Turrets;
	
};


#endif // TURRET_H
