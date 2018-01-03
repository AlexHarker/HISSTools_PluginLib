
#ifndef __HISSTOOLS_VU_BALLISTICS__
#define __HISSTOOLS_VU_BALLISTICS__

const double METER_ATTACK = 0.8, METER_DECAY = 0.12, RMS_TIME_CONST = 0.1, PEAK_HOLD_SAMPLES = 22050;  
const double LED_ATTACK = 1.0, LED_DECAY = 0.4;  


class HISSTools_VU_Ballistics
{
	
private:
	
	int mNumChans;
	
	double mPeakHoldTime;
	
	// Previous Values
	
	double mLastPeak;
	double mLastRMS;
	double mLastPeakHold;
	
	// Values
	
	double mPeaks[256];
	double mPeakHolds[256];
	
public:
	
	HISSTools_VU_Ballistics()
	{
		mLastPeak = 0;
		mLastRMS = 0;
		mLastPeakHold = 0;
		mPeakHoldTime = 0;
		
		for (int i = 0; i < 256; i++)
		{
			mPeaks[i] = 0.;
			mPeakHolds[i] = 0.;
		}
		
	}
	
	void calcVULevels(double **ins, long nChans, long nFrames)
	{
		double localPeak;
		double peak = 0;
		double rms = 0;
		double interp;
		double peakHold;
		
		for (int i = 0; i < nChans; i++)
		{
			localPeak = 0.;
			
			for (int j = 0; j < nFrames; j++)
			{
				localPeak = fabs(ins[i][j]) > localPeak ? fabs(ins[i][j]) : localPeak;
				rms += ins[i][j] * ins[i][j];
			}
			
			peak = localPeak > peak ? localPeak : peak;
			
			interp = (localPeak > mPeaks[i] ? LED_ATTACK : LED_DECAY);
			localPeak = mPeaks[i] - interp * (mPeaks[i] - localPeak);
			
			if (localPeak > mPeaks[i] || mPeakHolds[i] > PEAK_HOLD_SAMPLES)
			{
				mPeaks[i] = localPeak;
				mPeakHolds[i] = 0;
			}
			else
				mPeakHolds[i] += nFrames;
		}   
		
		if (peak > mLastPeakHold || mPeakHoldTime > PEAK_HOLD_SAMPLES)
		{
			peakHold = peak;
			mPeakHoldTime = 0;
		}
		else 
		{
			peakHold = mLastPeakHold;
			mPeakHoldTime += nFrames;
		}
		
		rms /= nFrames * nChans;
		rms = sqrt(rms);
		rms = mLastRMS + RMS_TIME_CONST * (rms -  mLastRMS); 
		
		interp = (peak > mLastPeak ? METER_ATTACK : METER_DECAY);
		peak = mLastPeak - interp * (mLastPeak - peak);
		
		mLastPeak = peak;
		mLastRMS = rms;
		mLastPeakHold = peakHold;
	}
	
	bool getOver()
	{
		return mLastPeakHold >= 1.0;
	}
	
	double getPeakHold()
	{
		return mLastPeakHold;
	}
	
	double getPeak()
	{
		return mLastPeak;
	}
	
	double getRMS()
	{
		return mLastRMS;
	}
	
	unsigned char getledVUState(int chan)
	{
		if (mPeaks[chan] < 0.001)
			return 0;
		if (mPeaks[chan] < 0.01)
			return 1;
		if (mPeaks[chan] < 0.1)
			return 2;
		if (mPeaks[chan] < 0.2)
			return 3;
		if (mPeaks[chan] < 0.4)
			return 4;
		if (mPeaks[chan] < 1.0)
			return 5;		
		
		return 6;		
	}
};

#endif /* __HISSTOOLS_VU_BALLISTICS__ */