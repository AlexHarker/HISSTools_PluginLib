
/**
 * @file HISSTools_Frame_Delay.hpp
 * @brief A class for implementing frame-based delay processing for multi-channel audio.
 *
 * This file defines the `HISSTools_Frame_Delay` class, which provides functionality for
 * frame-based delay processing. The class supports multiple audio channels and allows
 * for flexible delay amounts in terms of frames. It handles dynamic memory allocation
 * for delay buffers, processes input/output data for each channel, and offers methods
 * for managing the state of the delay buffer (such as clearing and resetting).
 *
 * Key features of this class include:
 * - Multi-channel support with independent buffers per channel.
 * - Frame-based processing with customizable frame sizes and delays.
 * - Efficient memory management for dynamic allocation and deallocation.
 *
 * This class is suitable for use in real-time audio applications where frame-based delay
 * processing is required for multiple channels.
 *
 * @see HISSTools_Frame_Delay
 */

#ifndef __HISSTOOLS_FRAME_DELAY__
#define __HISSTOOLS_FRAME_DELAY__

/**
 * @class HISSTools_Frame_Delay
 * @brief A class for managing frame-based audio delay for multiple channels.
 *
 * The `HISSTools_Frame_Delay` class provides an implementation of a delay buffer
 * that operates on audio data in frames. It supports multiple channels, allowing
 * independent delay processing for each channel, and handles dynamic memory allocation
 * based on the maximum frame size, number of frames, and number of channels.
 *
 * The class offers methods for performing delay processing, resetting the buffer,
 * and clearing the internal state. It is designed for real-time audio applications
 * that require accurate frame-based delay functionality.
 *
 * Key features include:
 * - Multi-channel support for flexible audio processing.
 * - Frame-based delay with customizable frame size and delay amount.
 * - Efficient memory management with dynamic allocation and deallocation.
 */

class HISSTools_Frame_Delay
{
	
public:
	
    /**
     * @brief Constructs a frame delay object with the specified parameters.
     *
     * This constructor initializes the frame delay buffer by allocating memory for the specified
     * number of frames, frame size, and channels. The constructor ensures the frame delay can
     * accommodate the given maximum frame size, number of frames, and number of channels.
     * It also allows for shared IO memory and a zero-frame delay by incrementing the maximum
     * number of frames. If memory allocation is successful, the internal state is set up accordingly.
     *
     * @param maxFrameSize The maximum size of each frame (in samples).
     * @param maxNumFrames The maximum number of frames to store in the delay buffer.
     * @param maxChans The number of channels (default is 1).
     */
    
	HISSTools_Frame_Delay(unsigned long maxFrameSize, unsigned long maxNumFrames, unsigned long maxChans = 1)
	{
		bool success;
		unsigned long i;
		
		maxFrameSize = maxFrameSize < 1 ? 1 : maxFrameSize;
		maxNumFrames = maxNumFrames < 1 ? 1 : maxNumFrames;
		maxChans = maxChans < 1 ? 1 : maxChans;
		
		// The below allows write before read, and hence shared IO memory and a zero frame delay
		
		maxNumFrames++;	
		
		// Force reset on first call
		
		mClear = TRUE;	
		
		// Allocate memory (assume failure)		

		mMaxFrameSize = 0;
		mMaxNumFrames = 0;
		mFrameData = 0;
		
		// Allocate channel array
		
		mFrameData = new double *[maxChans];

		if (mFrameData)
			mMaxChans = maxChans;
		
		// Allocate individual channel pointers
		
		for (i = 0; i < mMaxChans; i++)
			mFrameData[i] = new double[maxFrameSize * maxNumFrames];
		
		for (i = 0, success = TRUE; i < mMaxChans; i++)
			if (!mFrameData[i])
				success = FALSE;
		
		if (success == TRUE)
		{
			mMaxFrameSize = maxFrameSize;
			mMaxNumFrames = maxNumFrames;
		}
	};
	
    /**
     * @brief Destroys the frame delay object and releases allocated resources.
     *
     * This destructor cleans up the memory used by the frame delay buffer.
     * It deletes the memory allocated for each channel and then deletes the
     * array holding the channel pointers. This ensures proper deallocation of
     * all dynamically allocated memory to prevent memory leaks.
     */
    
	~HISSTools_Frame_Delay()
	{		
		// Delete individual channel pointers
		
		for (unsigned long i = 0; i < mMaxChans; i++)
			delete[] mFrameData[i];
		
		// Delete channel array
		
		delete[] mFrameData;
	};
	
	
private:
	
    /**
     * @brief Resets the internal state of the frame delay buffer.
     *
     * This method resets the frame delay by updating the frame size,
     * clearing the valid frames, resetting the read/write pointer, and
     * marking the internal state as not cleared. It ensures that the
     * frame delay buffer is ready to store a new set of frames.
     *
     * @param frameSize The size of the frame buffer to reset to.
     */
    
	void reset(unsigned long frameSize)
	{
		mFrameSize = frameSize;
		mValidFrames = 0;
		mPointer = 0;
		mClear = FALSE;
	}

    /**
     * @brief Performs input and output processing for a single channel of audio data.
     *
     * This method handles the input and output operations for a single channel in the frame delay buffer.
     * It writes the current input frame into the delay buffer and retrieves the corresponding output frame.
     * If the read pointer is valid, it retrieves the delayed frame from the buffer; otherwise, it outputs silence (zeros).
     *
     * @param in Pointer to the input buffer for the current frame.
     * @param out Pointer to the output buffer where the processed frame will be written.
     * @param chanFrameData Pointer to the frame data for the current channel.
     * @param frameSize The size of the current frame (number of samples).
     * @param readPointer The read pointer indicating the position in the buffer to retrieve the delayed frame.
     * @param writePointer The write pointer indicating the position in the buffer where the current frame is written.
     */
    
	void SingleChannelIO(double *in, double *out, double *chanFrameData, unsigned long frameSize, unsigned long readPointer, unsigned long writePointer)
	{
		double *frameData;
		unsigned long i;
		
		// Copy in current frame
		
		for (i = 0, frameData = chanFrameData + (writePointer * mMaxFrameSize); i < frameSize; i++)
			frameData[i] = in[i];
		
		// Get output frame
		
		if (readPointer >= 0)
		{
			for (i = 0, frameData = chanFrameData + (readPointer * mMaxFrameSize); i < frameSize; i++)
				out[i] = frameData[i];
		}
		else 
		{
			for (i = 0; i < frameSize; i++)
				out[i] = 0.;
		}
	}
	
	
public:
	
    /**
     * @brief Processes multi-channel input/output with a specified frame delay.
     *
     * This method processes audio data for multiple channels with a specified delay in frames.
     * It handles reading and writing frames for each channel, ensuring the input is delayed
     * appropriately based on the given frame delay. If the frame size or number of channels
     * exceeds the maximum limits, or if the frame size has changed, the method performs a reset.
     *
     * @param in A 2D array of input buffers, one for each channel.
     * @param out A 2D array of output buffers, one for each channel, where the delayed data is written.
     * @param frameSize The size of each frame (number of samples per channel).
     * @param nChans The number of channels to process.
     * @param frameDelay The number of frames by which to delay the input.
     * @return Returns TRUE if the processing is successful, otherwise FALSE if the input exceeds the allowed frame size or number of channels.
     */
    
	bool delayIO(double **in, double **out, unsigned long frameSize, unsigned long nChans, unsigned long frameDelay)
	{
		unsigned long writePointer;
		unsigned long readPointer;
		
		// Sanity Check
		
		if (frameSize > mMaxFrameSize || nChans > mMaxChans)
			return FALSE;
		
		// Reset
		
		if (frameSize != mFrameSize || mClear == TRUE)
			reset(frameSize);
		
		writePointer = mPointer;
		
		// Find output frame
		
		readPointer = mPointer - frameDelay;
		readPointer = readPointer < 0 ? readPointer + mMaxNumFrames: readPointer;
		readPointer = frameDelay > mValidFrames ? -1 : readPointer;
		
		for (unsigned long i = 0; i < nChans; i++) 
			SingleChannelIO(in[i], out[i], mFrameData[i], frameSize, readPointer, writePointer);
		
		mPointer = ++mPointer >= mMaxNumFrames ? 0 : mPointer;
		mValidFrames = ++mValidFrames >= mMaxNumFrames ? mMaxNumFrames : mValidFrames;
		
		return TRUE;
	}

    /**
     * @brief Processes single-channel input/output with a specified frame delay.
     *
     * This method is a simplified version of the multi-channel `delayIO` function, designed to handle
     * a single channel of audio data. It processes the input buffer with the specified frame delay
     * and writes the delayed output to the provided output buffer.
     *
     * @param in Pointer to the input buffer containing the audio data.
     * @param out Pointer to the output buffer where the delayed audio data will be written.
     * @param size The size of the input/output frame (number of samples).
     * @param frameDelay The number of frames by which to delay the input.
     */
    
	void delayIO(double *in, double *out, long size, long frameDelay)
	{
		delayIO(&in, &out, size, 1, frameDelay);
	}
	
    /**
     * @brief Marks the delay buffer to be cleared on the next operation.
     *
     * This method sets an internal flag to indicate that the delay buffer should be cleared
     * and reset during the next processing cycle. It ensures that the next input frame starts
     * with an empty buffer, effectively resetting the delay state.
     */
    
	void clear()
	{
		mClear = TRUE;
	}

	
private:
	
	// Data
	
    /**
     * @brief A 2D array storing the frame data for each channel.
     *
     * This member variable holds the audio data for all channels in the delay buffer.
     * Each element of the array points to a buffer containing the frame data for a single channel.
     * The array is dynamically allocated based on the number of channels and the maximum frame size.
     */
    
	double **mFrameData;
	
	// Current Parameters
	
    /**
     * @brief The number of valid frames currently stored in the delay buffer.
     *
     * This member variable tracks how many frames in the delay buffer contain valid data.
     * It is updated as new frames are written into the buffer and is used to determine
     * whether the buffer has sufficient valid frames to perform delayed output operations.
     */
    
	unsigned long mValidFrames;
    
    /**
     * @brief The current write pointer for the delay buffer.
     *
     * This member variable indicates the position in the delay buffer where the next input frame
     * will be written. It is incremented after each write operation and wraps around to the
     * beginning of the buffer when it reaches the maximum number of frames.
     */
    
	unsigned long mPointer;
    
    /**
     * @brief The size of each frame in the delay buffer (number of samples).
     *
     * This member variable stores the current frame size, which represents the number of samples
     * per frame for each channel in the delay buffer. It is updated when the frame delay buffer
     * is reset and is used to ensure consistent processing of audio data.
     */
    
	unsigned long mFrameSize;
	
	// Maximums
	
    /**
     * @brief The maximum allowed size of each frame in the delay buffer.
     *
     * This member variable holds the maximum number of samples that each frame can contain.
     * It is set during initialization and determines the upper limit for the size of frames
     * that can be processed by the delay buffer. Frames larger than this size will not be accepted.
     */
    
	unsigned long mMaxFrameSize;
    
    /**
     * @brief The maximum number of frames that can be stored in the delay buffer.
     *
     * This member variable defines the total capacity of the delay buffer in terms of frames.
     * It is set during initialization and determines how many frames can be held in memory
     * for delay processing. Exceeding this limit will cause the write pointer to wrap around
     * to the beginning of the buffer.
     */
    
	unsigned long mMaxNumFrames;
    
    /**
     * @brief The maximum number of audio channels supported by the delay buffer.
     *
     * This member variable stores the maximum number of channels that the delay buffer can handle.
     * It is set during initialization and determines how many independent channels of audio data
     * can be processed simultaneously. Each channel has its own dedicated buffer within the delay system.
     */
    
	unsigned long mMaxChans;
	
	// Clear
	
    /**
     * @brief A flag indicating whether the delay buffer should be cleared.
     *
     * This member variable is used to mark whether the delay buffer needs to be cleared
     * before the next operation. When set to TRUE, the delay buffer will be reset,
     * ensuring no old data is retained. It is typically set via the `clear()` method
     * and checked during processing.
     */
    
	bool mClear;
};


#endif
