/******************************************************************************
*******************************************************************************

	Vector2df32.h
	relminator
	http://rel.betterwebber.com


	Vector2df32


*******************************************************************************
******************************************************************************/


#include <nds.h>
#include <math.h>

#ifndef Vector2df32__H
#define Vector2df32__H


class Vector2df32
{
	public:
	s32		x;
	s32		y;
};

/******************************************************************************

	returns the dot product of 2 vectors

******************************************************************************/
inline s32 Vector2df32Dot(Vector2df32 &a, Vector2df32 &b)
{
	return ( mulf32(a.x , b.x) + mulf32(a.y , b.y) );
}

/******************************************************************************

	normalizes a vector

******************************************************************************/
inline void Vector2df32Normalize(Vector2df32 &a)
{
	s32 magnitude = sqrtf32( mulf32(a.x , a.x) + mulf32(a.y , a.y) );

	a.x = divf32(a.x, magnitude);
	a.y = divf32(a.y, magnitude);
}

/******************************************************************************

	returns the length of a vector

******************************************************************************/
inline s32 Vector2df32Magnitude(Vector2df32 &a)
{
	return sqrtf32( mulf32(a.x , a.x) + mulf32(a.y , a.y) );
}



/******************************************************************************

	OPERATORS

******************************************************************************/



/******************************************************************************

	USEFUL STUFF

******************************************************************************/

inline Vector2df32 GetNormalf32(s32 x1, s32 y1, s32 x2, s32 y2, bool type)
{
	
	Vector2df32 normal;
	
	if (type)
	{
		normal.x = -(y2-y1);
		normal.y =  (x2-x1);
	}
	else
	{
		normal.x =  (y2-y1);
		normal.y = -(x2-x1);
	}
	
	return normal;
	
}

#endif
