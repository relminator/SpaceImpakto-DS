/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	Spline class


*******************************************************************************
******************************************************************************/


/*
 *	Fixed Point catmull-rom spline interpolator
 *	relminator
 *	http://rel.betterwebber.com
 *
 */

#include "Spline.h"







/******************************************************************************


******************************************************************************/
/*
 *	params:
 *
 *		P0-Pn -> non fixed point int32 values
 *		T     -> 20.12 fixed point int32 value (0-4095)
 *
 *	return: 
 *		20.12 fixed point int32 (needs to be >> 12 when used)
 */

s32 Catmullf32(int p0, int p1, int p2, int p3, s32 t)
{
	
	s32 t2 = (t * t) >> 12;
	s32 t3 = (t2 * t) >> 12;
		
	s32 f1;
	
	f1 = 	(( 8192 * p1 ) + ( -p0 + p2 ) * t) + 
			( 2 * p0 - 5 * p1 + 4 * p2 - p3 ) * t2 +  		
			( -p0 + 3 * p1 - 3 * p2 + p3 ) * t3; 		
	
	
	
	return (2048 * (f1 >> 12));
	
}


/******************************************************************************


******************************************************************************/
Spline::Spline()
{

	T = 0;
	Speed = 0;
	Index = 0;
	MaxPoints = 0;
	TimeWrapped = false;
	
	x = 0;
	y = 0;
}

/******************************************************************************


******************************************************************************/
Spline::~Spline()
{
	
	Coords.clear();
	
}



/******************************************************************************


******************************************************************************/
void Spline::Init(int NumPoints)
{
	
	Coords.clear();
	Coords.reserve( NumPoints );
	T = 0;
	Speed = 0;
	Index = 0;
	MaxPoints = 0;
	
	x = 0;
	y = 0;
}

/******************************************************************************


******************************************************************************/
void Spline::Update()
{

	Coord p0,p1,p2,p3;
	
	int i1 = mod32( Index+1, MaxPoints );
	int i2 = mod32( Index+2, MaxPoints );
	int i3 = mod32( Index+3, MaxPoints );
	
	
	p0.x = Coords[Index].x;
	p0.y = Coords[Index].y;
	p1.x = Coords[i1].x;
	p1.y = Coords[i1].y;
	p2.x = Coords[i2].x;
	p2.y = Coords[i2].y;
	p3.x = Coords[i3].x;
	p3.y = Coords[i3].y;  


	x = Catmullf32( p0.x, p1.x, p2.x, p3.x, T );
	y = Catmullf32( p0.y, p1.y, p2.y, p3.y, T );
	
	TimeWrapped = false;
	
	T += Speed;

	if (T > 4096)
	{
		T -= 4096;
		Index = mod32( Index+1, MaxPoints );
		TimeWrapped = true;
	}
	
	
	
}


Spline::Coord Spline::GetNextPoint() const
{
	Coord p;
	s32 _t = T;
	int _i = Index;
	
	_t += Speed;

	if ( _t > 4096)
	{
		_t -= 4096;
		_i = mod32( Index+1, MaxPoints );	
	}
		
	Coord p0,p1,p2,p3;
	
	int i1 = mod32( _i+1, MaxPoints );
	int i2 = mod32( _i+2, MaxPoints );
	int i3 = mod32( _i+3, MaxPoints );
	
	
	p0.x = Coords[_i].x;
	p0.y = Coords[_i].y;
	p1.x = Coords[i1].x;
	p1.y = Coords[i1].y;
	p2.x = Coords[i2].x;
	p2.y = Coords[i2].y;
	p3.x = Coords[i3].x;
	p3.y = Coords[i3].y;  


	p.x = Catmullf32( p0.x, p1.x, p2.x, p3.x, _t );
	p.y = Catmullf32( p0.y, p1.y, p2.y, p3.y, _t );
	
	return p;
}