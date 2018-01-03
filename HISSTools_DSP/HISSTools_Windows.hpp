

#ifndef __HISSTOOLS_WINDOWS__
#define __HISSTOOLS_WINDOWS__


#include <math.h>


#define WIND_PI				3.14159265358979323846
#define WIND_TWOPI			6.28318530717958647692
#define WIND_THREEPI		9.42477796076937971538
#define WIND_FOURPI			12.56637061435817295384
#define WIND_SIXPI			18.84955592153875943076


enum WindowTypes {
	
	WIND_VON_HANN = 0,
	WIND_HAMMING = 1,
	WIND_KAISER = 2,
	WIND_TRIANGLE = 3,
	WIND_COSINE = 4,
	WIND_BLACKMAN = 5,
	WIND_BLACKMAN_62 = 6,
	WIND_BLACKMAN_70 = 7,
	WIND_BLACKMAN_74 = 8,
	WIND_BLACKMAN_92 = 9,
	WIND_BLACKMAN_HARRIS = 10,
	WIND_FLAT_TOP = 11,
	WIND_RECT = 12,
};


enum GainTypes {
	
	WIND_NO_GAIN = 0,
	WIND_LIN_GAIN = 1,
	WIND_SQ_GAIN = 2,
	WIND_SQ_OVER_LIN_GAIN = 3,
};

class HISSTools_Windows
{
	
public:
	
	HISSTools_Windows(unsigned long maxwindowSize) 
	{
		if (maxwindowSize < 1)
			maxwindowSize = 1;
		
		mWindow = new double[maxwindowSize];	
		
		if (mWindow)
			mMaxWindowSize = maxwindowSize;
		else
			mMaxWindowSize = 0;
		
		// Force calculation on the first call
		
		mWindowSize = -1;			
	};
	
	
	~HISSTools_Windows() 
	{
		delete[] mWindow;
	};
	
	
	bool applyWindow(double *in, double *out, WindowTypes windowType, unsigned long windowSize, bool sqrtWindow, double fixedGain, GainTypes compensateWindowGain)
	{
		double *window = mWindow;
		double gain = fixedGain;
		
		// Sanity Check

		if (windowSize > mMaxWindowSize)
			return false;
		
		if (windowSize != mWindowSize || windowType != mWindowType || sqrtWindow != mSqrtWindow)
			calculateWindow(windowSize, windowType, sqrtWindow);
		
		if (compensateWindowGain == WIND_LIN_GAIN)
			gain /= mWindowLinGain;
		if (compensateWindowGain == WIND_SQ_GAIN)
			gain /= mWindowSqGain;
		if (compensateWindowGain == WIND_SQ_OVER_LIN_GAIN)
			gain /= (mWindowSqGain / mWindowLinGain);
		
		for (long i = 0; i < windowSize; i++)
			out[i] = in[i] * window[i] * gain;
		
		return true;
	}
	
	
	void applyWindow(double *io, WindowTypes windowType, unsigned long windowSize, bool sqrtWindow, double fixedGain, GainTypes compensateWindowGain)
	{
		applyWindow(io, io, windowType, windowSize, sqrtWindow, fixedGain, compensateWindowGain);
	}
	
	
private:
	
	double IZero(double xSq)
	{
		unsigned long i;
		double newTerm = 1;
		double bFunction = 1;
		
		for (i = 1; newTerm; i++)		// Gives Maximum Accuracy
		{
			newTerm = newTerm * xSq * (1.0 / (4.0 * (double) i * (double) i));
			bFunction += newTerm;
		}
		
		return bFunction;
	}
	
	
	void calculateWindow(unsigned long windowSize, WindowTypes windowType, long sqrtWindow)
	{
		double *window = mWindow;
		
		double windowLinGain, windowSqGain, alpha, alphaBesselRecip, xSq, val;
		
		long halfWindowSize = windowSize >> 1;
		unsigned long i;
		
		switch (windowType)
		{
			case WIND_VON_HANN:
				for (i = 0; i < windowSize; i++)
					window[i] = 0.5 - (0.5 * cos(WIND_TWOPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_HAMMING:
				for (i = 0; i < windowSize; i++)
					window[i] = 0.54347826 - (0.45652174 * cos(WIND_TWOPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_KAISER:
				
				// First find bessel function of alpha
				
				alpha = 6.8;
				alphaBesselRecip = 1. / IZero(alpha * alpha);
								
				for (i = 0; i < windowSize; i++)
				{
					// Kaiser window
					
					val = ((double) i - halfWindowSize) / (double) halfWindowSize;
					xSq = (1.0 - val * val) * alpha * alpha;		
					window[i] = IZero(xSq) * alphaBesselRecip;
				}
				break;
				
			case WIND_TRIANGLE:
				for (i = 0; i < (windowSize >> 1); i++)
					window[i] = (double) i / (double) (windowSize / 2.0);
				for (; i < windowSize; i++)
					window[i] = (double) (((double) windowSize - 1.0) - (double) i) / (double) (windowSize / 2.0);
				break;
				
			case WIND_COSINE:
				for (i = 0; i < windowSize; i++)
					window[i] = sin(WIND_PI * ((double) i / (double) windowSize));
				break;
				
			case WIND_BLACKMAN:
				for (i = 0; i < windowSize; i++)
					window[i] = 0.42659071 - (0.49656062 * cos(WIND_TWOPI * ((double) i / (double) windowSize))) + (0.07684867 * cos(WIND_FOURPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_BLACKMAN_62:
				for (i = 0; i < windowSize; i++)
					window[i] = (0.44859f - 0.49364f * cos(WIND_TWOPI * ((double) i / (double) windowSize)) + 0.05677f * cos(WIND_FOURPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_BLACKMAN_70:
				for (i = 0; i < windowSize; i++)
					window[i] = (0.42323f - 0.49755f * cos(WIND_TWOPI * ((double) i / (double) windowSize)) + 0.07922f * cos(WIND_FOURPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_BLACKMAN_74:
				for (i = 0; i < windowSize; i++)
					window[i] = (0.402217f - 0.49703f * cos(WIND_TWOPI * ((double) i / (double) windowSize)) + 0.09892f * cos(WIND_FOURPI * ((double) i / (double) windowSize)) - 0.00188 * cos(WIND_THREEPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_BLACKMAN_92:
				for (i = 0; i < windowSize; i++)
					window[i] = (0.35875f - 0.48829f * cos(WIND_TWOPI * ((double) i / (double) windowSize)) + 0.14128f * cos(WIND_FOURPI * ((double) i / (double) windowSize)) - 0.01168 * cos(WIND_THREEPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_BLACKMAN_HARRIS:
				for (i = 0; i < windowSize; i++)
					window[i] = 0.35875 - (0.48829 * cos(WIND_TWOPI * ((double) i / (double) windowSize))) + (0.14128 * cos(WIND_FOURPI * ((double) i / (double) windowSize))) - (0.01168 * cos(WIND_SIXPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_FLAT_TOP:
				for (i = 0; i < windowSize; i++)
					window[i] = 0.2810639 - (0.5208972 * cos(WIND_TWOPI * ((double) i / (double) windowSize))) + (0.1980399 * cos(WIND_FOURPI * ((double) i / (double) windowSize)));
				break;
				
			case WIND_RECT:
				for (i = 0; i < windowSize; i++)
					window[i] = 1.;
				break;
		}
		
		if (sqrtWindow == true)
		{
			for (i = 0; i < windowSize; i++)
				window[i] = sqrt(window[i]);
		}
		
		// Calculate the gain of the window
		
		for (i = 0, windowLinGain = 0.; i < windowSize; i++)
			windowLinGain += window[i];
		windowLinGain /= (double) windowSize;
				
		for (i = 0, windowSqGain = 0.; i < windowSize; i++)
			windowSqGain += window[i] * window[i];
		windowSqGain /= (double) windowSize;

		mWindowSize = windowSize;
		mWindowType = windowType;
		mSqrtWindow = sqrtWindow;
		mWindowLinGain = windowLinGain;
		mWindowSqGain = windowSqGain;
	}
	
	
private:
	
	// Window
	
	double *mWindow;
	
	// Current Parameters
	
	unsigned long mWindowSize;
	WindowTypes mWindowType;
	bool mSqrtWindow;
	
	// Gain Values
	
	double mWindowLinGain;	
	double mWindowSqGain;	
	
	// Maximum Size
	
	unsigned long mMaxWindowSize;
};


#endif