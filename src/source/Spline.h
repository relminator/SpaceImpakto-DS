/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	Spline class


*******************************************************************************
******************************************************************************/

 
#include <nds.h>
#include <vector>

#include "UTIL.h"


#ifndef Spline__H
#define Spline__H


class Spline
{
	
	
public:
	
	struct Coord
	{
		int		x;
		int		y;
	};
	
	Spline();
	~Spline();

	//methods
	void Init(int NumPoints);
	void Update();
	
	inline int GetX() const
	{
		return x;
	}
	
	inline int GetY() const
	{
		return y;
	}
	
	inline int GetIndex() const
	{
		return Index;
	}
	
	inline int GetTime() const
	{
		return T;
	}
	
	inline bool WrappedTimer() const
	{
		return TimeWrapped;
	}
	
	inline int GetMaxPoints() const
	{
		return MaxPoints;
	}
	
	inline void SetPoint( int _index, int _x, int _y )
	{
		if( _index < 0 ) return;
		if( (unsigned int)_index >= (unsigned int)MaxPoints ) return;
		Coords[_index].x = _x;
		Coords[_index].y = _y;
	}
	
	inline void SetSpeed( s32 speed )
	{
		Speed = speed;
	}
	
	inline void SetT( s32 t )
	{
		T = t;
	}
	
	inline void AddPoint( const Coord &p )
	{
		Coords.push_back( p );
		MaxPoints++;
	}

	inline void AddPoint( int _x, int _y )
	{
		Coord p;
		p.x = _x;
		p.y = _y;
		Coords.push_back( p );
		MaxPoints++;
	}
	
	
	Coord GetNextPoint() const;
	
	
private:

	int		x;
	int		y;
	
	s32		T;
	s32		Speed;
	int		Index;
	int		MaxPoints;
	bool 	TimeWrapped;
	
	std::vector<Coord> Coords;
	
};




#endif
