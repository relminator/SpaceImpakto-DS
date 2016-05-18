/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Viper class


*******************************************************************************
******************************************************************************/

#include "Bomb.h"

Bomb::Bomb()
{
	
	Counter = 0;
	for(int i = 0; i < MAX_BOMBS; i++)
	{
		Bombs[i].x = qran_range( 0, 255 ) << 12;
		Bombs[i].y = qran_range( 0, 180 ) << 12;
		Bombs[i].Frame = i & 3;
		Bombs[i].Speed = qran_range( 4096 * 2, 4096 * 4 );
	}
	
	Ax = -64 << 12;
	Ay = 96 << 12;
	Aspeed = 0; //floattof32(1.0);
	
	for(int i = 0; i < MAX_RAYS; i++)
	{
		Rays[i].Angle = qran_range( 0, BRAD_2PI );
		Rays[i].SpreadAngle = qran_range( 0, BRAD_PI/8 );
		Rays[i].AngleSpeed = qran_range( 0, 1024 );
	}
}

Bomb::~Bomb()
{
}

void Bomb::Init( int Palette, glImage* const Sprites, glImage* const Sprites2, glImage* const Sprites3 )
{
	
	ColorTable = Palette;		// init needed data for drawing
	Images = Sprites;
	Images2 = Sprites2;
	Images3 = Sprites3;
}
	
void Bomb::Reset( int type )
{
	
	if( type == 0 )
	{
		Ax = -32 << 12;
		Ay = 96 << 12;
		Aspeed = 0;
		
		for(int i = 0; i < MAX_RAYS; i++)
		{
			Rays[i].Angle = qran_range( 0, BRAD_2PI );
			Rays[i].SpreadAngle = qran_range( 0, BRAD_PI/16 );
			Rays[i].AngleSpeed = qran_range( 0, 1024 );
		}		
	}
	else if( type == 1 )
	{
		for(int i = 0; i < MAX_BOMBS; i++)
		{
			Bombs[i].x = qran_range( -300, 0 ) << 12;
			Bombs[i].y = qran_range( 0, 180 ) << 12;
			Bombs[i].Frame = i & 3;
			Bombs[i].Speed = qran_range( 4096 * 3, 4096 * 5 );
		}
	}
	else
	{
		for(int i = 0; i < MAX_BOMBS; i++)
		{
			Bombs[i].x = inttof32(128);
			Bombs[i].y = inttof32(96);
			Bombs[i].Frame = i & 3;
			Bombs[i].Speed = i * 2000;
			Bombs[i].Active = false;
		}
		Angle = 0;
	}
	Counter = 0;
	
}

void Bomb::Update( int type )
{
	
	if( Active )
	{
		Counter++;
		if( type == 0 )
		{
			DoAgilaBomb();
		}
		else if( type == 1 )
		{
			for(int i = 0; i < MAX_BOMBS; i++)
			{
				Bombs[i].x += Bombs[i].Speed;
				if( CoolDownCounter > 60*2 )
					if( Bombs[i].x > ((SCREEN_WIDTH + 32) << 12) )  Bombs[i].x = -64 << 12;
				if( (Counter & 3) == 0 )
					Bombs[i].Frame = (Bombs[i].Frame + 1) & 3;
			}
		}
		else
		{
			DoVicBomb();
		}	
	
		CoolDownCounter--;
		if( CoolDownCounter < 0 ) Active = false;
		
	}

	
}


void Bomb::Draw( int type ) const
{

	if( Active )
	{
	
		// translucency
		if( type == 0 )
		{
			DrawAgilaBomb();
		}
		else if( type == 1 )
		{
			glPolyFmt( POLY_ALPHA(18) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
			for(int i = 0; i < MAX_BOMBS; i++)
			{
				glSprite( (Bombs[i].x >> 12) - 20, (Bombs[i].y >> 12) - 16, 
						   GL_FLIP_NONE, &Images[Bombs[i].Frame] );
			}
		}
		else
		{
			DrawVicBomb();
		}
	
	}
}

void Bomb::DoAgilaBomb()
{
	if( Counter < 100 )
	{
		Aspeed += floattof32(0.04); 
	}
	else if( (Counter >= 100 ) && (Counter < 256 ) )
	{
		Aspeed = 0;
	}
	else
	{
		Aspeed += floattof32(0.04); 		
	}
	
	Ax += Aspeed;
	
	for(int i = 0; i < MAX_RAYS; i++)
	{
		Rays[i].Angle += Rays[i].AngleSpeed;
	}
	
}
void Bomb::DrawAgilaBomb() const
{
	
	int ax = Ax >> 12;
	int ay = Ay >> 12;
	glPolyFmt( POLY_ALPHA(18) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	for(int i = 0; i < MAX_RAYS; i++)
	{
		
		glTriangleFilledGradient( ax, ay, 
						  ax + ((cosLerp(Rays[i].Angle)*512) >> 12),
						  ay + ((sinLerp(Rays[i].Angle)*512) >> 12),
						  ax + ((cosLerp(Rays[i].Angle+Rays[i].SpreadAngle)*512) >> 12),
						  ay + ((sinLerp(Rays[i].Angle+Rays[i].SpreadAngle)*512) >> 12),
						  RGB15((i+1) * Counter,(i+1) * Counter*2,(i+1) * Counter*3),
						  RGB15((i+1) * Counter*2,(i+1) * Counter*3,(i+1) * Counter*1),
						  RGB15((i+1) * Counter*3,(i+1) * Counter*1,(i+1) * Counter*2) );
	}
	
	glPolyFmt( POLY_ALPHA(24) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		
	glColor(RGB15(Counter*3,Counter*2,Counter*1));
	glSpriteRotateScale( Ax >> 12, Ay >> 12,
				 0, inttof32(1) + abs(sinLerp(Counter*1000)), 
				 GL_FLIP_NONE, &Images2[0]  );
	
	glPolyFmt( POLY_ALPHA(18) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	glColor(RGB15(Counter*1,Counter*3,Counter*2));
	glSpriteRotateScale( Ax >> 12, Ay >> 12,
				 0, inttof32(1) + abs(cosLerp(Counter*1600)*2), 
				 GL_FLIP_NONE, &Images2[3]  );
	
	glColor(RGB15(31,31,31));
	
}


void Bomb::DoVicBomb()
{
				
	if( ((Counter & 7) == 0 ) && (CoolDownCounter > 30) )
	{
		for( int j = 0; j < 8; j++ )
		{
			Angle += BRAD_2PI + 300;		
			s32 A = Angle + j * BRAD_PI/4;
			for(int i = 0; i < MAX_BOMBS; i++)
			{
				if( !Bombs[i].Active )
				{
					Bombs[i].Active = true;
					Bombs[i].x = inttof32(128);
					Bombs[i].y = inttof32(96);
					Bombs[i].Dx = cosLerp(A) * 5;
					Bombs[i].Dy = sinLerp(A) * 5;
					Bombs[i].Angle = A;
					break;
				}
			}
		}
	}
	
	for(int i = 0; i < MAX_BOMBS; i++)
	{
		if( Bombs[i].Active )
		{
			Bombs[i].x += Bombs[i].Dx;
			Bombs[i].y += Bombs[i].Dy;
			Bombs[i].Angle += 2048;
			if( Bombs[i].x < inttof32(-16) || Bombs[i].x > inttof32(256+16) ||
				Bombs[i].y < inttof32(-16) || Bombs[i].y > inttof32(192+16) )
			{
					Bombs[i].Active = false;
			}
		}
		
	}
		
}


void Bomb::DrawVicBomb() const
{
	glPolyFmt( POLY_ALPHA(14) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	glColor(RGB15(31,31,31));
	for(int i = 0; i < MAX_BOMBS; i++)
	{
		if( Bombs[i].Active )
		{
			glSpriteRotate( Bombs[i].x >> 12, Bombs[i].y >> 12,
							Bombs[i].Angle, 
							GL_FLIP_NONE, &Images3[6]  );
		}
	}
		
}
