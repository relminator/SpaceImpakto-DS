/******************************************************************************
*******************************************************************************

	UTIL.h
	relminator
	http://rel.betterwebber.com


	Misc Utilities


*******************************************************************************
******************************************************************************/

#include <nds.h>

#ifndef UTIL_H
#define UTIL_H

namespace UTIL
{
	
//--------------------------------------
//	
//--------------------------------------
#define RGB15_R(u) ( ( (u) >> 0  ) & 0x001F )
#define RGB15_G(u) ( ( (u) >> 5  ) & 0x001F )
#define RGB15_B(u) ( ( (u) >> 10 ) & 0x001F )


#define BRAD_PI ( 1 << 14 )
#define BRAD_2PI ( BRAD_PI * 2 )
#define BRAD_ONE 0x1000


#define SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

#define MAX(a,b) ((a)<(b)) ? (b) : (a)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)


#define RANDOM_MAX 1024

//extern int Randy[RANDOM_MAX];
//extern int Index;

extern int Index;
extern int Randy[RANDOM_MAX];

//--------------------------------------
//  Random Stuff	
//--------------------------------------
void InitRND( int seed );
void ResetRND();

inline int RND( int i )
{
	return Randy[i & 1023];
}

inline int RND()
{
	Index = (Index + 1) & 1023;
	return Randy[Index];
}

inline int RND_RANGE( int min, int max )
{
	Index = (Index + 1) & 1023;
	return (( Randy[Index] * (max - min) >> 15 ) + min);
}


//--------------------------------------
//	
//--------------------------------------
static inline s32 Lerpf32( s32 start, s32 end, s32 t )
{
	
	s32 d = ( end - start );
	return ( start + mulf32( d,t ) );
	
}


//--------------------------------------
//	
//--------------------------------------
static inline int Clamp( int in, int mins, int maxs )
{
	if( in < mins )
		in = mins;
	if( in > maxs )
		in = maxs;
	return in;
}


//--------------------------------------
//	
//--------------------------------------
static inline int Wrap( int in, int maxs )
{
	if( in > maxs )
		in -= maxs ;
	if( in < 0 )
		in += maxs;
	return in;
}

//--------------------------------------
//	
//--------------------------------------
static inline int Wrap( int in, int mins, int maxs )
{
	if( in > maxs )
		in = (in - maxs) + mins ;
	if( in < 0 )
		in = (in + maxs) - mins ;
	return in;
}



//--------------------------------------
//	
//--------------------------------------
static inline int Min( int a, int b )
{
	 return (a < b) ? a : b;
}


//--------------------------------------
//	
//--------------------------------------
static inline int Max( int a, int b )
{
	 return (a > b) ? a : b;
}


//--------------------------------------
//	
//--------------------------------------
static inline int Sign( int a )
{
	 return -(a < 0);
}


//--------------------------------------
//	BIT OPS
//--------------------------------------
typedef unsigned char ubyte;

static inline bool BitIsSet( ubyte t, ubyte flag )
{
	if(t & flag)
		return true;
	return false;
}


static inline void BitSet( ubyte& a, ubyte flags )
{
	a |= flags;
}

static inline void BitClear( ubyte& a, ubyte flags )
{
	a &= ~flags;
}

static inline void BitClearAll( ubyte& a )
{
	a=(ubyte)0;
}

static inline void BitToggleAll( ubyte& a, ubyte flags )
{
	a^=flags;	
}

static inline bool BitIsOffAlL( ubyte a )
{
	if( a == ubyte(0) )
		return true;
	return false;
}

static inline bool BitIsOnAll( ubyte a )
{
	if(a == ~ubyte(0) )
		return true;
	return false;
}




}  // end namespace

#endif // UTIL_H
