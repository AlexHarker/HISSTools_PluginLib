
/**
 * @file HISSTools_IOStream.hpp
 * @brief Defines the HISSTools_IOStream class for managing input/output streams.
 *
 * This file provides the declaration of the HISSTools_IOStream class, which is designed
 * to handle multi-channel input and output streams with configurable buffer sizes and
 * modes. It includes methods for reading and writing data, as well as managing internal
 * buffer operations.
 *
 * The class supports both input and output modes, as specified by the IOStreamMode enum.
 * Buffer sizes and the number of channels are defined upon object construction and are
 * managed internally to ensure proper allocation and data flow.
 */

#ifndef __HISSTOOLS_IOSTREAM__
#define __HISSTOOLS_IOSTREAM__

/**
 * @class HISSTools_IOStream
 * @brief A class for managing input and output streams with configurable parameters.
 *
 * The HISSTools_IOStream class provides an interface for handling input/output streams
 * with a specific mode, buffer size, and number of channels. It ensures efficient
 * streaming of data and manages resource allocation and deallocation automatically.
 * The class supports both input and output modes and can be used in various audio or
 * data processing contexts.
 */

class HISSTools_IOStream {
    
public:
    
    /**
     * @enum IOStreamMode
     * @brief Specifies the operating mode of the I/O stream.
     *
     * This enumeration defines the modes that a HISSTools_IOStream object can operate in.
     * It allows the stream to be either in input mode or output mode.
     */
    
	enum IOStreamMode {kInput, kOutput};
    
    /**
     * @brief Constructs a HISSTools_IOStream object with specified mode, buffer size, and channel count.
     *
     * Initializes the HISSTools_IOStream object by setting the stream mode, buffer size, and number of channels.
     * Ensures that the buffer size is at least 1 and the number of channels is within the valid range (1 to 256).
     *
     * @param mode The mode in which the I/O stream will operate (e.g., input or output mode).
     * @param size The desired buffer size for the stream. This value is clamped to be at least 1.
     * @param nChans The number of channels for the stream. This value is clamped between 1 and 256.
     */
    
	HISSTools_IOStream(IOStreamMode mode, unsigned long size, unsigned long nChans) : mMode(mode), mBufferSize(std::max(1UL, size)),
        mNChans(std::max(1UL, std::min(256UL, nChans)))
	{
		mBufferCounter = 0;
        mWriteOffset = mBufferSize;
        
        // Allocate individual channel pointers
		
		for (unsigned long i = 0; i < mNChans; i++)
			mBuffers[i] = new double[mBufferSize];
        
        // Clear buffers
        
        reset();
	}
		
    /**
     * @brief Destructor for the HISSTools_IOStream class.
     *
     * This destructor is responsible for cleaning up any resources used by the
     * HISSTools_IOStream object before it is destroyed. It ensures that any open
     * streams or associated memory are properly closed and deallocated.
     */
    
	~HISSTools_IOStream()
	{		
		// Delete individual channel pointers

		for (unsigned long i = 0; i < mNChans; i++)
			delete[] mBuffers[i];
	}
	
    /**
     * @brief Retrieves the size of the buffer used by the I/O stream.
     *
     * This method returns the current size of the buffer that the HISSTools_IOStream
     * object is using. The buffer size determines how much data can be stored in
     * the stream during input or output operations.
     *
     * @return The buffer size as an unsigned long.
     */
    
    unsigned long getBufferSize()
    {
        return mBufferSize;
    }
    
    /**
     * @brief Retrieves the number of channels used by the I/O stream.
     *
     * This method returns the current number of channels that the HISSTools_IOStream
     * object is using. The number of channels determines how many separate data streams
     * can be processed simultaneously (e.g., in audio processing, this could represent
     * the number of audio channels).
     *
     * @return The number of channels as an unsigned long.
     */
    
    unsigned long getNChans()
    {
        return mNChans;
    }
    
    /**
     * @brief Resets the I/O stream to its initial state.
     *
     * This method resets the internal state of the HISSTools_IOStream object. It is used
     * to clear any buffered data and restore the stream to its initial configuration,
     * allowing it to be reused without reallocating or reinitializing the object.
     * This can be useful in situations where the stream needs to be restarted.
     */
    
	void reset()
	{
		for (unsigned long i = 0; i < mNChans; i++)
            memset(mBuffers[i], 0, mBufferSize * sizeof(double));
        
        mWriteOffset = mBufferSize;
	}
	
    /**
     * @brief Reads data from the input stream into the specified output buffer.
     *
     * This method reads data from the HISSTools_IOStream input stream and writes it into
     * the provided output buffer. It processes a specified number of channels and samples,
     * starting from a given offset in the output buffer.
     *
     * @param outputs A double pointer to the array of output buffers, where the data will be written.
     * @param nChans The number of channels to be read. This value should match or be less than the stream's channel count.
     * @param size The number of samples to read for each channel.
     * @param outputOffset The starting position in the output buffer where the data should be written.
     * @return True if the read operation was successful, false otherwise.
     */
    
    bool read(double **outputs, unsigned long nChans, unsigned long size, unsigned long outputOffset)
    {
        // Load read and write parameters locally - FIX (check the effect of this later)...
        
        unsigned long readCounter = mBufferCounter;
        unsigned long writeOffset = mWriteOffset;
        
        double *output;
        
        // Sanity check (cannot read more than has been written or more channels than stored)
    
        // FIX - read zeros if not enough written in output mode....
        
        if (size > writeOffset || nChans > mNChans)
            return FALSE;
    
        // Adjust read counter if in input mode
        
        if (mMode == kInput)
            readCounter = (readCounter < size) ? mBufferSize + readCounter - size : readCounter - size;
        
        // Check for wraparound and copy in one or two steps
        
        unsigned long bufferRemain = mBufferSize - readCounter;
        unsigned long unwrappedSize = bufferRemain > size ? size : bufferRemain;
        
        for (unsigned long i = 0; i < nChans; i++)
        {
            output = outputs[i] + outputOffset;
            
            memcpy((void *) output, (void *) (mBuffers[i] + readCounter), unwrappedSize * sizeof (double));
            memcpy((void *) (output + unwrappedSize), mBuffers[i], (size - unwrappedSize) * sizeof (double));
        }
        
        // Update counter / offset if in output mode
        
        if (mMode == kOutput)
        {
            readCounter += size;
            mBufferCounter = (readCounter < mBufferSize) ? readCounter : readCounter - mBufferSize;
            mWriteOffset = writeOffset - size;
        }
        
        return TRUE;
    }
    
    /**
     * @brief Reads data from the input stream into a single output buffer.
     *
     * This method reads data from the HISSTools_IOStream input stream and writes it into
     * a single output buffer. It processes a specified number of samples, starting from
     * a given offset in the output buffer.
     *
     * @param output A pointer to the output buffer where the data will be written.
     * @param size The number of samples to read from the input stream.
     * @param outputOffset The starting position in the output buffer where the data should be written.
     * @return True if the read operation was successful, false otherwise.
     */
    
    bool read(double *output, unsigned long size, unsigned long outputOffset)
    {
        return read(&output, 1UL, size, outputOffset);
    }
	
    /**
     * @brief Writes data from the input buffer to the output stream.
     *
     * This method writes data from the provided input buffers into the HISSTools_IOStream
     * output stream. It processes a specified number of channels and samples, starting
     * from a given offset in the input buffers.
     *
     * @param inputs A double pointer to the array of input buffers, containing the data to be written.
     * @param nChans The number of channels to be written. This value should match or be less than the stream's channel count.
     * @param size The number of samples to write for each channel.
     * @param inputOffset The starting position in the input buffers from where the data should be read.
     * @return True if the write operation was successful, false otherwise.
     */
    
	bool write(double **inputs, unsigned long nChans, unsigned long size, unsigned long inputOffset)
	{
        // Load write offset parameter locally - FIX (check the effect of this later)...
        
        unsigned long writeCounter = mBufferCounter;
        unsigned long writeOffset = mWriteOffset;
        unsigned long i, j;
        
        double *bufferPointer = NULL;
        double *input = NULL;
        
		// Sanity check (cannot write past read counter or more channels than are allocated)
        
        if (size > mBufferSize || nChans > mNChans)
            return FALSE;
        
        // Calculate how much (if any) of the write is overlapped
        
        unsigned long overlappedSize = (writeOffset && mMode == kOutput) ? ((writeOffset < size) ? writeOffset : size) : 0UL;

        // Calculate loop sizes
        
        unsigned long bufferRemain = mBufferSize - writeCounter;
        unsigned long loop1 = (bufferRemain < overlappedSize) ? bufferRemain : overlappedSize;
        unsigned long loop2 = (bufferRemain < size) ? bufferRemain : size;
        
        for (i = 0; i < nChans; i++)
        {
            // Overlapping part (not wrapped)
				
            for (j = 0, input = inputs[i] + inputOffset, bufferPointer = mBuffers[i] + writeCounter;  j < loop1; j++)
                *bufferPointer++ += *input++;
		
            // Non-overlapping part (not wrapped)
        
            for (; j < loop2; j++)
                *bufferPointer++ = *input++;
        
            // Overlapping part (wrapped)
        
            for (bufferPointer -= mBufferSize; j < overlappedSize; j++)
                *bufferPointer++ = *input++;
        
            // Non-overlapping part (wrapped)
            
            for (; j < size; j++)
                *bufferPointer++ = *input++;
        }
        
        // Update counter / offset
        
        if (mMode == kInput)
        {
            writeCounter += size;
            mBufferCounter = (writeCounter < mBufferSize) ? writeCounter : writeCounter - mBufferSize;
        }
        else
        {
            mWriteOffset = size > writeOffset ? size : writeOffset;
        }
        
        return TRUE;
	}
    
    /**
     * @brief Writes data from a single input buffer to the output stream.
     *
     * This method writes data from the provided single input buffer into the HISSTools_IOStream
     * output stream. It processes a specified number of samples, starting from a given offset
     * in the input buffer.
     *
     * @param input A pointer to the input buffer containing the data to be written.
     * @param size The number of samples to write from the input buffer.
     * @param inputOffset The starting position in the input buffer from where the data should be read.
     * @return True if the write operation was successful, false otherwise.
     */
    
    bool write(double *input, unsigned long size, unsigned long inputOffset)
    {
        return write(&input, 1, size, inputOffset);
    }


private:
	
    // Mode
    
    /**
     * @brief The mode of the I/O stream (input or output).
     *
     * This constant member variable stores the mode of the HISSTools_IOStream object.
     * It defines whether the stream is operating in input mode (`kInput`) or output
     * mode (`kOutput`) and cannot be modified after the object is constructed.
     */
    
    const IOStreamMode mMode;
    
	// Data
	
    /**
     * @brief An array of pointers to buffers for storing stream data.
     *
     * This member variable holds up to 256 pointers to buffers, each used for storing
     * data associated with individual channels in the I/O stream. Each buffer in the array
     * corresponds to one channel of data, allowing for multi-channel input/output operations.
     *
     * @note The actual number of buffers used is determined by the number of channels
     * specified during the initialization of the HISSTools_IOStream object.
     */
    
	double *mBuffers[256];
	
	// Pointers
	
    /**
     * @brief Counter for tracking the position or usage of the buffer.
     *
     * This member variable keeps track of the current position or usage of the buffer
     * within the HISSTools_IOStream object. It may be used to monitor how much data has
     * been processed or to synchronize buffer read/write operations.
     *
     * @note The exact purpose of this counter depends on its usage within the stream's
     * internal logic.
     */
    
	unsigned long mBufferCounter;
    
    /**
     * @brief Offset used for tracking the current write position in the buffer.
     *
     * This member variable stores the current write offset in the buffer, indicating
     * the position where new data will be written in the HISSTools_IOStream object.
     * It helps in managing and keeping track of write operations across multiple
     * channels or buffer segments.
     *
     * @note This offset is updated as data is written to ensure that new data is placed
     * at the correct position within the buffer.
     */
    
	unsigned long mWriteOffset;
	
	// Sizes
	
    /**
     * @brief The size of the buffer used by the I/O stream.
     *
     * This constant member variable stores the size of the buffer for the HISSTools_IOStream object.
     * The buffer size determines how much data can be held at one time during input or output
     * operations. It is set during the construction of the object and remains unchanged thereafter.
     *
     * @note The buffer size is clamped to ensure it is at least 1.
     */
    
	const unsigned long mBufferSize;
    
    /**
     * @brief The number of channels used by the I/O stream.
     *
     * This constant member variable stores the number of channels in the HISSTools_IOStream object.
     * It defines how many independent streams of data (e.g., audio channels) the object can handle.
     * The number of channels is set during object construction and remains fixed throughout its lifetime.
     *
     * @note The number of channels is clamped between 1 and 256 to ensure a valid range.
     */
    
    const unsigned long mNChans;
};


#endif
