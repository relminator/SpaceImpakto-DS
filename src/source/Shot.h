/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Shot class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include "Aabb.h"

#ifndef SHOT_H
#define SHOT_H

#define VIPER_SHOT_SPEED ( 7 << 12 )

class Shot 
{

public:

	enum ID
	{
		NORMAL = 0,
		WAVE
	};

	Shot()
	{
		Active = false;
		ShotID = NORMAL; 
		Frame = 0;
		NumFrames = 1;
		BaseFrame = 0;
		Width = 2;
		Height = 2;
	}
	
	~Shot()
	{
	}
	
	
	inline void InitNormal( s32 sx, s32 sy, int frame, int level )
	{
		Active = true;
		ShotID = NORMAL;
		
		x = sx;
		y = sy;
		Dx = VIPER_SHOT_SPEED;
		Dy = 0;
		
		Frame = frame;
		NumFrames = 1;
		BaseFrame = 12 + (level-1) * 3;
		Width = 20;
		Height = 12;
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
	}

	inline void InitNormal( s32 sx, s32 sy, int frame, int baseframe, s32 angle, s32 speed, int level )
	{
		Active = true;
		ShotID = NORMAL;
		
		x = sx;
		y = sy;
		Dx = mulf32( cosLerp(angle), speed );
		Dy = mulf32( sinLerp(angle), speed );
		
		Frame = frame;
		NumFrames = 1;
		BaseFrame = baseframe + (level-1) * 3;
		Width = 20;
		Height = 12;
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
	}
	
	inline void InitWave( s32 sx, s32 sy, s32 angle, int frame, int level )
	{
		Active = true;
		ShotID = WAVE;
		
		x = sx;
		y = sy;
		Dx = VIPER_SHOT_SPEED - (2 << 12);
		Dy = sy;
		Angle = angle;
		Radius = 0;
		
		Frame = frame;
		NumFrames = 1;
		BaseFrame = 18 + (level-2) * 3;
		
		Width = 20;
		Height = 12;
		
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
	}
	
	
	inline void InitVicShot( s32 sx, s32 sy, int frame, int level )
	{
		Active = true;
		ShotID = NORMAL;
		
		x = sx;
		y = sy;
		Dx = VIPER_SHOT_SPEED;
		Dy = 0;
		
		Frame = frame;
		NumFrames = 1;
		BaseFrame = 18 + (level-1);
		Width = 20;
		Height = 12;
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
	}
	
	inline void Update()
	{
		
		if( ShotID == NORMAL )
		{
			x += Dx;
			y += Dy;
		}
		else //wave
		{
			Angle += 800;
			Radius += 2000;
			s32 sy = ( sinLerp(Angle) ) * ( Radius );
			
			x += Dx;
			y = Dy + (sy >> 12);         
		}
		
		
		if( x  > ((SCREEN_WIDTH + 16) << 12) )
		{
			Kill();
		}
		
		
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
			
	}
	
	inline bool IsActive() const
	{
		return Active;
	}
	
	inline void Kill()
	{
		Active = false;
		x = 300 << 12;
	}
	
	inline s32 GetX() const
	{
		return x;
	}
	
	inline s32 GetY() const
	{
		return y;
	}
	
	inline s32 GetWidth() const
	{
		return Width;
	}
	
	inline s32 GetHeight() const
	{
		return Height;
	}
	
	
	inline int GetDrawFrame() const
	{
		return BaseFrame + Frame;
	}
	
	inline AABB GetAABB() const
	{
		return Aabb;
	}
	
	
private:

	bool 	Active;
	ID 		ShotID;
	
	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;
	s32		Angle;		// 20.12
	s32		Radius;		// 20.12
	
	int 	Energy;
	
	int 	Frame;
	int 	NumFrames;
	int 	BaseFrame;
	
	int 	Width;
	int		Height;
	AABB	Aabb;
	
};

#endif // SHOT_H
