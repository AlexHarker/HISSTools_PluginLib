
/**
 * @file HISSTools_Pointers.hpp
 * @brief Provides a reference-counted pointer implementation for managing memory blocks.
 *
 * This file contains the definition of the HISSTools_RefPtr template class, which
 * implements a basic reference-counted pointer to manage dynamically allocated memory
 * blocks. It includes the MemoryBlock class, which handles memory allocation,
 * deallocation, and reference counting. The reference counting ensures that memory
 * is automatically deallocated when no references remain, preventing memory leaks.
 *
 * The HISSTools_RefPtr class provides several operators for memory access, including
 * dereference (`*`), pointer access (`->`), subscript (`[]`), and pointer arithmetic.
 * It also includes comparison operators for comparing with other reference pointers
 * and raw pointers.
 *
 * The MemoryBlock class is responsible for managing the internal memory block,
 * handling reference counting to ensure that memory is shared and deallocated safely.
 */

#ifndef __HISSTOOLS_POINTERS__
#define __HISSTOOLS_POINTERS__

#include <atomic>
#include <cstdint>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// A basic reference counted pointer /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief A reference-counted pointer template class.
 *
 * This class implements a basic reference-counting mechanism to manage
 * the lifecycle of dynamically allocated objects. It provides automatic
 * memory management by keeping track of how many references point to the
 * same object, releasing the memory when there are no more references.
 *
 * @tparam T The type of the object to manage with reference counting.
 */

template <class T>
class HISSTools_RefPtr
{
private:
	
    /**
     * @brief A helper class that manages a block of memory with reference counting.
     *
     * The MemoryBlock class handles memory allocation, deallocation, and
     * reference counting for a block of memory. It ensures that memory
     * is properly managed and freed when no longer in use. The class
     * provides functions to acquire and release the memory block,
     * keeping track of the number of active references.
     */
    
	class MemoryBlock
	{
	private:
		
        /**
         * @brief A pointer to the allocated memory block.
         *
         * This pointer holds the address of the dynamically allocated memory
         * block. The size of the memory block is defined by `mSize`, and
         * memory is managed by the MemoryBlock class.
         */
        
		T *mMemory;
        
        /**
         * @brief The size of the allocated memory block.
         *
         * This variable stores the number of elements in the memory block
         * pointed to by `mMemory`. It represents the capacity of the allocated memory.
         */
        
		unsigned long mSize;
        
        /**
         * @brief The atomic reference count for the memory block.
         *
         * This variable keeps track of how many references (or owners) are
         * pointing to the memory block. The use of an atomic type ensures
         * that increments and decrements to the reference count are thread-safe.
         */
        
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
        
        /**
         * @brief Constructs a MemoryBlock of the specified size.
         *
         * This constructor allocates a block of memory of the given size,
         * and initializes the reference count to zero.
         *
         * @param size The number of elements to allocate in the memory block.
         */
        
		MemoryBlock(unsigned long size)
        : mRefCount(0)
		{
			mMemory = new T[size];
			mSize = size;
		}
		
        /**
         * @brief Destructor for the MemoryBlock class.
         *
         * The destructor releases the allocated memory when the MemoryBlock
         * object is destroyed. It ensures that the dynamically allocated memory
         * pointed to by `mMemory` is properly deallocated to prevent memory leaks.
         */
        
        ~MemoryBlock()
        {
            delete[] mMemory;
        }
        
        /**
         * @brief Increments the reference count and returns the current MemoryBlock.
         *
         * This method increases the reference count of the memory block by
         * one and returns a pointer to the current MemoryBlock. It is used
         * to indicate that another reference has acquired ownership of the memory.
         *
         * @return A pointer to the current MemoryBlock with the incremented reference count.
         */
        
		MemoryBlock *acquire()
		{
			incrementRefCount();
			return this;
		}
		
        /**
         * @brief Acquires the memory block if the required size matches the current block's size.
         *
         * This method checks if the requested memory size matches the size of the current memory block.
         * If the sizes match, the reference count is incremented and the current MemoryBlock is returned.
         * Otherwise, the method returns `NULL`.
         *
         * @param requiredSize The size of the memory block being requested.
         * @return A pointer to the current MemoryBlock if the size matches, or `NULL` if it does not.
         */
        
		MemoryBlock *acquire(unsigned long requiredSize)
		{
			if (requiredSize == mSize) 
			{ 
				incrementRefCount();
				return this;
			}	
			return NULL;
		}
		
        /**
         * @brief Decrements the reference count and returns the updated count.
         *
         * This method decreases the reference count of the memory block by one.
         * It is used to release ownership of the memory block. The updated
         * reference count is returned, which can be used to determine if the
         * memory block should be deallocated.
         *
         * @return The updated reference count after decrementing.
         */
        
		int32_t release()
		{
			return decrementRefCount();
		}
		
        /**
         * @brief Retrieves the pointer to the allocated memory block.
         *
         * This method returns a pointer to the memory block managed by the
         * MemoryBlock class. It allows access to the memory for reading
         * or modifying its contents.
         *
         * @return A pointer to the allocated memory block.
         */
        
		T *getMemory()
		{
			return mMemory;
		}
		
        /**
         * @brief Retrieves the size of the allocated memory block.
         *
         * This method returns the number of elements in the memory block.
         * If no memory is allocated, it returns 0. It provides information
         * about the capacity of the memory block managed by the MemoryBlock class.
         *
         * @return The size of the allocated memory block, or 0 if no memory is allocated.
         */
        
		unsigned long getSize()
		{
			return mMemory != NULL ? mSize : 0;
		}
		
	private:
		
        /**
         * @brief Increments the reference count for the memory block.
         *
         * This method increases the reference count by one, indicating that
         * an additional reference now owns the memory block. It ensures
         * that the memory block remains allocated as long as there are active references.
         *
         * @return The updated reference count after incrementing.
         */
        
        int32_t incrementRefCount()
		{
			return ++mRefCount;
		}
		
        /**
         * @brief Decrements the reference count for the memory block.
         *
         * This method decreases the reference count by one, indicating that
         * a reference has released ownership of the memory block. If the reference
         * count reaches zero, it suggests that the memory block is no longer in use
         * and may be deallocated by the managing code.
         *
         * @return The updated reference count after decrementing.
         */
        
        int32_t decrementRefCount()
		{
			return --mRefCount;
		}
	};
	
private:
    
    /**
     * @brief A pointer to the associated MemoryBlock.
     *
     * This pointer holds the address of the MemoryBlock instance that
     * manages the memory and reference counting for the object. It
     * is responsible for ensuring proper memory allocation and deallocation.
     */
    
	MemoryBlock *mBlock;
		
    /**
     * @brief Initializes the current object with the given MemoryBlock.
     *
     * This method sets the current object's memory management to the provided
     * `MemoryBlock`. It increments the reference count of the provided block
     * to indicate that the current object now shares ownership of the memory.
     *
     * @param block A pointer to the MemoryBlock to be associated with the current object.
     */
    
	void init(MemoryBlock *block)
	{
		mBlock = block;
		if (block != NULL)
			block->acquire();
	}
	
    /**
     * @brief Releases the current MemoryBlock and manages its reference count.
     *
     * This method decreases the reference count of the associated MemoryBlock.
     * If the reference count reaches zero, it deallocates the memory.
     * This ensures that the memory is properly freed when no longer in use.
     */
    
	void releaseBlock()
	{
		if (mBlock != NULL && mBlock->release() < 1)
			delete mBlock;
		
		// FIX - schedule deletion
        // FIX - memory is deleted correctly?
	}
		
public:
	
    /**
     * @brief Default constructor for the HISSTools_RefPtr class.
     *
     * This constructor initializes an empty HISSTools_RefPtr object with
     * no associated MemoryBlock. It sets the internal memory block pointer
     * to `nullptr` and prepares the object for future memory management.
     */
    
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
    
    /**
     * @brief Constructs a HISSTools_RefPtr object with a new memory block of the specified size.
     *
     * This constructor allocates a new MemoryBlock of the given size and associates
     * it with the HISSTools_RefPtr object. The memory block will be reference-counted
     * and managed automatically.
     *
     * @param size The size of the memory block to allocate.
     */
    
	HISSTools_RefPtr(unsigned long size)
	{
		init(new MemoryBlock(size));
	}
	
    /**
     * @brief Copy constructor for the HISSTools_RefPtr class.
     *
     * This constructor creates a new HISSTools_RefPtr object by copying the reference
     * from another HISSTools_RefPtr object. It increments the reference count of the
     * shared MemoryBlock to manage the memory correctly.
     *
     * @param rhs The HISSTools_RefPtr object to copy from.
     */
    
	HISSTools_RefPtr(const HISSTools_RefPtr &rhs)
	{
		init(rhs.mBlock);
	}
	
    /**
     * @brief Pointer-based copy constructor for the HISSTools_RefPtr class.
     *
     * This constructor creates a new HISSTools_RefPtr object by copying the reference
     * from a pointer to another HISSTools_RefPtr object. It increments the reference
     * count of the shared MemoryBlock to manage the memory correctly.
     *
     * @param rhs A pointer to the HISSTools_RefPtr object to copy from.
     */
    
	HISSTools_RefPtr(const HISSTools_RefPtr *rhs)
	{
		init(rhs->mBlock);
	}
	
    /**
     * @brief Constructs a HISSTools_RefPtr object by copying from a pointer to another object, with size validation.
     *
     * This constructor initializes a HISSTools_RefPtr object by copying the reference
     * from a pointer to another HISSTools_RefPtr object, but only if the memory block
     * size matches the specified `requiredSize`. If the sizes do not match, the new object
     * is not associated with the memory block.
     *
     * @param rhs A pointer to the HISSTools_RefPtr object to copy from.
     * @param requiredSize The required size of the memory block for the copy to succeed.
     */
    
	HISSTools_RefPtr(const HISSTools_RefPtr *rhs, unsigned long requiredSize)
	{
		mBlock = rhs->mBlock->acquire(requiredSize);
	}
    
    /**
     * @brief Destructor for the HISSTools_RefPtr class.
     *
     * This destructor releases the associated MemoryBlock by decrementing
     * its reference count. If the reference count reaches zero, the memory
     * block is deallocated. This ensures proper memory management when
     * the HISSTools_RefPtr object is destroyed.
     */
    
	~HISSTools_RefPtr()
	{
		releaseBlock();
	}
		
    /**
     * @brief Returns the size of the memory block managed by the HISSTools_RefPtr.
     *
     * This method retrieves the number of elements in the associated memory block.
     * If there is no associated memory block, it returns 0. It provides information
     * about the capacity of the currently managed memory.
     *
     * @return The size of the memory block, or 0 if no block is associated.
     */
    
	unsigned long getSize()
	{
		return (mBlock != NULL) ? mBlock->getSize() : 0;
	}
	
    /**
     * @brief Assignment operator for the HISSTools_RefPtr class.
     *
     * This operator assigns the current HISSTools_RefPtr object to reference
     * the same MemoryBlock as the right-hand side (rhs) object. It increments
     * the reference count of the new MemoryBlock and decrements the reference
     * count of the previously managed MemoryBlock. If the old block's reference
     * count reaches zero, it is deallocated.
     *
     * @param rhs The HISSTools_RefPtr object to copy from.
     * @return A reference to the updated HISSTools_RefPtr object.
     */
    
	HISSTools_RefPtr &operator =(const HISSTools_RefPtr &rhs)
	{
		releaseBlock();
		init(rhs.mBlock);
		
		return *this;
	}
	
    /**
     * @brief Dereference operator for accessing the object managed by the HISSTools_RefPtr.
     *
     * This operator allows direct access to the object (or the first element in the memory block)
     * managed by the HISSTools_RefPtr. It returns a reference to the object, enabling its use
     * like a regular pointer.
     *
     * @return A reference to the object managed by the HISSTools_RefPtr.
     */
    
	T &operator *()
	{
		return *mBlock->getMemory();
	}
	
    /**
     * @brief Arrow operator for accessing members of the object managed by the HISSTools_RefPtr.
     *
     * This operator provides pointer-like access to the members of the object managed
     * by the HISSTools_RefPtr. It returns a pointer to the object, allowing the use of
     * the arrow syntax (->) to call methods or access members of the object.
     *
     * @return A pointer to the object managed by the HISSTools_RefPtr.
     */
    
	T *operator ->()
	{
		return mBlock->getMemory();
	}
	
    /**
     * @brief Subscript operator for accessing elements in the managed memory block.
     *
     * This operator allows array-style access to elements in the memory block managed
     * by the HISSTools_RefPtr. It returns a reference to the element at the specified index.
     *
     * @param index The position of the element to access within the memory block.
     * @return A reference to the element at the specified index.
     *
     * @throws std::out_of_range if the index is out of bounds.
     */
    
	T &operator [](const unsigned long index)
	{
		return mBlock->getMemory()[index];
	}
	
    /**
     * @brief Returns a pointer to the managed object or memory block.
     *
     * This method provides access to the raw pointer managed by the HISSTools_RefPtr.
     * It returns a pointer to the object or the first element in the memory block,
     * allowing direct pointer operations if needed.
     *
     * @return A pointer to the managed object or memory block.
     */
    
	T *get()
	{
		return mBlock->getMemory();
	}
	
    /**
     * @brief Pointer arithmetic operator for accessing an element at a specific offset.
     *
     * This operator performs pointer arithmetic, returning a pointer to the element
     * located at the specified offset from the beginning of the memory block managed
     * by the HISSTools_RefPtr. It allows access to elements relative to the base pointer.
     *
     * @param offset The number of elements to offset from the base pointer.
     * @return A pointer to the element at the specified offset from the base pointer.
     */
    
	T *operator +(unsigned int offset)
	{
		return mBlock->getMemory() + offset;
	}
	
    /**
     * @brief Equality operator for comparing two HISSTools_RefPtr objects.
     *
     * This operator checks if two HISSTools_RefPtr objects manage the same MemoryBlock.
     * It returns true if both objects point to the same memory, and false otherwise.
     *
     * @param rhs The HISSTools_RefPtr object to compare with.
     * @return True if both objects manage the same MemoryBlock, false otherwise.
     */
    
	bool operator ==(const HISSTools_RefPtr &rhs)
	{	
		return (rhs.mBlock == mBlock) ? TRUE : FALSE; 
	}
	
    /**
     * @brief Inequality operator for comparing two HISSTools_RefPtr objects.
     *
     * This operator checks if two HISSTools_RefPtr objects manage different MemoryBlocks.
     * It returns true if the objects point to different memory blocks, and false otherwise.
     *
     * @param rhs The HISSTools_RefPtr object to compare with.
     * @return True if the objects manage different MemoryBlocks, false otherwise.
     */
    
	bool operator !=(const HISSTools_RefPtr &rhs)
	{	
		return (*this == rhs) == TRUE ? FALSE : TRUE; 
	}
	
    /**
     * @brief Equality operator for comparing the managed memory with a raw pointer.
     *
     * This operator checks if the memory managed by the HISSTools_RefPtr object
     * is the same as the memory pointed to by the raw pointer `rhs`. It returns
     * true if both point to the same memory location, and false otherwise.
     *
     * @param rhs A raw pointer to compare with the managed memory.
     * @return True if the managed memory and the raw pointer point to the same location, false otherwise.
     */
    
	bool operator ==(const T *rhs)
	{	
		return (rhs == ((mBlock != NULL) ? mBlock->getMemory() : NULL)) ? TRUE : FALSE;
	}
	
    /**
     * @brief Inequality operator for comparing the managed memory with a raw pointer.
     *
     * This operator checks if the memory managed by the HISSTools_RefPtr object
     * is different from the memory pointed to by the raw pointer `rhs`. It returns
     * true if they point to different memory locations, and false otherwise.
     *
     * @param rhs A raw pointer to compare with the managed memory.
     * @return True if the managed memory and the raw pointer point to different locations, false otherwise.
     */
    
	bool operator !=(const T *rhs)
	{	
		return (*this == rhs) == TRUE ? FALSE : TRUE;
	}
};

#endif	/* __HISSTOOLS_POINTERS__ */
