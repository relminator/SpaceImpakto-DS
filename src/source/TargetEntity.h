/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	TargetEntity class


*******************************************************************************
******************************************************************************/

#include "Vector2df32.h"

#ifndef TARGETENTITY_H
#define TARGETENTITY_H

class TargetEntity 
{

public:
	TargetEntity()
	{
		Active = false;
	}
	~TargetEntity()
	{
	}
	
	inline bool IsActive() const
	{
		return Active;
	}
	
	inline s32 GetX() const
	{
		return Position.x;
	}
	
	inline s32 GetY() const
	{
		return Position.y;
	}

	inline void Init( s32 x, s32 y )
	{
		Position.x = x;
		Position.y = y;
		Active = true;
	}

	inline void Set( s32 x, s32 y )
	{
		Position.x = x;
		Position.y = y;
	}
	
	inline void Kill()
	{
		Active = false;
		Position.x = 300 << 12;
		Position.y = 96 << 12;
	}
	
	
private:

	Vector2df32 Position;
	bool Active;
};

#endif // TARGETENTITY_H
