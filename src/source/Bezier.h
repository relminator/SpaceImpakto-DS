/******************************************************************************
*******************************************************************************

	Time Constant Bezier Spline
	Relminator (Richard Eric M. Lope BSN RN)
	http://rel.phatcode.net
	
	
*******************************************************************************
******************************************************************************/

#include <nds.h>

#ifndef BEZIER_H
#define BEZIER_H

s32 Bezierf32( s32 p0, s32 p1, s32 p2, s32 p3, s32 t );
s32 BezierfDifferential( s32 p0, s32 p1, s32 p2, s32 p3, s32 t );

class Bezier 
{

public:
	struct Coord
	{
		int x;
		int y;
	};
	
	struct Table
	{
		s32 d;
		s32 t;
	};
	
	Bezier();
	~Bezier();

	void Init( Coord* _points, Table* _lut, int LutSize, s32 rate, s32 totaldist );
	
	bool Update();
	bool UpdateMirrored();
	bool Update( int xoffset, int yoffset );
	bool UpdateMirrored( int xoffset, int yoffset );
	
	inline s32 GetX() const
	{
		return x;
	}
	
	inline s32 GetY() const
	{
		return y;
	}
	
	inline s32 GetRate() const
	{
		return Rate;
	}
	
	inline s32 GetTime() const
	{
		return t;
	}
	
	inline s32 GetLinearDistance() const
	{
		return d;
	}
	
	
	inline s32 GetDx()
	{
		return BezierfDifferential( Points[0].x, Points[1].x, 
								 Points[2].x, Points[3].x, t );
	
	}
	
	inline s32 GetDy()
	{
		return BezierfDifferential( Points[0].y, Points[1].y, 
								 Points[2].y, Points[3].y, t );
	
	}
	
	inline void SetRate( s32 rate )
	{
		Rate = rate;
	}
	
private:

	inline s32 GetT();
	
	s32	x;
	s32 y;
	
	s32 d;
	
	s32 t;
	
	int Index;
	int TableSize;
	s32 Rate;
	s32 TotalDistance;
	
	Coord* Points;    // Point to generated contol points
	Table* Lut;		  // Same here

};



inline s32 Bezier::GetT()
{
	
	sassert(Lut, "Table not loaded!");
	
	while( d > Lut[Index+1].d )
	{
		Index += 1;
	}
	
	s32 d1 = Lut[Index].d; 
	s32 d2 = Lut[Index+1].d; 
	  
	s32 t1 = Lut[Index].t; 
	s32 t2 = Lut[Index+1].t; 
	
	// s32 Lf32 = ( ((d - d1) << 12 ) / (d2 - d1));
	
	s32 Lf32 = divf32( (d - d1), (d2 - d1) );
	return ( t1 + ( ( Lf32 * (t2 - t1) ) >> 12 ) );
	
}
	

#endif // BEZIER_H
