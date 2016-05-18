/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	TentakillBoss class


*******************************************************************************
******************************************************************************/

#ifndef TENTAKILLBOSS_H
#define TENTAKILLBOSS_H

#include "Boss.h" // Base class: Boss
#include "Matrix2D.h"

#define TENTAKILL_MAX_SEGMENTS 8

class TentakillBoss : public Boss 
{

public:
	struct Segment
	{
		s32		x;
		s32		y;
		s32		Angle;
		AABB	Aabb;
	};

	virtual void Spawn( TurretArray &Turrets );
	virtual void Update( PlayerShip &Vic );
	virtual void Draw();
	virtual void DrawAABB( int color ) const;
	virtual EntityContainer::CollisionPacket Collide( PlayerShip &Vic, bool BombIsActive, bool &IsDestroyed );
	virtual void BombMe( PlayerShip &Vic, int BombDamageValue );
	virtual void ForceDestruction();
	
	void Init( int Palette, glImage* const Sprites,
			   int Palette2, glImage* const Sprites2,
			   int _id );
	
private:

	void LoadPath();
	void Destroy();
	
	void UpdateArms();
	void DrawArms();
	void CloseArms();
	void OpenArms();   // wow! a pun!
	void AnimateArms();
	
	int ColorTable2;		// init needed data for drawing
	glImage *Images2;
	
	Spline Path;
	
	s32 BaseRotation;
	s32 ArmRotation;
	s32 RotationDelta;
	int ArmFrame;
	Segment Arms[TENTAKILL_MAX_SEGMENTS][4];
	
	
	void Load2WayRotate( TurretArray &Turrets,
						 Vector2df32 *Position,
						 Vector2df32 *Target,
						 std::vector<Turret>::iterator &TurretPtr,
						 s32 angle );

	void Add4WaySmart( Vector2df32 *Position,
					   Vector2df32 *Target,
					   std::vector<Turret>::iterator &TurretPtr );

	void LoadSelfTargetKetsuiSprayTrafalgar( TurretArray &Turrets,  
											 Vector2df32 *Position,
											 Vector2df32 *Target,
											 std::vector<Turret>::iterator &TurretPtr );

	void AddPsikyoSprayWave3(  Vector2df32 *Position,
							   Vector2df32 *Target,
							   std::vector<Turret>::iterator &TurretPtr,
							   int AngleDirection );

	void AddSprayWaveDelta3Waves( Vector2df32 *Position,
								  Vector2df32 *Target,
								  std::vector<Turret>::iterator &TurretPtr );

	void AddRings(  Vector2df32 *Position,
					Vector2df32 *Target,
					std::vector<Turret>::iterator &TurretPtr );

	void AddStraightLasers( Vector2df32 *Position,
							Vector2df32 *Target,
							std::vector<Turret>::iterator &TurretPtr,
							int SpawnDelay );

	void AddPsikyoSprayW5Decel(  Vector2df32 *Position,
								 Vector2df32 *Target,
								 std::vector<Turret>::iterator &TurretPtr );
	
};


#endif // TENTAKILLBOSS_H
