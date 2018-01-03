

#ifndef __HISSTOOLS_FRAME__
#define __HISSTOOLS_FRAME__

#include <cmath>
#include "HISSTools_IOStream.hpp"

class HISSTools_Frame {
	
public:
	
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
		
	
	~HISSTools_Frame()
	{
        // Delete Stream
        
        delete mInputStream;
        
		// Delete individual channel pointers

		for (unsigned long i = 0; i < mNChans; i++) 
            delete[] mFrameBuffers[i];
	}
	
	
private:
    
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
	
	void virtual process(double *iFrame, unsigned long frameSize)
	{
		// This function should be overridden for single channel operation (where you wish to ignore fractional offsets).
	}
	
	void virtual process(double **iFrames, unsigned long frameSize, unsigned long nChans)
	{
		// This function should be overridden for multichannel operation (where you wish to ignore fractional offsets).
	}
	
    void virtual process(double *iFrame, unsigned long frameSize, double fractionalOffset)
	{
		// This function should be overridden for single channel operation (where you wish to receive fractional offsets)
        
        process(iFrame, frameSize);
	}
	
	void virtual process(double **iFrames, unsigned long frameSize, unsigned long nChans, double fractionalOffset)
	{
		// This function should be overridden for multichannel operation where you wish to receive fractional offsets).
        
        process(iFrames, frameSize, nChans);
	}
	
public:
	
	   
    bool streamToFrame(double *in, unsigned long nSamps)
	{
		return streamToFrame(&in, 1UL, nSamps, TRUE);
    }
	
    bool streamToFrame(double **ins, unsigned long nChans, unsigned long nSamps)
	{
		return streamToFrame(ins, nChans, nSamps, FALSE);
    }
	
    void offsetHop(double offset)
    {
        mHopShift += offset;
    }
        
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
    
	void reset()
	{
        mResetStrean = TRUE;
        mResetHopCount = TRUE;
	}
	
// FIX - look at what is private here....
// FIX - add last frame facility
    
	
private:
	
	HISSTools_IOStream *mInputStream;

protected:
	double *mFrameBuffers[256];

private:

	// Hop Parameters
	
	double mBlockHopCounter;
	double mHopSize;
    double mHopShift;
    
	// Frame Size
	
	unsigned long mFrameSize;

    // Maximums
	
	unsigned long mMaxFrameSize;
	unsigned long mNChans;
    
	// Reset
	
	bool mResetStrean;
    bool mResetHopCount;
};


#endif