

#ifndef __HISSTOOLS_DWT__
#define __HISSTOOLS_DWT__


class HISSTools_Wavelet
{
	
public:

	// Note that analysis filters should be stored in reverse order, as they are applied through correlation, rather than convolution.....
	
	HISSTools_Wavelet()
	{
		mForwardLoPass = 0;
		mForwardHiPass = 0;
		mInverseLoPass = 0;
		mInverseHiPass = 0;
		
		mForwardLength = 0;
		mInverseLength = 0;
		
		mForwardOffset = 0;
		mInverseOffset = 0;
		
		mInverseIndependent = TRUE;
	}
	
	
	HISSTools_Wavelet(const double *loPass, unsigned long length, long offset = 0)
	{
		HISSTools_Wavelet();
		setForwardFilters(loPass, length, offset);
		setInverseFilters();
	}
	
	
	~HISSTools_Wavelet()
	{
		delete[] mForwardLoPass;
		delete[] mForwardHiPass;
		
		if (mInverseIndependent == TRUE)
		{
			delete[] mInverseLoPass;
			delete[] mInverseHiPass;
		}
	}
	
	
	virtual void setForwardFilters(const double *loPass, unsigned long length, long offset = 0)
	{
		unsigned long i;
		double flip;
		
		delete[] mForwardLoPass;
		delete[] mForwardHiPass;
		
		mForwardLoPass = new double[length];
		mForwardHiPass = new double[length];
		
		if (!mForwardLoPass || !mForwardHiPass)
		{
			delete[] mForwardLoPass;
			delete[] mForwardHiPass;
			
			length = 0;
			offset = 0;
		}
		
		for (i = 0; i < length; i++)
			mForwardLoPass[i] = loPass[i];
		
		for (i = 0, flip = 1; i < length; i++, flip *= -1)
			mForwardHiPass[i] = mForwardLoPass[length - i - 1] * flip;
				
		mForwardLength = length;
		mForwardOffset = offset;
	}
	
	
	virtual void setInverseFilters(const double *loPass, unsigned long length, long offset = 0)
	{
		unsigned long i;
		double flip;
		
		delete[] mInverseLoPass;
		delete[] mInverseHiPass;
		
		mInverseLoPass = new double[length];
		mInverseHiPass = new double[length];
		
		if (!mInverseLoPass || !mInverseHiPass)
		{
			delete[] mInverseLoPass;
			delete[] mInverseHiPass;
			
			length = 0;
			offset = 0;
		}
		
		for (i = 0; i < length; i++)
			mInverseLoPass[i] = loPass[i];
		
		for (i = 0, flip = 1; i < length; i++, flip *= -1)
			mInverseHiPass[i] = mInverseLoPass[length - i - 1] * flip;
		
		mInverseLength = length;
		mInverseOffset = offset;
		mInverseIndependent = TRUE;
	}
	
	
	virtual void setInverseFilters()
	{
		mInverseLength = mForwardLength;
		mInverseOffset = mForwardOffset;
		mInverseLoPass = mForwardLoPass;
		mInverseHiPass = mForwardHiPass;
		
		mInverseIndependent = false;
	}
	
		
	// FIR Filters
	
	double *mForwardLoPass;
	double *mForwardHiPass;
	double *mInverseLoPass;
	double *mInverseHiPass;
	
	// Parameters
	
	unsigned long mForwardLength;
	unsigned long mInverseLength;
	
	unsigned long mForwardOffset;
	unsigned long mInverseOffset;
	
	bool mInverseIndependent;
};


class HISSTools_DWT
{
	
public:
	
	HISSTools_DWT(unsigned long maxLength)
	{
		mTemp = new double[maxLength];
		
		if (mTemp)
			mMaxLength = maxLength;
		else 
			mMaxLength = 0;
	}
	
	
	~HISSTools_DWT()
	{
		delete[] mTemp;
	}
	
	
private:
	
	bool forwardDWT (double *in, double *out, unsigned long length, HISSTools_Wavelet *wavelet)
	{
		double *loPass = wavelet->mForwardLoPass;
		double *hiPass = wavelet->mForwardHiPass;
		
		unsigned long waveletLength = wavelet->mForwardLength;
		long offset = wavelet->mForwardOffset;
		long i, j, k;
		
		double lo, hi, in_val;
		
		// Sanity Check
		
		if (waveletLength > length)
			FALSE;
				
		// Loop by output sample
		
		for (i = 0; i < length >> 1; i++)
		{		
			lo = 0.;
			hi = 0.;
			k = (i << 1) + offset;
			
			while (k < 0)
				k += length;			
			while (k >= length)
				k -= length;
			
			// Loop over FIR
			
			for (j = 0; j < waveletLength && k < length; j++, k++)
			{
				in_val = in[k];
				lo += loPass[j] * in_val;
				hi += hiPass[j] * in_val;
			}
			
			// Do wrap
			
			for (k -= length; j < waveletLength; j++, k++)
			{
				in_val = in[k];
				lo += loPass[j] * in_val;
				hi += hiPass[j] * in_val;
			}
			
			out[i] = lo; 
			out[i + (length >> 1)] = hi;
		}
		
		return TRUE;
	}
	
	
	bool inverseDWT (double *in, double *out, unsigned long length, HISSTools_Wavelet *wavelet)
	{
		double *loPass = wavelet->mInverseLoPass;
		double *hiPass = wavelet->mInverseHiPass;
		
		unsigned long waveletLength = wavelet->mInverseLength;
		long offset = wavelet->mInverseOffset;
		long i, j, k;
			
		// Sanity Check
		
		if (waveletLength > length)
			return FALSE;
		
		// Zero output
		
		for (i = 0; i < length; i++)
			out[i] = 0;
		
		// Loop by output sample
		
		for (i = 0; i < length >> 1; i++)
		{		
			k = (i << 1) + offset;
			
			while (k < 0)
				k += length;			
			while (k >= length)
				k -= length;
			
			// Loop over FIR
			
			for (j = 0; j < waveletLength && k < length; j++, k++)
				out[k] += loPass[j] * in[i] + hiPass[j] * in[i + (length >> 1)];
			
			// Do wrap
			
			for (k -= length; j < waveletLength; j++, k++)
				out[k] += loPass[j] * in[i] + hiPass[j] * in[i + (length >> 1)];
		}
		
		return TRUE;
	}
	
	
public:
	
	bool forwardDWT (double *in, double *out, unsigned long length, unsigned long levels, HISSTools_Wavelet *wavelet)
	{
		bool success = TRUE;
		double *temp = mTemp;
		unsigned long i, j;
		
		// Sanity Check

		if (length > mMaxLength)
			return FALSE;
		
		for (i = 0; i < levels; i++, length >>= 1)
		{
			if(forwardDWT(in, temp, length, wavelet) == FALSE)
				success = FALSE;
			
			for (j = 0; j < length; j++)
				out[j] = temp[j];
		}
		
		return success;

	}
	
	
	bool inverseDWT (double *in, double *out, unsigned long length, unsigned long levels, HISSTools_Wavelet *wavelet)
	{
		bool success = TRUE;
		double *temp = mTemp;
		unsigned long i, j;
		
		// Sanity Check
		
		if (length > mMaxLength)
			return FALSE;
		
		length >>= (levels - 1);
		
		for (i = 0; i < levels; i++, length <<= 1)
		{
			if (inverseDWT(in, temp, length, wavelet) == FALSE)
				success = FALSE;
			
			for (j = 0; j < length; j++)
				out[j] = temp[j];
		}
		
		return success;
	}
	
	
	bool forwardDWT (double *io, unsigned long length, unsigned long levels, HISSTools_Wavelet *wavelet)
	{
		return forwardDWT (io, io, length, levels, wavelet);
	}
	
	
	bool inverseDWT (double *io, unsigned long length, unsigned long levels, HISSTools_Wavelet *wavelet)
	{
		return inverseDWT (io, io, length, levels, wavelet);
	}	
	
	
private:
	
	// Temp Data
	
	double *mTemp;
	
	// Maimum Length
	
	unsigned long mMaxLength;
	
};

#endif