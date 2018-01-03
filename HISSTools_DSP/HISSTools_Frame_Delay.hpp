

#ifndef __HISSTOOLS_FRAME_DELAY__
#define __HISSTOOLS_FRAME_DELAY__


class HISSTools_Frame_Delay
{
	
public:
	
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
	
	
	~HISSTools_Frame_Delay() 
	{		
		// Delete individual channel pointers
		
		for (unsigned long i = 0; i < mMaxChans; i++)
			delete[] mFrameData[i];
		
		// Delete channel array
		
		delete[] mFrameData;
	};
	
	
private:
	
	void reset(unsigned long frameSize)
	{
		mFrameSize = frameSize;
		mValidFrames = 0;
		mPointer = 0;
		mClear = FALSE;
	}

	
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

	
	void delayIO(double *in, double *out, long size, long frameDelay)
	{
		delayIO(&in, &out, size, 1, frameDelay);
	}
	
	
	void clear()
	{
		mClear = TRUE;
	}

	
private:
	
	// Data
	
	double **mFrameData;
	
	// Current Parameters
	
	unsigned long mValidFrames;
	unsigned long mPointer;
	unsigned long mFrameSize;
	
	// Maximums
	
	unsigned long mMaxFrameSize;
	unsigned long mMaxNumFrames;
	unsigned long mMaxChans;
	
	// Clear
	
	bool mClear;
};


#endif