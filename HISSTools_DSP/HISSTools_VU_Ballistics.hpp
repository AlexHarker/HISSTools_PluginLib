
/**
 * @file HISSTools_VU_Ballistics.hpp
 * @brief Header file for the HISSTools VU Ballistics system.
 *
 * This file contains the declaration of the HISSTools_VU_Ballistics class, which manages
 * VU meter ballistics for multi-channel audio processing. It includes methods for calculating
 * peak, RMS, and peak hold values, as well as handling LED meter states.
 *
 * The file defines:
 * - Constants for attack and decay times.
 * - Variables for tracking VU levels, peak hold values, and channel states.
 * - Methods for calculating VU levels, checking signal thresholds, and retrieving meter states.
 *
 * This class supports up to 256 audio channels and provides real-time analysis of audio signal dynamics.
 */

#ifndef __HISSTOOLS_VU_BALLISTICS__
#define __HISSTOOLS_VU_BALLISTICS__

/**
 * @brief Constants used for VU meter ballistics.
 *
 * These constants define the behavior of the VU meter, including attack and decay times,
 * the RMS time constant, and the number of samples for peak hold.
 *
 * @var METER_ATTACK Defines the attack time of the meter (0.8 seconds).
 * @var METER_DECAY Defines the decay time of the meter (0.12 seconds).
 * @var RMS_TIME_CONST Defines the time constant for RMS calculation (0.1 seconds).
 * @var PEAK_HOLD_SAMPLES Number of samples to hold the peak level (22050 samples, usually 0.5 seconds at 44.1 kHz).
 */

const double METER_ATTACK = 0.8, METER_DECAY = 0.12, RMS_TIME_CONST = 0.1, PEAK_HOLD_SAMPLES = 22050;

/**
 * @brief Constants used for LED meter ballistics.
 *
 * These constants define the behavior of the LED meters, specifically the attack and decay times.
 *
 * @var LED_ATTACK Defines the attack time for the LED meter (1.0 seconds).
 * @var LED_DECAY Defines the decay time for the LED meter (0.4 seconds).
 */

const double LED_ATTACK = 1.0, LED_DECAY = 0.4;

/**
 * @class HISSTools_VU_Ballistics
 * @brief A class for managing VU meter ballistics and levels.
 *
 * This class provides functionality for calculating and tracking VU levels, peak levels,
 * RMS values, and peak hold times for multi-channel audio signals. It is designed to handle
 * up to 256 channels and applies ballistics calculations to simulate the behavior of analog meters.
 *
 * Features include:
 * - Peak detection and hold.
 * - RMS level calculations.
 * - Adjustable attack and decay times for meter response.
 */

class HISSTools_VU_Ballistics
{
	
private:
	
    /**
     * @brief Stores the number of audio channels.
     *
     * This variable holds the number of channels being processed by the VU ballistics system.
     * It is used to manage the peak and RMS levels for each individual channel.
     */
    
	int mNumChans;
	
    /**
     * @brief Tracks the peak hold time duration.
     *
     * This variable stores the amount of time that the peak value is held before it starts to decay.
     * It is used to maintain the highest peak level for a specified duration, simulating a hold behavior
     * typically seen in VU and peak meters.
     */
    
	double mPeakHoldTime;
	
	// Previous Values
	
    /**
     * @brief Stores the most recent peak value.
     *
     * This variable holds the last calculated peak value for the audio signal.
     * It is used for comparing and updating the peak level during subsequent calculations.
     */
    
	double mLastPeak;
    
    /**
     * @brief Stores the most recent RMS (Root Mean Square) value.
     *
     * This variable holds the last calculated RMS value of the audio signal,
     * which represents the average power of the signal. It is used for tracking and
     * updating the RMS level during subsequent calculations.
     */
    
	double mLastRMS;
    
    /**
     * @brief Stores the most recent peak hold value.
     *
     * This variable holds the last peak hold value, which represents the highest peak
     * that has been held for a set duration. It is used to manage and update the peak hold
     * behavior during VU meter ballistics calculations.
     */
    
	double mLastPeakHold;
	
	// Values
	
    /**
     * @brief Stores the peak values for each audio channel.
     *
     * This array holds the current peak values for up to 256 audio channels.
     * Each element in the array corresponds to the peak level of an individual channel,
     * allowing the system to track and process the peak values separately for each channel.
     *
     * @var mPeaks Array size is 256, representing up to 256 audio channels.
     */
    
	double mPeaks[256];
    
    /**
     * @brief Stores the peak hold values for each audio channel.
     *
     * This array holds the peak hold values for up to 256 audio channels.
     * Each element in the array corresponds to the peak hold level of an individual channel,
     * ensuring that the peak level is maintained for a specified duration before decaying.
     *
     * @var mPeakHolds Array size is 256, representing up to 256 audio channels.
     */
    
	double mPeakHolds[256];
	
public:
	
    /**
     * @brief Constructor for HISSTools_VU_Ballistics class.
     *
     * Initializes the peak, RMS, and peak hold values, as well as setting up the arrays
     * for tracking peak levels and peak hold levels for up to 256 channels.
     */
    
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
	
    /**
     * @brief Calculates the VU meter levels for the given audio input.
     *
     * This method processes the input audio data to compute the VU levels, including peak and RMS values,
     * for each channel. It updates the peak, RMS, and peak hold values for all channels based on the
     * provided input frames and channels.
     *
     * @param ins A 2D array of input audio samples where each element represents an audio channel.
     * @param nChans The number of audio channels being processed.
     * @param nFrames The number of audio frames (samples) to process per channel.
     */
    
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
	
    /**
     * @brief Checks if the audio signal has exceeded a defined threshold.
     *
     * This method returns a boolean value indicating whether any channel's peak level
     * has exceeded a certain threshold, typically used to detect clipping or overloading
     * in the audio signal.
     *
     * @return True if the signal has gone over the threshold, false otherwise.
     */
    
	bool getOver()
	{
		return mLastPeakHold >= 1.0;
	}
	
    /**
     * @brief Retrieves the current peak hold value.
     *
     * This method returns the highest peak value that has been held for the defined peak hold time.
     * It is typically used to display the peak hold level in a VU meter.
     *
     * @return The current peak hold value as a double.
     */
    
	double getPeakHold()
	{
		return mLastPeakHold;
	}
	
    /**
     * @brief Retrieves the current peak value.
     *
     * This method returns the most recent peak value for the audio signal,
     * representing the highest amplitude detected during the current measurement period.
     *
     * @return The current peak value as a double.
     */
    
	double getPeak()
	{
		return mLastPeak;
	}
	
    /**
     * @brief Retrieves the current RMS (Root Mean Square) value.
     *
     * This method returns the most recent RMS value of the audio signal,
     * which represents the average power of the signal over time.
     *
     * @return The current RMS value as a double.
     */
    
	double getRMS()
	{
		return mLastRMS;
	}
	
    /**
     * @brief Retrieves the LED VU meter state for a specific channel.
     *
     * This method returns the current state of the LED VU meter for the specified channel,
     * indicating whether the LED should be on or off based on the audio signal's level.
     *
     * @param chan The channel number for which the LED VU state is being retrieved.
     * @return An unsigned char representing the LED state (on/off) for the specified channel.
     */
    
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
