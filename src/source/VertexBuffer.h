/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	VertexBuffer class


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <math.h>
#include <gl2d.h>

#include "Vector2df32.h"

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

//--------------------------------------
//	
//--------------------------------------
class Vector3df32
{
public:
	s32 x;
	s32 y;
	s32 z;
};

//--------------------------------------
//	
//--------------------------------------
class Rgbf32
{
public:
	s32 r;
	s32 g;
	s32 b;
};

//--------------------------------------
//	
//--------------------------------------
class Texcoordf32
{
public:
	s32 u;
	s32 v;
};

//--------------------------------------
//	
//--------------------------------------
class Polygon
{
public:
	u32 v1;
	u32 v2;
	u32 v3;

};


//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
class VertexBuffer
{
public:

friend class SuperShape;

friend
VertexBuffer *InitSuperShape( int rings,    	  	  // rings
							  int bands,            // bands
							  float radius,		  // radius 
							  int uscale,
							  int vscale,
							  float a,
							  float b,
							  float m,
							  float n1,
							  float n2,
							  float n3
							);


friend
VertexBuffer *InitAscaris( int rings,    	  	  // rings
						   int bands,              // bands
						   float radius,			  // radius of each ring from center
						   float center_offset,	  // swirl offset
						   int uscale,
						   int vscale
						 );

friend
VertexBuffer *InitGrid( int rings,    
						int bands,              
			  			float width,  
			  			float height,
						int uscale,
						int vscale);

	VertexBuffer();
	~VertexBuffer();

	void RescaleUV( s32 Uscale, s32 Vscale );
	void Draw( s32 TexU, s32 TexV, bool Colorize);
	int LoadTexture( GL_TEXTURE_TYPE_ENUM type,
					  int sizeX,
					  int sizeY,
					  int param,
					  const uint8 *texture,
					  int	pallette_width,
					  const u16 *palette	
				    );
	
	void CopyTexture( GL_TEXTURE_TYPE_ENUM type,
					  int TextureID
					);
	
	int GetTextureID() const
	{
		return i_TextureID;
	}
	
	int GetPalette() const
	{
		return ColorTable;
	}
	
	GL_TEXTURE_TYPE_ENUM GetTextureType() const
	{
		return TexType;
	}
	
	
protected:	
	int i_MaxPoly;
	int i_MaxVertex;
	int i_PrimitiveType;
	int i_TextureID;
	
	GL_TEXTURE_TYPE_ENUM TexType;
	int ColorTable;
	
	Vector3df32 *ips_Vertex;
	Texcoordf32 *ips_Texture;
	Rgbf32 *ips_Color;
	Polygon *ps_Poly;
};

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------

VertexBuffer *InitSuperShape( int rings,    	  	  // rings
							  int bands,            // bands
							  float radius,		  // radius 
							  int uscale,
							  int vscale,
							  float a,
							  float b,
							  float m,
							  float n1,
							  float n2,
							  float n3
							);



VertexBuffer *InitAscaris( int rings,    	  	  // rings
						   int bands,              // bands
						   float radius,			  // radius of each ring from center
						   float center_offset,	  // swirl offset
						   int uscale,
						   int vscale
						 );

VertexBuffer *InitGrid( int rings,    
						int bands,              
			  			float width,  
			  			float height,
						int uscale,
						int vscale);


#endif // VERTEXBUFFER_H
