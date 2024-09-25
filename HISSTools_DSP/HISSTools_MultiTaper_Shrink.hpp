
/**
 * @file HISSTools_MultiTaper_Shrink.hpp
 * @brief Header file for the HISSTools_MultiTaper_Shrink class, which performs multi-taper spectral analysis with shrinkage.
 *
 * This file contains the declaration of the HISSTools_MultiTaper_Shrink class, along with associated enumerations, methods,
 * and member variables. The class combines functionalities from HISSTools_MultiTaper_Spectrum, HISSTools_DWT, and HISSTools_PSpectrum
 * to provide robust spectral estimation with noise reduction through shrinkage techniques.
 *
 * Key functionalities include:
 * - Multi-taper spectral analysis
 * - Wavelet transforms
 * - Power spectrum calculation
 * - Shrinkage methods to reduce noise in spectral estimates
 */

#ifndef __HISSTools_MULTITAPER_SHRINK__
#define __HISSTools_MULTITAPER_SHRINK__


#include "HISSTools_FSpectrum.hpp"
#include "HISSTools_MultiTaper_Spectrum.hpp"
#include "HISSTools_DWT.hpp"

/**
 * @enum ShrinkTypes
 * @brief Defines the types of shrinkage used in the HISSTools_MultiTaper_Shrink class.
 *
 * This enumeration represents the various shrinkage methods that can be applied during spectral estimation.
 * Each value corresponds to a different type of shrinkage algorithm.
 */

enum ShrinkTypes {
		
		SHRINK_UNIVERSAL_SOFT = 0,
		SHRINK_UNIVERSAL_MID = 1,
		SHRINK_UNIVERSAL_HARD = 2,
};

/**
 * @class HISSTools_MultiTaper_Shrink
 * @brief A class that performs multi-taper spectral analysis with shrinkage.
 *
 * The HISSTools_MultiTaper_Shrink class inherits from HISSTools_MultiTaper_Spectrum, HISSTools_DWT,
 * and HISSTools_PSpectrum, combining functionalities from these classes to perform multi-taper spectral
 * analysis and apply shrinkage techniques. This class is designed to provide an efficient way to compute
 * spectral estimates while reducing noise and variance using shrinkage methods.
 *
 * Inheritance:
 * - Protected inheritance from HISSTools_MultiTaper_Spectrum: Utilizes multi-taper spectral analysis functionality.
 * - Protected inheritance from HISSTools_DWT: Adds discrete wavelet transform support.
 * - Protected inheritance from HISSTools_PSpectrum: Adds power spectrum analysis functionality.
 */

class HISSTools_MultiTaper_Shrink : protected HISSTools_MultiTaper_Spectrum, protected HISSTools_DWT, protected HISSTools_PSpectrum
{
	
public:
	
    /**
     * @brief Constructor for the HISSTools_MultiTaper_Shrink class.
     *
     * This constructor initializes the HISSTools_MultiTaper_Shrink object with the specified FFT size, wavelet,
     * and spectrum format. It also initializes the base class HISSTools_MultiTaper_Spectrum, HISSTools_DWT,
     * and HISSTools_PSpectrum with the provided FFT size and spectrum format.
     *
     * @param maxFFTSize The maximum FFT size to be used in computations.
     * @param wavelet A pointer to a HISSTools_Wavelet object, used for wavelet transforms.
     * @param format The format of the spectrum to be computed. Defaults to kSpectrumNyquist.
     */
    
	HISSTools_MultiTaper_Shrink(unsigned long maxFFTSize, HISSTools_Wavelet *wavelet, PSpectrumFormat format = kSpectrumNyquist):
	HISSTools_MultiTaper_Spectrum(maxFFTSize, kSpectrumFull), HISSTools_DWT(maxFFTSize), HISSTools_PSpectrum(maxFFTSize, kSpectrumFull)
	{			
		mWavelet = wavelet;
	}
	
    /**
     * @brief Destructor for the HISSTools_MultiTaper_Shrink class.
     *
     * This destructor is responsible for cleaning up resources used by the HISSTools_MultiTaper_Shrink object.
     * It ensures that any allocated memory or resources used by the inherited classes (HISSTools_MultiTaper_Spectrum,
     * HISSTools_DWT, HISSTools_PSpectrum) are properly released.
     */
    
	~HISSTools_MultiTaper_Shrink()
	{
	}
	
private:
	
    /**
     * @brief Applies shrinkage to wavelet coefficients.
     *
     * This method processes the provided wavelet coefficients by applying the specified shrinkage method.
     * Shrinkage is typically used to reduce noise in the wavelet-transformed data, improving spectral estimation.
     *
     * @param waveletCoeffients A pointer to an array of wavelet coefficients to be shrunk.
     * @param shrinkMethod The shrinkage method to be applied, chosen from the ShrinkTypes enumeration.
     * @param kTapers The number of tapers to be used for multi-taper spectral analysis.
     * @param shrinkLevel The level at which to apply the shrinkage in the wavelet domain.
     * @param FFTSize The size of the FFT (Fast Fourier Transform) to be used in processing.
     */
    
	void shrinkWavelet(double *waveletCoeffients, ShrinkTypes shrinkMethod, long kTapers, long shrinkLevel, long FFTSize)
	{
		double currentVal;
		double threshold = trigamma(kTapers) * sqrt(2 * log(FFTSize - 1));
		long i;
		
		switch (shrinkMethod)
		{
			case SHRINK_UNIVERSAL_SOFT:
								
				for (i = FFTSize >> shrinkLevel; i < FFTSize; i++)
				{
					currentVal = abs(waveletCoeffients[i]);
					
					if (currentVal > threshold)
					{
						currentVal -= threshold;
						waveletCoeffients[i] = copysign(currentVal, waveletCoeffients[i]);
					}
					else 
						waveletCoeffients[i] = 0;
				}
				break;
				
			case SHRINK_UNIVERSAL_MID:
								
				for (i = FFTSize >> shrinkLevel; i < FFTSize; i++)
				{
					currentVal = abs(waveletCoeffients[i]);
					
					if (currentVal < threshold * 2)
					{
						if (currentVal > threshold)
						{
							currentVal -= threshold;
							waveletCoeffients[i] = copysign(currentVal, waveletCoeffients[i]);
						}
						else 
							waveletCoeffients[i] = 0;
					}
				}
				break;
				
			case SHRINK_UNIVERSAL_HARD:
								
				for (i = FFTSize >> shrinkLevel; i < FFTSize; i++)
				{
					if (waveletCoeffients[i] < threshold && waveletCoeffients[i] > -threshold)
						waveletCoeffients[i] = 0.;	
				}
				break;
		}
	}
	
    /**
     * @brief Computes the digamma function for a given input.
     *
     * This method calculates the digamma function, which is the logarithmic derivative of the gamma function.
     * The digamma function is often used in statistics and spectral analysis for various calculations.
     *
     * @param x The input value for which to compute the digamma function.
     * @return The computed value of the digamma function for the input x.
     */
    
	double digamma(long x)
	{
		// Calculates diagamma for integer values
		
		double e_m_const = 0.57721566490153286060651209008240243104215933593992;
		double sum = 0;
			
		for (long i = 1; i < x ; i++)
				sum += 1. / (double) i;
			
		return sum - e_m_const;
	}
	
    /**
     * @brief Computes the trigamma function for a given input.
     *
     * This method calculates the trigamma function, which is the second derivative of the logarithm of the gamma function.
     * The trigamma function is used in various fields such as statistics, probability, and spectral analysis.
     *
     * @param x The input value for which to compute the trigamma function.
     * @return The computed value of the trigamma function for the input x.
     */
    
	double trigamma(long x)
	{
		// Calculates trigamma for integer values

		double recur = M_PI * M_PI / 6.;
		
		for (long i = 1; i < x; i++)
			recur -= 1 / (double) (i * i);
		
		return recur;
	}
	
	
public:
	
    /**
     * @brief Computes the power spectrum of the provided samples with optional shrinkage.
     *
     * This method calculates the power spectrum of the input sample data using multi-taper spectral analysis.
     * It optionally applies shrinkage to the spectrum to reduce noise and improve the accuracy of the estimation.
     *
     * @param samples A pointer to an array of input samples from which to compute the power spectrum.
     * @param outSpectrum A pointer to a HISSTools_PSpectrum object where the computed power spectrum will be stored.
     * @param shrinkMethod The shrinkage method to be applied, chosen from the ShrinkTypes enumeration.
     * @param kTapers The number of tapers to be used for multi-taper spectral analysis.
     * @param shrinkLevel The level of shrinkage to be applied in the wavelet domain.
     * @param nSamps The number of samples to be processed.
     * @param FFTSize The size of the FFT (Fast Fourier Transform) to be used for computation. If set to 0, it defaults to nSamps.
     * @param scale A scaling factor to be applied to the power spectrum. Defaults to 0.
     * @param samplingRate The sampling rate of the input signal, in Hz. Defaults to 44.1 kHz.
     * @param adaptIterations The number of iterations for adaptive weighting in the multi-taper method. Defaults to 0.
     * @return Returns true if the power spectrum was successfully computed, false otherwise.
     */
    
	bool calcPowerSpectrum(double *samples, HISSTools_PSpectrum *outSpectrum, ShrinkTypes shrinkMethod, long kTapers, unsigned long shrinkLevel, unsigned long nSamps, unsigned long FFTSize = 0, double scale = 0., double samplingRate = 44100, unsigned long adaptIterations = 0)
	{
		HISSTools_PSpectrum *tempPowerSpectrum = this;
		PSpectrumFormat format = outSpectrum->getFormat();
		double *temp = tempPowerSpectrum->getSpectrum();
		double *out = outSpectrum->getSpectrum();
		double noiseMean = digamma(kTapers) - log(kTapers);
		long i;
		
		// Fall back on Multitaper spectrum if no shrinking is required
		
		if (shrinkLevel == 0)
			return HISSTools_MultiTaper_Spectrum::calcPowerSpectrum(samples, outSpectrum, kTapers, nSamps, FFTSize, scale, samplingRate, adaptIterations);
		
		// Put Multitaper spectrum in temporary PSpectrum (with Sanity Check)
		
		if (HISSTools_MultiTaper_Spectrum::calcPowerSpectrum(samples, tempPowerSpectrum, kTapers, nSamps, FFTSize, scale, samplingRate, adaptIterations) == FALSE)
			return FALSE;

		// Get FFT size again, in case of default behaviour etc.
		
		FFTSize = tempPowerSpectrum->getFFTSize();
		
		// Attempt to set output size
		
		if (outSpectrum->setFFTSize(FFTSize) == FALSE)
			return FALSE;
		
		// Form Log Estimate FULL Spectrum (but only calc logs once - mirror second hald)
		// Should check here for -inf type situations....
		
		for (i = 0; i < (FFTSize >> 1) + 1; i++)
		temp[i] = log(temp[i]) - noiseMean;
		for (; i < FFTSize; i++)
		temp[i] = temp[FFTSize - i];
		
		// Wavelet shrinking
		// Transform
			
		forwardDWT(temp, FFTSize, shrinkLevel, mWavelet);
			
		// Wavelet Shrink
			
		shrinkWavelet(temp, shrinkMethod, kTapers, shrinkLevel, FFTSize);
			
		// Transform Back
			
		inverseDWT(temp, FFTSize, shrinkLevel, mWavelet);
		
		// Average Results
		// DC
		
		out[0] = exp(temp[0]);
		
		// First half of spectrum
		
		for (i = 1; i < FFTSize >> 1; i++)
			out[i] = (exp(temp[i]) + exp(temp[FFTSize - i])) / 2.;
		
		// Nyquist
		
		out[FFTSize >> 1] = exp(temp[i++]);
		
		// Mirror second half of spectrum if necessary
		
		if (format == kSpectrumFull)
			for (; i < FFTSize; i++)
				out[i] = out[FFTSize - i];
		
		return TRUE;
	}
	
	
private:
	
    /**
     * @brief A pointer to a HISSTools_Wavelet object used for wavelet transformations.
     *
     * This member variable holds a reference to the HISSTools_Wavelet object, which is responsible for performing
     * wavelet transformations within the HISSTools_MultiTaper_Shrink class. It is used during the process of
     * spectral analysis and shrinkage.
     */
    
	HISSTools_Wavelet *mWavelet;
    
    /**
     * @brief A pointer to a temporary HISSTools_PSpectrum object used for intermediate power spectrum calculations.
     *
     * This member variable holds a reference to a HISSTools_PSpectrum object that is used to store intermediate results
     * during the computation of the power spectrum. It is utilized within the HISSTools_MultiTaper_Shrink class
     * for temporary storage while performing spectral analysis with shrinkage.
     */
    
	HISSTools_PSpectrum *mTempPowerSpectrum;
};


#endif
