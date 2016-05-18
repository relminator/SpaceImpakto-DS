/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.betterwebber.com


	GLFont class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl2d.h>


#ifndef GLFont__H
#define GLFont__H

class GLFont
{
	public:
	
	GLFont();
	~GLFont();
	
	int Load( glImage              *_font_sprite,
              const unsigned int   numframes, 
			  const unsigned int   *texcoords,
			  GL_TEXTURE_TYPE_ENUM type,
			  int 	               sizeX,
		      int 	               sizeY,
		      int 	               param,
		      const uint8          *texture,
			  int 				   pallette_width,
			  const u16 		   *palette
            );
			
	int Load( glImage              *_font_sprite,
              int   			   tilewidth,
			  int   			   tileheight,
			  int   			   bmpwidth,
			  int   			   bmpheight, 
			  GL_TEXTURE_TYPE_ENUM type,
			  int 	               sizeX,
		      int 	               sizeY,
		      int 	               param,
		      const uint8          *texture,
			  int 				   pallette_width,
			  const u16 		   *palette
            );
	
	void Print(int x, int y, const char *text);
	
	void Print(int x, int y, int value);

	void Print(int x, int y, const char *text, int value);
	
	void PrintCentered(int x, int y, const char *text);
	
	void PrintCentered(int x, int y, int value);
	
	void PrintAscii(int x, int y, int ascii_value, int length);
	
	void PrintScore(int x, int y, int value, const char *filler, int length);
	
	void PrintSpaced(int x, int y, const char *text, int x_space, s32 x_offset, s32 y_offset);
	
	void PrintSine(int x, int y, const char *text, int width_offset, int height, int cycles, s32 start_angle);
	
	void PrintVertical(int x, int y, const char *text);
	
	void PrintSpacedVertical(int x, int y, const char *text, int x_space, s32 x_offset, s32 y_offset);
	
	void PrintSineVertical(int x, int y, const char *text, int height_offset, int _width, int cycles, s32 start_angle);
	
	private:
	glImage 	   *font_sprite;
	char			str[256];
	char			str2[256];
};




#endif

