/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.betterwebber.com


	Sprite class


*******************************************************************************
******************************************************************************/

#include <nds.h>

#ifndef SPRITE__H
#define SPRITE__H

class Sprite 
{
	public:
	
	
	Sprite();
	~Sprite();
	
	void Create(OamState *_oam, int _width, int _height, SpriteSize _size, SpriteColorFormat _format, int ID );
	void Destroy();
	void UpdateOam();

	
	static int 			NumSprites;

	OamState 			*Oam;
	
	int					OamID;
	
	int 				x;
	int 				y;


	int					Width;
	int					Height;
	
	SpriteColorFormat 	Format;
	SpriteSize 			Size;
	
	bool 				Active;
	
	u16* 				Gfx;
	
	
};


#endif // SPRITE__H


