
#ifndef __HISSTOOLS_CONTROLS__
#define __HISSTOOLS_CONTROLS__

#include "HISSTools_Control_Helpers.hpp"
#include <vector>

#include <IPlugAPIBase.h>
#include <IControl.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// HISSTools VU ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// A VU meter display. Ballistics should be provided on the DSP side
// The meter supports two on meter levels (drawn in order) and a side value, typically intended for peak hold


class HISSTools_VUMeter : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    enum
    {
        kUpdateTag = 0,
    };
    
    struct MeterValues
    {
        double mVU1;
        double mVU2;
        double mSide;
        bool mPeak;
        bool mLinear;
    };
    
public:
        
    class Sender
    {
        
    public:
        
        Sender(int controlTag) : mControlTag(controlTag), mQueue(32) {}
        
        void Set(double VU1, double VU2, double side, bool peak, bool linear = true)
        {
            mQueue.Push(MeterValues{ VU1, VU2, side, peak, linear });
        }
        
        void UpdateControl(IEditorDelegate& dlg)
        {
            while (mQueue.ElementsAvailable())
            {
                MeterValues v;
                mQueue.Pop(v);
                dlg.SendControlMsgFromDelegate(mControlTag, kUpdateTag, sizeof(MeterValues), (void*) &v);
            }
        }
        
        void Reset()
        {
            MeterValues v;
            
            while (mQueue.ElementsAvailable())
                mQueue.Pop(v);
        }
        
    private:
        
        int mControlTag;
        IPlugQueue<MeterValues> mQueue;
    };
    
private:

	// Positioning / Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	double mTick1;
	double mTick2;
	double mTick3;
	double mTick4;
	
	// Line Thicknesses
	
	double mOutlineTK;
	double mTickTK;
	double mPeakHoldTK;
	
	// Shadow Spec
	
	HISSTools_Shadow *mShadow;
	
	// Drawing parameters
		
	double mVU1Size;
	double mVU2Size;
	double mSideSize;
	
	bool mPeak;

	// Values
	
	double mMinDB;
	double mMaxDB;
		
	// Color Specs
	
	HISSTools_Color_Spec *mBackgroundCS;
	HISSTools_Color_Spec *mOutlineCS;
	HISSTools_Color_Spec *mVU1CS;
	HISSTools_Color_Spec *mVU2CS;
	HISSTools_Color_Spec *mVUSideCS;
	HISSTools_Color_Spec *mVU1PeakCS;
	HISSTools_Color_Spec *mVU2PeakCS;
	HISSTools_Color_Spec *mVUSidePeakCS;
	
	bool mOverlayFixedGradientBox;
	
	// N.B. currently we linearly interpolate dB but we could do something nicer here later.....
	
	double getSize(double value, bool linear)
	{
		double db = linear ? 20. * log10(value) : value;
		double size = (db - mMinDB) / (mMaxDB - mMinDB);
		
		if (size != size)
			size = 0;
		
		size = size > 1 ? 1 : size;
		
		return size;
	}
	
	void horzTick(HISSTools_VecLib& vecDraw, double x1, double x2, double y, double h, double normPosition, double thickness)
	{
		double yPos =  y + h * (1 - normPosition);
		
		vecDraw.line(x1, yPos, x2, yPos, thickness);
	}
	
	void vertTick(HISSTools_VecLib& vecDraw, double y1, double y2, double x, double w, double normPosition, double thickness)
	{
		double xPos =  x + w * normPosition;
		
		vecDraw.line(xPos, y1, xPos, y2, thickness);
	}
	
public:
	
	HISSTools_VUMeter(double x, double y, double w, double h, bool flip = false, double minDB = -60, double maxDB = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(IRECT()), HISSTools_Control_Layers()
	{
		// Dimensions
		
		// Parameters
		
		double VUPadding = 8;
		double VUTickPad = 6;
		double VUTicks = 4;
		
		if (w < h)
		{
			mX = x + VUPadding;
			mY = y;
			mW = w - (2 * VUPadding);
			mH = h;
			mTick1 = flip ? mX + mW + (VUTickPad - VUTickPad) : mX - VUTickPad;
			mTick2 = flip ? mX + mW + VUTickPad : mX - VUTickPad + VUTicks;
			mTick3 = flip ? mX + mW : mX - VUPadding;
			mTick4 = flip ? mX + mW + VUPadding : mX;
		}
		else 
		{
			mX = x;
			mY = y + VUPadding;
			mW = w;
			mH = h - (2 * VUPadding);
			mTick1 = flip ? mY + mH + (VUTickPad - VUTickPad) : mY - VUTickPad;
			mTick2 = flip ? mY + mH + VUTickPad : mY - VUTickPad + VUTicks;
			mTick3 = flip ? mY + mH : mY - VUPadding;
			mTick4 = flip ? mY + mH + VUPadding : mY;
		}
		
		// Get Appearance
		
		mOutlineTK = designScheme->getDimension("VUOutline", type);
		mTickTK = designScheme->getDimension("VUTick", type);
		mPeakHoldTK = designScheme->getDimension("VUPeakHold", type);
		
		mShadow = designScheme->getShadow("Meter", type);

		mOverlayFixedGradientBox = designScheme->getFlag("VUOverlayFixedGradientBox", type);
	
		mBackgroundCS = designScheme->getColorSpec("VUBackground", type);
		mOutlineCS = designScheme->getColorSpec("VUOutline", type);
		mVU1CS = designScheme->getColorSpec("VU1", type);
		mVU1PeakCS = designScheme->getColorSpec("VU1Peak", type);
		mVU2CS = designScheme->getColorSpec("VU2", type);
		mVU2PeakCS = designScheme->getColorSpec("VU2Peak", type);
		mVUSideCS = designScheme->getColorSpec("VUSide", type);
		mVUSidePeakCS = designScheme->getColorSpec("VUSidePeak", type);
		
		mMinDB = minDB;
		mMaxDB = maxDB;

		mVU1Size = 0;
		mVU2Size = 0;
		mSideSize = 0;
		
		mPeak = false;
		
		// Area
		
		// FIX - Hack
		
		mRECT = IRECT(floor(mX) - 10, floor(mY) - 10, ceil(mX + mW) + 10, ceil(mY + mH) + 10);
	}
	
	~HISSTools_VUMeter() {}
	
    void OnMsgFromDelegate(int messageTag, int dataSize, const void* pData) override
	{
        if (messageTag == kUpdateTag && dataSize == sizeof(MeterValues))
        {
            MeterValues* pTypedData = (MeterValues*) pData;
            
            mVU1Size = getSize(pTypedData->mVU1, pTypedData->mLinear);
            mVU2Size = getSize(pTypedData->mVU2, pTypedData->mLinear);
            mSideSize = getSize(pTypedData->mSide, pTypedData->mLinear);
            mPeak = pTypedData->mPeak;
		
            SetDirty(false);
        }
	}
	
	void Draw(IGraphics& g) override
	{
        HISSTools_VecLib vecDraw(g);
		vecDraw.setColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);

		// Parameters
		
		int nTicks = 10;
		
		if (StartBackground(vecDraw, mRECT))
		{
			vecDraw.setColor(mBackgroundCS);
			vecDraw.fillRect(mX, mY, mW, mH);
		
			// Frame Rectangle
		
			vecDraw.startShadow(mShadow, mRECT);
			vecDraw.setColor(mOutlineCS);
			vecDraw.frameRect(mX, mY, mW, mH, mOutlineTK);
			vecDraw.renderShadow();
		}
		
        RenderBackground(vecDraw, mRECT);
		
		// Meter Rectangles
		
		vecDraw.forceGradientBox(mX, mY, mX + mW, mY + mH);

		// VU 1
		
		if (mVU1Size > 0)
		{
			if (mPeak)
				vecDraw.setColor(mVU1PeakCS);
			else
				vecDraw.setColor(mVU1CS);
		
			if (mW < mH)
				vecDraw.fillRect(mX, mY + mH * (1 - mVU1Size), mW, mH * mVU1Size);
			else 
				vecDraw.fillRect(mX, mY, mW * mVU1Size, mH);
		}
		
		// VU 2
		
		if (mVU2Size > 0)
		{
			if (mOverlayFixedGradientBox == false)
				vecDraw.forceGradientBox();
			
			if (mPeak)
				vecDraw.setColor(mVU2PeakCS);
			else
				vecDraw.setColor(mVU2CS);
					
			if (mW < mH)
				vecDraw.fillRect(mX, mY + mH * (1 - mVU2Size),  mW, mH * mVU2Size);
			else
				vecDraw.fillRect(mX, mY, mW * mVU2Size, mH);
		}
		
		// Ticks
		
		vecDraw.setColor(mOutlineCS);
				
		if (mW < mH)
			for (int i = 0; i < nTicks; i++)
				horzTick(vecDraw, mTick1, mTick2, mY, mH, (i / (double) (nTicks - 1)), mTickTK);
		else
			for (int i = 0; i < nTicks; i++)
				vertTick(vecDraw, mTick1, mTick2, mX, mW, (i / (double) (nTicks - 1)), mTickTK);
			
		// Side VU
		
		if (mSideSize >= 0)
		{
			vecDraw.forceGradientBox(mX, mY, mX + mW, mY + mH);
		
			if (mPeak)
				vecDraw.setColor(mVUSidePeakCS);
			else
				vecDraw.setColor(mVUSideCS);
		
			if (mW < mH)
				horzTick(vecDraw, mTick3, mTick4, mY, mH, mSideSize, mPeakHoldTK);
			else
				vertTick(vecDraw, mTick3, mTick4, mX, mW, mSideSize, mPeakHoldTK);
		}
         
		vecDraw.forceGradientBox();
		
		// Outline Again
		// FIX - Draw ALL PROPERLY
		
		vecDraw.setColor(mOutlineCS);
		vecDraw.frameRect(mX, mY, mW, mH, mOutlineTK);

		// Reset Orientation (default is always horizontal)
		
		vecDraw.setColorOrientation(kCSOrientHorizontal);
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// HISSTools FileSelector ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


class HISSTools_FileSelector : public HISSTools_Button
{
	
public:
	
	enum EFileSelectorState { kFSNone, kFSSelecting, kFSDone };
	
private:
	
	// Strings
	
	WDL_String mDir;
	WDL_String mFile;
	WDL_String mExtensions;
	
	// States
	
	EFileAction mFileAction;
	EFileSelectorState mState;
		
private:

	virtual void reportToPlug() {}
	
public:
	
	// FIX - turn automation off (also for matrix)
		
	HISSTools_FileSelector(int paramIdx, double x, double y, double w, double h, EFileAction action, char* dir = "", char* extensions = "", const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char *label = "")
	: HISSTools_Button(paramIdx, x, y, w, h, type, designScheme, label) , mState(kFSNone), mFileAction(action), mDir(dir), mExtensions(extensions)
	{}

    void OnMouseDown(float x, float y, const IMouseMod& pMod) override
    {
        mState = kFSSelecting;
        SetDirty(false);
    }
    
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override
    {
        if (mRECT.Contains(x, y))
        {
            if (mState != kFSSelecting)
                SetDirty(false);
            mState = kFSSelecting;
        }
        else
        {
            if (mState != kFSNone)
                SetDirty(false);
            mState = kFSNone;
        }
    }

	void OnMouseUp(float x, float y, const IMouseMod& pMod) override
	{
        if (!mRECT.Contains(x, y))
            return;
        
		if (GetUI())
		{
			WDL_String tempFile;

			if (pMod.A)
			{
				mState = kFSDone;

				mFile.Set("");
				reportToPlug();
			}
			else 
			{
				GetUI()->PromptForFile(tempFile, mDir, mFileAction, mExtensions.Get());
				
				mState = kFSDone;
				
				if (tempFile.GetLength())
				{
					mFile.Set(tempFile.Get());
					reportToPlug();
				}
			}
			
			SetDirty(false);
		}
	}
	
	void Draw(IGraphics& g) override
	{
		switch (mState)
		{
			case kFSDone:
			case kFSNone:
                SetValue(0);
				break;
				
			case kFSSelecting:
				SetValue(1);
				break;
		}
		
		HISSTools_Button::Draw(g);
	}
	
	const WDL_String& GetLastSelectedFileForPlug()
	{
		return mFile;
	}
	
	void SetLastSelectedFileFromPlug(const char* file)
	{
		mFile.Set(file);
	}
    
    void SetExtensions(char *extensions)
    {
        mExtensions.Set(extensions);
    }
};

#endif /* __HISSTOOLS_CONTROLS__ */
