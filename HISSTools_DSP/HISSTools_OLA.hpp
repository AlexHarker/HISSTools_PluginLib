
/**
 * @file HISSTools_OLA.hpp
 * @brief Header file for the HISSTools_OLA class, implementing the Overlap-Add (OLA) processing technique.
 *
 * This file defines the HISSTools_OLA class, which provides functionality for handling
 * Overlap-Add (OLA) processing for audio data. The OLA technique is used to process audio in frames,
 * combining them with overlap to ensure smooth transitions between consecutive frames. This class
 * supports multi-channel audio and allows for dynamic configuration of frame size, hop size, and hop
 * offset. It also provides methods for resetting and updating the internal state as needed.
 *
 * The OLA process is crucial for efficient time-domain signal processing, particularly for effects
 * like convolution, filtering, and time-stretching.
 *
 * Classes:
 * - HISSTools_OLA: Manages OLA processing, buffering, and channel management.
 */

#ifndef __HISSTOOLS_OLA__
#define __HISSTOOLS_OLA__

/**
 * @brief The HISSTools_OLA class for Overlap-Add (OLA) processing.
 *
 * This class implements the Overlap-Add (OLA) method for efficient signal processing in the time domain.
 * It provides methods to manage buffering, frame overlap, and channel processing for audio signals.
 * The class is designed to handle multiple channels and configurable frame sizes.
 *
 * Usage of this class involves initializing it with the desired maximum frame size and number of channels,
 * followed by processing frames of audio data through the provided methods.
 */

class HISSTools_OLA {
	
public:
	
    /**
     * @brief Constructor for the HISSTools_OLA class.
     *
     * This constructor initializes an instance of the HISSTools_OLA class, which is responsible for handling
     * overlap-add (OLA) processing. The constructor sets up the object to handle a specified maximum frame size
     * and number of channels.
     *
     * @param maxFrameSize The maximum size of frames that can be processed (in samples).
     * @param maxChans The maximum number of channels to be handled by the OLA processing.
     */
    
	HISSTools_OLA(unsigned long maxFrameSize, unsigned long maxChans)
	{		
		bool success = TRUE;
		
		maxFrameSize = (maxFrameSize < 2) ? 2 : maxFrameSize;

		maxChans = (maxChans < 1) ? 1 : maxChans;
		maxChans = (maxChans > 256) ? 256 : maxChans;
		mMaxChans = maxChans;
		
		// Allocate individual channel pointers
		
		for (unsigned long i = 0; i < mMaxChans; i++)
		{
			mInputBuffers[i] = new double[maxFrameSize * 2];
			mOutputBuffers[i] = new double[maxFrameSize];
			mFrameBuffers[i] = new double[maxFrameSize];
		}
		
		for (unsigned long  i = 0; i < mMaxChans; i++)
			if (!mInputBuffers[i] || !mOutputBuffers[i] || !mFrameBuffers[i])
				success = FALSE;
		
		mMaxFrameSize = (success == TRUE) ? maxFrameSize : 0;
	
		setParams(maxFrameSize, maxFrameSize / 2, TRUE);
	}
		
    /**
     * @brief Destructor for the HISSTools_OLA class.
     *
     * This destructor is responsible for cleaning up any resources or memory that were allocated during the
     * lifetime of the HISSTools_OLA object. It ensures that all dynamic memory and resources are properly released
     * when the object is destroyed.
     */
    
	~HISSTools_OLA()
	{		
		// Delete individual channel pointers

		for (unsigned long i = 0; i < mMaxChans; i++) 
		{
			delete[] mInputBuffers[i];
			delete[] mOutputBuffers[i];
			delete[] mFrameBuffers[i];
		}
	}
	
	
private:
	
    /**
     * @brief Resets the state of the HISSTools_OLA object.
     *
     * This method resets the internal state of the HISSTools_OLA object to prepare for processing new audio data.
     * It is typically called when the frame size changes or when reinitialization is needed. The frame size is updated
     * with the new value provided, and any internal buffers or settings are reset accordingly.
     *
     * @param frameSize The new frame size (in samples) to be used for future processing.
     */
    
	void reset(unsigned long frameSize)
	{
		for (unsigned long i = 0; i < mMaxChans; i++)
		{
			for (unsigned long j = 0; j < frameSize * 2; j++)
				mInputBuffers[i][j] = 0.;
			
			for (unsigned long j = 0; j < frameSize; j++)
				mOutputBuffers[i][j] = 0.;
		}
	}
	
    /**
     * @brief Writes a single frame of audio to the output buffer for a specific channel.
     *
     * This method performs the overlap-add (OLA) process by writing a frame of audio data from the frame buffer
     * to the specified output buffer. It manages the overlap based on the current frame size and hop size, starting
     * from the given I/O pointer. This method is intended for use on a single channel of audio at a time.
     *
     * @param outputBuffer Pointer to the output buffer where the processed frame will be written.
     * @param frameBuffer Pointer to the input frame buffer containing the audio data to be processed.
     * @param IOPointer The position in the output buffer where writing should begin.
     * @param frameSize The size of the frame (in samples) to be written to the output buffer.
     * @param hopSize The hop size (in samples) indicating the overlap between consecutive frames.
     */
    
	void writeFrameChannel(double *outputBuffer, double *frameBuffer, long IOPointer, unsigned long frameSize, unsigned long hopSize)
	{
		long testLength;
		long i;
				
		// Overlappoing part
		
		IOPointer = IOPointer >= frameSize ? 0 : IOPointer;
		
		for (i = 0, testLength = frameSize - IOPointer; i < frameSize - hopSize && i < testLength; i++)
			outputBuffer[IOPointer++] += *frameBuffer++;
		
		IOPointer = IOPointer >= frameSize ? 0 : IOPointer;
		
		for (; i < frameSize - hopSize; i++)
			outputBuffer[IOPointer++] += *frameBuffer++;
			
		// Non-overlapping part
		
		IOPointer = IOPointer >= frameSize ? 0 : IOPointer;			

		for (i = 0, testLength = frameSize - IOPointer; i < hopSize && i < testLength; i++)
			outputBuffer[IOPointer++] = *frameBuffer++;

		IOPointer = IOPointer >= frameSize ? 0 : IOPointer;			

		for (; i < hopSize; i++)
			outputBuffer[IOPointer++] = *frameBuffer++;
	}
	
    /**
     * @brief Updates the internal state of the HISSTools_OLA object.
     *
     * This method is used to update or refresh the internal parameters and state of the HISSTools_OLA object.
     * It may be called when changes are made to the object's configuration or after processing, ensuring that
     * the object is ready for the next processing cycle.
     */
    
	void update()
	{
		// FIX - Threadsafety!!
		// FIX - do this so we can keep the last values coming in??
        
		if (mReset == TRUE || mNewFrameSize != mFrameSize || mNewHopSize != mHopSize)
		{	
			// Reset
			
			reset(mNewFrameSize);
			
			// Update parameters
			
			mFrameSize = mNewFrameSize;
			mHopSize = mNewHopSize;
			mBlockIOPointer = 0;
			mBlockHopPointer = mNewHopOffset;
			mReset = FALSE;
		}
	}
	
    /**
     * @brief Calculates the minimum loop time for the Overlap-Add (OLA) process.
     *
     * This method determines the minimum value among the provided times for the OLA process. It is typically used
     * to calculate the earliest time point (in samples) at which a new frame can be written or processed, ensuring
     * proper overlap handling between frames.
     *
     * @param hopTime The time (in samples) representing the hop size between frames.
     * @param writeTime The time (in samples) representing the current write pointer position in the buffer.
     * @param blockTime The time (in samples) representing the size of the processing block.
     *
     * @return The minimum of the three time values, which is used to manage the OLA processing loop.
     */
    
	long loopMin(long hopTime, long writeTime, long blockTime)
	{
		long minTime = hopTime;
		
		if (writeTime < minTime)
			minTime = writeTime;
		if (blockTime < minTime)
			minTime = blockTime;
		
		return minTime;
	}
	
	
protected:
	
    /**
     * @brief Processes a frame of audio using the Overlap-Add (OLA) method.
     *
     * This virtual method processes a single frame of audio data by applying the Overlap-Add (OLA) technique.
     * It takes an input/output frame buffer and processes it in-place. The method is intended to be overridden by
     * derived classes to implement specific audio processing algorithms, while managing the OLA buffering and
     * overlap.
     *
     * @param ioFrame Pointer to the input/output frame buffer containing the audio data to be processed.
     * @param frameSize The size of the frame (in samples) to be processed.
     */
    
	void virtual process(double *ioFrame, unsigned long frameSize)
	{
		// This function should be overridden for single channel operation. 
		// IO is on a single shared buffer
	}
	
    /**
     * @brief Processes multiple channels of audio using the Overlap-Add (OLA) method.
     *
     * This virtual method processes multiple channels of audio data by applying the Overlap-Add (OLA) technique.
     * It operates on an array of input/output frame buffers, one for each channel, and processes them in-place.
     * Derived classes should override this method to implement specific multi-channel audio processing algorithms
     * while managing OLA buffering and overlap for each channel.
     *
     * @param ioFrames Pointer to an array of input/output frame buffers, one for each channel, containing the audio data to be processed.
     * @param frameSize The size of the frame (in samples) to be processed for each channel.
     * @param nChans The number of audio channels to be processed.
     */
    
	void virtual process(double **ioFrames, unsigned long frameSize, unsigned long nChans)
	{
		// This function should be overridden for multichannel operation. 
		// IO is on a single shared buffer per channel
	}
	
	
public:
	
    /**
     * @brief Performs the Overlap-Add (OLA) operation on a block of audio samples.
     *
     * This method applies the Overlap-Add (OLA) technique by combining input audio samples with an output buffer.
     * The input samples are added to the output buffer, with overlap handling based on the provided number of
     * samples. This method is typically used to mix processed audio frames with previously processed frames
     * to achieve smooth transitions between consecutive frames.
     *
     * @param in Pointer to the input buffer containing the audio samples to be added.
     * @param out Pointer to the output buffer where the result of the OLA operation will be written.
     * @param nSamps The number of samples to process in the OLA operation.
     *
     * @return Returns true if the overlap-add operation was successful, otherwise returns false.
     */
    
	bool overlapAdd(double *in, double *out, unsigned long nSamps)
	{
        bool processedFrames = FALSE;
        
		double *inputBuffer = mInputBuffers[0];
		double *outputBuffer = mOutputBuffers[0];
		double *frameBuffer = mFrameBuffers[0];
				
		unsigned long frameSize;
		unsigned long hopSize;
		
		long IOPointer;
		long hopPointer;
		long loopSize;
	
		// Update parameters
		
		update();
		
		// Get parameters
		
		frameSize = mFrameSize;
		hopSize = mHopSize <= frameSize ? mHopSize : frameSize;
		IOPointer = mBlockIOPointer >= frameSize ? 0 : mBlockIOPointer;
		hopPointer = mBlockHopPointer;
		
		// Loop over vector grabbing frames as appropriate

		for (long i = 0; i < nSamps;)
		{			
			// Grab a frame and OLA with processing

			if (hopPointer >= hopSize)
			{
                processedFrames = TRUE;
                hopPointer = 0;
                
				for (long j = 0; j < frameSize; j++)
					frameBuffer[j] = inputBuffer[IOPointer + j];

				process(frameBuffer, frameSize);
				writeFrameChannel(outputBuffer, frameBuffer, IOPointer, frameSize, hopSize);
			}
			
			// Update pointers and check loop size

			IOPointer = IOPointer >= frameSize ? 0 : IOPointer;
			loopSize = loopMin(hopSize - hopPointer, frameSize - IOPointer, nSamps - i);
			
			// Copy samples in/out

			for (long j = 0; j < loopSize; j++, IOPointer++)
			{
				inputBuffer[IOPointer] = inputBuffer[IOPointer + frameSize] = *in++;
				*out++ = outputBuffer[IOPointer];
			}
			
			hopPointer += loopSize;
			i += loopSize;
		}
		
		mBlockIOPointer = IOPointer;
		mBlockHopPointer = hopPointer;
        
        return processedFrames;
	}
	
    /**
     * @brief Performs the Overlap-Add (OLA) operation on multiple channels of audio samples.
     *
     * This method applies the Overlap-Add (OLA) technique across multiple channels of audio data. It combines the input
     * audio samples from each channel with their respective output buffers, handling the overlap based on the number of
     * samples and channels provided. This is typically used to process and mix multi-channel audio frames, ensuring smooth
     * transitions between consecutive frames for each channel.
     *
     * @param ins Pointer to an array of input buffers, one for each channel, containing the audio samples to be added.
     * @param outs Pointer to an array of output buffers, one for each channel, where the result of the OLA operation will be written.
     * @param nSamps The number of samples to process for each channel in the OLA operation.
     * @param nChans The number of audio channels to be processed.
     *
     * @return Returns true if the overlap-add operation was successful for all channels, otherwise returns false.
     */
    
	bool overlapAdd(double **ins, double **outs, unsigned long nSamps, unsigned long nChans)
	{
        bool processedFrames = FALSE;

        unsigned long frameSize;
		unsigned long hopSize;
		
		long IOPointer;
		long hopPointer;
		long loopSize;

		// Sanity Check

		if (nChans > mMaxChans)
			return FALSE;
			
		// Update parameters
		
		update();
		
		// Get parameters
		
		frameSize = mFrameSize;
		hopSize = mHopSize <= frameSize ? mHopSize : frameSize;
		IOPointer = mBlockIOPointer >= frameSize ? 0 : mBlockIOPointer;
		hopPointer = mBlockHopPointer;
		
		// Loop over vector grabbing frames as appropriate
		
		for (long i = 0; i < nSamps;)
		{	
			// Grab a frame and OLA with processing
			
			if (hopPointer >= hopSize)
			{
                processedFrames = TRUE;
                hopPointer = 0;
                
				for (long j = 0; j < nChans; j++)
					for (long k = 0; k < frameSize; k++)
						mFrameBuffers[j][k] = mInputBuffers[j][IOPointer + k];
				
				process(mFrameBuffers, frameSize, nChans);
				
				for (long j = 0; j < nChans; j++)					
					writeFrameChannel(mOutputBuffers[j], mFrameBuffers[j], IOPointer, frameSize, hopSize);
			}
			
			// Update pointers and check loop size
			
			IOPointer = IOPointer >= frameSize ? 0 : IOPointer;
			loopSize = loopMin(hopSize - hopPointer, frameSize - IOPointer, nSamps - i);
			
			// Loop over channels and copy samples in/out
			
			for (long j = 0; j < nChans; j++)
			{
				for (long k = i, l = IOPointer; k < (i + loopSize); k++, l++)
				{
					mInputBuffers[j][l] = mInputBuffers[j][l + frameSize] = ins[j][k];
					outs[j][k] = mOutputBuffers[j][l];
				}
			}
			
			IOPointer += loopSize;
			hopPointer += loopSize;
			i += loopSize;
		}
		
		mBlockIOPointer = IOPointer;
		mBlockHopPointer = hopPointer;
		
		return processedFrames;
	}
	
    /**
     * @brief Configures the parameters for the Overlap-Add (OLA) process.
     *
     * This method sets the key parameters for the Overlap-Add (OLA) processing, including the frame size and hop size.
     * Optionally, the method can reset the internal state and adjust the hop offset. These parameters control how
     * frames of audio data are processed and overlapped. Calling this method allows for reconfiguration of the
     * OLA process as needed.
     *
     * @param frameSize The size of the frame (in samples) to be used for OLA processing.
     * @param hopSize The hop size (in samples) indicating the amount of overlap between consecutive frames.
     * @param reset If true, the internal state of the OLA process is reset (default is false).
     * @param hopOffset The offset (in samples) for the hop position, allowing finer control over the frame alignment (default is 0).
     */
    
	void setParams(unsigned long frameSize, unsigned long hopSize, bool reset = FALSE, unsigned long hopOffset = 0)
	{
		mNewFrameSize = frameSize < mMaxFrameSize ? (frameSize ? frameSize : 1) : mMaxFrameSize;
		mNewHopSize = hopSize <= mNewFrameSize ? (hopSize ? hopSize : 1) : mNewFrameSize;
		mNewHopOffset = hopOffset > mNewHopSize ? mNewHopSize : hopOffset;
		mReset = reset;
	}
	
	
private:
	
	// Data
	
    /**
     * @brief Array of input buffers for storing audio data during processing.
     *
     * This member variable holds an array of pointers to input buffers used for storing audio data during the
     * Overlap-Add (OLA) processing. Each buffer can store a frame of audio data for up to 256 channels. The
     * input buffers are used to accumulate incoming audio frames before they are processed and combined with
     * output buffers using the OLA technique.
     *
     * @note The size of the array is fixed at 256, meaning this object can handle up to 256 audio channels.
     */
    
	double *mInputBuffers[256];
   
    /**
     * @brief Array of output buffers for storing processed audio data.
     *
     * This member variable holds an array of pointers to output buffers used for storing audio data after
     * it has been processed using the Overlap-Add (OLA) method. Each buffer corresponds to a channel of
     * processed audio, and the array can accommodate up to 256 channels. The processed data in these buffers
     * is typically written back to the output audio stream.
     *
     * @note The size of the array is fixed at 256, supporting up to 256 audio channels.
     */
    
	double *mOutputBuffers[256];
    
    /**
     * @brief Array of frame buffers for temporary storage of audio frames during processing.
     *
     * This member variable holds an array of pointers to frame buffers, which are used to store intermediate
     * audio frames during the Overlap-Add (OLA) processing. Each buffer corresponds to one channel of audio,
     * and the array supports up to 256 channels. The frame buffers temporarily hold audio data that is processed
     * before being added to the output buffers.
     *
     * @note The size of the array is fixed at 256, allowing for the processing of up to 256 audio channels.
     */
    
	double *mFrameBuffers[256];
	
	// Pointers
	
    /**
     * @brief Pointer indicating the current position within the block for I/O operations.
     *
     * This member variable tracks the current position within the audio processing block during
     * input/output (I/O) operations. It is used to keep track of where the next read or write
     * should occur in the input or output buffers during Overlap-Add (OLA) processing. The value
     * of this pointer is updated as audio frames are processed.
     *
     * @note This is a long integer to accommodate large sample counts in extended processing sessions.
     */
    
	long mBlockIOPointer;
    
    /**
     * @brief Pointer indicating the current position for the hop during the Overlap-Add (OLA) process.
     *
     * This member variable tracks the current position of the hop pointer during the Overlap-Add (OLA)
     * process. It is used to determine where the next hop in the audio data should occur, controlling
     * the overlap between consecutive audio frames. The value of this pointer is incremented based on
     * the hop size as audio frames are processed.
     *
     * @note This is a long integer to allow for large sample counts during extended audio processing sessions.
     */
    
	long mBlockHopPointer;
	
	// Current Parameters
	
    /**
     * @brief Stores the current frame size used in the Overlap-Add (OLA) process.
     *
     * This member variable holds the size of the audio frame (in samples) that is being processed in the
     * Overlap-Add (OLA) operation. The frame size determines how many samples are processed in each frame
     * and is crucial for controlling the buffer sizes and processing loops within the OLA method.
     *
     * @note The frame size is typically set during initialization or when the OLA parameters are updated.
     */
    
	unsigned long mFrameSize;
    
    /**
     * @brief Stores the current hop size used in the Overlap-Add (OLA) process.
     *
     * This member variable holds the hop size (in samples) used for the Overlap-Add (OLA) operation. The hop size
     * determines the amount of overlap between consecutive audio frames, where a smaller hop size results in more
     * overlap. It is a critical parameter in controlling how smoothly frames are combined during the OLA process.
     *
     * @note The hop size is typically set during initialization or when OLA parameters are updated.
     */
    
	unsigned long mHopSize;
	
	// Maximums
	
    /**
     * @brief Stores the maximum allowable frame size for the Overlap-Add (OLA) process.
     *
     * This member variable holds the maximum frame size (in samples) that the Overlap-Add (OLA) process can handle.
     * It defines the upper limit for the size of the audio frames that can be processed, ensuring that internal
     * buffers and memory allocations are sized accordingly. The frame size used during processing should not exceed
     * this value.
     *
     * @note The maximum frame size is typically set during the initialization of the OLA object.
     */
    
	unsigned long mMaxFrameSize;
    
    /**
     * @brief Stores the maximum number of audio channels supported by the Overlap-Add (OLA) process.
     *
     * This member variable holds the maximum number of audio channels that the Overlap-Add (OLA) process
     * can handle. It defines the upper limit for the number of channels that can be processed simultaneously,
     * ensuring that the internal buffers and processing routines are allocated and managed appropriately.
     *
     * @note The maximum number of channels is typically set during the initialization of the OLA object.
     */
    
	unsigned long mMaxChans;
	
	// Update Parameters
	
    /**
     * @brief Stores the new frame size to be applied during the next Overlap-Add (OLA) process update.
     *
     * This member variable holds a new frame size (in samples) that is set to be applied during the next
     * update of the Overlap-Add (OLA) process. It allows the system to schedule changes in frame size
     * without immediately disrupting the ongoing processing. The new frame size will be adopted the next
     * time the parameters are updated or reset.
     *
     * @note This value may differ from the current `mFrameSize` until the update is applied.
     */
    
	unsigned long mNewFrameSize;
    
    /**
     * @brief Stores the new hop size to be applied during the next Overlap-Add (OLA) process update.
     *
     * This member variable holds a new hop size (in samples) that will be applied during the next update of the
     * Overlap-Add (OLA) process. It allows the system to schedule changes to the hop size, which controls the
     * overlap between audio frames, without immediately affecting the current processing. The new hop size will
     * take effect the next time the OLA parameters are updated or reset.
     *
     * @note This value may differ from the current `mHopSize` until the update is applied.
     */
    
	unsigned long mNewHopSize;
    
    /**
     * @brief Stores the new hop offset to be applied during the next Overlap-Add (OLA) process update.
     *
     * This member variable holds a new hop offset (in samples) that will be applied during the next update of the
     * Overlap-Add (OLA) process. The hop offset allows fine control over the alignment of frames during processing,
     * determining where the hop starts relative to the audio frames. This value will take effect the next time
     * the OLA parameters are updated or reset.
     *
     * @note This value may differ from the current hop offset until the update is applied.
     */
    
	unsigned long mNewHopOffset;
	
	// Reset
	
    /**
     * @brief Indicates whether the Overlap-Add (OLA) process should be reset.
     *
     * This boolean member variable signals whether the internal state of the Overlap-Add (OLA) process
     * should be reset. If `true`, the OLA process will reset its internal buffers and state, typically
     * in response to changes in parameters such as frame size or hop size. This flag ensures that the
     * system can reinitialize properly before continuing with audio processing.
     *
     * @note The value of this variable is checked during updates or reconfigurations of the OLA process.
     */
    
	bool mReset;
};


#endif
