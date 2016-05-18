/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	SuperShape class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <math.h>
#include "VertexBuffer.h"

#ifndef SUPERSHAPE_H
#define SUPERSHAPE_H

class SuperShape
{

	friend class Fx3D;
public:
	SuperShape();
	~SuperShape();
	
	void Update();
	void Init( s32  _x, s32  _y, s32  _z,
			   s32 _xa, s32 _ya, s32 _za,
			   s32 _xs, s32 _ys, s32 _zs
			 );
	
private:
	s32	x;		// position
	s32	y;
	s32 z;
	
	s32	xa;		// angle
	s32	ya;
	s32 za;
	
	s32	xs;		// rotation speed
	s32	ys;
	s32 zs;
	
	s32 Speed;
	
	int WaitValue;
	int Counter;
	bool Active;
	
	VertexBuffer* ps_Vb;
	
};

#endif // SUPERSHAPE_H
