
#ifndef __HISSTOOLS_CONTROLS__
#define __HISSTOOLS_CONTROLS__

#include "HISSTools_Control_Helpers.hpp"
#include <vector>

#include <IPlugAPIBase.h>
#include <IControl.h>

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
