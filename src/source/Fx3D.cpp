/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	Singleton Fx3D class


*******************************************************************************
******************************************************************************/

#include "Fx3D.h"

#define ARINGS  8
#define ABANDS  8
#define ARADIUS  1.8f

#define SRINGS  8
#define SBANDS  8
#define SRADIUS  1.0f


#define GRID_RINGS  2
#define GRID_BANDS  2
#define GRID_WIDTH  15
#define GRID_HEIGHT  12


static const int TILE_SIZE = 16;
static const int XRES = SCREEN_WIDTH/TILE_SIZE;
static const int YRES = SCREEN_HEIGHT/TILE_SIZE;

int Xcoord[XRES+4][YRES+4];
int Ycoord[XRES+4][YRES+4];


Fx3D* Fx3D::ms_instance = 0;


//--------------------------------------
//	
//--------------------------------------
Fx3D::Fx3D()
{
}

Fx3D::~Fx3D()
{
}

//--------------------------------------
//	
//--------------------------------------
Fx3D* Fx3D::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Fx3D();
	}
	return ms_instance;
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}



//--------------------------------------
//	
//--------------------------------------
int Fx3D::LoadAscaris( GL_TEXTURE_TYPE_ENUM type,
					    int sizeX,
					    int sizeY,
					    int param,
					    const uint8 *texture,
					    int pallette_width,
						const u16 *palette
				      )
{
 
	Ascaris = InitAscaris( ARINGS,    	  	  // rings
						   ABANDS,              // bands
						   ARADIUS,			  // radius of each ring from center
						   0,	  // swirl offset
						   32,
						   32
						  );
	
	int texID = Ascaris->LoadTexture( type,
					      sizeX,
					      sizeY,
					      param,
					      texture,
					      pallette_width,
						  palette
					    );
	
	return texID;
	
}


//--------------------------------------
//	
//--------------------------------------
void Fx3D::ReloadAscarisTexture( GL_TEXTURE_TYPE_ENUM type,
								 int TextureID
							   )
{
 
	Ascaris->CopyTexture( type,
						  TextureID
						);
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::DrawAscaris()
{
	static int speed = 0;
	speed++;
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
		// save current projecttion matrix
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
			// reset projection matrix to fish-eye view
			glLoadIdentity();
			gluPerspective( 45, 256.0 / 192.0, 200, 200 );
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			
			s32 x, y, z;
			x = -4 << 12;
			y = 0;
			z = -10 << 12;                  
			gluLookAtf32( 0, 0, 0,
						  x, y, z,
						  0, 1 << 12, 0
						);
			glRotateZi( speed * -4 );
			Ascaris->Draw( ( speed*16 ) & 4095, ( speed*2 ) & 4095, true );
		
		// restore projection matrix
		glMatrixMode( GL_PROJECTION );
		glPopMatrix(1);
		
	// Go back to modelview
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix(1);	
}


//--------------------------------------
//	
//--------------------------------------
void Fx3D::DrawAscaris2()
{
	static int speed = 0;
	speed += 6;
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	
		// save current projecttion matrix
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
			// reset projection matrix to fish-eye view
			glLoadIdentity();
			gluPerspective( 45, 256.0 / 192.0, 200, 200 );
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			glTranslate3f32( 0, 0, floattof32(6.0) );
			glScalef32(floattof32(8),floattof32(2),floattof32(1.5));
		
		
			s32 x, y, z;
			x = -4 << 12;
			y = -floattof32(0.5);
			z = -10 << 12;                  
			gluLookAtf32( 0, 0, 0,
						  x, y, z,
						  0, 1 << 12, 0
						);
			//glRotateZi( speed * -4 );
			Ascaris->Draw( ( speed*16 ) & 4095, ( speed*2 ) & 4095, true );
		
		// restore projection matrix
		glMatrixMode( GL_PROJECTION );
		glPopMatrix(1);
		
	// Go back to modelview
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix(1);
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::LoadShapes( GL_TEXTURE_TYPE_ENUM type,
						int TextureID
					 )
{
	
	
	
	float a,b,m,n1,n2,n3;
	
	a = 1;
	b = 1;
	m = 4;
	n1 = 0.3;
	n2 = 1.0;
	n3 = 1.0;
	
	
	Shapes[0].ps_Vb =  InitSuperShape( SRINGS,    	  	  // rings
								SBANDS,            // bands
								SRADIUS,		  // radius 
								2,
								2,
								a,
								b,
								m,
								n1,
								n2,
								n3
							  );
	
	a = 5.784;
	b = 1.436;
	m = 8.14;
	n1 = 5.867;
	n2 = -0.630;
	n3 = 15.179;
	
	Shapes[1].ps_Vb = InitSuperShape( SRINGS,    	  	  // rings
								SBANDS,            // bands
								SRADIUS,		  // radius 
								2,
								2,
								a,
								b,
								m,
								n1,
								n2,
								n3
							  );
											
	a = -5.784;
	b = -1.436;
	m = 8.14;
	n1 = 7.867;
	n2 = -0.630;
	n3 = -12.179;
	
	Shapes[2].ps_Vb = InitSuperShape( SRINGS,    	  	  // rings
								SBANDS,            // bands
								SRADIUS,		  // radius 
								2,
								2,
								a,
								b,
								m,
								n1,
								n2,
								n3
							  );
	
	a = 1;
	b = 3;
	m = 1;
	n1 = 0.3;
	n2 = 1.0;
	n3 = 1.0;
	
	Shapes[3].ps_Vb = InitSuperShape( SRINGS,    	  	  // rings
								SBANDS,            // bands
								SRADIUS,		  // radius 
								2,
								2,
								a,
								b,
								m,
								n1,
								n2,
								n3
							  );
	
	
	
	//Shapes[0].ps_Vb->LoadTexture( type,
	//						sizeX, 
	//						sizeY, 
	//						param,
	//						(u8*)texture, 
	//						Palette
	//					  );

	//int TextureID = Shapes[0].ps_Vb->GetTextureID();
	
	for( int i = 0; i < MAX_SHAPES; i++ )
	{
		Shapes[i].ps_Vb->CopyTexture( type,
									  TextureID
									);
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::InitShapes()
{
	
	for( int i = 0; i < MAX_SHAPES; i++ )
	{
		
		Shapes[i].x = 6 << 12;
		Shapes[i].y = (-2 + qran_range(0, 4)) << 12;
		Shapes[i].z = (-qran_range(floattof32(2.5), floattof32(3.0)));
	
		Shapes[i].xa = rand();
		Shapes[i].ya = rand();
		Shapes[i].za = rand();
		
			
		
		Shapes[i].xs = qran_range(64, 256);
		Shapes[i].ys = qran_range(64, 256);
		Shapes[i].zs = qran_range(64, 256);
		
		Shapes[i].Speed = qran_range(64, 256);
		
		Shapes[i].WaitValue = (rand() % 15 ) * 60;
		Shapes[i].Counter = 0;
		Shapes[i].Active = false;
		
		
	}
	
}
	


//--------------------------------------
//	
//--------------------------------------
void Fx3D::UpdateShapes()
{

	for( int i = 0; i < MAX_SHAPES; i++ )
	{
		if (!Shapes[i].Active)
		{
			Shapes[i].Counter++;
			if (Shapes[i].Counter >= Shapes[i].WaitValue)
			{
			
				Shapes[i].Active = true;
				Shapes[i].Counter = 0;
				
			}
		}
		else
		{
			Shapes[i].x -= Shapes[i].Speed;
			if( Shapes[i].x < (-6 << 12) ) 
			{
				Shapes[i].x = 6 << 12;
				Shapes[i].Active = false;
			}
			Shapes[i].xa +=128;
		}
	}  // next i
	
}


//--------------------------------------
//	
//--------------------------------------
void Fx3D::DrawShapes()
{
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
		
	for( int i = 0; i < MAX_SHAPES; i++ )
	{
		
		if( Shapes[i].Active )
		{
			
			glPushMatrix();
			
				glTranslate3f32( Shapes[i].x, Shapes[i].y, Shapes[i].z );
				glRotateXi( Shapes[i].xa );
				glRotateYi( Shapes[i].ya );
				glRotateZi( Shapes[i].za );
			
				Shapes[i].ps_Vb->Draw( Shapes[i].xa, Shapes[i].ya, false );
			
			glPopMatrix(1);
		}
		
	}	
	
	
}
	
	
//--------------------------------------
//	
//--------------------------------------	
int Fx3D::LoadGrid( GL_TEXTURE_TYPE_ENUM type,
					 int sizeX,
					 int sizeY,
					 int param,
					 const uint8 *texture,
					 int pallette_width,
					 const u16 *palette
				   )
{

	Grid = InitGrid( GRID_RINGS, GRID_BANDS, GRID_WIDTH, GRID_HEIGHT, 8, 8 );
	
	int TexID = Grid->LoadTexture( type,
					   sizeX,
					   sizeY,
					   param,
					   texture,
					   pallette_width,
					   palette
					 );
	
	return TexID;
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::DrawGrid( s32 text_off_x, s32 text_off_y )
{

	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
		
		glLoadIdentity();
		
		glTranslate3f32( 0, 0, floattof32(3.0) );
		glScalef32(floattof32(1),floattof32(1.5),floattof32(1.5));
	
		// floor
		glPushMatrix();

			glTranslate3f32( 0, -1 << 12, floattof32(-2.0) );		
			glRotateXi( inttof32(244) );
			
			Grid->Draw( text_off_x, text_off_y, true );
			
		glPopMatrix(1);
		
		// ceiling
		glPushMatrix();

			glTranslate3f32( 0, 1 << 12, floattof32(-2.0) );
			glRotateXi( inttof32(180) );
			
			Grid->Draw( text_off_x, text_off_y, true);
		
		glPopMatrix(1);

	glPopMatrix(1);
	
	
}


//--------------------------------------
//	
//--------------------------------------
void Fx3D::ReloadGridTexture( GL_TEXTURE_TYPE_ENUM type,
							  int TextureID
							)
{
 
	Grid->CopyTexture( type,
					   TextureID
					 );
	
}


//--------------------------------------
//	
//--------------------------------------
void Fx3D::InitStars()
{
	
	for( int i = 0; i < MAX_PARTS; i++ )
	{
		s32 val = -rand() % (4<<12); 
		s32 val2 = rand() % (5<<12);
		
		Parts[i].x = val + val2;
		val = -rand() % (3<<12); 
		val2 = rand() % (4<<12); 
		Parts[i].y = val + val2;
		
		val = -rand() % (floattof32(1.5)); 
		val2 = -rand() % (floattof32(5)); 
		Parts[i].z = val + val2;
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::UpdateStars()
{
	for( int i = 0; i < MAX_PARTS; i++ )
	{
		Parts[i].z += 128;
		if ( Parts[i].z >= 4096 )
			Parts[i].z = floattof32(-5);
	}
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::DrawParts()
{
	static s32 rot = 0;
	
	rot += 8;
	
	glDisable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glPushMatrix();
		glRotateZi( rot );
		for( int i = 0; i < MAX_PARTS; i++ )
		{
			glPushMatrix();
				glColor(RGB15(Parts[i].x,Parts[i].y,Parts[i].z));
				glTranslate3f32( Parts[i].x, Parts[i].y, Parts[i].z );
				glBegin( GL_TRIANGLES );
					glVertex3v16( 0,0,0 );
					glVertex3v16( 0,0,floattov16(0.2) );
					glVertex3v16( 0,0,0 );
				glEnd();
			glPopMatrix(1);
		}
	glPopMatrix(1);

	glColor(RGB15(31,31,31));
	glEnable( GL_TEXTURE_2D );
	
}


//--------------------------------------
//	 
//--------------------------------------
void Fx3D::Fade( int Color1,    // Color to fade to 
			     int Color2,
				 int Color3,
				 int Color4,
				 s32 &Interp,
			     s32 Speed )
{

	Interp += Speed;
	Interp = UTIL::Clamp( Interp, 0, 4096 );
	int Trans = UTIL::Lerpf32( 1 << 12 , 31 << 12, Interp ) >> 12;  
	Trans = UTIL::Clamp( Trans, 1, 31 );
	
	glPolyFmt( POLY_ALPHA(Trans) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );

	glBoxFilledGradient( 0, 0, 256, 192,
						 Color1,
						 Color2,
						 Color3,
						 Color4
					   );
	
}



//--------------------------------------
//	 
//--------------------------------------
void Fx3D::WarpScreen( int Frame, const glImage *sprite )
              
{
	

	// Calculate warp
	for( int y = -1; y <= (YRES+1); y++)
	{
		for( int x = -1; x <= (XRES+1); x++)
		{
			s32 tx = (x * TILE_SIZE) << 12;
			s32 ty = (y * TILE_SIZE) << 12;
			s32 stx = sinLerp(((x+y)*1230 + Frame*130)) +
					  sinLerp((y*2750 + Frame*600));
			s32 sty = sinLerp((y*4630 + Frame*400)) +
					  cosLerp(((y+x)*6400 + Frame*290));
			
			stx *= 5;
			sty *= 5;
			
			Xcoord[x+1][y+1] = (tx+stx)>>12;
			Ycoord[x+1][y+1] = (ty+sty)>>12;
		}
	}
	
	
	// Draw warp
	//glColor( RGB15(31,31,31) );

	for( int y = -1; y <= YRES; y++)
	{
		for( int x = -1; x <= XRES; x++)
		{
			int xx = x+1;
			int yy = y+1;
			
			int x1 = Xcoord[xx][yy];
			int y1 = Ycoord[xx][yy];
			int x2 = Xcoord[xx+1][yy];
			int y2 = Ycoord[xx+1][yy];
			int x3 = Xcoord[xx+1][yy+1];
			int y3 = Ycoord[xx+1][yy+1];
			int x4 = Xcoord[xx][yy+1];
			int y4 = Ycoord[xx][yy+1];
			
			glSpriteOnQuad( x1, y1,
							x2, y2,
							x3, y3,
							x4, y4,
							Frame & 31,(Frame*2) & 31,
							GL_FLIP_NONE, 
							&sprite[0] );
		}
	}
	
			
	
}


//--------------------------------------
//	 
//--------------------------------------
void Fx3D::WarpScreenPolar( int Frame, const glImage *sprite )              
{
	

	const int XMID = (XRES+2) / 2;
	const int YMID = (YRES+2) / 2;
	
	// Calculate warp
	for( int y = -1; y <= (YRES+1); y++)
	{
		for( int x = -1; x <= (XRES+1); x++)
		{
			s32 tx = (x * TILE_SIZE) << 12;
			s32 ty = (y * TILE_SIZE) << 12;
			s32 dx = ( XMID - x );
			s32 dy = ( YMID - y );
			s32 dist = sqrt32(dx * dx + dy * dy);
			s32 stx = sinLerp(((dist << 12) + atan2Lerp(dy,dx)*8) + Frame*500);
			
			stx *= 5;
			
			Xcoord[x+1][y+1] = (tx+stx)>>12;
			Ycoord[x+1][y+1] = (ty+stx)>>12;
		}
	}
	
	
	// Draw warp
	//glColor( RGB15(31,31,31) );

	for( int y = -1; y <= YRES; y++)
	{
		for( int x = -1; x <= XRES; x++)
		{
			int xx = x+1;
			int yy = y+1;
			
			int x1 = Xcoord[xx][yy];
			int y1 = Ycoord[xx][yy];
			int x2 = Xcoord[xx+1][yy];
			int y2 = Ycoord[xx+1][yy];
			int x3 = Xcoord[xx+1][yy+1];
			int y3 = Ycoord[xx+1][yy+1];
			int x4 = Xcoord[xx][yy+1];
			int y4 = Ycoord[xx][yy+1];
			
			glSpriteOnQuad( x1, y1,
							x2, y2,
							x3, y3,
							x4, y4,
							(-Frame*2) & 31, 0,
							GL_FLIP_NONE, 
							&sprite[0] );
		}
	}
	
			
	
}



