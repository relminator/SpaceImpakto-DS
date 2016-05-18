/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.phatcode.net

	Singleton Fx3D class


*******************************************************************************
******************************************************************************/

#include "Fx2D.h"

Fx2D* Fx2D::ms_instance = 0;

//--------------------------------------
//	
//--------------------------------------
Fx2D::Fx2D()
{
}

Fx2D::~Fx2D()
{
}

//--------------------------------------
//	
//--------------------------------------
Fx2D* Fx2D::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Fx2D();
	}
	return ms_instance;
}

//--------------------------------------
//	
//--------------------------------------
void Fx2D::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}


void Fx2D::Init()
{
	
    		
	// generate our 256 color palette
	for( int i = 0; i < 256; i++ )
	{
	    u8 r = (u8)(abs(int(16 - 15 * sin(i * M_PI / 16.0f))));
		u8 g = (u8)(abs(int(16 - 15 * sin(i * M_PI / 12.0f))));
		u8 b = (u8)(abs(int(16 - 15 * sin(i * M_PI / 18.0f))));
		PlasmaPal[i] = RGB15(r,g,b);
	}
	
}
//--------------------------------------
//	
//--------------------------------------
void Fx2D::Fade( unsigned short* Target,    // sub or main bg/oam 
				 const unsigned short* StartPal,  // pal to lerp from 
				 const unsigned short* EndPal,    // pal to lerp to 
				 s32 &Interp,
				 s32 Speed,
			     int MaxColors 
				)
{
   
	Interp += Speed;
	Interp = UTIL::Clamp(Interp,0,4096);
	
	//dmaCopy( Palettes::Instance()->LerpPal( StartPal, EndPal,  Interp ),
	//		 Target,
	//		 MaxColors );
		   
	//memcpy(buffer2, buffer1, SCREEN_HEIGHT * SCREEN_WIDTH*sizeof(u16));
	
	memcpy( Target, 
			Palettes::Instance()->LerpPal( StartPal, EndPal,  Interp), 
			MaxColors * sizeof(byte) );
	
}



//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaPolar1()
{
	
	
	//write to vpage directly
	u8 *vpage_offset = Vpage;
	

	u8 rot = 75;
	int c;
	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH-1; xa++ )
		{
			rot = -rot;		// draw plasmas every other pixel
			s32 dx2 = (abs(SCREEN_WIDTH/2 - xa));
			s32 dy2 = (abs(SCREEN_HEIGHT/2 - ya));
			s32 dist = sqrt32(dx2 * dx2 + dy2 * dy2);
			c = (sinLerp(xa * BRAD_PI / (34)) * 23) +
					(cosLerp(ya * BRAD_PI / (80)) * 15) +
					(cosLerp((xa + ya) * BRAD_PI / (60)) * 32) +
					(sinLerp(dist * BRAD_PI / (150)) * 35) +
					(cosLerp(cosLerp(atan2Lerp(dx2,dy2) * 20)) * 45);
			// write to vram
			*vpage_offset++ = ((c >> 12) & 255);
		}
		*vpage_offset++ = ((c >> 12) & 255);
	}	

	return Vpage;
	
}


//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaPolar2()
{
	
	
	//write to vpage directly
	u8 *vpage_offset = Vpage;
	

	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH; xa++ )
		{
			s32 dx2 = (abs(SCREEN_WIDTH/2 - xa));
			s32 dy2 = (abs(SCREEN_HEIGHT/2 - ya));
			s32 dist = sqrt32(dx2 * dx2 + dy2 * dy2);
			int c = (sinLerp(xa * BRAD_PI / (34)) * 23) +
					(cosLerp(ya * BRAD_PI / (80)) * 15) +
					(cosLerp((xa + ya) * BRAD_PI / (60)) * 32) +
					sinLerp((sinLerp(dist * BRAD_PI / (150)) * 35) + ((dist * BRAD_PI / (250)) * 25)) * 20 + 
					(cosLerp(cosLerp(atan2Lerp(dx2,dy2) * 20)) * 45);
			// write to vram
			*vpage_offset++ = ((c >> 12) & 255);
		}
		
	}	

	return Vpage;
	
}


//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaPolar3()
{
	
	
	//write to vpage directly
	u8 *vpage_offset = Vpage;
	

	u8 rot = 75;
	int c;
	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH-1; xa++ )
		{
			rot = -rot;		// draw plasmas every other pixel
			s32 dx2 = (abs(SCREEN_WIDTH/2 - xa));
			s32 dy2 = (abs(SCREEN_HEIGHT/2 - ya));
			s32 dist = sqrt32(dx2 * dx2 + dy2 * dy2);
			c = (sinLerp((xa+rot) * BRAD_PI / (34)) * 23) +
					(cosLerp((ya - rot) * BRAD_PI / (80)) * 15) +
					(cosLerp((xa + ya + rot) * BRAD_PI / (60)) * 32) +
					sinLerp((sinLerp(dist * BRAD_PI / (150)) * 35) + ((dist * BRAD_PI / (250)) * 25)) * 20 + 
					(sinLerp(cosLerp(atan2Lerp(dx2,dy2) * 20)) * 45);
			// write to vram
			*vpage_offset++ = ((c >> 12) & 255);
		}
		*vpage_offset++ = ((c >> 12) & 255);
	}	

	return Vpage;
	
}


//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaPolar4()
{
	
	
	//write to vpage directly
	u8 *vpage_offset = Vpage;
	

	u8 rot = 16;
	int c;
	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH-1; xa++ )
		{
			rot = -rot;		// draw plasmas every other pixel
			s32 dx2 =  xa - SCREEN_WIDTH/2;
			s32 dy2 =  ya - SCREEN_HEIGHT/2;
			s32 dist = sqrt32(dx2 * dx2 + dy2 * dy2) + rot;
			c = (dist << 12) + sinLerp(((dist << 12) + atan2Lerp(dx2,dy2)*64)/8)*12;		
			// write to vram
			*vpage_offset++ = ((c >> 12) & 255);
		}
		*vpage_offset++ = ((c >> 12) & 255);
	}	
		
	return Vpage;
	
}

//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaPolar5()
{
	
	
	//write to vpage directly
	u8 *vpage_offset = Vpage;
	

	u8 rot = 16;
	int c;
	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH-1; xa++ )
		{
			rot = -rot;		// draw plasmas every other pixel
			s32 dx2 =  xa - SCREEN_WIDTH/2;
			s32 dy2 =  ya - SCREEN_HEIGHT/2;
			s32 dist = sqrt32(dx2 * dx2 + dy2 * dy2) + rot;
			//int c = (dist << 12) + sinLerp(((dist << 12) + atan2Lerp(dx2,dy2)*64)/8)*12;		
			c = sinLerp(atan2Lerp(dx2,dy2)) * 45 + sinLerp((dist << 12)/16) * 42;
			// write to vram
			*vpage_offset++ = ((c >> 12) & 255);
		}
		*vpage_offset++ = ((c >> 12) & 255);
	}	
		
	return Vpage;
	
}

//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaPolar6()
{
	
	
	//write to vpage directly
	u8 *vpage_offset = Vpage;
	

	int c;
	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH; xa++ )
		{
			s32 dx2 =  xa - SCREEN_WIDTH/2;
			s32 dy2 =  ya - SCREEN_HEIGHT/2;
			s32 dist = sqrt32(dx2 * dx2 + dy2 * dy2);
			c = (dist << 12)*2 + sinLerp(((dist << 12)*2 + sinLerp(atan2Lerp(dx2,dy2))*64)/8)*12;		
			// write to vram
			*vpage_offset++ = ((c >> 12) & 255);
		}
	}	
		
	return Vpage;
	
}

//--------------------------------------
//	
//--------------------------------------
u8* Fx2D::PlasmaStencil( const unsigned int *image )
{
	u8 *vpage_offset = Vpage;
	u8 *image_offset = (u8*)image;
	
	for(int ya = 0; ya < SCREEN_HEIGHT; ya++ )
	{
		for(int xa = 0; xa < SCREEN_WIDTH; xa++ )
		{
			if( (*image_offset) == 255 )
			{
				int c = sinLerp(xa * BRAD_PI / (70)) * 25;
				c += sinLerp(ya * BRAD_PI / (45)) * 32;
				c += sinLerp((xa + ya) * BRAD_PI / (120)) * 64;
				// write to vram
				*vpage_offset = 128 + ((c >> 12) & 127);
			}
			else
			{
				*vpage_offset = *image_offset;
			}
			image_offset++;
			vpage_offset++;
		}
	}	

	return Vpage;
	
}
	

//--------------------------------------
//	
//--------------------------------------
void Fx2D::CyclePlasmaPal()
{
	
	for( int i = 0; i < 256; i++ )
	{
		int j = UTIL::Wrap(i+1, 255);
		PlasmaPal[i] = PlasmaPal[j];
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx2D::CyclePlasmaPal( int start, int end )
{
	
	for( int i = start; i <= end; i++ )
	{
		int j = i + 1;
		if( j > end ) j = start;
		
		PlasmaPal[i] = PlasmaPal[j];
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx2D::LoadPal( const unsigned short *pal )
{
	
	for( int i = 0; i < 256; i++ )
	{
		PlasmaPal[i] = pal[i];
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx2D::LoadPal(  int start, int end, const unsigned short *pal )
{
	
	for( int i = start; i <= end; i++ )
	{
		PlasmaPal[i] = pal[i];
	}
	
}
	
	