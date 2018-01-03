

#ifndef __HISSTOOLS_SPECTRAL_PEAKS__
#define __HISSTOOLS_SPECTRAL_PEAKS__


#include "HISSTools_PSpectrum.hpp"


struct FFTPeak {
	
	long startBin;
	long peakBin;
	
	double peakFreq;
	double peakAmp;
	
};


class HISSTools_Spectral_Peaks
{
	
public:
	
	HISSTools_Spectral_Peaks(long maxFFTSize) 
	{
		maxFFTSize = maxFFTSize < 8 ? 1 : maxFFTSize;		
		mPeakData = new FFTPeak[(maxFFTSize >> 1) / 3 + 1];

		if (mPeakData)
			mMaxFFTSize = maxFFTSize;
		
		mNPeaks = 0;
		mFFTSize = 0;
	};
	
	~HISSTools_Spectral_Peaks() 
	{
		delete[] mPeakData;
	};
	
	
private:
	
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
	
	
	double interpolatePeak(double a, double b, double c, long peakBin, long FFTSize, double *peakAmp)
	{				
		// Peak interpolation (amplitude and bin location)
		
		double d = a + c - (2.0 * b);
		double p = d ? (0.5 * (a - c)) / d: 0;
		
		*peakAmp = b - (0.25 * (a - c) * p);
		return (peakBin + p) / FFTSize;
	}
	
public:
	
	
	unsigned long getFFTSize()
	{
		return mFFTSize;
	}
	
	
	unsigned long getStartBin (long peak)
	{
		return mPeakData[peak].startBin;
	}
	
	
	unsigned long getEndBin (unsigned long peak)
	{
		if (peak < mNPeaks - 1)
			return mPeakData[peak + 1].startBin;
		
		return HISSTools_PSpectrum::calcMaxBin(mFFTSize, kSpectrumNyquist);
	}
	
	
	unsigned long getPeakBin (unsigned long peak)
	{
		return mPeakData[peak].peakBin;
	}
	
	
	double getPeakFreq (unsigned long peak)
	{
		return mPeakData[peak].peakFreq;
	}
	
	
	double getPeakAmp (unsigned long peak)
	{
		return mPeakData[peak].peakAmp;
	}
	
	
	unsigned long getNPeaks ()
	{
		return mNPeaks;
	}
	
	
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
	
	FFTPeak *mPeakData;
		
	// Current Parameters
	
	unsigned long mFFTSize;
	unsigned long mNPeaks;
	
	// Maximum FFT Size
	
	unsigned long mMaxFFTSize;
	
};

#endif