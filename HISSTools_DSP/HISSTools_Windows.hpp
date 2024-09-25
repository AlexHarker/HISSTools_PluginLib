
/**
 * @file HISSTools_Windows.hpp
 * @brief Defines the HISSTools_Windows class for applying windowing functions in signal processing.
 *
 * This file contains the declaration of the HISSTools_Windows class and related enumerations.
 * The class provides methods to apply various windowing functions (e.g., Hann, Hamming) to input signals,
 * with options for gain compensation and square root processing. It also manages internal storage for window values.
 *
 * The windowing functions are essential for tasks such as spectral analysis, filtering, or any operation
 * that involves smoothing signal edges in time-domain processing.
 */

#ifndef __HISSTOOLS_WINDOWS__
#define __HISSTOOLS_WINDOWS__


#include <math.h>


#define WIND_PI				3.14159265358979323846
#define WIND_TWOPI			6.28318530717958647692
#define WIND_THREEPI		9.42477796076937971538
#define WIND_FOURPI			12.56637061435817295384
#define WIND_SIXPI			18.84955592153875943076

/**
 * @brief Enumeration of different window types.
 *
 * This enum defines the various types of windows that can be used in signal processing.
 * Each type corresponds to a specific windowing function.
 */

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

/**
 * @brief Enumeration of different gain types.
 *
 * This enum defines the various types of gain adjustments that can be applied.
 * Each gain type represents a specific method or mode of adjusting the signal amplitude.
 */

enum GainTypes {
	
	WIND_NO_GAIN = 0,
	WIND_LIN_GAIN = 1,
	WIND_SQ_GAIN = 2,
	WIND_SQ_OVER_LIN_GAIN = 3,
};

class HISSTools_Windows
{
	
public:
	
    /**
     * @brief Constructs an instance of the HISSTools_Windows class.
     *
     * This constructor initializes the HISSTools_Windows object and sets the maximum allowable window size.
     *
     * @param maxwindowSize The maximum size of the window in samples or points.
     */
    
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
	
    /**
     * @brief Destructor for the HISSTools_Windows class.
     *
     * This destructor is responsible for cleaning up any resources or memory allocated by the HISSTools_Windows object.
     * It ensures proper deallocation when the object goes out of scope or is deleted.
     */
    
	~HISSTools_Windows()
	{
		delete[] mWindow;
	};
	
    /**
     * @brief Applies a windowing function to the input data.
     *
     * This method applies a selected windowing function to the input array `in` and stores the result in the output array `out`.
     * It allows for customization of the window type, size, and optional square-root processing.
     * Additionally, a fixed gain can be applied, and gain compensation can be selected for the window.
     *
     * @param in Pointer to the input data array.
     * @param out Pointer to the output data array where the windowed result will be stored.
     * @param windowType The type of windowing function to apply (e.g., Hann, Hamming).
     * @param windowSize The size of the window to be applied.
     * @param sqrtWindow If true, applies the square root of the window function.
     * @param fixedGain A fixed gain factor to be applied to the windowed result.
     * @param compensateWindowGain The type of gain compensation to be applied based on the window.
     *
     * @return Returns true if the window was applied successfully, false otherwise.
     */
    
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
	
    /**
     * @brief Applies a windowing function to the input/output data array.
     *
     * This method applies a selected windowing function to the input/output array `io` in-place.
     * It allows for the customization of the window type, size, and optional square-root processing.
     * Additionally, a fixed gain can be applied, and gain compensation can be selected for the window.
     *
     * @param io Pointer to the input/output data array that will be modified by the windowing function.
     * @param windowType The type of windowing function to apply (e.g., Hann, Hamming).
     * @param windowSize The size of the window to be applied.
     * @param sqrtWindow If true, applies the square root of the window function.
     * @param fixedGain A fixed gain factor to be applied to the windowed result.
     * @param compensateWindowGain The type of gain compensation to be applied based on the window.
     */
    
	void applyWindow(double *io, WindowTypes windowType, unsigned long windowSize, bool sqrtWindow, double fixedGain, GainTypes compensateWindowGain)
	{
		applyWindow(io, io, windowType, windowSize, sqrtWindow, fixedGain, compensateWindowGain);
	}
	
	
private:
	
    /**
     * @brief Computes the zeroth-order modified Bessel function of the first kind.
     *
     * This method calculates the zeroth-order modified Bessel function of the first kind (I₀) for a given input.
     * It is commonly used in windowing functions such as the Kaiser window.
     *
     * @param xSq The square of the input value for which the Bessel function will be computed.
     *
     * @return The computed value of the I₀ Bessel function.
     */
    
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
	
    /**
     * @brief Calculates and stores the windowing function for a given window size and type.
     *
     * This method computes the windowing function based on the specified window size and type.
     * It also offers an option to apply the square root of the windowing function if required.
     * The computed window values are typically stored internally for later use.
     *
     * @param windowSize The size of the window to be calculated.
     * @param windowType The type of windowing function to generate (e.g., Hann, Hamming).
     * @param sqrtWindow If non-zero, the square root of the windowing function is applied.
     */
    
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
	
    /**
     * @brief Pointer to the array storing the calculated window values.
     *
     * This member variable holds a dynamically allocated array that contains the computed windowing function values.
     * It is used internally to apply the window to input data during signal processing.
     */
    
	double *mWindow;
	
	// Current Parameters
	
    /**
     * @brief Stores the size of the currently calculated window.
     *
     * This member variable holds the size of the window that has been calculated and stored in the `mWindow` array.
     * It determines the number of elements in the windowing function and is used during window application.
     */
    
	unsigned long mWindowSize;
    
    /**
     * @brief Stores the type of the currently selected window function.
     *
     * This member variable holds the type of window function (e.g., Hann, Hamming) that is currently in use or has been calculated.
     * It is used to determine the windowing function to apply during signal processing.
     */
    
	WindowTypes mWindowType;
    
    /**
     * @brief Indicates whether the square root of the window function is applied.
     *
     * This member variable holds a boolean value that determines if the square root of the window function is applied.
     * If true, the square root of the windowing function will be used during calculations.
     */
    
	bool mSqrtWindow;
	
	// Gain Values
	
    /**
     * @brief Stores the linear gain factor applied to the window function.
     *
     * This member variable holds the linear gain value that is applied to the window function.
     * It is used to scale the windowed signal, affecting the overall amplitude of the processed output.
     */
    
	double mWindowLinGain;
    
    /**
     * @brief Stores the squared gain factor applied to the window function.
     *
     * This member variable holds the squared gain value applied to the window function.
     * It is used to adjust the amplitude of the windowed signal when working with the square root of the window or other gain adjustments.
     */
    
	double mWindowSqGain;
	
	// Maximum Size
	
    /**
     * @brief Stores the maximum allowable window size.
     *
     * This member variable holds the maximum window size that can be used or allocated by the HISSTools_Windows class.
     * It is used to ensure that the window size does not exceed a predefined limit during window calculations.
     */
    
	unsigned long mMaxWindowSize;
};


#endif
