
/**
 * @file HISSTools_ThreadSafety.hpp
 * @brief Provides classes and utilities for thread-safe memory management and synchronization.
 *
 * This file defines a set of classes, including `HISSTools_SpinLock` and `HISSTools_ThreadSafeMemory`,
 * that facilitate thread-safe operations in multi-threaded environments. It includes low-level
 * synchronization primitives like spinlocks and higher-level memory management classes
 * that ensure safe access to shared resources across multiple threads.
 *
 * The key features provided by this file include:
 * - A simple spinlock (`HISSTools_SpinLock`) for managing critical sections of code.
 * - A reference-counted memory management class (`HISSTools_ThreadSafeMemory`) that allows
 *   multiple threads to access and modify shared memory blocks safely.
 * - Thread-safe utilities to handle memory allocation and resizing.
 *
 * These classes are designed to be efficient for cases where short, critical sections
 * or quick memory operations are required, reducing overhead compared to more complex
 * synchronization methods.
 *
 * @note Proper usage of the provided spinlocks and memory management methods is crucial
 * to avoid deadlocks or race conditions.
 */

#ifndef __HISSTOOLS_THREADSAFETY__
#define __HISSTOOLS_THREADSAFETY__

#include "HISSTools_Pointers.hpp"

#include <atomic>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// Lightweight Spinlock ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class HISSTools_SpinLock
 * @brief A class that implements a simple spinlock for thread synchronization.
 *
 * This class provides a mechanism to lock and unlock a resource using a spinlock.
 * A spinlock is a low-level synchronization primitive that repeatedly checks if
 * a lock is available and thus can be useful in scenarios where locking is expected
 * to be held for a very short time.
 *
 * @note Spinlocks can be efficient for protecting resources that are only locked
 * for short durations, but they can be inefficient and waste CPU cycles if used
 * inappropriately.
 */

class HISSTools_SpinLock
{
	
private:
	
    /**
     * @brief A flag used for implementing a simple spinlock.
     *
     * This atomic flag is initialized to an unset state using ATOMIC_FLAG_INIT.
     * It is typically used to implement low-level locking mechanisms.
     *
     * @note std::atomic_flag provides a simple and efficient way to manage
     * synchronization without the overhead of more complex locking structures.
     */
    
    std::atomic_flag mLock = ATOMIC_FLAG_INIT;
	
public:
	
    /**
     * @brief Constructs a new HISSTools_SpinLock object.
     *
     * This constructor initializes the internal atomic flag to an unlocked state.
     * After construction, the spinlock is ready to be used for locking and unlocking operations.
     *
     * @note The lock is initially not set (unlocked) when the spinlock is created.
     */
    
	HISSTools_SpinLock()
	{
	}
	
    // Non-copyable
    
    /**
     * @brief Deleted copy constructor.
     *
     * This prevents copying of the HISSTools_SpinLock object.
     * Spinlocks should not be copied because copying could lead to inconsistent lock states
     * and thread synchronization issues.
     *
     * @note Copying a spinlock would undermine its purpose of managing exclusive access
     * to resources, so this operation is explicitly disabled.
     */
    
    HISSTools_SpinLock(const HISSTools_SpinLock&) = delete;
    
    /**
     * @brief Deleted copy assignment operator.
     *
     * This prevents assignment of one HISSTools_SpinLock object to another.
     * Spinlocks should not be assignable because this could lead to inconsistent
     * lock states, potentially causing thread synchronization issues.
     *
     * @note Assigning a spinlock would result in ambiguous ownership of the lock,
     * which is why this operation is explicitly disabled.
     */
    
    HISSTools_SpinLock& operator=(const HISSTools_SpinLock&) = delete;
    
    /**
     * @brief Acquires the spinlock by repeatedly checking until the lock is available.
     *
     * This method performs a busy-wait loop (spinning) until the lock is acquired.
     * Once acquired, the calling thread gains exclusive access to the protected resource.
     *
     * @note Use with caution as this method can lead to high CPU usage if the lock is
     * held by another thread for an extended period. Spinlocks are best used for short
     * lock durations.
     */
    
	void acquire()
	{
		while(!attempt());
	}
	
    /**
     * @brief Attempts to acquire the spinlock without blocking.
     *
     * This method tries to acquire the lock without entering a busy-wait loop.
     * If the lock is available, it is acquired and the method returns true.
     * If the lock is already held by another thread, the method returns false immediately.
     *
     * @return true if the lock was successfully acquired, false otherwise.
     *
     * @note This method provides a non-blocking alternative to `acquire()`, allowing
     * the caller to attempt to gain the lock without being forced to wait.
     */
    
	bool attempt()
	{
		return !mLock.test_and_set();
	}
	
    /**
     * @brief Releases the spinlock.
     *
     * This method unlocks the spinlock, allowing other threads to acquire it.
     * It sets the internal atomic flag to indicate that the lock is no longer held.
     *
     * @note The calling thread must have previously acquired the lock before releasing it.
     * Failing to do so could lead to undefined behavior.
     */
    
    void release()
	{
        mLock.clear();
	}
};

/**
 * @brief A template class for managing thread-safe memory operations.
 *
 * This class provides a thread-safe wrapper around a given type `T`, ensuring
 * that operations on the underlying object are protected from race conditions.
 * It is particularly useful when multiple threads need to safely access and modify
 * a shared resource.
 *
 * @tparam T The type of the object that this class manages in a thread-safe manner.
 *
 * @note This class uses synchronization mechanisms to ensure that the object is
 * safely accessed by multiple threads without causing data corruption or undefined behavior.
 */

template <class T>
class HISSTools_ThreadSafeMemory
{
	
public:
    
    /**
     * @brief A nested class that provides a reference-counted, thread-safe pointer to an object of type `T`.
     *
     * This class extends `HISSTools_RefPtr<T>` and is used to manage a thread-safe reference to
     * an object. It ensures that the underlying object is not destroyed while it is still being referenced.
     * The reference counting mechanism automatically handles memory management, releasing the object
     * when there are no more references to it.
     *
     * @tparam T The type of object that the pointer manages.
     *
     * @note This class simplifies memory management in multi-threaded environments, preventing
     * potential data races and ensuring safe access to shared resources.
     */
    
	class Ptr : public HISSTools_RefPtr <T>
	{
	
		// FIX - deletion scheduling

	public:
		
        /**
         * @brief Default constructor for the Ptr class.
         *
         * This constructor initializes the Ptr object, which internally calls the default
         * constructor of the base class `HISSTools_RefPtr<T>`. This sets up an empty,
         * reference-counted pointer that does not yet reference any object.
         *
         * @note After calling this constructor, the Ptr will not point to any valid object
         * until it is assigned a valid reference.
         */
        
		Ptr() : HISSTools_RefPtr <T> ()
		{
		}
		
        /**
         * @brief Copy constructor for the Ptr class.
         *
         * This constructor creates a new Ptr object as a copy of an existing Ptr object.
         * It copies the reference from the `rhs` (right-hand side) object and increments
         * the reference count on the underlying object. The internal reference-counting mechanism
         * ensures that the object will not be destroyed while it is still being referenced.
         *
         * @param rhs The Ptr object to be copied.
         *
         * @note This constructor ensures that both the original and the new Ptr objects
         * point to the same underlying object, sharing ownership in a thread-safe manner.
         */
        
		Ptr(const Ptr &rhs) : HISSTools_RefPtr <T> (rhs)
		{
		}

        /**
         * @brief Pointer-based copy constructor for the Ptr class.
         *
         * This constructor creates a new Ptr object as a copy of another Ptr object,
         * where the source is passed as a pointer. It copies the reference from the
         * `rhs` pointer and increments the reference count on the underlying object.
         * The reference-counting mechanism ensures thread-safe ownership of the object.
         *
         * @param rhs A pointer to the Ptr object to be copied.
         *
         * @note This constructor ensures that the new Ptr object shares ownership of the
         * same underlying object as the one pointed to by `rhs`. The reference count is
         * incremented to reflect the additional reference.
         */
        
		Ptr(const Ptr *rhs) : HISSTools_RefPtr <T> (rhs)
		{
		}
		
        /**
         * @brief Constructs a Ptr object with a size requirement.
         *
         * This constructor creates a new Ptr object based on an existing Ptr object
         * (`rhs`) and enforces a minimum size requirement (`requiredSize`) for the
         * underlying object. It copies the reference from the `rhs` pointer and increments
         * the reference count on the underlying object, ensuring that it meets the specified
         * size before sharing ownership.
         *
         * @param rhs A pointer to the Ptr object to be copied.
         * @param requiredSize The minimum required size of the underlying object.
         *
         * @note If the underlying object does not meet the required size, this constructor
         * may trigger an allocation or other resizing mechanism to ensure the object is of
         * sufficient size. The reference-counting mechanism ensures safe sharing of the object.
         */
        
		Ptr(const Ptr *rhs, unsigned long requiredSize) : HISSTools_RefPtr <T> (rhs, requiredSize)
		{
		}
		
        /**
         * @brief Constructs a Ptr object with a specified size requirement.
         *
         * This constructor creates a new Ptr object and allocates or ensures that the
         * underlying object has at least the specified size (`requiredSize`). It initializes
         * the reference-counted pointer to manage an object that meets the required size.
         *
         * @param requiredSize The minimum required size of the underlying object.
         *
         * @note This constructor ensures that the managed object is properly sized before
         * being accessed, providing thread-safe memory management and ownership handling.
         */
        
		Ptr(unsigned long requiredSize) : HISSTools_RefPtr <T> (requiredSize)
		{
		}
	};
	
private:
	
	// Atomic Spinlocks
	
    /**
     * @brief A spinlock used to synchronize access to the current object or resource.
     *
     * This member variable provides thread-safe synchronization for the operations
     * that modify or access the associated object. The spinlock ensures that only
     * one thread can access the protected resource at a time, preventing race conditions.
     *
     * @note The `HISSTools_SpinLock` is typically used for short-duration locks where
     * contention is expected to be minimal, making it efficient for quick access control.
     */
    
	HISSTools_SpinLock mCurrentLock;
    
    /**
     * @brief A spinlock used to synchronize memory resizing operations.
     *
     * This member variable ensures thread-safe access when resizing the underlying
     * memory or resource. By locking during resize operations, it prevents multiple
     * threads from concurrently resizing the resource, which could lead to memory
     * corruption or inconsistent states.
     *
     * @note The `HISSTools_SpinLock` provides efficient synchronization for short
     * and infrequent operations like resizing, where low overhead is preferred.
     */
    
	HISSTools_SpinLock mResizeLock;
	
	// Pointers to Memory Blocks
	
    /**
     * @brief A pointer to the current memory block managed by the thread-safe system.
     *
     * This member variable holds a pointer to the `Ptr` object, which manages a
     * reference-counted memory block. The `Ptr` class ensures that the memory block
     * is accessed and modified in a thread-safe manner, preventing data corruption
     * during concurrent access by multiple threads.
     *
     * @note The memory block is dynamically managed and may be resized or updated
     * based on the operations performed on the object.
     */
    
	Ptr *mCurrentMemoryBlock;
	
private:
	
    /**
     * @brief Retrieves a pointer to the current memory block.
     *
     * This method returns a pointer to the `Ptr` object that manages the current
     * memory block. The returned pointer allows access to the memory block in a
     * thread-safe manner, ensuring that the reference-counting mechanism is respected.
     *
     * @return A pointer to the current memory block (`Ptr*`), or nullptr if no block is set.
     *
     * @note The returned pointer should be handled carefully to avoid race conditions
     * in multithreaded environments.
     */
    
	Ptr *getCurrentBlockPtr()
	{
		mCurrentLock.acquire();
		Ptr *current = mCurrentMemoryBlock;
		mCurrentLock.release();
		
		return current;
	}

public:
	
    /**
     * @brief Constructs a new thread-safe memory manager with a specified initial size.
     *
     * This constructor initializes the `HISSTools_ThreadSafeMemory` object with a
     * starting memory size. The memory is allocated and managed in a thread-safe
     * manner, allowing safe access and modification by multiple threads.
     *
     * @param startingSize The initial size of the memory block to be allocated and managed.
     *
     * @note The allocated memory can be resized later as needed, and the internal
     * synchronization mechanisms ensure that all operations are thread-safe.
     */
    
	HISSTools_ThreadSafeMemory(unsigned long startingSize)
	{
		mCurrentMemoryBlock = NULL;
		resizeMemory(startingSize, FALSE);
	}
	
    /**
     * @brief Default constructor for the thread-safe memory manager.
     *
     * This constructor initializes a `HISSTools_ThreadSafeMemory` object without
     * allocating any initial memory. Memory allocation and management can be
     * handled later as needed, with thread-safe mechanisms in place to ensure safe
     * concurrent access by multiple threads.
     *
     * @note This constructor does not allocate any memory initially. Memory can be
     * dynamically allocated when required.
     */
    
	HISSTools_ThreadSafeMemory()
	{
		mCurrentMemoryBlock = NULL;
		resizeMemory(0, FALSE);
	}
	
    /**
     * @brief Destructor for the thread-safe memory manager.
     *
     * This destructor cleans up the `HISSTools_ThreadSafeMemory` object, releasing any
     * allocated memory and ensuring that all resources are properly deallocated in a
     * thread-safe manner. The reference-counting mechanism ensures that the memory
     * is only released when no more references to the memory block exist.
     *
     * @note It is important that the memory is safely freed and no dangling pointers
     * remain after the object is destroyed.
     */
	~HISSTools_ThreadSafeMemory()
	{		
        mResizeLock.acquire();
        mCurrentLock.acquire();
		delete mCurrentMemoryBlock;
		mCurrentMemoryBlock = NULL;
		mCurrentLock.release();
        mResizeLock.release();
	}
	
    /**
     * @brief Provides thread-safe access to a memory block of the specified size.
     *
     * This method ensures that the memory block is at least as large as the `requiredSize`.
     * If the current memory block is too small, it may trigger a resize operation in a
     * thread-safe manner. The method returns a `Ptr` object, which manages the memory
     * block with reference counting and thread-safe access.
     *
     * @param requiredSize The minimum size required for the memory block.
     * @return A `Ptr` object that provides thread-safe access to the memory block.
     *
     * @note If a resize is necessary, the operation is protected by a spinlock to
     * ensure that no race conditions occur during memory reallocation.
     */
    
	Ptr accessMemory(unsigned long requiredSize)
	{
		return Ptr(getCurrentBlockPtr(), requiredSize);
	}
	
    /**
     * @brief Provides thread-safe access to the current memory block.
     *
     * This method returns a `Ptr` object that manages the current memory block without
     * modifying its size. The `Ptr` object allows thread-safe access to the memory block
     * using reference counting, ensuring that the memory is not deallocated while
     * it is being used.
     *
     * @return A `Ptr` object that provides thread-safe access to the current memory block.
     *
     * @note This method does not check or modify the size of the memory block. If a specific
     * size is required, use the overloaded `accessMemory(unsigned long requiredSize)` method instead.
     */
    
	Ptr accessMemory()
	{
		return Ptr(getCurrentBlockPtr());
	}
	
    /**
     * @brief Resizes the memory block to the specified size in a thread-safe manner.
     *
     * This method resizes the memory block to at least the `requiredSize`. If `acquire` is set to `true`,
     * the method also acquires the spinlock before performing the resize operation, ensuring thread-safe access.
     * The method returns a `Ptr` object that manages the resized memory block.
     *
     * @param requiredSize The minimum size required for the memory block.
     * @param acquire If true, the method acquires the spinlock before resizing, ensuring thread-safe access.
     *                If false, it assumes the caller already holds the necessary lock.
     * @return A `Ptr` object that provides thread-safe access to the resized memory block.
     *
     * @note The resize operation is protected by a spinlock if `acquire` is true, preventing race conditions
     * during memory reallocation.
     */
    
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
