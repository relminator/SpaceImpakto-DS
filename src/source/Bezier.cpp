/******************************************************************************
*******************************************************************************
	
	Time Constant Bezier Spline
	Relminator (Richard Eric M. Lope BSN RN)
	http://rel.phatcode.net
	
	
*******************************************************************************
******************************************************************************/ 

#include "Bezier.h"


s32 Bezierf32( s32 p0, s32 p1, s32 p2, s32 p3, s32 t )
{
    
    s32 bez;
    
    // A = p1
    // B = p0
    // C = p3
    // D = p2
               
    s32 b = (1 << 12) - t;
    s32 b2 = (b * b) >> 12;
    s32 b3 = (b2 * b) >> 12;
    
    s32 t2 = (t * t) >> 12;
    s32 t3 = (t2 * t) >> 12;
    
    s32 b2t = ( b2 * t ) >> 12;
    s32 bt2 = ( b * t2 ) >> 12;
            
    bez = p1 * b3 + 
    	  ( (3 * p0 * b2t) ) + 
    	  ( (3 * p3 * bt2) )  + 
    	  p2 * t3;
    
    return bez;

  
}    

s32 BezierfDifferential( s32 p0, s32 p1, s32 p2, s32 p3, s32 t )
{
	
	s32 _t2 = (t * t) >> 12;
    s32 _2t = (2 * t);
    s32 _3t2 = (3 * _t2);
    
	s32 a = p0 * ( (1<<12) - (4*t) + _3t2 );
	s32 b = p1 * ( _2t - (1<<12) - _t2 );
	s32 c = p2 * _t2;
	s32 d = p3 * ( _2t -  _3t2 );
	
	return 3 * ( (a + b + c + d) >> 12 );
	
}



Bezier::Bezier()
{
	Points = 0;
	Lut = 0;
}

Bezier::~Bezier()
{
}


void Bezier::Init( Coord* _points, Table* _lut, int LutSize, s32 rate, s32 totaldist )
{
	
	d = 0;
	Index = 0;
	
	TableSize = LutSize;
	Rate = rate;
	TotalDistance = totaldist;
	Points = _points;
	Lut = _lut;
	
	
}


bool Bezier::Update()
{
	t = GetT();
	
	sassert(Points, "Control Points not loaded!");
	
	x = Bezierf32( Points[0].x, Points[1].x, 
				   Points[2].x, Points[3].x, t );
	y = Bezierf32( Points[0].y, Points[1].y, 
				   Points[2].y, Points[3].y, t );
	
	d += Rate;

	if ( d > TotalDistance )
	{
		d = TotalDistance;
		return true;
	}
	
	return false;
	
}


bool Bezier::UpdateMirrored()
{
	t = GetT();
	
	sassert(Points, "Control Points not loaded!");
	
	x = Bezierf32( Points[3].x, Points[2].x, 
				   Points[1].x, Points[0].x, t );
	y = Bezierf32( Points[3].y, Points[2].y, 
				   Points[1].y, Points[0].y, t );
	
	d += Rate;

	if ( d > TotalDistance )
	{
		d = TotalDistance;
		return true;
	}
	
	return false;
	
}

bool Bezier::Update( int xoffset, int yoffset )
{
	t = GetT();
	
	sassert(Points, "Control Points not loaded!");
	
	x = Bezierf32( Points[0].x + xoffset, Points[1].x + xoffset, 
				   Points[2].x + xoffset, Points[3].x + xoffset, t );
	y = Bezierf32( Points[0].y + yoffset, Points[1].y + yoffset, 
				   Points[2].y + yoffset, Points[3].y + yoffset, t );
	
	d += Rate;

	if ( d > TotalDistance )
	{
		d = TotalDistance;
		return true;
	}
	
	return false;
	
}


bool Bezier::UpdateMirrored( int xoffset, int yoffset )
{
	t = GetT();
	
	sassert(Points, "Control Points not loaded!");
	
	x = Bezierf32( Points[3].x + xoffset, Points[2].x + xoffset, 
				   Points[1].x + xoffset, Points[0].x + xoffset, t );
	y = Bezierf32( Points[3].y + yoffset, Points[2].y + yoffset, 
				   Points[1].y + yoffset, Points[0].y + yoffset, t );
	
	d += Rate;

	if ( d > TotalDistance )
	{
		d = TotalDistance;
		return true;
	}
	
	return false;
	
}
