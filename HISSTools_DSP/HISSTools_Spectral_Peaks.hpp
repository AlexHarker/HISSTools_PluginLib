
/**
 * @file HISSTools_Spectral_Peaks.hpp
 * @brief Header file for the HISSTools_Spectral_Peaks class and related structures.
 *
 * This file defines the HISSTools_Spectral_Peaks class and associated structures for detecting and analyzing
 * spectral peaks in FFT (Fast Fourier Transform) data. It includes methods for peak detection, interpolation,
 * and retrieval of peak properties such as frequency and amplitude. The class is designed to process FFT data
 * efficiently, allowing for spectral analysis in audio or other signal processing applications.
 *
 * The file also contains the FFTPeak structure, which holds information about individual peaks such as their
 * frequency, amplitude, and bin locations.
 */

#ifndef __HISSTOOLS_SPECTRAL_PEAKS__
#define __HISSTOOLS_SPECTRAL_PEAKS__


#include "HISSTools_PSpectrum.hpp"

/**
 * @brief A structure to represent a peak in the FFT (Fast Fourier Transform) spectrum.
 *
 * This structure is used to store information about individual spectral peaks found in the FFT analysis.
 * It typically includes properties such as the frequency, amplitude, or other characteristics of a peak.
 */

struct FFTPeak {
	
    /**
     * @brief The index of the starting bin in the FFT data.
     *
     * This variable represents the index of the frequency bin where a spectral peak starts in the FFT spectrum.
     * It helps identify the location of a peak within the FFT's frequency bins.
     */
    
	long startBin;
    
    /**
     * @brief The index of the bin corresponding to the peak in the FFT data.
     *
     * This variable represents the index of the frequency bin where the actual spectral peak is located.
     * It marks the highest amplitude point within the range of bins considered for the peak.
     */
    
	long peakBin;
	
    /**
     * @brief The frequency value corresponding to the spectral peak.
     *
     * This variable stores the frequency (in Hz) of the detected spectral peak. It is derived from the FFT data,
     * typically based on the bin index and the FFT resolution, and represents the actual frequency of the peak.
     */
    
	double peakFreq;
    
    /**
     * @brief The amplitude value of the spectral peak.
     *
     * This variable stores the amplitude of the detected spectral peak. It represents the strength or magnitude
     * of the peak in the FFT data, typically reflecting how prominent the peak is within the frequency spectrum.
     */
    
	double peakAmp;
	
};

/**
 * @brief A class for detecting and analyzing spectral peaks in FFT data.
 *
 * The HISSTools_Spectral_Peaks class is designed to perform spectral peak analysis on data processed through
 * FFT (Fast Fourier Transform). It identifies and extracts relevant information about spectral peaks, such as their
 * frequency, amplitude, and position in the frequency spectrum. This class provides methods for analyzing FFT data
 * and handling spectral peak detection with configurable parameters.
 */

class HISSTools_Spectral_Peaks
{
	
public:
	
    /**
     * @brief Constructor for the HISSTools_Spectral_Peaks class.
     *
     * This constructor initializes the spectral peaks analysis object with the given maximum FFT size.
     * It allocates any necessary resources based on the provided FFT size and prepares the class for processing spectral data.
     *
     * @param maxFFTSize The maximum size of the FFT (Fast Fourier Transform) that will be processed by this class.
     */
    
	HISSTools_Spectral_Peaks(long maxFFTSize)
	{
		maxFFTSize = maxFFTSize < 8 ? 1 : maxFFTSize;		
		mPeakData = new FFTPeak[(maxFFTSize >> 1) / 3 + 1];

		if (mPeakData)
			mMaxFFTSize = maxFFTSize;
		
		mNPeaks = 0;
		mFFTSize = 0;
	};
	
    /**
     * @brief Destructor for the HISSTools_Spectral_Peaks class.
     *
     * This destructor is responsible for cleaning up resources used by the HISSTools_Spectral_Peaks object.
     * It ensures that any memory or resources allocated during the lifetime of the object are properly released
     * when the object is destroyed.
     */
    
	~HISSTools_Spectral_Peaks()
	{
		delete[] mPeakData;
	};
	
	
private:
	
    /**
     * @brief Clips the provided read bin index to ensure it falls within valid FFT bounds.
     *
     * This function ensures that the given `readBin` value is within the acceptable range based on the FFT size and format.
     * It helps prevent out-of-bounds errors by adjusting the read bin index according to the FFT data's structure.
     *
     * @param readBin The index of the bin to be read from the FFT data.
     * @param FFTSize The size of the FFT (number of bins).
     * @param format The format of the spectrum, which can influence how bins are interpreted or processed.
     * @return The adjusted bin index, ensuring it stays within the valid range for the FFT size and format.
     */
    
	unsigned long clipReadBin(unsigned long readBin, unsigned long FFTSize, PSpectrumFormat format)
	{
		unsigned long nyquistBin = (FFTSize >> 1);
		
		// Clip bin according to the input format
		
		switch (format) 
		{
			case kSpectrumNyquist:
				if (readBin < nyquistBin)
					return readBin;
				return FFTSize - readBin;
				
			case kSpectrumFull:
				return readBin;
		}
		
		return 0;
	}
	
    /**
     * @brief Interpolates the position and amplitude of a spectral peak using parabolic interpolation.
     *
     * This function performs parabolic interpolation to estimate the precise frequency and amplitude of a spectral peak
     * based on three adjacent FFT bins. It improves the accuracy of peak detection by adjusting the peak location
     * between FFT bins.
     *
     * @param a The amplitude value of the bin before the peak.
     * @param b The amplitude value of the peak bin.
     * @param c The amplitude value of the bin after the peak.
     * @param peakBin The index of the peak bin in the FFT data.
     * @param FFTSize The size of the FFT (number of bins).
     * @param peakAmp A pointer to store the interpolated amplitude of the peak.
     * @return The interpolated frequency of the peak, adjusted to a more precise location between bins.
     */
    
	double interpolatePeak(double a, double b, double c, long peakBin, long FFTSize, double *peakAmp)
	{				
		// Peak interpolation (amplitude and bin location)
		
		double d = a + c - (2.0 * b);
		double p = d ? (0.5 * (a - c)) / d: 0;
		
		*peakAmp = b - (0.25 * (a - c) * p);
		return (peakBin + p) / FFTSize;
	}
	
public:
	
    /**
     * @brief Retrieves the size of the FFT used by the HISSTools_Spectral_Peaks object.
     *
     * This function returns the current size of the FFT (Fast Fourier Transform) being processed.
     * The FFT size corresponds to the number of frequency bins in the spectrum.
     *
     * @return The size of the FFT as an unsigned long.
     */
    
	unsigned long getFFTSize()
	{
		return mFFTSize;
	}
	
    /**
     * @brief Retrieves the starting bin index for a given peak.
     *
     * This function returns the index of the start bin for a specific spectral peak in the FFT data.
     * It helps identify where the peak begins within the frequency spectrum.
     *
     * @param peak The index of the peak for which the starting bin is requested.
     * @return The starting bin index for the given peak as an unsigned long.
     */
    
	unsigned long getStartBin (long peak)
	{
		return mPeakData[peak].startBin;
	}
	
    /**
     * @brief Retrieves the ending bin index for a given peak.
     *
     * This function returns the index of the end bin for a specific spectral peak in the FFT data.
     * It identifies where the peak ends within the frequency spectrum, allowing for precise peak boundaries.
     *
     * @param peak The index of the peak for which the ending bin is requested.
     * @return The ending bin index for the given peak as an unsigned long.
     */
    
	unsigned long getEndBin (unsigned long peak)
	{
		if (peak < mNPeaks - 1)
			return mPeakData[peak + 1].startBin;
		
		return HISSTools_PSpectrum::calcMaxBin(mFFTSize, kSpectrumNyquist);
	}
	
    /**
     * @brief Retrieves the bin index corresponding to the peak's highest amplitude.
     *
     * This function returns the bin index where the highest amplitude for a given spectral peak is located within the FFT data.
     * It helps identify the central point of the peak in the frequency spectrum.
     *
     * @param peak The index of the peak for which the peak bin is requested.
     * @return The bin index where the peak's amplitude is at its maximum as an unsigned long.
     */
    
	unsigned long getPeakBin (unsigned long peak)
	{
		return mPeakData[peak].peakBin;
	}
	
    /**
     * @brief Retrieves the frequency of a specified spectral peak.
     *
     * This function returns the frequency (in Hz) of the specified peak based on its position in the FFT spectrum.
     * It calculates the frequency using the peak's bin index and the FFT size.
     *
     * @param peak The index of the peak for which the frequency is requested.
     * @return The frequency of the given peak as a double.
     */
    
	double getPeakFreq (unsigned long peak)
	{
		return mPeakData[peak].peakFreq;
	}
	
    /**
     * @brief Retrieves the amplitude of a specified spectral peak.
     *
     * This function returns the amplitude of the specified peak in the FFT spectrum.
     * The amplitude represents the strength or magnitude of the peak at the given bin index.
     *
     * @param peak The index of the peak for which the amplitude is requested.
     * @return The amplitude of the given peak as a double.
     */
    
	double getPeakAmp (unsigned long peak)
	{
		return mPeakData[peak].peakAmp;
	}
	
    /**
     * @brief Retrieves the total number of detected spectral peaks.
     *
     * This function returns the total number of spectral peaks that have been identified in the FFT data.
     * It provides a count of the peaks currently available for analysis.
     *
     * @return The number of detected peaks as an unsigned long.
     */
    
	unsigned long getNPeaks ()
	{
		return mNPeaks;
	}
	
    /**
     * @brief Detects spectral peaks from the provided input spectrum.
     *
     * This function analyzes the given input spectrum and identifies spectral peaks based on amplitude and other criteria.
     * It processes the FFT data and extracts peak information, storing it for further analysis.
     *
     * @param inSpectrum A pointer to an input spectrum (HISSTools_PSpectrum) to be analyzed for peak detection.
     * @return Returns `true` if peaks were successfully found, otherwise returns `false`.
     */
    
	bool findPeaks (HISSTools_PSpectrum *inSpectrum)
	{		
		PSpectrumFormat format = inSpectrum->getFormat();
		FFTPeak *peakData = mPeakData;
		
		double *spectrum = inSpectrum->getSpectrum();
		double v1, v2, v3, v4, v5;
		double peakFreq, peakAmp;
		double minVal = HUGE_VAL;
		
		unsigned long FFTSize = inSpectrum->getFFTSize();
		unsigned long highestBin = HISSTools_PSpectrum::calcMaxBin(FFTSize, kSpectrumNyquist);
		unsigned long readBin;
		unsigned long minBin = 0;
		unsigned long NPeaks = 0;
		unsigned long i;

		// Sanity Check
		
		if (FFTSize > mMaxFFTSize)
			return FALSE;
		
		// Initialise peak array
		
		v2 = spectrum[2];
		v3 = spectrum[1];
		v4 = spectrum[0];
		v5 = spectrum[1];
		
		// Loop over spectrum to find peaks
		
		for (i = 0; i < highestBin; i++)
		{
			readBin = clipReadBin(i + 2, FFTSize, format);
			
			v1 = v2;
			v2 = v3;
			v3 = v4;
			v4 = v5;
			v5 = spectrum[readBin];
			
			if (v3 > v2 && v3 > v1 && v3 > v4 && v3 > v5)
			{
				// We have new peak - can skip two bins (which cannot by defintion by peaks)
				
				peakFreq = interpolatePeak(v2, v3, v4, i, FFTSize, &peakAmp);
				
				peakData[NPeaks].startBin = minBin;
				peakData[NPeaks].peakBin = i;
				peakData[NPeaks].peakFreq = peakFreq;
				peakData[NPeaks].peakAmp = peakAmp;
				
				minVal = v4 < v5 ? v4 : v5;
				minBin = v4 < v5 ? i + 1 : i + 2;
				
				NPeaks++;
				i += 2;
			}
			else 
			{
				// Check for the minimum inter-peak bin
				
				if (v3 < minVal)
				{
					minVal = v3;
					minBin = i;
				}
			}

		}
		
		mNPeaks = NPeaks;
		mFFTSize = FFTSize;
		
		return TRUE;
	}
	
private:
	
	// Data
	
    /**
     * @brief A pointer to the array of FFTPeak structures containing data for detected spectral peaks.
     *
     * This variable points to an array that stores information about the detected spectral peaks.
     * Each element in the array is an instance of the FFTPeak structure, which holds details such as
     * the frequency, amplitude, and bin location of each peak.
     */
    
	FFTPeak *mPeakData;
		
	// Current Parameters
	
    /**
     * @brief The size of the FFT used in the spectral peak analysis.
     *
     * This variable stores the number of bins (FFT size) used for processing the input spectrum.
     * It defines the resolution of the FFT and determines the frequency range and granularity of the analysis.
     */
    
	unsigned long mFFTSize;
    
    /**
     * @brief The number of spectral peaks detected in the FFT analysis.
     *
     * This variable stores the total number of peaks that have been identified in the current FFT data.
     * It represents the count of detected spectral peaks available for analysis.
     */
    
	unsigned long mNPeaks;
	
	// Maximum FFT Size
	
    /**
     * @brief The maximum FFT size that the HISSTools_Spectral_Peaks object can process.
     *
     * This variable stores the maximum number of bins (FFT size) that the object is configured to handle.
     * It defines the upper limit for the FFT size used in spectral analysis and resource allocation.
     */
    
	unsigned long mMaxFFTSize;
	
};

#endif
