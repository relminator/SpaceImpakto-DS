/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.betterwebber.com


	Sprite class


*******************************************************************************
******************************************************************************/

 
#include <nds.h>
#include <assert.h>

#include "Sprite.h"


int Sprite::NumSprites = 0;

/******************************************************************************


******************************************************************************/
Sprite::Sprite()
{

}

/******************************************************************************


******************************************************************************/
Sprite::~Sprite()
{

	Active = false;  
 
	//api: free the graphics
	if(Gfx)
	{	
		oamFreeGfx(Oam, Gfx);
	}

	Gfx = 0;
	
}


/******************************************************************************
	Allocates vram mem for gfx
	
	1. if _reset_id_counter = true s_oamID is reset to zero
		as I'm planning to reload all the sprite gfx every level
		including the ships, bullets, etc.
	2. _increase_id is a parameter I added to control animations
		ie. frames of the same object(sprite) share the same oamID so I'll
		only inc the oamID when I create sprites for another object.
		eg. viper[frame0-frame5] has oam_id =0
		enemy[frame0-frame8] has oam_id = 1

******************************************************************************/
void Sprite::Create(OamState *_oam, int _width, int _height, SpriteSize _size, SpriteColorFormat _format, int ID ) 
{
	
	
	x = 0;
	y = 0;
	
	Oam = _oam;
	OamID = ID;
	
	Active = true;
	Width = _width;
	Height = _height;
	Size = _size;
	Format = _format;
    
	//api: allocate a chunk of sprite graphics memory
	Gfx = oamAllocateGfx(Oam, Size, Format);
	
	assert(Gfx);
	
	
	NumSprites++;
}

/******************************************************************************


******************************************************************************/
void Sprite::Destroy()
{

	Active = false;  
 
	//api: free the graphics
	if(Gfx)
	{	
		oamFreeGfx(Oam, Gfx);
	}

	Gfx = 0;
	
	NumSprites--;

}


/******************************************************************************

	map our sprite to oam entries

******************************************************************************/
void Sprite::UpdateOam()
{

	//oamSet (OamState *oam, int id, int x, int y, int priority,
	//		  int palette_alpha, SpriteSize size, SpriteColorFormat format,
	//		  const void *gfxOffset, int affineIndex, bool sizeDouble,
	//		  bool hide, bool hflip, bool vflip, bool mosaic)
	oamSet(Oam, 
		OamID,				// id 
		x, y,    			// x,y
		0, 					// priority
		0,					// palette alpha
		Size,				// size
		Format, 			// color format
		Gfx, 				// gfx offset
		-1, 				// affine index
		false,				// double if rotating? 
		!Active,			// hidden if true
		false,				// hflip
		false, 				// vflip
		false);				// apply mosaic?

}

