/******************************************************************************
*******************************************************************************

	Bubble Fight EX
	relminator
	http://rel.betterwebber.com


	Singleton Fx3D class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <math.h>
#include <gl2d.h>

#include "GlobalManager.h"
#include "UTIL.h"

#include "VertexBuffer.h"
#include "SuperShape.h"

#include "cearn_atan.h"

#ifndef FX3D_H
#define FX3D_H

#define MAX_SHAPES 4
#define MAX_PARTS 256


class Fx3D {

	static Fx3D* ms_instance;

public:
	static Fx3D* Instance();
	static void Release();

	//--------------------------------------
	//	
	//--------------------------------------
	int LoadAscaris( GL_TEXTURE_TYPE_ENUM type,
					  int sizeX,
					  int sizeY,
					  int param,
					  const uint8 *texture,
					  int pallette_width,
					  const u16 *palette
				    );
					
	void ReloadAscarisTexture( GL_TEXTURE_TYPE_ENUM type,
							   int TextureID
							 );
	
	void RescaleAscarisUV( s32 Uscale, s32 Vscale )
	{
		Ascaris->RescaleUV( Uscale, Vscale );
	}
							 
	void DrawAscaris();
	
	void DrawAscaris2();
	
	//--------------------------------------
	//	
	//--------------------------------------
	void LoadShapes( GL_TEXTURE_TYPE_ENUM type,
					 int TextureID
				   );
					
	void InitShapes();
	
	void UpdateShapes();
	
	void DrawShapes();
	
	//--------------------------------------
	//	
	//--------------------------------------
	int LoadGrid( GL_TEXTURE_TYPE_ENUM type,
				   int sizeX,
				   int sizeY,
				   int param,
				   const uint8 *texture,
				   int pallette_width,
				   const u16 *palette	
				 );
	
	void DrawGrid( s32 text_off_x, s32 text_off_y );
	
	void ReloadGridTexture( GL_TEXTURE_TYPE_ENUM type,
							   int TextureID
							 );
	
	//--------------------------------------
	//	
	//--------------------------------------
	void InitStars();
	
	void UpdateStars();
	
	void DrawParts();
	
	void Fade( int Color1,    // Color to fade to 
		       int Color2,
			   int Color3,
			   int Color4,
			   s32 &Interp,
		       s32 Speed );
	
	void WarpScreen( int Frame, const glImage *sprite );
	void WarpScreenPolar( int Frame, const glImage *sprite );
	
private:
	Fx3D();
	~Fx3D();
	
	VertexBuffer* Ascaris;
	VertexBuffer* Grid;
	
	SuperShape Shapes[MAX_SHAPES];
	
	Vector3df32 Parts[MAX_PARTS];
	
};

#endif // FX3D_H
