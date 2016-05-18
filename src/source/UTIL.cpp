#include "UTIL.h"

namespace UTIL
{

int Index = 0;
int Randy[RANDOM_MAX];


void InitRND( int seed )
{
	srand(seed);
	Index = 0;
	for( int i = 0; i < RANDOM_MAX; i++ )
	{
		Randy[i] = rand() & 32767;
	}
}

void ResetRND()
{
	Index = 0;
}


}  // namespace