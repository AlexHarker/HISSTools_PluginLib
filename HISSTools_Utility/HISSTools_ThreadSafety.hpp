

#ifndef __HISSTOOLS_THREADSAFETY__
#define __HISSTOOLS_THREADSAFETY__

#include "HISSTools_Pointers.hpp"

#include <atomic>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// Lightweight Spinlock ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HISSTools_SpinLock
{
	
private:
	
    std::atomic_flag mLock = ATOMIC_FLAG_INIT;
	
public:
	
	HISSTools_SpinLock()
	{
	}
	
    // Non-copyable
    
    HISSTools_SpinLock(const HISSTools_SpinLock&) = delete;
    HISSTools_SpinLock& operator=(const HISSTools_SpinLock&) = delete;
    
	void acquire()
	{
		while(!attempt());
	}
	
	bool attempt()
	{
		return !mLock.test_and_set();
	}
	
	void release()
	{
        mLock.clear();
	}
};


template <class T>
class HISSTools_ThreadSafeMemory
{
	
public:
	class Ptr : public HISSTools_RefPtr <T>
	{
	
		// FIX - deletion scheduling

	public:
		
		Ptr() : HISSTools_RefPtr <T> ()
		{
		}
		
		Ptr(const Ptr &rhs) : HISSTools_RefPtr <T> (rhs)
		{
		}

		Ptr(const Ptr *rhs) : HISSTools_RefPtr <T> (rhs)
		{
		}
		
		Ptr(const Ptr *rhs, unsigned long requiredSize) : HISSTools_RefPtr <T> (rhs, requiredSize)
		{
		}
		
		Ptr(unsigned long requiredSize) : HISSTools_RefPtr <T> (requiredSize)
		{
		}
	};
	
private:
	
	// Atomic Spinlocks
	
	HISSTools_SpinLock mCurrentLock;
	HISSTools_SpinLock mResizeLock;
	
	// Pointers to Memory Blocks
	
	Ptr *mCurrentMemoryBlock;
	
private:
		
	Ptr *getCurrentBlockPtr()
	{
		mCurrentLock.acquire();
		Ptr *current = mCurrentMemoryBlock;
		mCurrentLock.release();
		
		return current;
	}

public:
	
	HISSTools_ThreadSafeMemory(unsigned long startingSize)
	{
		mCurrentMemoryBlock = NULL;
		resizeMemory(startingSize, FALSE);
	}
	
	HISSTools_ThreadSafeMemory()
	{
		mCurrentMemoryBlock = NULL;
		resizeMemory(0, FALSE);
	}
	
	~HISSTools_ThreadSafeMemory()
	{		
        mResizeLock.acquire();
        mCurrentLock.acquire();
		delete mCurrentMemoryBlock;
		mCurrentMemoryBlock = NULL;
		mCurrentLock.release();
        mResizeLock.release();
	}
	
	Ptr accessMemory(unsigned long requiredSize)
	{
		return Ptr(getCurrentBlockPtr(), requiredSize);
	}
	
	Ptr accessMemory()
	{
		return Ptr(getCurrentBlockPtr());
	}
	
	Ptr resizeMemory(unsigned long requiredSize, bool acquire)
	{
		Ptr *newBlockPtr;
		Ptr *oldBlockPtr;
		
		mResizeLock.acquire();
				
		if (mCurrentMemoryBlock == NULL || (mCurrentMemoryBlock->getSize() != requiredSize))
		{
			newBlockPtr = new Ptr(requiredSize);
			oldBlockPtr = mCurrentMemoryBlock;
					
			mCurrentLock.acquire();
			mCurrentMemoryBlock = newBlockPtr;
			mCurrentLock.release();

			delete oldBlockPtr;
		}
		else 
			newBlockPtr = mCurrentMemoryBlock;

		mResizeLock.release();
		
		return (acquire == TRUE) ? Ptr(newBlockPtr) : Ptr();	
	}
};

#endif	/* __HISSTOOLS_THREADSAFETY__ */
