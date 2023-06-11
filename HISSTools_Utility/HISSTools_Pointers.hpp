

#ifndef __HISSTOOLS_POINTERS__
#define __HISSTOOLS_POINTERS__

#include <atomic>
#include <cstdint>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// A basic reference counted pointer /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class HISSTools_RefPtr
{
private:
	
	class MemoryBlock
	{
	private:
		
		T *mMemory;
		unsigned long mSize;
		std::atomic<int32_t> mRefCount;
		
	public:

        // FIX - This seems dangerous to allow this...
        
        /*
        MemoryBlock(T *memory, unsigned long size)
		{
			mMemory = memory;
            mSize = size;
        }

		MemoryBlock(T *memory)
		{
			mMemory = memory;
			mSize = 1;
		}
		*/
		MemoryBlock(unsigned long size)
        : mRefCount(0)
		{
			mMemory = new T[size];
			mSize = size;
		}
		
        ~MemoryBlock()
        {
            delete[] mMemory;
        }
        
		MemoryBlock *acquire()
		{
			incrementRefCount();
			return this;
		}
		
		MemoryBlock *acquire(unsigned long requiredSize)
		{
			if (requiredSize == mSize) 
			{ 
				incrementRefCount();
				return this;
			}	
			return NULL;
		}
		
		int32_t release()
		{
			return decrementRefCount();
		}
		
		T *getMemory()
		{
			return mMemory;
		}
		
		unsigned long getSize()
		{
			return mMemory != NULL ? mSize : 0;
		}
		
	private:
		
        int32_t incrementRefCount()
		{
			return ++mRefCount;
		}
		
        int32_t decrementRefCount()
		{
			return --mRefCount;
		}
	};
	
private:
		
	MemoryBlock *mBlock;
		
	void init(MemoryBlock *block)
	{
		mBlock = block;
		if (block != NULL)
			block->acquire();
	}
		
	void releaseBlock()
	{
		if (mBlock != NULL && mBlock->release() < 1)
			delete mBlock;
		
		// FIX - schedule deletion
        // FIX - memory is deleted correctly?
	}
		
public:
	
	HISSTools_RefPtr()
	{
		mBlock = NULL;
	}
    
    
    // FIX - This seems dangerous to allow this...
    
	/*
    HISSTools_RefPtr(T *newObject, unsigned long size)
	{
		init(new MemoryBlock(newObject, size));
	}
 
	HISSTools_RefPtr(T *newObject)
	{
		init(new MemoryBlock(newObject));
	}
	*/
    
	HISSTools_RefPtr(unsigned long size)
	{
		init(new MemoryBlock(size));
	}
	
	HISSTools_RefPtr(const HISSTools_RefPtr &rhs)
	{
		init(rhs.mBlock);
	}
	
	HISSTools_RefPtr(const HISSTools_RefPtr *rhs)
	{
		init(rhs->mBlock);
	}
	
	HISSTools_RefPtr(const HISSTools_RefPtr *rhs, unsigned long requiredSize)
	{
		mBlock = rhs->mBlock->acquire(requiredSize);
	}
		
	~HISSTools_RefPtr()
	{
		releaseBlock();
	}
		
	unsigned long getSize()
	{
		return (mBlock != NULL) ? mBlock->getSize() : 0;
	}
	
	HISSTools_RefPtr &operator =(const HISSTools_RefPtr &rhs)  
	{
		releaseBlock();
		init(rhs.mBlock);
		
		return *this;
	}
	
	T &operator *()  
	{
		return *mBlock->getMemory();
	}
	
	T *operator ->() 
	{
		return mBlock->getMemory();
	}
	
	T &operator [](const unsigned long index) 
	{
		return mBlock->getMemory()[index];
	}
	
	T *get()
	{
		return mBlock->getMemory();
	}
	
	T *operator +(unsigned int offset) 
	{
		return mBlock->getMemory() + offset;
	}
	
	bool operator ==(const HISSTools_RefPtr &rhs)	
	{	
		return (rhs.mBlock == mBlock) ? TRUE : FALSE; 
	}
	
	bool operator !=(const HISSTools_RefPtr &rhs)	
	{	
		return (*this == rhs) == TRUE ? FALSE : TRUE; 
	}
	
	bool operator ==(const T *rhs)
	{	
		return (rhs == ((mBlock != NULL) ? mBlock->getMemory() : NULL)) ? TRUE : FALSE;
	}
	
	bool operator !=(const T *rhs)
	{	
		return (*this == rhs) == TRUE ? FALSE : TRUE;
	}
};

#endif	/* __HISSTOOLS_POINTERS__ */
