/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.betterwebber.com


	GLFont class


*******************************************************************************
******************************************************************************/

#include "GLFont.h"


/******************************************************************************



******************************************************************************/
GLFont::GLFont()
{

}

/******************************************************************************



******************************************************************************/
GLFont::~GLFont()
{

}

/******************************************************************************

	Loads our font

******************************************************************************/
int GLFont::Load( glImage              *_font_sprite,
				   const unsigned int   numframes, 
				   const unsigned int   *texcoords,
				   GL_TEXTURE_TYPE_ENUM type,
				   int 	              	sizeX,
				   int 	              	sizeY,
				   int 	              	param,
				   const uint8          *texture,
				   int 				   pallette_width,
				   const u16 		   *palette
				 )

{
	font_sprite = _font_sprite;
	
	int textureID = 
		glLoadSpriteSet( font_sprite,
						 numframes, 
						 texcoords,
						 type,
						 sizeX,
						 sizeY,
						 param,
						 pallette_width,
						 palette,
						 texture	 
					   );
					   
	return textureID;

}


/******************************************************************************

	Loads our font

******************************************************************************/
int GLFont::Load( glImage             *_font_sprite,
				   int   			   tilewidth,
				   int   			   tileheight,
				   int   			   bmpwidth,
				   int   			   bmpheight, 
				   GL_TEXTURE_TYPE_ENUM type,
				   int 	               sizeX,
				   int 	               sizeY,
				   int 	               param,
				   const uint8         *texture,
				   int 				   pallette_width,
				   const u16 		   *palette
				 )
{
	font_sprite = _font_sprite;
	
	int textureID = 
		glLoadTileSet( font_sprite,
					   tilewidth, 
					   tileheight,
					   bmpwidth,
					   bmpheight,
					   type,
					   sizeX,
					   sizeY,
					   param,
					   pallette_width,
					   palette,
					   texture	 
					 );
					   
	return textureID;

}


/******************************************************************************



******************************************************************************/
void GLFont::Print(int x, int y, const char *text)
{

	unsigned char font_char;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		glSprite( x, y, GL_FLIP_NONE, &font_sprite[font_char] );
		x += font_sprite[font_char].width; 
	}
	
}

/******************************************************************************



******************************************************************************/
void GLFont::Print(int x, int y, int value)
{

	
	sprintf(str,"%i",value);
	
	Print(x, y, str);
	
}

/******************************************************************************



******************************************************************************/
void GLFont::Print(int x, int y, const char *text, int value)
{

	strcpy(str, text);
	sprintf(str2,"%i",value);
	strcat(str, str2);
	
	Print(x, y, str);
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintCentered(int x, int y, const char *text)
{

	unsigned char font_char;
	int total_width = 0;
	char *o_text = (char*)text;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		total_width += font_sprite[font_char].width; 
	}
	
	x = (SCREEN_WIDTH - total_width) / 2; 
	
	text = o_text;
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		glSprite( x, y, GL_FLIP_NONE, &font_sprite[font_char] );
		x += font_sprite[font_char].width; 
	}
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintCentered(int x, int y, int value)
{

	
	sprintf(str,"%i",value);
	
	PrintCentered(x, y, str);
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintAscii(int x, int y, int ascii_value, int length)
{

	for (int i=0; i < length; i++)
	{
		str[i]= ascii_value;
	}
	
	str[length]='\0';			
	
	Print(x, y, str);
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintScore(int x, int y, int value, const char *filler, int length)
{

	sprintf(str2,"%i",value);
	int value_len = strlen(str2);
	
	if (length < value_len)
		length = value_len;
		
	int new_length = length - value_len;
	
	for (int i=0; i < new_length; i++)
	{
		str[i]=*filler;
	}
	
	//str[new_length]='\0';			// not needed
	
	char *p_str = &str[new_length];
 
	strcpy(p_str, str2);
	
	Print(x, y, str);
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintSpaced(int x, int y, const char *text, int x_space, s32 x_offset, s32 y_offset)
{

	unsigned char font_char;
	s32 spacing = x_space << 12;
	s32 y_spacing = 0;
	s32 _x = x << 12;
	s32 _y = y << 12;
	s32 _x_space = x_space << 12;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		glSprite( _x >> 12, _y >> 12, GL_FLIP_NONE, &font_sprite[font_char] );
		
		y_spacing += y_offset;
		spacing += x_offset;
		_x_space = spacing;
		
		_x += ((font_sprite[font_char].width << 12) + _x_space); 
		_y += (y_spacing);
	}
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintSine(int x, int y, const char *text, int width_offset, int _height, int cycles, s32 start_angle)
{

	unsigned char font_char;
	unsigned int length = strlen(text);
	s32 angle_inc = div32(32768/2, length-1)*cycles;
	
	s32 _y;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		
		_y = sinLerp(start_angle) * _height; 
		
		glSprite( x,y + (_y >> 12), GL_FLIP_NONE, &font_sprite[font_char] );

		
		x += font_sprite[font_char].width + width_offset; 
		start_angle += angle_inc;
	}


}

/******************************************************************************



******************************************************************************/
void GLFont::PrintVertical(int x, int y, const char *text)
{
	unsigned char font_char;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		glSprite( x, y, GL_FLIP_NONE, &font_sprite[font_char] );

		y += font_sprite[font_char].height; 
	}

}

/******************************************************************************



******************************************************************************/
void GLFont::PrintSpacedVertical(int x, int y, const char *text, int x_space, s32 x_offset, s32 y_offset)
{

	unsigned char font_char;
	s32 spacing = x_space << 12;
	s32 y_spacing = 0;
	s32 _x = x << 12;
	s32 _y = y << 12;
	s32 _x_space = x_space << 12;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		glSprite( _x >> 12, _y >> 12, GL_FLIP_NONE, &font_sprite[font_char] );

		y_spacing += y_offset;
		spacing += x_offset;
		_x_space = spacing;
		
		_x += (_x_space); 
		_y += ((font_sprite[font_char].width << 12) + y_spacing);
	}
	
}

/******************************************************************************



******************************************************************************/
void GLFont::PrintSineVertical(int x, int y, const char *text, int height_offset, int _width, int cycles, s32 start_angle)
{

	unsigned char font_char;
	unsigned int length = strlen(text);
	s32 angle_inc = div32(32768/2, length-1)*cycles;
	
	s32 _x;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		
		_x = sinLerp(start_angle) * _width; 
		
		glSprite( x + (_x >> 12), y, GL_FLIP_NONE, &font_sprite[font_char] );

		
		y += font_sprite[font_char].height + height_offset; 
		start_angle += angle_inc;
	}


}
