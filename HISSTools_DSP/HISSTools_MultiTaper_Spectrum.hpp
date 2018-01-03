

#ifndef __HISSTOOLS_MULTITAPER_SPECTRUM__
#define __HISSTOOLS_MULTITAPER_SPECTRUM__


#include "HISSTools_FFT.hpp"


class HISSTools_MultiTaper_Spectrum : protected HISSTools_FFT, protected HISSTools_FSpectrum 
{
	
public:
	
	HISSTools_MultiTaper_Spectrum (unsigned long maxFFTSize, PSpectrumFormat format = kSpectrumNyquist) : HISSTools_FFT(maxFFTSize * 2), HISSTools_FSpectrum(maxFFTSize * 2, kSpectrumComplex)
	{		
	}
	
	~HISSTools_MultiTaper_Spectrum()
	{
	}
	
private:
	
	double estimateDifferential(double pm1, double p0, double pp1, double binWidth)
	{
		return (pm1 + pp1 - (2.0 * p0)) / (binWidth * binWidth);
	}
	
	double estimateDifferential(double pm2, double pm1, double p0, double pp1,  double pp2, double binWidth)
	{
		return (16.0 * (pm1 + pp1) - (30.0 * p0) - (pm2 + pp2)) / (12.0 * binWidth * binWidth);
	}
	
	double optimalTapers(double powValue, double powDifferential, unsigned long N)
	{
		double kTapers = (12 * powValue * (N * N)) / powDifferential;
		kTapers = pow(kTapers * kTapers, 1.0/5.0);
		kTapers = kTapers < 1.0 ? 1.0 : kTapers;
		kTapers = kTapers < (N >> 2) ? kTapers : (N >> 2);

		kTapers = kTapers < 20 ? kTapers : 20;
		
		return kTapers;//(long) round(kTapers);
	}
	
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