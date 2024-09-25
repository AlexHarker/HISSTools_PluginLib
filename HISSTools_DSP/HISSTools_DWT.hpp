
/**
 * @file HISSTools_DWT.hpp
 * @brief Header file for the HISSTools Discrete Wavelet Transform (DWT) classes.
 *
 * This file contains the declaration of the HISSTools_Wavelet and HISSTools_DWT classes, which implement
 * the Discrete Wavelet Transform for multi-resolution analysis and signal processing. The classes provide
 * methods for performing both forward and inverse wavelet transforms, as well as handling multi-level
 * decomposition and reconstruction using configurable wavelet filters.
 *
 * Key classes:
 * - HISSTools_Wavelet: Manages wavelet filters (low-pass and high-pass) for analysis and synthesis.
 * - HISSTools_DWT: Performs forward and inverse DWT using the wavelet filters, supporting multi-level transforms.
 *
 * The file also includes member variables and methods for handling in-place transformations and temporary buffers.
 */

#ifndef __HISSTOOLS_DWT__
#define __HISSTOOLS_DWT__

/**
 * @brief The HISSTools_Wavelet class.
 *
 * This class implements wavelet transforms using provided low-pass filter coefficients.
 * It facilitates multi-resolution analysis of signals, allowing for decomposition
 * and reconstruction at various frequency scales.
 *
 * The class provides methods for applying discrete wavelet transforms (DWT) and
 * handling wavelet-based signal processing tasks.
 */

class HISSTools_Wavelet
{
	
public:

	// Note that analysis filters should be stored in reverse order, as they are applied through correlation, rather than convolution.....
    
    /**
     * @brief Default constructor for the HISSTools_Wavelet class.
     *
     * Initializes an empty HISSTools_Wavelet object with no low-pass filter set.
     * Use this constructor when creating an instance of the class without immediately providing filter data.
     */
    
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
	
    /**
     * @brief Constructor for the HISSTools_Wavelet class.
     *
     * Initializes the HISSTools_Wavelet object with the provided low-pass filter, length, and optional offset.
     *
     * @param loPass Pointer to the array containing the low-pass filter coefficients.
     * @param length The number of coefficients in the low-pass filter array.
     * @param offset Optional parameter specifying the offset for alignment purposes. Default is 0.
     */
    
	HISSTools_Wavelet(const double *loPass, unsigned long length, long offset = 0)
	{
		HISSTools_Wavelet();
		setForwardFilters(loPass, length, offset);
		setInverseFilters();
	}
	
    /**
     * @brief Destructor for the HISSTools_Wavelet class.
     *
     * Cleans up any resources used by the HISSTools_Wavelet object.
     * This ensures that any dynamically allocated memory or other resources are properly released when the object is destroyed.
     */
    
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
	
    /**
     * @brief Sets the forward (analysis) filters for the wavelet transform.
     *
     * This method configures the low-pass filter coefficients used in the forward wavelet transform.
     * It allows for setting the filter length and an optional offset for alignment purposes.
     *
     * @param loPass Pointer to the array containing the low-pass filter coefficients.
     * @param length The number of coefficients in the low-pass filter array.
     * @param offset Optional parameter specifying the offset for alignment purposes. Default is 0.
     */
    
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
	
    /**
     * @brief Sets the inverse (synthesis) filters for the wavelet transform.
     *
     * This method configures the low-pass filter coefficients used in the inverse wavelet transform.
     * It allows for setting the filter length and an optional offset for alignment purposes.
     *
     * @param loPass Pointer to the array containing the low-pass filter coefficients.
     * @param length The number of coefficients in the low-pass filter array.
     * @param offset Optional parameter specifying the offset for alignment purposes. Default is 0.
     */
    
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
	
    /**
     * @brief Sets the inverse (synthesis) filters to default values.
     *
     * This method configures the inverse wavelet transform filters using default settings.
     * It is typically used when no specific filter coefficients are provided, relying on
     * internal or pre-determined filters for the inverse transform process.
     */
    
	virtual void setInverseFilters()
	{
		mInverseLength = mForwardLength;
		mInverseOffset = mForwardOffset;
		mInverseLoPass = mForwardLoPass;
		mInverseHiPass = mForwardHiPass;
		
		mInverseIndependent = false;
	}
	
		
	// FIR Filters
	
    /**
     * @brief Pointer to the forward low-pass filter coefficients.
     *
     * This member variable stores the coefficients used in the forward (analysis) wavelet transform.
     * It points to the array of double values representing the low-pass filter applied during the decomposition of signals.
     */
    
	double *mForwardLoPass;
    
    /**
     * @brief Pointer to the forward high-pass filter coefficients.
     *
     * This member variable stores the coefficients used in the forward (analysis) wavelet transform.
     * It points to the array of double values representing the high-pass filter applied during the decomposition of signals.
     */
    
	double *mForwardHiPass;
    
    /**
     * @brief Pointer to the inverse low-pass filter coefficients.
     *
     * This member variable stores the coefficients used in the inverse (synthesis) wavelet transform.
     * It points to the array of double values representing the low-pass filter applied during the reconstruction of signals.
     */
    
	double *mInverseLoPass;
    
    /**
     * @brief Pointer to the inverse high-pass filter coefficients.
     *
     * This member variable stores the coefficients used in the inverse (synthesis) wavelet transform.
     * It points to the array of double values representing the high-pass filter applied during the reconstruction of signals.
     */
    
	double *mInverseHiPass;
	
	// Parameters
	
    /**
     * @brief The length of the forward (analysis) filter.
     *
     * This member variable stores the number of coefficients in the forward low-pass and high-pass filters
     * used in the forward (analysis) wavelet transform.
     */
    
	unsigned long mForwardLength;
    
    /**
     * @brief The length of the inverse (synthesis) filter.
     *
     * This member variable stores the number of coefficients in the inverse low-pass and high-pass filters
     * used in the inverse (synthesis) wavelet transform.
     */
    
	unsigned long mInverseLength;
	
    /**
     * @brief The offset for the forward (analysis) filter.
     *
     * This member variable stores the offset value used to align the forward low-pass and high-pass filters
     * during the forward (analysis) wavelet transform.
     */
    
	unsigned long mForwardOffset;
    
    /**
     * @brief The offset for the inverse (synthesis) filter.
     *
     * This member variable stores the offset value used to align the inverse low-pass and high-pass filters
     * during the inverse (synthesis) wavelet transform.
     */
    
	unsigned long mInverseOffset;
	
    /**
     * @brief Flag indicating whether the inverse filters are independent of the forward filters.
     *
     * This boolean variable determines if the inverse (synthesis) filters are independently set,
     * or if they are derived from the forward (analysis) filters. If set to true, the inverse
     * filters are independently defined.
     */
    
	bool mInverseIndependent;
};

/**
 * @brief The HISSTools_DWT class.
 *
 * This class implements the Discrete Wavelet Transform (DWT) for signal processing tasks.
 * It provides methods for performing multi-resolution analysis, allowing signals to be
 * decomposed and reconstructed at various levels of resolution using wavelet filters.
 */

class HISSTools_DWT
{
	
public:
	
    /**
     * @brief Constructor for the HISSTools_DWT class.
     *
     * Initializes the HISSTools_DWT object with a specified maximum signal length. This sets up the necessary
     * internal structures to handle wavelet transforms on signals up to the specified length.
     *
     * @param maxLength The maximum length of the signal that can be processed by the DWT.
     */
    
	HISSTools_DWT(unsigned long maxLength)
	{
		mTemp = new double[maxLength];
		
		if (mTemp)
			mMaxLength = maxLength;
		else 
			mMaxLength = 0;
	}
	
    /**
     * @brief Destructor for the HISSTools_DWT class.
     *
     * Cleans up any resources used by the HISSTools_DWT object.
     * This ensures that all dynamically allocated memory and other resources used for wavelet transforms
     * are properly released when the object is destroyed.
     */
    
	~HISSTools_DWT()
	{
		delete[] mTemp;
	}
	
	
private:
	
    /**
     * @brief Performs the forward Discrete Wavelet Transform (DWT) on the input signal.
     *
     * This method applies the forward (analysis) wavelet transform to the input signal, producing the wavelet coefficients.
     *
     * @param in Pointer to the input signal array.
     * @param out Pointer to the output array where the transformed coefficients will be stored.
     * @param length The length of the input signal and output coefficient array.
     * @param wavelet Pointer to the HISSTools_Wavelet object containing the filters for the transform.
     * @return Returns true if the forward DWT was successful, otherwise returns false.
     */
    
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
	
    /**
     * @brief Performs the inverse Discrete Wavelet Transform (DWT) on the wavelet coefficients.
     *
     * This method applies the inverse (synthesis) wavelet transform to reconstruct the original signal from the wavelet coefficients.
     *
     * @param in Pointer to the input array containing the wavelet coefficients.
     * @param out Pointer to the output array where the reconstructed signal will be stored.
     * @param length The length of the input coefficients and the output signal array.
     * @param wavelet Pointer to the HISSTools_Wavelet object containing the filters for the inverse transform.
     * @return Returns true if the inverse DWT was successful, otherwise returns false.
     */
    
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
	
    /**
     * @brief Performs a multi-level forward Discrete Wavelet Transform (DWT) on the input signal.
     *
     * This method applies the forward (analysis) wavelet transform to the input signal over multiple levels,
     * producing wavelet coefficients at each level of decomposition.
     *
     * @param in Pointer to the input signal array.
     * @param out Pointer to the output array where the transformed coefficients will be stored.
     * @param length The length of the input signal and output coefficient array.
     * @param levels The number of decomposition levels to apply in the wavelet transform.
     * @param wavelet Pointer to the HISSTools_Wavelet object containing the filters for the transform.
     * @return Returns true if the forward multi-level DWT was successful, otherwise returns false.
     */
    
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
	
    /**
     * @brief Performs a multi-level inverse Discrete Wavelet Transform (DWT) to reconstruct the signal.
     *
     * This method applies the inverse (synthesis) wavelet transform to the wavelet coefficients over multiple levels,
     * reconstructing the original signal from the multi-level decomposition.
     *
     * @param in Pointer to the input array containing the wavelet coefficients.
     * @param out Pointer to the output array where the reconstructed signal will be stored.
     * @param length The length of the input coefficients and the output signal array.
     * @param levels The number of decomposition levels to apply in the inverse wavelet transform.
     * @param wavelet Pointer to the HISSTools_Wavelet object containing the filters for the inverse transform.
     * @return Returns true if the inverse multi-level DWT was successful, otherwise returns false.
     */
    
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
	
    /**
     * @brief Performs an in-place multi-level forward Discrete Wavelet Transform (DWT) on the input signal.
     *
     * This method applies the forward (analysis) wavelet transform to the input signal over multiple levels,
     * transforming the signal in-place. The input signal is replaced with the wavelet coefficients in the same array.
     *
     * @param io Pointer to the input/output signal array. The input signal is replaced with the wavelet coefficients.
     * @param length The length of the input signal and output coefficient array.
     * @param levels The number of decomposition levels to apply in the wavelet transform.
     * @param wavelet Pointer to the HISSTools_Wavelet object containing the filters for the transform.
     * @return Returns true if the forward multi-level DWT was successful, otherwise returns false.
     */
    
	bool forwardDWT (double *io, unsigned long length, unsigned long levels, HISSTools_Wavelet *wavelet)
	{
		return forwardDWT (io, io, length, levels, wavelet);
	}
	
    /**
     * @brief Performs an in-place multi-level inverse Discrete Wavelet Transform (DWT) to reconstruct the signal.
     *
     * This method applies the inverse (synthesis) wavelet transform to the wavelet coefficients over multiple levels,
     * reconstructing the original signal in-place. The wavelet coefficients are replaced with the reconstructed signal
     * in the same array.
     *
     * @param io Pointer to the input/output array. The wavelet coefficients are replaced with the reconstructed signal.
     * @param length The length of the input coefficients and output signal array.
     * @param levels The number of decomposition levels to apply in the inverse wavelet transform.
     * @param wavelet Pointer to the HISSTools_Wavelet object containing the filters for the inverse transform.
     * @return Returns true if the inverse multi-level DWT was successful, otherwise returns false.
     */
    
	bool inverseDWT (double *io, unsigned long length, unsigned long levels, HISSTools_Wavelet *wavelet)
	{
		return inverseDWT (io, io, length, levels, wavelet);
	}	
	
	
private:
	
	// Temp Data
	
    /**
     * @brief Temporary buffer used for intermediate calculations in the DWT.
     *
     * This member variable points to an array used as a temporary buffer during forward and inverse
     * wavelet transforms. It stores intermediate results to facilitate in-place processing or multi-level transformations.
     */
    
	double *mTemp;
	
	// Maimum Length
	
    /**
     * @brief The maximum length of the signal that can be processed.
     *
     * This member variable stores the maximum allowable length for the input signal that the DWT object can handle.
     * It is set during the initialization and defines the size limits for wavelet transforms.
     */
    
	unsigned long mMaxLength;
	
};

#endif
