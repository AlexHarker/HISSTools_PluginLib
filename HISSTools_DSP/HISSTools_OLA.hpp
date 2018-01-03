

#ifndef __HISSTOOLS_OLA__
#define __HISSTOOLS_OLA__


class HISSTools_OLA {
	
public:
	
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
	
	void virtual process(double *ioFrame, unsigned long frameSize)
	{
		// This function should be overridden for single channel operation. 
		// IO is on a single shared buffer
	}
	
	
	void virtual process(double **ioFrames, unsigned long frameSize, unsigned long nChans)
	{
		// This function should be overridden for multichannel operation. 
		// IO is on a single shared buffer per channel
	}
	
	
public:
	
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
	
	
	void setParams(unsigned long frameSize, unsigned long hopSize, bool reset = FALSE, unsigned long hopOffset = 0)
	{
		mNewFrameSize = frameSize < mMaxFrameSize ? (frameSize ? frameSize : 1) : mMaxFrameSize;
		mNewHopSize = hopSize <= mNewFrameSize ? (hopSize ? hopSize : 1) : mNewFrameSize;
		mNewHopOffset = hopOffset > mNewHopSize ? mNewHopSize : hopOffset;
		mReset = reset;
	}
	
	
private:
	
	// Data
	
	double *mInputBuffers[256];
	double *mOutputBuffers[256];
	double *mFrameBuffers[256];
	
	// Pointers
	
	long mBlockIOPointer;
	long mBlockHopPointer;
	
	// Current Parameters
	
	unsigned long mFrameSize;
	unsigned long mHopSize;
	
	// Maximums
	
	unsigned long mMaxFrameSize;
	unsigned long mMaxChans;
	
	// Update Parameters
	
	unsigned long mNewFrameSize;
	unsigned long mNewHopSize;
	unsigned long mNewHopOffset;
	
	// Reset
	
	bool mReset;
};


#endif