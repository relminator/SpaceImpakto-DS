/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	Memory Feedback class

	Original code by WinterMute

*******************************************************************************
******************************************************************************/

#include <nds.h> 
#include <unistd.h> 
#include <malloc.h> 

#ifndef MEMORYFEEDBACK_H
#define MEMORYFEEDBACK_H

extern u8 *fake_heap_end;   // current heap start 
extern u8 *fake_heap_start;   // current heap end 

class MemoryFeedback 
{

public:
	MemoryFeedback();
	~MemoryFeedback();

	int GetMemUsed() // returns the amount of used memory in bytes
	{  
	   mi = mallinfo(); 
	   return mi.uordblks; 
	} 

	int GetMemFree() // returns the amount of free memory in bytes
	{  
	   mi = mallinfo(); 
	   return mi.fordblks + (getHeapLimit() - getHeapEnd()); 
	}
	
private:
	
	struct mallinfo mi;
	
	u8* getHeapStart() 
	{ 
	   return fake_heap_start; 
	} 

	u8* getHeapEnd() 
	{ 
	   return (u8*)sbrk(0); 
	} 

	u8* getHeapLimit() 
	{ 
	   return fake_heap_end; 
	} 
	 
	
};

#endif // MEMORYFEEDBACK_H


