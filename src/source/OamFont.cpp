/******************************************************************************
*******************************************************************************

	Space Impakto DS
	relminator
	http://rel.betterwebber.com


	OamFont class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include "OamFont.h"



/******************************************************************************



******************************************************************************/
void OamFont::Init(OamState *_oam,u8 *_gfx, int _width, int _height, SpriteSize _size, int _max_chars, int _minID )
{
	ActiveChars = 0;
	Oam = _oam;
	
	if (_max_chars > MAX_FONT_SPRITES)
		_max_chars = MAX_FONT_SPRITES;
	
	Gfx = _gfx;
	MaxChars = _max_chars;
	Width = _width;
	Height = _height;
	TileMemSize = (Width * Height);	// we are using 8 bit 256 colors
	
	for (int i = 0; i < MaxChars; i++)
	{
		Fonts[i].Create (Oam, Width, Height, _size, SpriteColorFormat_256Color, i + _minID);
		Fonts[i].Active = false;
	}

}


/******************************************************************************



******************************************************************************/
void OamFont::LoadPalette( const u16* ColorTable, u16* target, int numbytes)
{
	dmaCopy(ColorTable, target, numbytes);
	
}
/******************************************************************************



******************************************************************************/
void OamFont::Clear()
{
	ActiveChars = 0;
	for (int i = 0; i < MaxChars; i++)
	{
		Fonts[i].Active = false;
		Fonts[i].UpdateOam();
	}

}

/******************************************************************************



******************************************************************************/
void OamFont::Print(int x, int y, const char *text)
{

	unsigned char font_char;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		if (font_char)	// don't print space
		{
			for (int i = 0; i < MaxChars; i++)
			{
				if (!Fonts[i].Active)
				{
					dmaCopy(Gfx + (TileMemSize*font_char) , Fonts[i].Gfx, TileMemSize);
					Fonts[i].Active = true;
					Fonts[i].x = x;
					Fonts[i].y = y;
					Fonts[i].UpdateOam();
					ActiveChars++;
					break;
				}
			}
		}
		x += Width; 
	}
	
}

/******************************************************************************



******************************************************************************/
void OamFont::Print(int x, int y, int value)
{

	
	sprintf(Str,"%i",value);
	
	Print(x, y, Str);
	
}

/******************************************************************************



******************************************************************************/
void OamFont::Print(int x, int y, const char *text, int value)
{

	strcpy(Str, text);
	sprintf(Str2,"%i",value);
	strcat(Str, Str2);
	
	Print(x, y, Str);
	
}

/******************************************************************************



******************************************************************************/
void OamFont::PrintCentered(int x, int y, const char *text)
{

	unsigned char font_char;
	int total_width = 0;
	
	total_width = Width * strlen(text);
	
	x = (SCREEN_WIDTH - total_width) / 2; 
	
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		if (font_char)	// don't print space
		{
			for (int i = 0; i < MaxChars; i++)
			{
				if (!Fonts[i].Active)
				{
					dmaCopy(Gfx + (TileMemSize*font_char) , Fonts[i].Gfx, TileMemSize);
					Fonts[i].Active = true;
					Fonts[i].x = x;
					Fonts[i].y = y;
					Fonts[i].UpdateOam();
					ActiveChars++;
					break;
				}
			}
		}
		x += Width; 
	}
	
}

/******************************************************************************



******************************************************************************/
void OamFont::PrintScore(int x, int y, int value, const char *filler, int length)
{

	sprintf(Str2,"%i",value);
	int value_len = strlen(Str2);
	
	if (length < value_len)
		length = value_len;
		
	int new_length = length - value_len;
	
	for (int i=0; i < new_length; i++)
	{
		Str[i]=*filler;
	}
	
	//Str[new_length]='\0';			// not needed
	
	char *p_str = &Str[new_length];
 
	strcpy(p_str, Str2);
	
	Print(x, y, Str);
	
}

/******************************************************************************



******************************************************************************/
void OamFont::PrintSpaced(int x, int y, const char *text, int x_space, s32 x_offset, s32 y_offset)
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
		if (font_char)	// don't print space
		{
			for (int i = 0; i < MaxChars; i++)
			{
				if (!Fonts[i].Active)
				{
					dmaCopy(Gfx + (TileMemSize*font_char) , Fonts[i].Gfx, TileMemSize);
					Fonts[i].Active = true;
					Fonts[i].x = _x >> 12;
					Fonts[i].y = _y >> 12;
					Fonts[i].UpdateOam();
					ActiveChars++;
					break;
				}
			}
		}
		
		y_spacing += y_offset;
		
		spacing += x_offset;
		_x_space = spacing;
		
		_x += ((Width<<12) + _x_space); 
		_y += (y_spacing);
	}
	
}


/******************************************************************************



******************************************************************************/
void OamFont::PrintSine(int x, int y, const char *text, int width_offset, int _height, int cycles, s32 start_angle)
{

	unsigned char font_char;
	unsigned int length = strlen(text);
	s32 angle_inc = div32(32768/2, length-1)*cycles;
	
	s32 _y;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		
		_y = sinLerp(start_angle) * _height; 
		
		if (font_char)	// don't print space
		{
			for (int i = 0; i < MaxChars; i++)
			{
				if (!Fonts[i].Active)
				{
					dmaCopy(Gfx + (TileMemSize*font_char) , Fonts[i].Gfx, TileMemSize);
					Fonts[i].Active = true;
					Fonts[i].x = x;
					Fonts[i].y = y + (_y>>12);
					Fonts[i].UpdateOam();
					ActiveChars++;
					break;
				}
			}
		}
		
		x += Width + width_offset; 
		start_angle += angle_inc;
	}


}

/******************************************************************************



******************************************************************************/
void OamFont::PrintVertical(int x, int y, const char *text)
{

	unsigned char font_char;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		if (font_char)	// don't print space
		{
			for (int i = 0; i < MaxChars; i++)
			{
				if (!Fonts[i].Active)
				{
					dmaCopy(Gfx + (TileMemSize*font_char) , Fonts[i].Gfx, TileMemSize);
					Fonts[i].Active = true;
					Fonts[i].x = x;
					Fonts[i].y = y;
					Fonts[i].UpdateOam();
					ActiveChars++;
					break;
				}
			}
		}
		y += Height; 
	}
	
}

/******************************************************************************



******************************************************************************/
void OamFont::PrintSineVertical(int x, int y, const char *text, int height_offset, int _width, int cycles, s32 start_angle)
{

	unsigned char font_char;
	unsigned int length = strlen(text);
	s32 angle_inc = div32(32768/2, length-1)*cycles;
	
	s32 _x;
	
	while(*text)
	{
		font_char = (*(unsigned char*)text++) - 32;
		
		_x = sinLerp(start_angle) * _width; 
		
		if (font_char)	// don't print space
		{
			for (int i = 0; i < MaxChars; i++)
			{
				if (!Fonts[i].Active)
				{
					dmaCopy(Gfx + (TileMemSize*font_char) , Fonts[i].Gfx, TileMemSize);
					Fonts[i].Active = true;
					Fonts[i].x = (_x >> 12) + x;
					Fonts[i].y = y;
					Fonts[i].UpdateOam();
					ActiveChars++;
					break;
				}
			}
		}
		
		
		y += Height + height_offset; 
		start_angle += angle_inc;
	}


}
