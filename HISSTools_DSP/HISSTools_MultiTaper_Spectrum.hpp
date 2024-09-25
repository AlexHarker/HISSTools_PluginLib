
/**
 * @file HISSTools_MultiTaper_Spectrum.hpp
 * @brief Defines the HISSTools_MultiTaper_Spectrum class for multi-taper spectral analysis.
 *
 * This file contains the declaration and implementation of the HISSTools_MultiTaper_Spectrum class,
 * which is responsible for calculating the power spectrum of signals using the multi-taper method.
 * The multi-taper method improves spectral estimates by reducing variance through the use of multiple
 * orthogonal tapers applied to the signal.
 *
 * The class is designed for efficient FFT-based spectrum calculation and supports adaptive methods
 * for refining spectral estimates. It inherits from HISSTools_FFT and HISSTools_FSpectrum to leverage
 * fast Fourier transforms and spectrum data handling.
 *
 * Key features:
 * - Multi-taper spectral estimation
 * - Power spectrum calculation with optional adaptive refinement
 * - Differential estimation for improved spectral analysis
 * - Customizable FFT size, taper count, and scaling
 *
 * This class is typically used in applications where precise spectral analysis is required, such as
 * audio signal processing, biomedical signal analysis, or any domain where time-series frequency content
 * needs to be analyzed with high accuracy.
 */

#ifndef __HISSTOOLS_MULTITAPER_SPECTRUM__
#define __HISSTOOLS_MULTITAPER_SPECTRUM__


#include "HISSTools_FFT.hpp"

/**
 * @brief The HISSTools_MultiTaper_Spectrum class.
 *
 * This class implements the Multi-Taper Spectrum analysis, utilizing FFT (Fast Fourier Transform)
 * for efficient frequency domain analysis. It inherits from the HISSTools_FFT and HISSTools_FSpectrum
 * classes in a protected manner, which means that these base class functionalities are available only to
 * this class and its derived classes.
 *
 * Inheritance:
 * - HISSTools_FFT: Provides FFT processing functionality for efficient spectral transformations.
 * - HISSTools_FSpectrum: Manages spectrum data structures and spectral format handling.
 *
 * The class is designed to analyze signals using the multi-taper method, which reduces variance and provides
 * better spectral estimates, especially for non-stationary signals.
 */

class HISSTools_MultiTaper_Spectrum : protected HISSTools_FFT, protected HISSTools_FSpectrum
{
	
public:
	
    /**
     * @brief Constructor for the HISSTools_MultiTaper_Spectrum class.
     *
     * This constructor initializes an instance of the HISSTools_MultiTaper_Spectrum class,
     * which calculates the spectrum using the multi-taper method. It sets up FFT size and
     * format for processing.
     *
     * @param maxFFTSize The maximum size for FFT (Fast Fourier Transform). The FFT size will be double this value.
     * @param format The format of the spectrum output. Defaults to kSpectrumNyquist, but can be modified.
     *
     * The constructor also initializes the base classes:
     * - HISSTools_FFT with an FFT size that is twice the maxFFTSize.
     * - HISSTools_FSpectrum with a size twice the maxFFTSize and spectrum format set to kSpectrumComplex.
     */
    
	HISSTools_MultiTaper_Spectrum (unsigned long maxFFTSize, PSpectrumFormat format = kSpectrumNyquist) : HISSTools_FFT(maxFFTSize * 2), HISSTools_FSpectrum(maxFFTSize * 2, kSpectrumComplex)
	{		
	}
	
    /**
     * @brief Destructor for the HISSTools_MultiTaper_Spectrum class.
     *
     * This destructor is responsible for cleaning up any resources or memory allocated
     * by the HISSTools_MultiTaper_Spectrum class. It ensures that any required teardown
     * of internal structures is handled properly when an instance of this class is destroyed.
     *
     * Since this class inherits from HISSTools_FFT and HISSTools_FSpectrum, their destructors
     * are also called automatically to release any resources they may have allocated.
     */
    
	~HISSTools_MultiTaper_Spectrum()
	{
	}
	
private:
	
    /**
     * @brief Estimates the differential of a given power spectrum.
     *
     * This function computes an estimate of the differential (rate of change) of the power
     * spectrum at a given point using three power spectrum values: one before (`pm1`),
     * one at the current bin (`p0`), and one after (`pp1`). The differential is calculated
     * with respect to the given bin width, which represents the frequency interval.
     *
     * @param pm1 The power spectrum value at the previous frequency bin (f - binWidth).
     * @param p0 The power spectrum value at the current frequency bin (f).
     * @param pp1 The power spectrum value at the next frequency bin (f + binWidth).
     * @param binWidth The width of the frequency bin, used to normalize the differential.
     *
     * @return The estimated differential of the power spectrum.
     */
    
	double estimateDifferential(double pm1, double p0, double pp1, double binWidth)
	{
		return (pm1 + pp1 - (2.0 * p0)) / (binWidth * binWidth);
	}
	
    /**
     * @brief Estimates the differential of a given power spectrum using a five-point stencil.
     *
     * This function calculates a more accurate estimate of the differential (rate of change)
     * of the power spectrum at a given frequency bin using five points: two before (`pm2`, `pm1`),
     * the current bin (`p0`), and two after (`pp1`, `pp2`). The bin width (`binWidth`) represents
     * the frequency interval and is used to normalize the differential.
     *
     * A five-point stencil is used to achieve a more refined estimate of the differential, improving
     * the accuracy compared to simpler methods like three-point estimation.
     *
     * @param pm2 The power spectrum value at the frequency bin (f - 2 * binWidth).
     * @param pm1 The power spectrum value at the frequency bin (f - binWidth).
     * @param p0 The power spectrum value at the current frequency bin (f).
     * @param pp1 The power spectrum value at the frequency bin (f + binWidth).
     * @param pp2 The power spectrum value at the frequency bin (f + 2 * binWidth).
     * @param binWidth The width of the frequency bin, used to normalize the differential.
     *
     * @return The estimated differential of the power spectrum.
     */
    
	double estimateDifferential(double pm2, double pm1, double p0, double pp1,  double pp2, double binWidth)
	{
		return (16.0 * (pm1 + pp1) - (30.0 * p0) - (pm2 + pp2)) / (12.0 * binWidth * binWidth);
	}
	
    /**
     * @brief Computes the optimal number of tapers for multi-taper spectral estimation.
     *
     * This function estimates the optimal number of tapers to use in the multi-taper method based on the
     * power spectrum value, the differential of the power spectrum, and the number of data points (`N`).
     * The multi-taper method uses multiple data tapers to reduce variance and provide more stable
     * spectral estimates, and the choice of the optimal number of tapers depends on the properties of
     * the signal.
     *
     * @param powValue The power spectrum value at the current frequency bin (f).
     * @param powDifferential The differential (rate of change) of the power spectrum at the current frequency bin.
     * @param N The number of data points in the signal being analyzed.
     *
     * @return The estimated optimal number of tapers for the multi-taper method.
     */
    
	double optimalTapers(double powValue, double powDifferential, unsigned long N)
	{
		double kTapers = (12 * powValue * (N * N)) / powDifferential;
		kTapers = pow(kTapers * kTapers, 1.0/5.0);
		kTapers = kTapers < 1.0 ? 1.0 : kTapers;
		kTapers = kTapers < (N >> 2) ? kTapers : (N >> 2);

		kTapers = kTapers < 20 ? kTapers : 20;
		
		return kTapers;//(long) round(kTapers);
	}
	
    /**
     * @brief Adapts the FFT data to compute the power spectrum using multi-taper methods.
     *
     * This function processes the FFT data by adapting it according to the specified format
     * to calculate the power spectrum. The power spectrum is computed over a range of bins
     * (up to `maxBin`), with an optional scaling factor applied. The result is stored in the
     * provided `spectrum` array.
     *
     * @param FFTData The FFT data, stored as a split-complex structure (real and imaginary parts separated).
     * @param format The desired format for the power spectrum output, specified by the PSpectrumFormat enum.
     * @param spectrum A pointer to an array where the calculated power spectrum will be stored.
     * @param FFTSize The size of the FFT (number of frequency bins in the FFT).
     * @param maxBin The maximum frequency bin up to which the power spectrum will be calculated.
     * @param scale A scaling factor to be applied to the spectrum values.
     */
    
	void adapt(FFT_SPLIT_COMPLEX_D FFTData, PSpectrumFormat format, double *spectrum, unsigned long FFTSize, unsigned long maxBin, double scale)
	{
		//long kTapers[FFTSize];
		double kTapers[16384];
		double differential;
		double binWidth = 1.0 / FFTSize;
		long minTapers = FFTSize;
		long maxTapers = 1;
		
		unsigned long FFTBinMask = (FFTSize << 1) - 1;
		unsigned long i, j;
		
		// Calculate optimal tapers based on current power values
		
		//differential = estimateDifferential(spectrum[1], spectrum[0], spectrum[1], binWidth);
		differential = estimateDifferential(spectrum[2], spectrum[1], spectrum[0], spectrum[1], spectrum[2], binWidth);
		kTapers[0] = optimalTapers(spectrum[0], differential, FFTSize);
		
		differential = estimateDifferential(spectrum[1], spectrum[0], spectrum[1], spectrum[2], spectrum[3], binWidth);
		kTapers[1] = optimalTapers(spectrum[1], differential, FFTSize);
		
		// FIX - doesn't work for half spectrum 
		
		for (i = 2; i < (maxBin - 2); i++)
		{
			//differential = estimateDifferential(spectrum[i - 1], spectrum[i], spectrum[i + 1], binWidth);
			differential = estimateDifferential(spectrum[i-2], spectrum[i-1], spectrum[i], spectrum[i+1], spectrum[i+2], binWidth);
			kTapers[i] = optimalTapers(spectrum[i], differential, FFTSize);
		}
			
		//differential = estimateDifferential(spectrum[i - 1], spectrum[i], spectrum[i - 1], binWidth);
        differential = estimateDifferential(spectrum[i-2], spectrum[i-1], spectrum[i], spectrum[i+1], spectrum[i-1], binWidth);
        kTapers[i] = optimalTapers(spectrum[i], differential, FFTSize);
			
        i++;
			
        differential = estimateDifferential(spectrum[i-2], spectrum[i-1], spectrum[i], spectrum[i-1], spectrum[i-2], binWidth);
        kTapers[i] = optimalTapers(spectrum[i], differential, FFTSize);
		
		for (i = 0; i < maxBin; i++)
		{
			minTapers = kTapers[i] < minTapers ? kTapers[i] : minTapers;
			maxTapers = kTapers[i] > maxTapers ? kTapers[i] : maxTapers;
		}
		
		// Recalculate spectrum
			
		double changes = 0;
		
		for (i = 0; i < maxBin; i++)
		{			
			double powerValue = 0.0;			
			double real, imag;
			//double weightTotal = kTapers[i] - (((1.0 / kTapers[i]) - 3.0 + 2.0 * kTapers[i]) / 6.0);

			// FIX - this calculates slightly different to a manual sum, but probably good enough
			
			long nTapers = (long) ceil(kTapers[i]);
			double weightSum = nTapers - ((nTapers - (3.0 * (nTapers * nTapers)) + 2.0 * (nTapers * nTapers * nTapers)) / (6.0 * kTapers[i] * kTapers[i]));
			double normFactor = sqrt(2.) / (2 * FFTSize);// * weightTotal);
			
			long above = ((i << 1) + 1);
			long below = ((i << 1) - 1);
			
			for (j = 1; j <= nTapers && below >= 0; above++, below--, j++)
			{
				double weight = (1.0 - ((j - 1) * (j - 1)) / (kTapers[i] * kTapers[i]));
				
				real = FFTData.imagp[above] - FFTData.imagp[below];
				imag = FFTData.realp[above] - FFTData.realp[below];
				
				powerValue += ((real * real) + (imag * imag)) * weight; 
			}
			for (; j <= nTapers; above++, below--, j++)
			{
				double weight = (1.0 - ((j - 1) * (j - 1)) / (kTapers[i] * kTapers[i]));
				
				real = FFTData.imagp[above] + FFTData.imagp[-below];
				imag = FFTData.realp[above] - FFTData.realp[-below];
				
				powerValue += ((real * real) + (imag * imag)) * weight; 
			}
			
			powerValue *= (normFactor * scale) / weightSum;

			changes += (powerValue - spectrum[i]) * (powerValue - spectrum[i]);
			
			spectrum[i] = powerValue;
		}
		
		
	}
	
public:
	
    /**
     * @brief Calculates the multi-taper power spectrum from a given set of input samples.
     *
     * This function computes the power spectrum using the multi-taper method. It processes a given number of input samples (`samples`)
     * to generate an output power spectrum (`outSpectrum`). The function allows for customization of the number of tapers, sample size,
     * FFT size, and scaling. Optionally, it can perform adaptive weighting through multiple iterations to refine the spectral estimate.
     *
     * @param samples A pointer to the input samples (time-domain signal) for which the power spectrum will be calculated.
     * @param outSpectrum A pointer to a HISSTools_PSpectrum structure where the calculated power spectrum will be stored.
     * @param kTapers The number of tapers to be used in the multi-taper method for spectral estimation.
     * @param nSamps The number of input samples to process.
     * @param FFTSize The size of the FFT (number of frequency bins), defaults to 0 which may trigger an automatic determination.
     * @param scale A scaling factor to be applied to the resulting power spectrum, defaults to 0 for no scaling.
     * @param samplingRate The sampling rate of the input signal, used to calculate frequency resolution, defaults to 44100 Hz.
     * @param adaptIterations The number of adaptive weighting iterations to perform, defaults to 0 for no adaptation.
     *
     * @return Returns `true` if the power spectrum calculation was successful, `false` otherwise.
     */
     
	bool calcPowerSpectrum(double *samples, HISSTools_PSpectrum *outSpectrum, unsigned long kTapers, unsigned long nSamps, unsigned long FFTSize = 0, double scale = 0., double samplingRate = 44100, unsigned long adaptIterations = 0)
	{
		FFT_SPLIT_COMPLEX_D FFTData = *this->getSpectrum(); 
		PSpectrumFormat format = outSpectrum->getFormat();
		
		double *spectrum = outSpectrum->getSpectrum();
		
		unsigned long FFTBinMask;
		unsigned long maxBin;
		long below;
		long above;
		
		double weight = sqrt(2.) / (2 * FFTSize);
		double taperScale;
		double real, imag;
		
		// Sanity check for number of tapers
		
		kTapers = kTapers < (FFTSize >> 1) ? kTapers : (FFTSize >> 1) - 1;
		
		weight /= kTapers;
		scale = scale == 0 ? 1 : scale;
		
		// Check arguments
		
		if (FFTSize == 0)
			FFTSize = nSamps;
		
		FFTSize = 1 << ((HISSTools_FFT *) this)->log2(FFTSize);
		
		if (nSamps > FFTSize)
			nSamps = FFTSize;
			
		// Transform to time domain (with Sanity Check)
		
		if (timeToSpectrum(samples, this, nSamps, (FFTSize << 1), samplingRate) == FALSE)
			return FALSE;
		
		// Attempt to set output size
		
		if (outSpectrum->setFFTSize(FFTSize) == FALSE)
			return FALSE;
		
		FFTBinMask = (FFTSize << 1) - 1;
		maxBin = (FFTSize >> 1) + 1;
		
		// N.B. zero-padded FFT has same amplitude scaling as smaller size FFT (without padding)
		// Real valued sine wave has amplitude of N/2, but MT window is divided by N, so must deal with 1/2 factor
				
		// Zero relevant part of output spectrum
		
		for (unsigned long j = 0; j < maxBin; j++)
			spectrum[j] = 0.; 
		
		// Do tapers
	
		double weightSum = kTapers - (((1.0 / (double) kTapers) - 3.0 + 2.0 * kTapers) / 6.0);
		double normFactor = sqrt(2.) / (2 * FFTSize * weightSum);
				
		for (unsigned long i = 1; i <= kTapers; i++)
		{			
			weight = (1.0 - ((i - 1) * (i - 1)) / (double) (kTapers * kTapers));
			taperScale = weight * scale  * normFactor;
			
			// Deal with lower end wraparound
			
			for (unsigned long j = 0; j < i; j++)
			{
				above = ((j << 1) + i);
				below = ((j << 1) - i) & FFTBinMask;
				
                // FIX - why is this swapped?
                
				real = FFTData.imagp[above] - FFTData.imagp[below];
				imag = FFTData.realp[above] - FFTData.realp[below];
				
				spectrum[j] += ((real * real) + (imag * imag)) * taperScale; 
			}

			// Deal wih bins withgut wraparound

			for (unsigned long j = i; j < maxBin; j++)
			{
				above = ((j << 1) + i);
				below = ((j << 1) - i);
				
				real = FFTData.imagp[above] - FFTData.imagp[below];
				imag = FFTData.realp[above] - FFTData.realp[below];
				
				spectrum[j] += ((real * real) + (imag * imag)) * taperScale; 
			}
		}
					
		// EXPERIMENTAL - Data adaption for better balance of resolution/smoothing according to data
		
		if (adaptIterations)
		{
			for (unsigned long i = 0; i < adaptIterations; i++)
				adapt(FFTData, format, spectrum, FFTSize, maxBin, scale);
		}
		
		// Mirror second half of output spectrum if relevant
		
		if (format == kSpectrumFull)
			for (long j = maxBin; j < FFTSize; j++)
				spectrum[j] = spectrum[FFTSize - j];
				
		setSamplingRate(samplingRate);
		outSpectrum->setSamplingRate(samplingRate);
		
		return TRUE;
	}

};

#endif
