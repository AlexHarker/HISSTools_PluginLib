

#ifndef __HISSTOOLS_ATOMIC__
#define __HISSTOOLS_ATOMIC__

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
typedef int32_t Atomic32; 
#else
#include <windows.h>
typedef volatile long Atomic32;
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Crossplatform Atomic Opernations //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HISSTools_Atomic32
{
	
private:
	
	Atomic32 mValue;
	
public:
	
	HISSTools_Atomic32()
	{
		mValue = 0;
	}
	
	bool atomicSwap(Atomic32 comparand, Atomic32 exchange)
	{
#ifdef __APPLE__
		return OSAtomicCompareAndSwap32Barrier(comparand, exchange, &mValue);
#else
		return (InterlockedCompareExchange (&mValue, exchange, comparand) == comparand);
#endif
	}
	
	Atomic32 atomicIncrementBarrier()
	{
#ifdef __APPLE__
		return OSAtomicIncrement32Barrier(&mValue);
#else
		return InterlockedIncrement(&mValue);
#endif 
	}
	
	Atomic32 atomicDecrementBarrier()
	{
#ifdef __APPLE__
		return OSAtomicDecrement32Barrier(&mValue);
#else
		return InterlockedDecrement(&mValue);
#endif
	}	
};

#endif	/* __HISSTOOLS_ATOMIC__ */