/******************************************************************************
*******************************************************************************

	Bullet
	relminator
	http://rel.betterwebber.com


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <gl2d.h>
#include <vector>

#include "UTIL.h"
#include "Aabb.h"
#include "Vector2df32.h"
#include "cearn_atan.h"

#include "PlayerShip.h"
#include "EntityContainer.h"

#ifndef BULLET_H
#define BULLET_H


#define  B_MIN_X (-64 << 12)
#define  B_MIN_Y (-64 << 12)
#define  B_MAX_X ((256+64) << 12)
#define  B_MAX_Y ((192+64) << 12)


// Forward declare the class so that we coul inline Init()
class Bullet;

void StopAndGo( Bullet *B );
void TypeNormal( Bullet *B );
void TypeAccel( Bullet *B );
void TypeDecel( Bullet *B );

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
class Bullet
{

public:

	friend class BulletArray;
	friend void StopAndGo( Bullet *B );
	friend void TypeNormal( Bullet *B );
	friend void TypeAccel( Bullet *B );
	friend void TypeDecel( Bullet *B );
	
	enum TYPE
	{
		NORMAL = 0,
		ACCEL,
		DECEL
	};


	enum ANIMODE
	{
		DEFAULT = 0,
		ROTATED,
		ORIENTED
	};
	
	
	struct Params
	{
		s32 x; 				// pos
		s32 y;				
		s32 Dx; 			// direction
		s32 Dy;				
		s32 Gx; 			// gravity
		s32 Gy;				
		s32 AccelX; 		// acceleration factor
		s32 AccelY;			
		s32 Speed; 			// speed (used for smart ones)
		s32 Angle; 			// angle
		s32 Rotation; 		// rotation angle for rotated bullets
		s32 RotationDelta;	// rotation angle speed for rotated bullets
		int StopDelay; 		// stop delay for stop 'n go bullets
		int Frame; 			// frame
		int NumFrames; 		// numframes
		int BaseFrame;	  	// start frame
		
		bool Smart;			// if smart then shoot to player direction
		
		int	Width;
		int  Height;
		
		TYPE Type; 
		ANIMODE AniMode;
		
	};
	

	Bullet();
	~Bullet();
	
	
	inline void Spawn( Params const &P, Vector2df32 *target );
	inline void Update();
	inline void Kill();
	inline void SetTarget( Vector2df32 *target );
	inline AABB GetAABB() const;
	
private:

	void (*DoType)( Bullet *B );	
	
	s32 	x;
	s32		y;
	s32		Dx;
	s32		Dy;
	s32		Gx;
	s32		Gy;
	s32		DxO;
	s32		DyO;
	s32		AccelX;
	s32		AccelY;
	s32		Speed;
	s32		Angle;
	s32		Rotation;
	s32		RotationDelta;
	int 	Counter;
	int		StopDelay;
	int		Frame;
	int		NumFrames;
	int 	BaseFrame;
	int 	DrawFrame;
	bool	Active;
	bool 	Smart;
	
	int		Width;
	int  	Height;
		
	TYPE 	Type;
	ANIMODE AniMode;
	
	Vector2df32 *Target;
	
	AABB	Aabb;
	
};




//--------------------------------------
//	
//-------------------------------------- 
inline void Bullet::Spawn( Params const &P, Vector2df32 *target )
{
	
	Target = target;
	
	Active = true;
	Counter = 0;
	
	x = P.x;
	y = P.y;
	Dx = P.Dx;
	Dy = P.Dy;
	Gx = P.Gx;
	Gy = P.Gy;
	AccelX = P.AccelX;
	AccelY = P.AccelY;
	Speed = P.Speed;
	Angle = P.Angle;
	Rotation = P.Rotation;
	RotationDelta = P.RotationDelta;
	StopDelay = P.StopDelay;
	Frame = P.Frame;
	NumFrames = P.NumFrames;
	BaseFrame = P.BaseFrame;
	Smart = P.Smart;

	Width = P.Width;
	Height = P.Height;
	
	Type = P.Type;
	AniMode = P.AniMode;
	
	
	switch( Type )
	{
		case NORMAL:
			DoType = TypeNormal;
			break;
		case ACCEL:
			DoType = TypeAccel;
			break;
		case DECEL:
			DoType = TypeDecel;
			break;
		default:
			DoType = TypeNormal;
	}

}
	

//--------------------------------------
//	
//--------------------------------------
inline void Bullet::Update()
{
	
	Counter++;
	DoType( this );
	
	if ( (x < B_MIN_X) || (x > B_MAX_X) || (y < B_MIN_Y) || (y > B_MAX_Y)) 
	{
		Active = false;
		return;
	}
	
	
	// Reset AABB
	
	// Use this if final engine is fast enough
	//Aabb.Init( (x >> 12) - Width/2 - 1, (y >> 12) - Height/2 - 1, Width, Height );
	//Aabb.Resize( 1024*3 );

	Aabb.Init( (x >> 12) - (Width >> 3) - 1, (y >> 12) - (Height >> 3) - 1, 
			   Width >> 2, Height >> 2 
			 );

	// Animate
	if( (Counter & 3) == 0 )
	{
		Frame++;
		if(Frame >= NumFrames ) Frame = 0;
	}
	DrawFrame = BaseFrame + Frame;
	
	if (AniMode != DEFAULT )
	{
		if(AniMode == ROTATED ) 
			Rotation += RotationDelta;
		else  
			Rotation = Angle;
	}
	
}


//--------------------------------------
//	
//--------------------------------------
inline void Bullet::Kill()
{
	Active = false;
}

//--------------------------------------
//	
//--------------------------------------
inline void Bullet::SetTarget( Vector2df32 *target )
{
	Target = target;
}


inline AABB Bullet::GetAABB() const
{
	return Aabb;
}
	

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
class BulletArray
{

public:

	BulletArray();
	~BulletArray();
	
	void Init( int Palette, glImage* const Sprites, int NumBullets );
	void UpdateEntities();
	void DrawEntities();
	void KillEntities();
	EntityContainer::CollisionPacket Collide(  PlayerShip &Vic  );
	void DrawAABB( int color );
	void SpawnBullets( Bullet::Params &Bp,
					   s32 Px,
					   s32 Py,
					   int BulletsPerShot,
					   s32 SpreadAngle,
					   s32 BulletSpeedDelta,
					   Vector2df32 *target );
	
	inline void Spawn( Bullet::Params const &P, Vector2df32 *target );
	inline int NumEntities();
	
private:

	int numEntities;
	
	glImage *Images;
	int ColorTable;
	
	std::vector<Bullet> Bullets;
	
};



//--------------------------------------
//	
//--------------------------------------
inline void	BulletArray::Spawn( Bullet::Params const &P, Vector2df32 *target )
{
	std::vector<Bullet>::iterator iter;
	for( iter = Bullets.begin(); iter != Bullets.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{
			iter->Spawn( P, target );
			break;
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
inline int BulletArray::NumEntities() 
{
	return numEntities;
}

#endif // BULLET_H
