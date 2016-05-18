/******************************************************************************
*******************************************************************************
	
	Space Impakto DS
	Relminator (Richard Eric M. Lope BSN RN)
	Http://Rel.Phatcode.Net
	
	Matrix 2D Class (Not OpenGL compliant)
	
	20.12 Fixed point
	Row-Major-Order
	3x3
	Only 2D
	Non-Stack based so mathematically correct but can't be used with GL
	Reversed Order of Operations than GL 
	
*******************************************************************************
******************************************************************************/ 

#include <nds.h>
#include "Vector2Df32.h"

#ifndef MATRIX2D_H
#define MATRIX2D_H

class Matrix2D {

public:
	Matrix2D()
	{
	}

	~Matrix2D()
	{
	}
	
	void LoadIndentity()
	{
		// [1,0,0]
		// [0,1,0]
		// [0,0,1]

		Elements[0][0] = 1 << 12;
		Elements[0][1] = 0;
		Elements[0][2] = 0;
		Elements[1][0] = 0;
		Elements[1][1] = 1 << 12;
		Elements[1][2] = 0;
		Elements[2][0] = 0;
		Elements[2][1] = 0;
		Elements[2][2] = 1 << 12;
		
	}

	void LoadTranslation( s32 tx, s32 ty )
	{
		// scale translate
		// [sx,0 ,tx]
		// [0 ,sy,ty]
		
		Elements[0][0] = 1 << 12;
		Elements[0][1] = 0;
		Elements[0][2] = tx;
		Elements[1][0] = 0;
		Elements[1][1] = 1 << 12;
		Elements[1][2] = ty;
		Elements[2][0] = 0;
		Elements[2][1] = 0;
		Elements[2][2] = 1 << 12;
		
			
	}
	
	void LoadScaling( s32 sx, s32 sy )
	{
		// scale translate
		// [sx,0 ,tx]
		// [0 ,sy,ty]
		
		Elements[0][0] = sx;
		Elements[0][1] = 0;
		Elements[0][2] = 0;
		Elements[1][0] = 0;
		Elements[1][1] = sy;
		Elements[1][2] = 0;
		Elements[2][0] = 0;
		Elements[2][1] = 0;
		Elements[2][2] = 1 << 12;
		
	}
	
	void LoadRotation( s32 angle )
	{
		// rotation
		// [ca,-sa,0]
		// [sa, ca,0]
		
		s32 sa = sinLerp(angle);
		s32 ca = cosLerp(angle);
		
		Elements[0][0] = ca;
		Elements[0][1] = -sa;
		Elements[0][2] = 0;
		Elements[1][0] = sa;
		Elements[1][1] = ca;
		Elements[1][2] = 0;
		Elements[2][0] = 0;
		Elements[2][1] = 0;
		Elements[2][2] = 1 << 12;
		
		
	}
	
	void Copy( const Matrix2D &Matrix )
	{
		for( int i = 0; i < 3; i++ )
		{
			for( int j = 0; j < 3; j++ )
			{
				Elements[i][j] = Matrix.Elements[i][j];
			}
		}
	}
	
	void Copy( const s32 Array[3][3] )
	{
		for( int i = 0; i < 3; i++ )
		{
			for( int j = 0; j < 3; j++ )
			{
				Elements[i][j] = Array[i][j];
			}
		}
	}
	
	void Multiply( const Matrix2D &Matrix )
	{
		// Combines 2 matrices this() and matrix()
		// ie. Result = matrix x this
		// Warning matrix multiplication is not commutative.
		// matrix x this != this x matrix
		
		s32 Result[3][3];
		for( int i = 0; i < 3; i++ )
		{
			for( int j = 0; j < 3; j++ )
			{
				Result[i][j] = 0;
				for( int k = 0; k < 3; k++ )
				{
					Result[i][j] = Result[i][j] + mulf32(Matrix.Elements[i][k], Elements[k][j]);  
				}
			}
		}
	
		Copy( Result );
		
	}
	
	
	Vector2df32 TransformPoint( const Vector2df32 &P )
	{
		Vector2df32 Out;
		Out.x = mulf32(P.x, Elements[0][0]) +  
			    mulf32(P.y, Elements[0][1]) + 
			    Elements[0][2];
	
		Out.y = mulf32(P.x, Elements[1][0]) +  
		        mulf32(P.y, Elements[1][1]) + 
			    Elements[1][2];
	
		return Out;
	}
	
	void Rotate( s32 angle )
	{
		Matrix2D M2;
		
		M2.LoadRotation( angle );
		
		this->Multiply( M2 );
		
	}
	
	void Translate( s32 tx, s32 ty )
	{
		Matrix2D M2;
		
		M2.LoadTranslation( tx, ty );
		
		this->Multiply( M2 );
		
	}
	
	void Scale( s32 sx, s32 sy )
	{
		Matrix2D M2;
		
		M2.LoadScaling( sx, sy );
		
		this->Multiply( M2 );
		
	}
	
private:

	s32 Elements[3][3];		// [x][y]
	
};

#endif // MATRIX2D_H
