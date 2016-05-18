/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	Level class


*******************************************************************************
******************************************************************************/

#include "Level.h"

//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
Level::Level()
{
	Width	= MAP_WIDTH;
	Height	= MAP_HEIGHT;
	
	CameraX = 0;
	CameraY = 0;
	
	TileX 	= 0;
	TileY 	= 0;
	
	PixelX 	= 0;
	PixelY 	= 0;
	
	for( int i = 0; i < MAX_STARS; i++ )
	{
		Stars[i].x = qran_range( 0, 255 ) << 12;
		Stars[i].y = qran_range( 0, 70 ) << 12;
		Stars[i].Color = RGB15(qran_range( 0, 31 ),qran_range( 0, 31 ),qran_range( 0, 31 ));
		Stars[i].Speed = -qran_range( 4096 / 2, 4096 * 4 );
	}
	
}

Level::~Level()
{
}


//--------------------------------------
//	
//--------------------------------------
void Level::Init( int Palette, glImage* const Sprites,  int stage )
{
	
	Width	= MAP_WIDTH;
	Height	= MAP_HEIGHT;
	
	CameraX = 0;
	CameraY = 0;
	
	TileX 	= 0;
	TileY 	= 0;
	
	PixelX 	= 0;
	PixelY 	= 0;
	
	ColorTable = Palette;		// init needed data for drawing
	Tiles = Sprites;
	
	EnemyColumn = 0;
	
	// Fill Map
	switch( stage ) // Level 01
	{
		case 1:
			for( int y = 0; y < MAP_HEIGHT; y++ )
			{
				for( int x = 0; x < MAP_WIDTH; x++ ) 
				{
					
					map[x][y] = ( (y & 15)* 8 + (x & 7) ) & 127;
				}
			}
			break;
		case 2:
			// Level 02
			for( int y = 0; y < MAP_HEIGHT; y++ )
			{
				for( int x = 0; x < MAP_WIDTH; x++ ) 
				{
					
					map[x][y] = ( (y & 3)* 16 + (x & 15) ) & 63;
				}
			}
			break;
		case 3:
			// Level 03
			for( int y = 0; y < MAP_HEIGHT; y++ )
			{
				for( int x = 0; x < MAP_WIDTH; x++ ) 
				{
					
					map[x][y] = ( (y & 15)* 8 + (x & 7) ) & 127;
				}
			}
			break;
		default:
			for( int y = 0; y < MAP_HEIGHT; y++ )
			{
				for( int x = 0; x < MAP_WIDTH; x++ ) 
				{
					
					map[x][y] = ( (y & 15)* 8 + (x & 7) ) & 127;
				}
			}
			
	}  // stage switch
	
}



//--------------------------------------
//	
//--------------------------------------
void Level::Load( int Stage )
{

	switch( Stage )
	{
		case 0:
			break;
		case 1:
			break;
		case 2:			
			break;
		case 3:
			break;
			
	}
	
}



//--------------------------------------
//	
//--------------------------------------
void Level::Update( int Stage, int Frame )
{

	// Update enemy sequencer
	if( (Frame & 31) == 0 )
	{
		EnemyColumn++;
	}
		
	switch( Stage )
	{
		case 0:			// M7
			Fx3D::Instance()->UpdateShapes();
			break;
		case 1:			// 2d scroller
			UpdateMap( floattof32(1.5) );
			break;
		case 2:			
			UpdateMap( floattof32(2.0) );
			break;
		case 3:			
			UpdateMap( floattof32(1.0) );
			UpdateStars();
			break;
		case 4:        // Tunnel
			break;
			
	}
		
	
		
	
}
	

//--------------------------------------
//	
//--------------------------------------
void Level::Draw3D( int Stage, int Frame ) const
{

	switch( Stage )
	{
		case 0:			// mode7
			Fx3D::Instance()->DrawGrid((Frame * 32) & 4095, (sinLerp(Frame * 32)) & 4095);
			Fx3D::Instance()->DrawShapes();
			break;
		case 1:			// 2d scroller is 2d so no
			break;
		case 2:			// 2d scroller is 2d so no
			break;
		case 3:			// 
			break;
		case 4:			// 
			Fx3D::Instance()->DrawAscaris2();
			
	}
		
	
}

//--------------------------------------
//	
//--------------------------------------
void Level::Draw2D( int Stage, int Frame ) const
{

	switch( Stage )
	{
		case 0:			// tunnel is 3d
			break;
		case 1:			// 2d scroller is 2d so draw
			DrawMap();
			break;
		case 2:			// 2d scroller is 2d so draw
			DrawMap();
			break;
		case 3:			// 2d scroller is 2d so draw
			DrawStars(Frame);
			int color = abs(sinLerp(Frame * 64) * 24) >> 12;
			glColor(RGB15(12,color,color));
			DrawMap();
			glColor(RGB15(31,31,31));
			break;
			
	}
		
	
}

//--------------------------------------
//	
//--------------------------------------
void Level::UpdateMap( s32 speed )
{
	const int TILE_SIZE = 16;
 
	// update the camera
	CameraX += speed >> 12;
	CameraY = 0;
	
	// limit camera X values
	if ( CameraX > ((Width) * TILE_SIZE ) - SCREEN_WIDTH )
	{
		CameraX = 0;
	}
	
	
	// calculate level starting tiles
	TileX = CameraX / TILE_SIZE; 
	TileY = CameraY / TILE_SIZE; 
	
	// calculate tile pixel offsets
	// Only works with power of 2 tilesize
	// use "%" for non-power of 2 sizes
	PixelX = CameraX & (TILE_SIZE - 1);
	PixelY = CameraY & (TILE_SIZE - 1);
	
}


//--------------------------------------
//	
//--------------------------------------
void Level::DrawMap( int Palette ) const
{
	
	// tiles are 16x16 pixels
	const int TILE_SIZE = 16;
 
	// calculate number of tiles per row and column
	const int SCREEN_TILE_X = SCREEN_WIDTH / TILE_SIZE;
	const int SCREEN_TILE_Y = SCREEN_HEIGHT / TILE_SIZE;
	
	// we need to draw an extra tile at the bottom and right 
	// since we are scrolling
	for( int y = 0; y < SCREEN_TILE_Y; y++ )
	{
		for( int x = 0; x <= SCREEN_TILE_X; x++ ) 
		{
			int tile_x = TileX + x;		// get relative tile positions
			int tile_y = TileY + y;
			int i = map[tile_x][tile_y];		// get map index
			if( i >= 0 )
			{
				int screen_x = (x * TILE_SIZE) - PixelX;      //Calculate where to put a
				int screen_y = (y * TILE_SIZE) - PixelY;      //particular tile
				glSprite(screen_x, screen_y, GL_FLIP_NONE , &Tiles[i & 127]);
			}
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Level::UpdateStars()
{
	
	for( int i = 0; i < MAX_STARS; i++ )
	{
		Stars[i].x += Stars[i].Speed;
		if( Stars[i].x < 0 )  Stars[i].x = 256 << 12;
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Level::DrawStars( int Frame ) const 
{
	
	for( int i = 0; i < MAX_STARS; i++ )
	{
		glPutPixel( Stars[i].x >> 12, Stars[i].y >> 12, Stars[i].Color );
	}
	
}