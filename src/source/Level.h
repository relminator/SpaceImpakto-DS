/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	Level class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <gl2d.h>
#include <stdio.h>

#include "Fx3D.h"


#ifndef LEVEL_H
#define LEVEL_H

#define  MAP_WIDTH 32
#define  MAP_HEIGHT 12

#define MAX_STARS 32

class Level 
{

public:
	struct Particle : public Vector2df32
	{
		s32 Speed;
		int Color;
	};

	Level();
	~Level();
	void Init( int Palette, glImage* const Sprites, int stage );
	void Load( int Stage );
	void Update( int Stage, int Frame );
	void UpdateMap( s32 speed );
	void Draw3D( int Stage, int Frame ) const;
	void Draw2D( int Stage, int Frame ) const;
	void DrawMap( int Palette = 0 ) const;
	void UpdateStars();
	void DrawStars( int Frame ) const;
	
	void LoadEnemySequence( const u8 *EnemyMap )
	{
		Enemies = EnemyMap;
		EnemyColumn = 0;
	}
	
	int GetEnemyID( int Frame, int Row )
	{
		if( EnemyColumn >= 255 ) return 0;
		
		if( (Frame & 31) == 0 )
			return (int) Enemies[  (Row * 256) +  (EnemyColumn) ];
		else
			return 0;
	}
	
	int GetEnemyColumn() const
	{
		return EnemyColumn;
	}
	
	
private:	
	int Width;			// dimensions of the map
	int Height;
	
	int CameraX;		// top-left cooordinates of our virtual camera
	int CameraY;		// Works almost the same the 2d BG scroller
	
	int TileX;			// current tile the top-left coordinate of our
	int TileY;			// camera occupies
	
	int PixelX;			// scrolling tile offsets
	int PixelY;

	glImage *Tiles;
	int ColorTable;
	
	short map[MAP_WIDTH][MAP_HEIGHT];
	
	Particle Stars[MAX_STARS];
	
	const u8 *Enemies;
	int EnemyColumn;
	
};

#endif // LEVEL_H
