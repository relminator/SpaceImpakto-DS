/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Viper class


*******************************************************************************
******************************************************************************/


#ifndef AABB_H
#define AABB_H

class AABB 
{

public:
	AABB()
	{
	}
	~AABB()
	{
	}

	inline int Intersects( const AABB &other ) const
	{
	
		if (x2 < other.x1) { return false; }
		if (x1 > other.x2) { return false; }
		if (y2 < other.y1) { return false; }
		if (y1 > other.y2) { return false; }
		return true;

}
	
	inline void Init( int x, int y, int width, int height )
	{
		
		x1 = x;
		y1 = y;
		x2 = ( x + width ) - 1;
		y2 = ( y + height ) - 1;
	
	}
	
	inline AABB Get() const
	{
		return *this;
	}
	
	inline void Resize( int x, int y )
	{
		
		x1 += x;
		y1 += y; 
		x2 -= x;
		y2 -= y;
		
	}
	
	inline void Resize( s32 factor )
	{

		int _w = ( (x2-x1) + 1 );
		int _h = ( (y2-y1) + 1 );
	
		int wd = mulf32( _w << 12, factor >> 1 ) >> 12;
		int hd = mulf32( _h << 12, factor >> 1 ) >> 12;
		
		x1 += wd;
		y1 += hd; 
		x2 -= wd;
		y2 -= hd;
		
	}
	
	int x1;
	int y1;
	int x2;
	int y2;
};

#endif // AABB_H
