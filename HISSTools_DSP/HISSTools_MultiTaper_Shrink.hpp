

#ifndef __HISSTools_MULTITAPER_SHRINK__
#define __HISSTools_MULTITAPER_SHRINK__


#include "HISSTools_FSpectrum.hpp"
#include "HISSTools_MultiTaper_Spectrum.hpp"
#include "HISSTools_DWT.hpp"


enum ShrinkTypes {
		
		SHRINK_UNIVERSAL_SOFT = 0,
		SHRINK_UNIVERSAL_MID = 1,
		SHRINK_UNIVERSAL_HARD = 2,
};


class HISSTools_MultiTaper_Shrink : protected HISSTools_MultiTaper_Spectrum, protected HISSTools_DWT, protected HISSTools_PSpectrum
{
	
public:
	
	HISSTools_MultiTaper_Shrink(unsigned long maxFFTSize, HISSTools_Wavelet *wavelet, PSpectrumFormat format = kSpectrumNyquist):
	HISSTools_MultiTaper_Spectrum(maxFFTSize, kSpectrumFull), HISSTools_DWT(maxFFTSize), HISSTools_PSpectrum(maxFFTSize, kSpectrumFull)
	{			
		mWavelet = wavelet;
	}
	
	~HISSTools_MultiTaper_Shrink()
	{
	}
	
private:
	
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
	
	
	double digamma(long x)
	{
		// Calculates diagamma for integer values
		
		double e_m_const = 0.57721566490153286060651209008240243104215933593992;
		double sum = 0;
			
		for (long i = 1; i < x ; i++)
				sum += 1. / (double) i;
			
		return sum - e_m_const;
	}
	
	
	double trigamma(long x)
	{
		// Calculates trigamma for integer values

		double recur = M_PI * M_PI / 6.;
		
		for (long i = 1; i < x; i++)
			recur -= 1 / (double) (i * i);
		
		return recur;
	}
	
	
public:
	
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
	
	HISSTools_Wavelet *mWavelet;	
	HISSTools_PSpectrum *mTempPowerSpectrum;
};


#endif