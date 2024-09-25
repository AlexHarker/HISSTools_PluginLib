
/**
 * @file HISSTools_Frame.hpp
 * @brief Header file for the HISSTools_Frame class, which manages audio frame processing.
 *
 * This file defines the HISSTools_Frame class and its associated methods and member variables.
 * The class provides functionality for handling audio frames, including streaming data into
 * frames, processing multi-channel and single-channel audio data, and managing hop counters
 * for efficient audio stream processing. It also offers flexibility for controlling frame size,
 * hop size, and other audio stream parameters.
 *
 * Key functionalities:
 * - Frame size and hop size management
 * - Single and multi-channel audio processing
 * - Fractional hop offsets for sub-sample precision
 * - Resetting internal states for reinitializing streams and hop counters
 *
 * This class is part of the HISSTools library, which provides a set of tools for advanced audio processing.
 */

#ifndef __HISSTOOLS_FRAME__
#define __HISSTOOLS_FRAME__

#include <cmath>
#include "HISSTools_IOStream.hpp"

/**
 * @brief A class for handling audio frame data.
 *
 * The HISSTools_Frame class provides functionality for managing and processing audio frames,
 * including handling multiple channels and frame sizes. It allows for efficient audio data
 * manipulation, storage, and retrieval.
 */

class HISSTools_Frame {
	
public:
	
    /**
     * @brief Constructor for HISSTools_Frame.
     *
     * This constructor initializes a HISSTools_Frame object with the specified maximum frame size and maximum number of channels.
     *
     * @param maxFrameSize The maximum size of a frame (in samples).
     * @param maxChans The maximum number of channels to be used.
     */
    
	HISSTools_Frame(unsigned long maxFrameSize, unsigned long maxChans)
	{
        mInputStream = new HISSTools_IOStream(HISSTools_IOStream::kInput, maxFrameSize, maxChans);
        
        mMaxFrameSize = mInputStream->getBufferSize();
        mNChans = mInputStream->getNChans();
        
		// Allocate individual channel pointers
		
		for (unsigned long i = 0; i < mNChans; i++)
			mFrameBuffers[i] = new double[mMaxFrameSize];
	
        mBlockHopCounter = 0;
        mHopShift = 0;
        
        reset();
		setParams(maxFrameSize, maxFrameSize, TRUE);
	}
		
    /**
     * @brief Destructor for HISSTools_Frame.
     *
     * Cleans up and releases any resources used by the HISSTools_Frame object.
     * This ensures proper memory management and prevents memory leaks when
     * an instance of HISSTools_Frame is destroyed.
     */
    
	~HISSTools_Frame()
	{
        // Delete Stream
        
        delete mInputStream;
        
		// Delete individual channel pointers

		for (unsigned long i = 0; i < mNChans; i++) 
            delete[] mFrameBuffers[i];
	}
	
	
private:
    
    /**
     * @brief Get the current hop counter value.
     *
     * Retrieves the current value of the hop counter, which tracks the number of hops or steps
     * processed in the frame.
     *
     * @return The current hop counter as a double.
     */
    
    double getHopCounter()
    {
        // FIX (use division and rounding to make this fast)
        
        double hopCounter = mBlockHopCounter - mHopShift;
        
        mHopShift = 0;
        
        if (mHopSize)
        {
            // Wrap
        
            while (hopCounter < 0)
                hopCounter += mHopSize;
            while (hopCounter > mHopSize + 1.0)
                hopCounter -= mHopSize;
        }
        
        return hopCounter;
    }
    
    /**
     * @brief Stream input data into the frame buffer.
     *
     * This method takes a stream of input audio data and populates the frame buffer with the given samples.
     * It supports both multi-channel and single-channel input, depending on the SingleChannel flag.
     *
     * @param ins A 2D array of input audio data, where each pointer in the array represents a channel.
     * @param nChans The number of channels in the input data.
     * @param nSamps The number of samples per channel.
     * @param SingleChannel A flag indicating whether to treat the input as single-channel data
     *                      (if true, the same data will be copied across all channels).
     * @return True if the data was successfully streamed into the frame, otherwise false.
     */
    
    bool streamToFrame(double **ins, unsigned long nChans, unsigned long nSamps, bool SingleChannel)
	{
        bool processedFrames = FALSE;
        
		unsigned long frameSize;
        unsigned long loopSize;
        
		double hopSize;
        double hopCounter;
        
		// Sanity Check
        
		if (nChans > mNChans)
			return FALSE;
        
        // Reset if relevant (none of this setup is threadsafe)
		
        if (mResetStrean == TRUE)
        {
            mInputStream->reset();
            mResetStrean = FALSE;
        }
        
        if (mResetHopCount == TRUE)
        {
            mBlockHopCounter = 0.0;
            mResetHopCount = FALSE;
        }
        
		// Get parameters
		
		frameSize = mFrameSize;
		hopSize = mHopSize;
		hopCounter = getHopCounter();
		
		// Loop over vector grabbing frames as appropriate
		
		for (unsigned long i = 0; i < nSamps; i += loopSize, hopCounter += loopSize)
		{
			// Grab a frame and process
			
			if (hopSize && hopCounter >= hopSize)
			{
                processedFrames = TRUE;
                
                hopCounter -= hopSize;
                hopCounter = hopCounter <= 0.0 ? 0.0: hopCounter;
                hopCounter = hopCounter >= 1.0 ? 0.0: hopCounter;
                
                mInputStream->read(mFrameBuffers, nChans, frameSize, 0);
				
                if (SingleChannel == TRUE)
                    process(mFrameBuffers[0], frameSize, hopCounter ? 1.0 - hopCounter : 0.0);
                else
                    process(mFrameBuffers, frameSize, nChans, hopCounter ? 1.0 - hopCounter : 0.0);
			}
			
			// Check loop size
			
            unsigned long hopRemain = (unsigned long) ceil(hopSize - hopCounter);
            unsigned long blockRemain = nSamps - i;
            
            loopSize = (hopRemain && (hopRemain < blockRemain)) ? hopRemain : blockRemain;
			
            mInputStream->write(ins, nChans, loopSize, i);
		}
		
		mBlockHopCounter = hopCounter;
		
		return processedFrames;
	}

	
protected:
	
    /**
     * @brief Process an input frame of audio data.
     *
     * This virtual method processes an input frame of audio data. Derived classes should implement
     * their specific audio processing logic by overriding this method. The method operates on the
     * input frame provided and processes it according to the frame size.
     *
     * @param iFrame A pointer to the input frame of audio data.
     * @param frameSize The number of samples in the input frame.
     */
    
	void virtual process(double *iFrame, unsigned long frameSize)
	{
		// This function should be overridden for single channel operation (where you wish to ignore fractional offsets).
	}
	
    /**
     * @brief Process multiple input frames of audio data.
     *
     * This virtual method processes multiple input frames of audio data across multiple channels.
     * Derived classes should implement their specific multi-channel audio processing logic by overriding
     * this method. The method operates on the input frames provided, processing the data based on the
     * number of samples and channels.
     *
     * @param iFrames A 2D array of input frames, where each pointer in the array represents a channel's frame of audio data.
     * @param frameSize The number of samples per frame.
     * @param nChans The number of audio channels to be processed.
     */
    
	void virtual process(double **iFrames, unsigned long frameSize, unsigned long nChans)
	{
		// This function should be overridden for multichannel operation (where you wish to ignore fractional offsets).
	}
	
    /**
     * @brief Process an input frame of audio data with a fractional offset.
     *
     * This virtual method processes an input frame of audio data, applying a fractional offset to the processing.
     * Derived classes should override this method to implement specific audio processing that accounts for the
     * offset, which can be used to manage sub-sample accuracy or other timing-based effects.
     *
     * @param iFrame A pointer to the input frame of audio data.
     * @param frameSize The number of samples in the input frame.
     * @param fractionalOffset A fractional value indicating an offset to be applied during the processing,
     *                         typically used for sub-sample timing adjustments.
     */
    
    void virtual process(double *iFrame, unsigned long frameSize, double fractionalOffset)
	{
		// This function should be overridden for single channel operation (where you wish to receive fractional offsets)
        
        process(iFrame, frameSize);
	}
	
    /**
     * @brief Process multiple input frames of audio data with a fractional offset.
     *
     * This virtual method processes multiple input frames of audio data across multiple channels,
     * applying a fractional offset for sub-sample accuracy or timing adjustments.
     * Derived classes should override this method to implement specific multi-channel audio processing
     * that accounts for both the frame size, number of channels, and the fractional offset.
     *
     * @param iFrames A 2D array of input frames, where each pointer in the array represents a channel's frame of audio data.
     * @param frameSize The number of samples per frame.
     * @param nChans The number of audio channels to be processed.
     * @param fractionalOffset A fractional value indicating an offset to be applied during the processing,
     *                         typically used for sub-sample timing adjustments.
     */
    
	void virtual process(double **iFrames, unsigned long frameSize, unsigned long nChans, double fractionalOffset)
	{
		// This function should be overridden for multichannel operation where you wish to receive fractional offsets).
        
        process(iFrames, frameSize, nChans);
	}
	
public:
	
    /**
     * @brief Stream single-channel input data into the frame buffer.
     *
     * This method takes a stream of single-channel input audio data and populates the frame buffer with the given samples.
     *
     * @param in A pointer to the input audio data for a single channel.
     * @param nSamps The number of samples to be streamed into the frame.
     * @return True if the data was successfully streamed into the frame, otherwise false.
     */
    
    bool streamToFrame(double *in, unsigned long nSamps)
	{
		return streamToFrame(&in, 1UL, nSamps, TRUE);
    }
	
    /**
     * @brief Stream multi-channel input data into the frame buffer.
     *
     * This method takes a stream of multi-channel input audio data and populates the frame buffer with the given samples.
     * Each channel's data is processed independently based on the number of samples per channel.
     *
     * @param ins A 2D array of input audio data, where each pointer in the array represents a channel's data.
     * @param nChans The number of channels in the input data.
     * @param nSamps The number of samples per channel to be streamed into the frame.
     * @return True if the data was successfully streamed into the frame, otherwise false.
     */
    
    bool streamToFrame(double **ins, unsigned long nChans, unsigned long nSamps)
	{
		return streamToFrame(ins, nChans, nSamps, FALSE);
    }
	
    /**
     * @brief Adjust the hop counter by a specified offset.
     *
     * This method adjusts the internal hop counter by the given offset value. The hop counter is
     * typically used for tracking the progression of audio frames, and this method allows for
     * manual adjustment of the hop timing.
     *
     * @param offset The value by which to offset the hop counter, typically representing a fractional
     *               or whole frame offset.
     */
    
    void offsetHop(double offset)
    {
        mHopShift += offset;
    }
    
    /**
     * @brief Set parameters for frame size and hop size with optional immediate application and offset.
     *
     * This method configures the frame size and hop size for audio processing. It also provides options
     * to immediately apply the changes and set an initial hop offset.
     *
     * @param frameSize The size of the frame in samples.
     * @param hopSize The hop size, typically representing the number of samples between successive frames.
     * @param immediate If true, applies the parameter changes immediately; if false, changes will be applied later.
     * @param hopOffset An optional offset to apply to the hop counter, typically used for sub-sample timing adjustments. Default is 0.
     */
    
	void setParams(unsigned long frameSize, double hopSize, bool immediate = FALSE, double hopOffset = 0)
	{
		mFrameSize = std::max(1UL, std::min(mMaxFrameSize, frameSize));
		mHopSize = hopSize ? std::max(1.0, std::fabs(hopSize)) : 0.0;
        
        if (immediate == TRUE)
        {
            mHopShift = hopOffset;
            mResetHopCount = TRUE;
        }
        else
        {
            offsetHop(hopOffset);
            mResetHopCount = FALSE;
        }
	}
    
    /**
     * @brief Reset the internal state of the frame processor.
     *
     * This method resets the internal state of the frame processor, including the hop counter and
     * any other relevant processing states. It is typically used to reinitialize the processor
     * before starting a new audio stream or sequence of frames.
     */
    
	void reset()
	{
        mResetStrean = TRUE;
        mResetHopCount = TRUE;
	}
	
// FIX - look at what is private here....
// FIX - add last frame facility
    
	
private:
	
    /**
     * @brief A pointer to the input stream object.
     *
     * This member variable holds a pointer to an instance of HISSTools_IOStream,
     * which is used to manage and handle the input stream of audio data for the frame processor.
     * It facilitates the reading of audio data from the input stream during processing.
     */
    
	HISSTools_IOStream *mInputStream;

protected:
    
    /**
     * @brief An array of pointers to frame buffers for storing audio data.
     *
     * This member variable is an array of pointers, each pointing to a buffer that holds audio data for a specific channel.
     * It allows the storage of up to 256 channels of audio data, with each buffer corresponding to a channel's frame.
     */
    
	double *mFrameBuffers[256];

private:

	// Hop Parameters
	
    /**
     * @brief A variable to track the hop counter for audio processing blocks.
     *
     * This member variable holds the current value of the block hop counter, which tracks the
     * progress of audio processing in terms of hops or steps between successive blocks of audio data.
     * It is typically used to manage timing and synchronization during audio processing.
     */
    
	double mBlockHopCounter;
    
    /**
     * @brief The size of the hop between successive frames in audio processing.
     *
     * This member variable defines the hop size, which represents the number of samples to move forward
     * between successive audio frames during processing. It is used to control the overlap or spacing
     * of frames in the audio stream.
     */
    
	double mHopSize;
    
    /**
     * @brief The shift applied to the hop during audio processing.
     *
     * This member variable stores the amount of shift applied to the hop size.
     * It is typically used to adjust the position or timing of the hop in relation to
     * the current frame, allowing for more precise control over frame progression.
     */
    
    double mHopShift;
    
	// Frame Size
	
    /**
     * @brief The size of the audio frame in samples.
     *
     * This member variable defines the number of samples in a single frame of audio data.
     * It is used to determine the length of each frame during audio processing and buffer management.
     */
    
	unsigned long mFrameSize;

    // Maximums
	
    /**
     * @brief The maximum allowable size of an audio frame in samples.
     *
     * This member variable defines the maximum number of samples that an audio frame can hold.
     * It is used to allocate sufficient buffer space and ensure that frames do not exceed this limit during processing.
     */
    
	unsigned long mMaxFrameSize;
    
    /**
     * @brief The number of audio channels being processed.
     *
     * This member variable holds the number of channels in the audio data. It is used to manage
     * multi-channel audio processing, ensuring that the correct number of frame buffers are allocated
     * and that all channels are properly handled during processing.
     */
    
	unsigned long mNChans;
    
	// Reset
	
    /**
     * @brief A flag indicating whether the audio stream should be reset.
     *
     * This member variable is a boolean flag that, when set to true, signals that the audio stream
     * needs to be reset. This is typically used to reinitialize the stream or reset internal states
     * before starting a new sequence of audio frames.
     */
    
	bool mResetStrean;
    
    /**
     * @brief A flag indicating whether the hop counter should be reset.
     *
     * This member variable is a boolean flag that, when set to true, signals that the hop counter
     * needs to be reset. This is typically used to reinitialize the hop counting mechanism, ensuring
     * accurate tracking of frame progression during audio processing.
     */
    
    bool mResetHopCount;
};


#endif
