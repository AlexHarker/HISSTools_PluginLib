

#ifndef __HISSTOOLS_IOSTREAM__
#define __HISSTOOLS_IOSTREAM__


class HISSTools_IOStream {
    
public:
	enum IOStreamMode {kInput, kOutput};
    

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
		
	
	~HISSTools_IOStream()
	{		
		// Delete individual channel pointers

		for (unsigned long i = 0; i < mNChans; i++)
			delete[] mBuffers[i];
	}
	
		
    unsigned long getBufferSize()
    {
        return mBufferSize;
    }
    
    unsigned long getNChans()
    {
        return mNChans;
    }
    
	void reset()
	{
		for (unsigned long i = 0; i < mNChans; i++)
            memset(mBuffers[i], 0, mBufferSize * sizeof(double));
        
        mWriteOffset = mBufferSize;
	}
	
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
    
    bool read(double *output, unsigned long size, unsigned long outputOffset)
    {
        return read(&output, 1UL, size, outputOffset);
    }
	
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
    
    bool write(double *input, unsigned long size, unsigned long inputOffset)
    {
        return write(&input, 1, size, inputOffset);
    }


private:
	
    // Mode
    
    const IOStreamMode mMode;
    
	// Data
	
	double *mBuffers[256];
	
	// Pointers
	
	unsigned long mBufferCounter;
	unsigned long mWriteOffset;
	
	// Sizes
	
	const unsigned long mBufferSize;
    const unsigned long mNChans;
};


#endif