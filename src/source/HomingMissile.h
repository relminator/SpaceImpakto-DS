/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	HomingMissile class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <vector>
#include "Aabb.h"

#ifndef HOMINGMISSILE_H
#define HOMINGMISSILE_H

#define HOMING_SPEED ( 2 << 12 )

class HomingMissile 
{

public:

	
	HomingMissile()
	{
		Active = false;
		Frame = 0;
		NumFrames = 1;
		BaseFrame = 0;
		Width = 2;
		Height = 2;
		Target.x = 256 << 12;
		Target.y = 96 << 12;
		Speed = HOMING_SPEED;
	}
	
	~HomingMissile()
	{
	}
	
	
	inline void Init( s32 sx, s32 sy, int frame, int irotation )
	{
		Active = true;
		NewTarget = true;
		InitialRotation = irotation;
		Counter = 0;
		Angle = BRAD_PI;
		Delta = 512;
		x = sx;
		y = sy;
		
		Speed = HOMING_SPEED;
		Dx = mulf32(cosLerp(Angle), Speed);
		Dy = mulf32(sinLerp(Angle), Speed);
	
		Frame = frame;
		NumFrames = 1;
		BaseFrame = 9;
		Width = 24;
		Height = 20;
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
	}

	inline void InitVic( s32 sx, s32 sy, int frame, int irotation )
	{
		Active = true;
		NewTarget = true;
		InitialRotation = irotation;
		Counter = 0;
		Angle = BRAD_PI;
		Delta = 512;
		x = sx;
		y = sy;
		
		Speed = HOMING_SPEED;
		Dx = mulf32(cosLerp(Angle), Speed);
		Dy = mulf32(sinLerp(Angle), Speed);
	
		Frame = frame;
		NumFrames = 1;
		BaseFrame = 0;
		Width = 20;
		Height = 20;
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
	}
	
	
	inline void Update()
	{
		
		Dx = mulf32(cosLerp(Angle), Speed);
		Dy = mulf32(sinLerp(Angle), Speed);
	
		x += Dx;
		y += Dy;
		
		// distance
		if ( (abs(x - Target.x) + abs(y - Target.y)) < (16 << 12) )
		{
				NewTarget = true;
				Delta = 512;  // reset initial rotation speed
		}
		
		// Project normal
		s32 dot = (mulf32(Dy , (Target.x - x)) - mulf32(Dx, (Target.y - y)));
		
		// target when counter is > 32
		if(Counter > 32)
		{
			if( dot > 0)
				Angle = (Angle - Delta);
			else
				Angle = (Angle + Delta);
				
			Delta += 4;  // inc delta so that we home aggressively
			Speed += 128; // inc speed to make effect cooler
		
		}
		else  // rotate from back to front
		{
			Angle += Delta * InitialRotation;
		}
		
		if( x  > ((SCREEN_WIDTH + 16) << 12) )
		{
			Kill();
		}
		
		// Animate
		if( (Counter & 3) == 0 )
		{
			Frame++;
			if( Frame > 2) Frame = 0;
		}
		
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
		
		Counter++;
	}
	
	
	
	inline void UpdateNonHoming()
	{
		
		// From center if counter is < 12
		if(Counter < 16)
		{
			Dx = -2048;
			Dy = InitialRotation * (1 << 12);
		}
		else
		{
			Dx = HOMING_SPEED + ( Counter * 512 );
			Dy = 0;
		}
		
		
		x += Dx;
		y += Dy;
			
		
		if( x  > ((SCREEN_WIDTH + 16) << 12) )
		{
			Kill();
		}
		
		// Animate
		if( (Counter & 3) == 0 )
		{
			Frame++;
			if( Frame > 2) Frame = 0;
		}
		
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
		
		Counter++;
	}
	
	
	inline void UpdateVicMissile()
	{
		
		// From center if counter is < 12
		if(Counter < 16)
		{
			Dx = -2048;
			Dy = InitialRotation * (1 << 12);
		}
		else if(Counter == 16)
		{
			Dy = -Dy;
		}
		else
		{
			Dx = HOMING_SPEED + ( Counter * 512 );
			Dy -= (Dy/32);
		}
		
		
		x += Dx;
		y += Dy;
			
		
		if( x  > ((SCREEN_WIDTH + 16) << 12) )
		{
			Kill();
		}
		
		// Animate
		if( (Counter & 3) == 0 )
		{
			Frame++;
			if( Frame > 2) Frame = 0;
		}
		
		// Reset AABB
		Aabb.Init( (x >> 12) - (Width >> 1), (y >> 12) - (Height >> 1), 
				   Width, Height 
				 );
		
		
		Counter++;
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
	
	inline s32 GetAngle() const
	{
		return Angle;
	}
	
	inline int GetEnergy() const
	{
		return Energy;
	}
	inline void SetTarget( s32 _x, s32 _y )
	{
		Target.x = _x;
		Target.y = _y;
	}
	
	inline void SetEnergy( int damage )
	{
		Energy = damage;
	}
	
private:

	bool 	Active;
	int 	Counter;
	
	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;
	s32		Speed;
	s32		Angle;		
	s32		Delta;
	
	bool	NewTarget;
	
	int 	Energy;
	
	int 	Frame;
	int 	NumFrames;
	int 	BaseFrame;
	
	int 	Width;
	int		Height;
	AABB	Aabb;
	
	Vector2df32 Target;
	int		InitialRotation;
	
};

#endif // HOMINGMISSILE_H
