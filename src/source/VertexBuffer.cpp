/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	VertexBuffer class


*******************************************************************************
******************************************************************************/


#include "VertexBuffer.h"

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
{
    ips_Vertex = NULL;
    ips_Texture = NULL;
	ips_Color = NULL;
    ps_Poly = NULL;

    i_MaxPoly = 0;
    i_PrimitiveType = 0;
	i_TextureID = 0;
}

//--------------------------------------
//	
//--------------------------------------
VertexBuffer::~VertexBuffer()
{
	if (ips_Vertex) delete[] ips_Vertex;
    ips_Vertex = NULL;
    
    if (ps_Poly) delete[] ps_Poly;
    ps_Poly = NULL;

	
	if (ips_Texture) delete[] ips_Texture;
    ips_Texture = NULL;
	
	if (ips_Color) delete[] ips_Color;
    ips_Color = NULL;

    i_MaxPoly = 0;
    i_PrimitiveType = 0;
	i_TextureID = 0;

}

//--------------------------------------
//	
//--------------------------------------
void VertexBuffer::RescaleUV( s32 Uscale, s32 Vscale )
{
	
	for( int i = 0; i < i_MaxVertex; i++ )
	{
		ips_Texture[i].u = mulf32( ips_Texture[i].u, Uscale ); 
		ips_Texture[i].v = mulf32( ips_Texture[i].v, Vscale ); 
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void VertexBuffer::Draw( s32 TexU, s32 TexV, bool Colorize)
{
	
	glEnable( GL_TEXTURE_2D );
	glBindTexture( 0, i_TextureID );
	//glColorTable( TexType, ColorTable );
	
	if( Colorize )
	{
		glBegin( GL_TRIANGLES );
			//glNormal(NORMAL_PACK(0,inttov10(-1),0));
			for( int i = 0; i < (i_MaxPoly); i++ )
			{
				int i1 = ps_Poly[i].v1;
				int i2 = ps_Poly[i].v2;
				int i3 = ps_Poly[i].v3;
				
				glColor3b( ips_Color[i1].r, ips_Color[i1].g, ips_Color[i1].b );
				glTexCoord2f32( ips_Texture[i1].u + TexU, ips_Texture[i1].v + TexV );
				glVertex3v16 ( ips_Vertex[i1].x, ips_Vertex[i1].y,ips_Vertex[i1].z );
				
				glColor3b( ips_Color[i2].r, ips_Color[i2].g, ips_Color[i2].b );
				glTexCoord2f32( ips_Texture[i2].u + TexU, ips_Texture[i2].v + TexV );
				glVertex3v16( ips_Vertex[i2].x, ips_Vertex[i2].y, ips_Vertex[i2].z );

				glColor3b( ips_Color[i3].r, ips_Color[i3].g, ips_Color[i3].b );
				glTexCoord2f32( ips_Texture[i3].u + TexU, ips_Texture[i3].v + TexV );
				glVertex3v16( ips_Vertex[i3].x, ips_Vertex[i3].y, ips_Vertex[i3].z );
			}
		glEnd(); 
	}
	else
	{
		glBegin( GL_TRIANGLES );
			//glNormal(NORMAL_PACK(0,inttov10(-1),0));
			for( int i = 0; i < (i_MaxPoly); i++ )
			{
				int i1 = ps_Poly[i].v1;
				int i2 = ps_Poly[i].v2;
				int i3 = ps_Poly[i].v3;
				
				glTexCoord2f32( ips_Texture[i1].u + TexU, ips_Texture[i1].v + TexV );
				glVertex3v16 ( ips_Vertex[i1].x, ips_Vertex[i1].y,ips_Vertex[i1].z );
				
				glTexCoord2f32( ips_Texture[i2].u + TexU, ips_Texture[i2].v + TexV );
				glVertex3v16( ips_Vertex[i2].x, ips_Vertex[i2].y, ips_Vertex[i2].z );

				glTexCoord2f32( ips_Texture[i3].u + TexU, ips_Texture[i3].v + TexV );
				glVertex3v16( ips_Vertex[i3].x, ips_Vertex[i3].y, ips_Vertex[i3].z );
			}
		glEnd(); 
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
int VertexBuffer::LoadTexture( GL_TEXTURE_TYPE_ENUM type,
								int sizeX,
							    int sizeY,
							    int param,
							    const uint8 *texture,
								int	pallette_width,
								const u16 *palette				   
							  )
{
	
	glGenTextures(1, &i_TextureID);
	glBindTexture(0, i_TextureID);
	glTexImage2D( 0, 0, type, sizeX, sizeY, 0, param, texture );
	glColorTableEXT( 0, 0, pallette_width, 0, 0, palette );
	
	TexType = type;
	
	return i_TextureID;
	
}


//--------------------------------------
//	
//--------------------------------------
void VertexBuffer::CopyTexture( GL_TEXTURE_TYPE_ENUM type,
								int TextureID
							  )
{
	
	i_TextureID = TextureID;
	TexType = type;
	
}

	
//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------


//--------------------------------------
//	
//--------------------------------------
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
							)
{
	/*
	'	a1 = abs(cos(m*theta/4)/a)^n2
    '	a2 = abs(sin(m*theta/4)/a)^n3
    '	r(theta) = (a1+a2)^(-1/n1)
    '
    '	x = r*(theta)  * cos(theta) * r*(phi)* cos(phi)
    '	y = r*(theta)  * sin(theta) * r*(phi)* cos(phi)
    '	z = r*(phi)  * sin(phi)
    '
    '	-pi/2 <= phi <= pi/2
    '	-pi <= theta <= pi
	*/
	
	
    VertexBuffer *vb = new VertexBuffer;

    int max_point = (rings) * (bands+1);      //+1 for last band duplicate
   	Vector3df32 *ivertex = new Vector3df32[max_point]; 
    Texcoordf32 *itexture = new Texcoordf32[max_point];  //
	Rgbf32 *icolor = new Rgbf32[max_point]; 
    Polygon  *poly = new Polygon[max_point * 2];  //
    
    vb->ips_Vertex = ivertex;
	vb->ips_Texture =  itexture;
	vb->ips_Color =  icolor;
    vb->ps_Poly = poly;
    
    vb->i_MaxVertex = max_point;
    vb->i_MaxPoly = max_point * 2;
    vb->i_PrimitiveType = GL_TRIANGLES;
    
	
	//lathing
	int s, u, slice, maxvert;
	maxvert = max_point;
    int i = 0;
	for (s = 0; s < rings; s++)
	{
 		 slice = s * bands;
		 for (u = 0; u <= bands; u++)	  //duplicate texture ( not bands - 1)
		 {
			 poly[i].v1=(u+bands+1+(slice)) % maxvert;
			 poly[i].v2=(u+bands+(slice)) % maxvert;
			 poly[i].v3=(u+(slice)) % maxvert;
			 poly[i+1].v1=(u+(slice)) % maxvert;
			 poly[i+1].v2=(u+1+(slice)) % maxvert;
			 poly[i+1].v3=(u+bands+1+(slice)) % maxvert;
			 i += 2;
		 }
	}
				
	
	
	float phi, theta;
	float r1, r2, a1, a2;
	
	float Tpi_d = (M_PI*2)/bands;
	float Ppi_d = M_PI/rings;
	
    phi = -M_PI/2;
	
	int k = 0;
	
	for (int i = 0; i < rings; i++)
	{
		
		a1 = pow(fabs(cos(m*phi/4)/a),n2);
        a2 = pow(fabs(sin(m*phi/4)/b),n3);
        r2 = pow(a1+a2,-1/n1) ;
        r2 *= radius;
        
        phi += Ppi_d;
        
        theta = -M_PI;
		
		for (int j = 0; j <= bands; j++)
		{

			a1 = pow(fabs(cos(m*theta/4)/a),n2);
			a2 = pow(fabs(sin(m*theta/4)/b),n3);
			r1 = pow(a1+a2,-1/n1) ;
			r1 *= radius;
			
			
			float x = r1  * cos(theta) * r2* cos(phi);
			float y = r1  * sin(theta) * r2* cos(phi);
			float z = r2  * sin(phi);
			
			theta += Tpi_d; 
			
			
						
			ivertex[k].x = floattov16 (x);
			ivertex[k].y = floattov16 (y);
			ivertex[k].z = floattov16 (z);
			
			
			           
            icolor[k].r = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].g = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].b = (1.0f - (i*4 / (float)rings)) * 255;

			float u = ((float)i / rings ) * uscale;
			float v = ((float)j / bands ) * vscale;
			
			
			itexture[k].u = floattof32(u);
			itexture[k].v = floattof32(v);
			
			k++;
			
		}
	}
	
	return vb;
	
}


//--------------------------------------
//	
//--------------------------------------
VertexBuffer *InitAscaris( int rings,    	  	  // rings
						   int bands,              // bands
						   float radius,			  // radius of each ring from center
						   float center_offset,	  // swirl offset
						   int uscale,
						   int vscale
						 )
{

	const float TWOPI = M_PI *2;
	
	VertexBuffer *vb = new VertexBuffer;

    int max_point = (rings) * (bands + 1);      //+1 for last band duplicate
   	Vector3df32 *ivertex = new Vector3df32[max_point]; 
    Texcoordf32 *itexture = new Texcoordf32[max_point];  //
	Rgbf32 *icolor = new Rgbf32[max_point]; 
    Polygon  *poly = new Polygon[max_point * 2];  //
    
    vb->ips_Vertex = ivertex;
	vb->ips_Texture =  itexture;
	vb->ips_Color =  icolor;
    vb->ps_Poly = poly;
    
    vb->i_MaxVertex = max_point;
    vb->i_MaxPoly = max_point * 2;
    vb->i_PrimitiveType = GL_TRIANGLES;
    
	
	//lathing
	int s, u, slice, maxvert;
	maxvert = max_point;
    int i = 0;
	for (s = 0; s < (rings); s++)
	{
		 slice = s * bands;
		 for (u = 0; u < bands; u++)	  //duplicate texture ( not bands - 1)
		 {
			 poly[i].v1=(u+bands+1+(slice)) % maxvert;
			 poly[i].v2=(u+bands+(slice)) % maxvert;
			 poly[i].v3=(u+(slice)) % maxvert;
			 poly[i+1].v1=(u+(slice)) % maxvert;
			 poly[i+1].v2=(u+1+(slice)) % maxvert;
			 poly[i+1].v3=(u+bands+1+(slice)) % maxvert;
			 i += 2;
		 }
	}


	int k = 0;
	float xc, yc, zc;
	for (int i = 0 ; i < rings; i++)
	{
		zc = i - (i/20);
	    xc = cos(TWOPI * zc / rings)* center_offset;
      	yc = sin(TWOPI * zc / rings)* center_offset;
		for (int j = 0; j <= bands; j++)
		{

			float x = xc + cos(j * TWOPI / bands ) * radius;
			float y = yc + sin(j * TWOPI / bands ) * radius;
			float z = 0-(i*2);
						
			ivertex[k].x = floattov16 (x);
			ivertex[k].y = floattov16 (y);
			ivertex[k].z = floattov16 (z);
			
			
			           
            icolor[k].r = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].g = (1.0f - (i*4 / (float)rings)) * 255;
            icolor[k].b = (1.0f - (i*4 / (float)rings)) * 255;

			float u = ((float)i / rings ) * uscale;
			float v = ((float)j / bands ) * vscale;
			
			
			itexture[k].u = floattof32(u);
			itexture[k].v = floattof32(v);
			
			k++;
		}
	}
	
	return vb;	
	
}



//--------------------------------------
//	
//--------------------------------------
VertexBuffer *InitGrid( int rings,    
						int bands,              
			  			float width,  
			  			float height,
						int uscale,
						int vscale)
{

    VertexBuffer *vb = new VertexBuffer;

    int max_point = (rings) * (bands);      //+1 for last band duplicate
   	Vector3df32 *ivertex = new Vector3df32[max_point]; 
    Texcoordf32 *itexture = new Texcoordf32[max_point];  //
	Rgbf32 *icolor = new Rgbf32[max_point]; 
    Polygon  *poly = new Polygon[max_point * 2];  //
    
    vb->ips_Vertex = ivertex;
	vb->ips_Texture =  itexture;
	vb->ips_Color =  icolor;
    vb->ps_Poly = poly;
    
    vb->i_MaxVertex = max_point;
    vb->i_MaxPoly = max_point * 2;
    vb->i_PrimitiveType = GL_TRIANGLES;
    
	
	//lathing
	int s, u, slice, maxvert;
	maxvert = max_point;
    int i = 0;
	for (s = 0; s < rings; s++)
	{
 		 slice = s * bands;
		 for (u = 0; u < bands; u++)	  //duplicate texture ( not bands - 1)
		 {
			 poly[i].v1=(u+bands+1+(slice)) % maxvert;
			 poly[i].v2=(u+bands+(slice)) % maxvert;
			 poly[i].v3=(u+(slice)) % maxvert;
			 poly[i+1].v1=(u+(slice)) % maxvert;
			 poly[i+1].v2=(u+1+(slice)) % maxvert;
			 poly[i+1].v3=(u+bands+1+(slice)) % maxvert;
			 i += 2;
		 }
	}
				

	float half_width = width/2;
	float half_height = height/2;
	
	
	float a1 = 2*width/rings;
	float a2 = 2*height/bands;
	int k = 0;
	
			
	for (int i = 0; i < rings; i++)
	{
		for (int j = 0; j < bands; j++)
		{

			
			
			float x = -half_width + (i * a1);
			float y = 0;
			float z = -half_height + (j * a2);;
						
			ivertex[k].x = floattov16 (x);
			ivertex[k].y = floattov16 (y);
			ivertex[k].z = floattov16 (z);
			
			
			           
            icolor[k].r = (1.0f - (j*4 / (float)bands)) * 255;
            icolor[k].g = (1.0f - (j*4 / (float)bands)) * 255;
            icolor[k].b = (1.0f - (j*4 / (float)bands)) * 255;

			float u = ((float)i / rings ) * uscale;
			float v = ((float)j / bands ) * vscale;
			
			
			itexture[k].u = floattof32(u);
			itexture[k].v = floattof32(v);
			
			k++;
			
		}
	}
	
	return vb;

}
