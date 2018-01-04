
#ifndef __HISSTOOLS_CONTROLS__
#define __HISSTOOLS_CONTROLS__

static int ConvertMouseDeltaToNative(int x) { return x; }

#include <HISSTools_Graphics/HISSTools_LICE_Vec_Lib_Cairo.hpp>
#include "HISSTools_Design_Scheme.hpp"
#include <IControl.h>
#include <IControls.h>
#include <vector>

const double TEXT_PROMPT_PADDING = 1.;

enum MousingAction {kMouseDown, kMouseUp, kMouseDblClick, kMouseDrag, kMouseWheel, kMouseOver, kMouseOut};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// A class to cache layers in control drawing (currently only supports a single background...)

class HISSTools_Control_Layers
{

private:
	
    cairo_pattern_t *mBackground;
	bool mDrawBackground;
    bool mNoCaching;
    
public:
	
    HISSTools_Control_Layers() : mBackground(NULL), mDrawBackground(true), mNoCaching(false)
	{}
    
    ~HISSTools_Control_Layers()
    {
        if (mBackground)
            cairo_pattern_destroy(mBackground);
    }
	
    bool startBackground(IGraphics& pGraphics, HISSTools_LICE_Vec_Lib *vecDraw, IRECT area)
    {
        if (mDrawBackground && !mNoCaching)
            vecDraw->startGroup();
        
        return mDrawBackground || mNoCaching;
    }
    
    void renderBackground(IGraphics& pGraphics, HISSTools_LICE_Vec_Lib *vecDraw, IRECT area)
    {
        if (!mNoCaching)
        {
            if (!mBackground)
                mBackground = vecDraw->endGroup();
            if (mBackground)
                vecDraw->renderPattern(mBackground);
        }
        
        mDrawBackground = false;
    }
   
	void redrawBackground()
	{
		mDrawBackground = true;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// An abstract class to control tabs in plug-ins

class HISSTools_Tabs 
{

private:
	
    struct TabItem
	{
		IControl *mControl;
		int mTabNumber;
		
		TabItem(IControl *control, int tabNumber) : mControl(control), mTabNumber(tabNumber)
		{}
	};
	
	IParam *mParam;
	IControl *mTabControl;
	std::vector<TabItem> mItems;
	
	int mCurrentTabNumber;
	int mMaxTabNumber;	
	
	void updateItems()
	{
		std::vector<TabItem>::iterator it;
		
		bool tabObjectHidden = mTabControl->IsHidden();
		
		for (it = mItems.begin(); it != mItems.end(); it++)
			if (it->mTabNumber != mCurrentTabNumber)
				it->mControl->Hide(true);
				
		for (it = mItems.begin(); it != mItems.end(); it++)
			if (it->mTabNumber == mCurrentTabNumber)
				it->mControl->Hide(tabObjectHidden);
	}
	
	int clipTabNumber(int tabNumber)
	{
		tabNumber = tabNumber < 0 ? 0 : tabNumber;
		tabNumber = tabNumber > mMaxTabNumber ? mMaxTabNumber : tabNumber;

		return tabNumber;
	}
	
public:
	
	// You should pass the inheriting class here, after constructing the control, which must be mapped to a valid parameter of the plug - the tabs are tied to the parameter, rather than the control
	
	HISSTools_Tabs (IControl *tabControl) 
	{		
		mTabControl = tabControl;
		mParam = tabControl->GetParam();
		
		// N.B. - mMaxTabNumber is one lass than the number of actual tabs (zero referenced)

		mMaxTabNumber = mParam != NULL ? round(mParam->GetRange()) : 0;
		tabSetDirty(false);
	}
	
	void attachControl(IControl *control, int tabNumber)
	{
		mItems.push_back(TabItem(control, clipTabNumber(tabNumber)));
		updateItems();
	}
	
	void tabHide(bool hide)
	{
		mTabControl->IControl::Hide(hide);
		updateItems();
	}
	
	void tabSetDirty(bool pushParamToPlug)
	{		
		mTabControl->IControl::SetDirty(pushParamToPlug);
		mCurrentTabNumber = mParam ? clipTabNumber(mParam->Int() - mParam->GetMin()) : 0;
		updateItems();
	}
	
	void setTabFromPlug(int tabNumber)
	{
		if (mParam)
			mParam->Set(tabNumber + mParam->GetMin());
			
		tabSetDirty(false);
	}
	
	// These functions should be declared in any inheriting classes, and should call the related tab versions
	
	virtual void Hide(bool hide) = 0;
	virtual void SetDirty(bool pushParamToPlug) = 0;
};


// This class allows Live-style tabs that have no explicit selector, and instead are set from the plug via other related controls

class HISSTools_Invisible_Tabs : public IControl, public HISSTools_Tabs
{
    
public:
	
	HISSTools_Invisible_Tabs(IPlugBaseGraphics *plug, int paramInx) : IControl(*plug, IRECT(), paramInx), HISSTools_Tabs(this) {}
	
	void Draw(IGraphics& pGraphics)         { }
	void Hide(bool hide)                    { tabHide(hide); }
	void SetDirty(bool pushParamToPlug)     { tabSetDirty(pushParamToPlug); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper class to deal with blocks of text 

class HISSTools_Text_Helper_Block
{
	
protected:
	
	// Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	
	// Appearance
	
	HISSTools_Text *mTextTS;
	HISSTools_Color_Spec *mTextCS;
	HISSTools_Shadow *mTextSD;
	HTextAlign mHAlign;
	VTextAlign mVAlign;
	
	// String
	
	WDL_String mStr;
	
private:
	
	void setup(double x, double y, double w, double h, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{
		// Dimensions
		
		resizeText(x, y, w, h);
		
		// Get Appearance
		
		mTextTS = designScheme->getTextStyle(name, type);
		mTextCS = designScheme->getColorSpec(name, type);
		mTextSD = designScheme->getShadow(name, type);
		mHAlign = hAlign;
		mVAlign = vAlign;		
	}
	
public:
	
	// Constructors
	
	HISSTools_Text_Helper_Block (double x, double y, double w, double h,  HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{		
		setup(x, y, w, h, hAlign, vAlign, name, type, designScheme);
	}
	
	HISSTools_Text_Helper_Block (HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{
		setup(0, 0, 0, 0, hAlign, vAlign, name, type, designScheme);
	}
	
	bool Draw(HISSTools_LICE_Vec_Lib *vecDraw)
	{
		if (mTextSD)
			vecDraw->startShadow(mTextSD);
			
		vecDraw->setColor(mTextCS);
		vecDraw->text(mTextTS, mStr.Get(), mX, mY, mW, mH, mHAlign, mVAlign);

		if (mTextSD)
			vecDraw->renderShadow();
		
		return true;
	}
	
	void resizeText(double x, double y, double w, double h)
	{
		mX = w > 0 ? x : x + w;
		mY = h > 0 ? y : y + h;
		mW = fabs(w);
		mH = fabs(h);
	}
    
	void setTextColor(HISSTools_Color_Spec *colorSpec)
	{
		mTextCS = colorSpec;
	}
	
	void setText(const char *str)
	{
		mStr.Set(str);
	}
	
	HISSTools_Bounds bounds()
	{
		HISSTools_Bounds boxBounds(mX, mY, mW, mH);
		
		if (mTextSD)
			boxBounds.include(mTextSD->getBlurBounds(boxBounds));
		
		return boxBounds;
	}	
};


class HISSTools_Text_Helper_Panel : public HISSTools_Text_Helper_Block
{
	
protected:
	
	// Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	
	double mLPad;
	double mRPad;
	double mHPad;
	
	// Appearance
	
	HISSTools_Color_Spec *mPanelFillCS;
	HISSTools_Color_Spec *mPanelOutlineCS;
	HISSTools_Shadow *mPanelSD;
	double mPanelRoundness;
	double mPanelOutlineTK;	
	
private:
	
	// FIX - padding
	
	void setup(double x, double y, double w, double h, double wPad, double hPad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{
		char concatenatedName[256];
	
		// Dimensions
	
		mLPad = wPad;
		mRPad = wPad;
		mHPad = hPad;
		resize(x, y, w, h);
	
		// Get Appearance
	
		sprintf(concatenatedName, "%sPanel", name);
	
		mPanelFillCS = designScheme->getColorSpec(concatenatedName, type);
		mPanelSD = designScheme->getShadow(concatenatedName, type);
	
		sprintf(concatenatedName, "%sPanelOutline", name);
	
		mPanelOutlineCS = designScheme->getColorSpec(concatenatedName, type);
		mPanelOutlineTK = designScheme->getDimension(concatenatedName, type);
	
		sprintf(concatenatedName, "%sPanelRoundness", name);
	
		double roundness = designScheme->getDimension(concatenatedName, type);
		roundness = ((roundness * 2) > mW) ? mW / 2.: roundness;
		roundness = ((roundness * 2) > mH) ? mH / 2.: roundness;
		mPanelRoundness = roundness < 0 ? mH / 2 : roundness;		
	}
	
protected:
    
    bool doDrawOutline()
    {
        return (mPanelOutlineTK && mPanelOutlineCS != NULL);
    }
				
    bool doDrawPanel()
    {
        return ((mPanelFillCS != NULL) || doDrawOutline());
    }

public:
	
	HISSTools_Text_Helper_Panel(double x, double y, double w, double h, double wPad, double hPad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	: HISSTools_Text_Helper_Block (hAlign, vAlign, name, type, designScheme)
	{
		setup(x, y, w, h, wPad, hPad, hAlign, vAlign, name, type, designScheme);
	}
	
	HISSTools_Text_Helper_Panel(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme) 
	: HISSTools_Text_Helper_Block (hAlign, vAlign, name, type, designScheme)
	{
		setup(0, 0, 0, 0, 0, 0, hAlign, vAlign, name, type, designScheme);
	}
	
	bool Draw(HISSTools_LICE_Vec_Lib *vecDraw)
	{	
		if (doDrawPanel())
		{
			if (mPanelSD)
				vecDraw->startShadow(mPanelSD);
		
			vecDraw->setColor(mPanelFillCS);		
			vecDraw->fillRoundRect(mX, mY, mW, mH, mPanelRoundness);
			
			if (doDrawOutline())
			{	
				vecDraw->setColor(mPanelOutlineCS);		
				vecDraw->frameRoundRect(mX, mY, mW, mH, mPanelRoundness, mPanelOutlineTK);
			}

			if (mPanelSD)
				vecDraw->renderShadow();
		}
		
		HISSTools_Text_Helper_Block::Draw(vecDraw);
		
		return true;
	}

	void changePadding(double lPad, double rPad, double hPad)
	{
		mLPad = lPad;
		mRPad = rPad;
		mHPad = hPad;
		
        resize(mX, mY, mW, mH);
	}
	
	void changePadding(double wPad, double hPad)
	{
		changePadding(wPad, wPad, hPad);
	}

	void resize(double x, double y, double w, double h)
	{
		mX = w > 0 ? x: x + w;
		mY = h > 0 ? y : y + h;
		mW = fabs(w);
		mH = fabs(h);
				
		double lPad = mLPad > mW / 2.0 ? mW / 2.0 : mLPad;
		double rPad = mRPad > mW / 2.0 ? mW / 2.0 : mRPad;
		double hPad = mHPad > mH / 2.0 ? mH / 2.0 : mHPad;
		
		resizeText(x + lPad, y + hPad, w - (lPad + rPad), h - (2.0 * hPad));
	}
	
	void setPanelColor(HISSTools_Color_Spec *colorSpec)
	{
		mPanelFillCS = colorSpec;
	}
	
	void setOutlineColor(HISSTools_Color_Spec *colorSpec)
	{
		mPanelOutlineCS = colorSpec;
	}
	
	HISSTools_Bounds bounds()
	{
		HISSTools_Bounds boxBounds(mX, mY, mW, mH);
		
		boxBounds.addThickness(mPanelOutlineTK);
		
		if (mPanelSD)
			boxBounds.include(mPanelSD->getBlurBounds(boxBounds));
		
		boxBounds.include(HISSTools_Text_Helper_Block::bounds());
		
		return boxBounds;
	}
};

class HISSTools_Text_Helper_Param : public HISSTools_Text_Helper_Panel
{
	
private:
	
	IControl *mControl;
	
	double mTextHPad;
	double mSeparatorX;
	
	bool mDrawSeparator;
	bool mDrawTriangle;
	bool mMenuFlipTriangle;
	
	double mMenuTriangleTop;
	double mMenuTriangleBtm;
	double mMenuTriangleL;
	double mMenuTriangleR;
	double mMenuTriangleC;
	
	HISSTools_Color_Spec *mTextHiliteCS;
	HISSTools_Color_Spec *mPanelHiliteCS;
	HISSTools_Color_Spec *mOutlineHiliteCS;
	HISSTools_Color_Spec *mTextCS;
	HISSTools_Color_Spec *mPanelFillCS;
	HISSTools_Color_Spec *mPanelOutlineCS;
		
	// Show Units
	
	bool mShowUnits;
	
	void setControlText()
	{
		IText text;
		
		strcpy(text.mFont, mTextTS->mFont);
		text.mSize = mTextTS->mSize;
		
		switch (mTextTS->mStyle)
		{
			case HISSTools_Text::kStyleNormal:
				text.mStyle = IText::kStyleNormal;
				break;
				
			case HISSTools_Text::kStyleBold:
				text.mStyle = IText::kStyleBold;
				break;
				
			case HISSTools_Text::kStyleItalic:
				text.mStyle = IText::kStyleItalic;
				break;
		}
		
		switch (mHAlign)
		{
			case kHAlignLeft:
				text.mAlign = IText::kAlignNear;
				break;
				
			case kHAlignCenter:
				text.mAlign = IText::kAlignCenter;
				break;
				
			case kHAlignRight:
				text.mAlign = IText::kAlignFar;
				break;
		}
		
		mControl->SetText(text);
	}
	
	double roundnessCompensate(double menuTriangleHeight)
	{
		if (menuTriangleHeight > mH)
			return mPanelRoundness;
		if (menuTriangleHeight <= mH - (2.0 * mPanelRoundness))
			return 0;
		
		return mPanelRoundness - sqrt(mPanelRoundness * mPanelRoundness - (0.25 * menuTriangleHeight * menuTriangleHeight) - (0.5 * mH) - mPanelRoundness);
	}
	
public:
	
	// Constructors
	
	HISSTools_Text_Helper_Param(IControl *control, double x, double y, double w, double h, double pad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	: HISSTools_Text_Helper_Panel(x, y, w, h, 0, 0, hAlign, vAlign, name, type, designScheme)
	{			
        double textHeight = HISSTools_LICE_Vec_Lib::getTextLineHeight(mTextTS);
		char concatenatedName[256];
		
		mControl = control;
		mTextHPad = pad;
		
		mTextCS = HISSTools_Text_Helper_Panel::mTextCS;
		mPanelFillCS = HISSTools_Text_Helper_Panel::mPanelFillCS;
		mPanelOutlineCS = HISSTools_Text_Helper_Panel::mPanelOutlineCS;
		
		// Get Appearance
		
		bool drawMenuTriangle;
		
		sprintf(concatenatedName, "%sDrawTriangle", name);
		drawMenuTriangle = designScheme->getFlag(concatenatedName, type);
		sprintf(concatenatedName, "%sHilite", name);
		mTextHiliteCS = designScheme->getColorSpec(concatenatedName, type);
		sprintf(concatenatedName, "%sPanelHilite", name);
		mPanelHiliteCS = designScheme->getColorSpec(concatenatedName, type);
		sprintf(concatenatedName, "%sPanelOutlineHilite", name);
		mOutlineHiliteCS = designScheme->getColorSpec(concatenatedName, type);
		mShowUnits = designScheme->getFlag("ShowUnits", type);
		
		// FIX - Padding!
		
		double wPad = 9;
		double hPad = 0;
		
		//changePadding(wPad, hPad);
		changePadding(0, hPad);
        
		if (drawMenuTriangle && menuParam() == true)
		{
			mDrawTriangle = true;
			sprintf(concatenatedName, "%sDrawSeparator", name);
			mDrawSeparator = designScheme->getFlag(concatenatedName, type);
			sprintf(concatenatedName, "%sFlipTriangle", name);
			mMenuFlipTriangle = designScheme->getFlag(concatenatedName, type);
						
			double menuTriangleWidthRatio = designScheme->getDimension("MenuTriangleWidthRatio", type);
			double menuTriangleHeightRatio = designScheme->getDimension("MenuTriangleHeightRatio", type);
			double menuTriangleWidth = textHeight * menuTriangleWidthRatio;
			double menuTriangleHeight = textHeight * menuTriangleHeightRatio;
					
			double separatorWidth = wPad * 2.0 + menuTriangleWidth + roundnessCompensate(menuTriangleHeight);
			mSeparatorX = mX + (mMenuFlipTriangle == true ? separatorWidth : mW - separatorWidth);
			
			mMenuTriangleTop = mY + (mH - menuTriangleHeight) / 2.0;
			mMenuTriangleBtm = mMenuTriangleTop + menuTriangleHeight;
			mMenuTriangleL = mSeparatorX + (mMenuFlipTriangle == true ? -(menuTriangleWidth + wPad) : wPad);
			mMenuTriangleR = mMenuTriangleL + menuTriangleWidth;
			mMenuTriangleC = mMenuTriangleL + (menuTriangleWidth / 2.0);
			
			changePadding(wPad + (mMenuFlipTriangle == true ? separatorWidth: 0), wPad + (mMenuFlipTriangle == true ? 0 : separatorWidth), hPad);
		}
	}
	
	bool menuParam()
	{
		if (mControl->GetParam() == NULL)
			return false;
		
		if (mControl->GetParam()->GetNDisplayTexts())
			return true;
		
		return false;
	}
	
	void promptUserInput(HISSTools_LICE_Vec_Lib *vecDraw)
	{
		HISSTools_Bounds entryBounds;
		IRECT iEntryBounds;
		
		// FIX - Widths ??
		// FIX - Text Prompt Vertical Centering??
		// FIX - Text Prompt - proper matching font....
		// FIX - Prompt Menus - Centering for text and box
		// FIX - Prompt Menus - font....
		
		if (mControl->GetParam() == NULL)
			return;
		
		double textHeight = vecDraw->getTextLineHeight(mTextTS);
		double promptHeight = (menuParam() == true) ? 0 : textHeight + (2.0 * mTextHPad);

		// FIX - Set Padding, rather than variable???
		// FIX - is this one justified?
		
		double promptTop = mY - ((textHeight / 2.0) + mTextHPad + 1.0);
		double promptLeft = mX + mLPad;
		double promptWidth = mW - (mLPad + mRPad);

		switch (mVAlign)
		{
			case kVAlignTop:
				promptTop += textHeight / 2.0;
				break;
			case kVAlignCenter:
				promptTop += mH / 2.0;
				break;
			case kVAlignBottom:
				promptTop += mH - (textHeight / 2.0);
				break;
		}
		
		if (menuParam() == true)
		{
			if (mDrawSeparator)
			{
				promptTop = mY + mH;
				promptLeft = mMenuFlipTriangle ? mX : mSeparatorX;
			}
		}
		
		entryBounds = HISSTools_Bounds(promptLeft, promptTop, promptWidth, promptHeight);
		iEntryBounds = entryBounds.iBounds();
		
		setControlText();
		mControl->DisablePrompt(false);
		mControl->PromptUserInput(iEntryBounds);
	}
	
	bool promptUserInput(HISSTools_LICE_Vec_Lib *vecDraw, int x, int y)
	{
		if (bounds().iBounds().Contains(x, y))
		{
			if (!mDrawSeparator || (mMenuFlipTriangle == (x < mSeparatorX)))
			{
				promptUserInput(vecDraw);
				return true;
			}
		}
		
		return false;
	}
	
	void hilite(bool on)
	{
		if (on == true)
		{
			if (mTextHiliteCS)
				HISSTools_Text_Helper_Panel::mTextCS = mTextHiliteCS;
			if (mPanelHiliteCS)
				HISSTools_Text_Helper_Panel::mPanelFillCS = mPanelHiliteCS;
			if (mOutlineHiliteCS) 
				HISSTools_Text_Helper_Panel::mPanelOutlineCS = mOutlineHiliteCS;
				
		}
		else 
		{
			HISSTools_Text_Helper_Panel::mTextCS = mTextCS;
			HISSTools_Text_Helper_Panel::mPanelFillCS = mPanelFillCS;
			HISSTools_Text_Helper_Panel::mPanelOutlineCS = mPanelOutlineCS;
		}

	}
	
	bool Draw(HISSTools_LICE_Vec_Lib *vecDraw)
	{
		IParam *param = mControl->GetParam();
		
		char displayValue[256];
		
		if (param == NULL)
			return true;
		
		// Retrieve Value
		
		param->GetDisplayForHost(displayValue);
		
		// If label is NULL don't add the space
		
		if (mShowUnits == true && *(param->GetLabelForHost()))
		{
			strcat(displayValue, " ");
			strcat(displayValue, param->GetLabelForHost());
		}
		
		// Draw Text (with Panel)
		
		setText(displayValue);
		HISSTools_Text_Helper_Panel::Draw(vecDraw);
		
		// Menu Separator 
		
		if (mDrawSeparator && doDrawOutline())
		{
			vecDraw->setColor(mPanelOutlineCS);
			vecDraw->line(mSeparatorX, mY, mSeparatorX, mY + mH, mPanelOutlineTK);
		}
				
		// Menu Triangle
		
		if (mDrawTriangle)
		{
			if (mTextSD)
				vecDraw->startShadow(mTextSD);
		 
			vecDraw->setColor(mTextCS);
			vecDraw->fillTriangle(mMenuTriangleL, mMenuTriangleTop, mMenuTriangleC, mMenuTriangleBtm, mMenuTriangleR, mMenuTriangleTop);
		 
			if (mTextSD)
				vecDraw->renderShadow();
		}
		
		return true;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// HISSTools Value /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Multipurpose numerical / menu control

// FIX - do your own mousing later...

class HISSTools_Value : public IKnobControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
	// Text
	
	HISSTools_Text_Helper_Param *mTextParam;
	HISSTools_Text_Helper_Block *mTextLabel;
	
    // Mousing
    
    bool mDrag;
    
	// Stored Dimensions
	
	double mTextArea;
	
public:
	
	HISSTools_Value(IPlugBaseGraphics* plug, int paramIdx, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IKnobControl(*plug, IRECT(), paramIdx), HISSTools_Control_Layers()
	{
		mVecDraw = vecDraw;
        
		// FIX - perhaps just inherit these??
		
        bool label = designScheme->getFlag("ValueDrawLabel", type);
		mTextArea = designScheme->getDimension("ValueTextArea", type);
		
		mTextParam = new HISSTools_Text_Helper_Param(this, x, y, w, h, 1, kHAlignCenter, kVAlignCenter, "Value", type, designScheme);
		mTextLabel = new HISSTools_Text_Helper_Block(x, y - mTextArea, w, mTextArea, kHAlignCenter, kVAlignTop, "ValueLabel", type, designScheme);
		mTextLabel->setText((GetParam() != NULL && label) ? GetParam()->GetNameForHost() : "");

		SetTargetArea(HISSTools_Bounds(x, y, w, h).iBounds());
		
		HISSTools_Bounds fullBoxBounds = mTextParam->bounds();
		fullBoxBounds.include(mTextLabel->bounds());
		mRECT = fullBoxBounds.iBounds();
	}
	
	~HISSTools_Value()
	{
		delete mTextParam;
		delete mTextLabel;
	}
	
	void OnMouseDown(int x, int y, const IMouseMod& pMod)
	{
		if (pMod.S == true)
		{
			if (mDefaultValue >= 0.0)
			{
				mValue = mDefaultValue;
				SetDirty();
				return;
			}
		}

		if (mTextParam->menuParam() == true)
		{
			if (mTextParam->promptUserInput(mVecDraw, x, y) == false && GetParam())
			{
				mValue = round(mValue * (GetParam()->GetRange()) + 1) / (GetParam()->GetRange());
				mValue = mValue > 1.0 ? 0 : mValue;
			}
		}
        else
            mTextParam->hilite(true);
        
        
        mDrag = false;
		SetDirty();
	}
	
	void OnMouseUp(int x, int y, const IMouseMod& pMod)
	{
        if (mDrag == false)
        {
            if (mTextParam->menuParam() == false)
                mTextParam->promptUserInput(mVecDraw);
        }
        else
        {
            mTextParam->hilite(false);
            SetDirty();
        }
	}
	
    void OnMouseDrag(int x, int y, int dX, int dY, const IMouseMod& pMod)
    {
        mDrag = true;
    
        IKnobControl::OnMouseDrag(x, y, dX, dY, pMod);
    }
    
	void OnMouseDblClick(int x, int y, const IMouseMod& pMod)
	{
        OnMouseDown(x, y, pMod);
	}
    
    virtual void SetValueFromUserInput(double value)
    {
        mDrag = false;
        mTextParam->hilite(false);
        IKnobControl::SetValueFromUserInput(value);
    }
	
	void Draw(IGraphics& pGraphics)
	{
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);

        // Label
        
		if (startBackground(pGraphics, mVecDraw, mRECT))
			mTextLabel->Draw(mVecDraw);
		
		renderBackground(pGraphics, mVecDraw, mRECT);
		mTextParam->Draw(mVecDraw);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// HISSTools Dial //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Multipurpose dial with auto value display

// FIX - do your own mousing later...

class HISSTools_Dial : public IKnobControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
	// Positioning / Dimensions
	
	double mCx;
	double mCy;
	double mR;
	
	// Pointer Appearance
	
	double mPointerCircRadius;			
	double mPointerTipRadius;			
	double mPointerAngle;		
	
	// Display Angles
	
	double mRefValue;
	double mStartAngle;
	double mThrowAngle;
	
	// Text Area
	
	double mTextArea;
	
	// Text Prompt
	
	double mPromptHalfHeight;
	double mPromptRatio;
    bool mInEdit;
	
	// Line Thicknesses
	
	double mOutlineTK;
	double mPointerTK;
	double mPointerOutlineTK;
	
	// Shadow Specs
	
	HISSTools_Shadow *mOutlineSD;
	HISSTools_Shadow *mPointerSD;
	
	// Color Specs
	
	HISSTools_Color_Spec *mIndicatorCS;
	HISSTools_Color_Spec *mCircleFillCS;
	HISSTools_Color_Spec *mPointerFillCS;
	HISSTools_Color_Spec *mOutlineCS;
	HISSTools_Color_Spec *mPointerOutlineCS;
	HISSTools_Color_Spec *mInactiveOverlayCS;
	
	// Text Blocks
	
	HISSTools_Text_Helper_Block *mTextLabel;
	HISSTools_Text_Helper_Param *mTextParam;
	
public:

	// Constructor

	HISSTools_Dial(IPlugBaseGraphics* plug, int paramIdx, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IKnobControl(*plug, IRECT(), paramIdx), HISSTools_Control_Layers(), mInEdit(false)
	{
		mVecDraw = vecDraw;
        
		// Calculate Measurements
		
		double d = designScheme->getDimension("DialDiameter", type);
		
		mTextArea = designScheme->getDimension("DialTextArea", type);
		
		mR = d / 2.0;
		mCx = x + mR;
		mCy = y + mR;
				
		// Pointer Appearance
		
		double pointerCircRatio = designScheme->getDimension("DialPointerRatio", type);
		double pointerTipRatio = designScheme->getDimension("DialTipRatio", type);
		double pointerAngle = designScheme->getDimension("DialPointerAngle", type);
		
		setPointerAppearance(pointerCircRatio, pointerTipRatio, pointerAngle);
		
		// Set Throw
		
		mRefValue = designScheme->getDimension("DialRefValue", type);
		mStartAngle = designScheme->getDimension("DialStartAngle", type);
		mThrowAngle = designScheme->getDimension("DialThrowAngle", type);
		
		// Get Appearance
		
		mOutlineTK = designScheme->getDimension("DialOutline", type);
		mPointerTK = designScheme->getDimension("DialIndicatorLineWidth", type);
		mPointerOutlineTK = designScheme->getDimension("DialPointerOutline", type);
		
		mOutlineSD = designScheme->getShadow("DialOutline", type);
		mPointerSD = designScheme->getShadow("DialPointer", type);
		
		mIndicatorCS = designScheme->getColorSpec("DialIndicator", type);
		mCircleFillCS = designScheme->getColorSpec("DialCircleFill", type);
		mPointerFillCS = designScheme->getColorSpec("DialPointerFill", type);
		mOutlineCS = designScheme->getColorSpec("DialOutline", type);
		mPointerOutlineCS = designScheme->getColorSpec("DialPointerOutline", type);
		mInactiveOverlayCS = designScheme->getColorSpec("DialInactiveOverlay", type);
		
		// Text 
				
		mPromptHalfHeight = (0.5 * mVecDraw->getTextLineHeight(designScheme->getTextStyle("DialValue", type))) + designScheme->getDimension("DialPromptPadding", type);
		mPromptRatio = designScheme->getDimension("DialPromptRatio", type);

		double textPad = designScheme->getDimension("DialPromptPadding", type);
        double halfWidth = mR;//mPromptRatio * mPointerCircRadius;
		//FIX - above
		mTextParam = new HISSTools_Text_Helper_Param(this, mCx - halfWidth, mCy - mPromptHalfHeight, 2. * halfWidth, 2. * mPromptHalfHeight, textPad, kHAlignCenter, kVAlignCenter, "DialValue", type, designScheme);
		mTextLabel = new HISSTools_Text_Helper_Block(mCx - mR, mCy + mR, 2 * mR, mTextArea, kHAlignCenter, kVAlignBottom, "DialLabel", type, designScheme);
		mTextLabel->setText(GetParam() != NULL ? GetParam()->GetNameForHost() : "");
								
		// Calculate Areas (including shadows and thicknesses)
		
		HISSTools_Bounds fullBoxBounds(x, y, d, d + mTextArea);
		HISSTools_Bounds dialBoxBounds(x, y, d, d);
		HISSTools_Bounds ptrBoxBounds(mCx - mPointerTipRadius, mCy - mPointerTipRadius, 2 * mPointerTipRadius, 2 * mPointerTipRadius);
		
		dialBoxBounds.addThickness(mOutlineTK);
		ptrBoxBounds.addThickness(mPointerOutlineTK);
		
		fullBoxBounds.include(mOutlineSD->getBlurBounds(dialBoxBounds));
		fullBoxBounds.include(mPointerSD->getBlurBounds(ptrBoxBounds));
		
		mRECT = fullBoxBounds.iBounds();
		SetTargetArea(dialBoxBounds.iBounds());
	}	
	
	~HISSTools_Dial()
	{
		delete mTextLabel;
		delete mTextParam;
	}
	
private:
		
	void setPointerAppearance(double pointerCircRatio, double pointerTipRatio, double pointerAngle)
	{
		pointerCircRatio = pointerCircRatio > 0.99 ? 0.99 : pointerCircRatio;
		
		pointerTipRatio = pointerTipRatio > 1.0 / pointerCircRatio ? 1.0 / pointerCircRatio : pointerTipRatio;
		pointerTipRatio = pointerTipRatio < 1.01 ? 1.01 : pointerTipRatio;
		
		mPointerCircRadius = mR * pointerCircRatio;		
		mPointerTipRadius = mPointerCircRadius * pointerTipRatio;	
		mPointerAngle = pointerAngle;
	}

public:
	
	void OnMouseDown(int x, int y, const IMouseMod& pMod)
	{
		if (pMod.S == true)
		{
			if (mDefaultValue >= 0.0)
			{
				mValue = mDefaultValue;
				SetDirty();
			}
		}
	}
	
	void OnMouseDblClick(int x, int y, const IMouseMod& pMod)
	{
		// FIX - Confirm best key combo...
		
		if (pMod.S == true)
		{
			OnMouseDown(x, y, pMod);
			return;
		}
		
        mInEdit = true;
		mTextParam->promptUserInput(mVecDraw);
	}
	
    virtual void SetValueFromUserInput(double value)
    {
        mInEdit = false;
        IControl::SetValueFromUserInput(value);
    }
    
	/*
	 // Mousing Functions
	 
	 // FIX - better gearing??
	 // FIX - click through options?
	 
	 void OnMouseDown(int x, int y, const IMouseMod& pMod)
	 {
	 OnMouseDrag(x, y, 0, 0, pMod);
	 }
	 
	 void OnMouseDrag(int x, int y, int dX, int dY, const IMouseMod& pMod)
	 {		
	 IParam *param = GetParam();
	 
	 IControl::OnMouseDrag(x, y, 0, 0, pMod);
	 
	 if (param && param->Type() != IParam::kTypeDouble && param->Type() != IParam::kTypeNone)
	 {
	 double value = param->Int();
	 double delta;
	 
	 if (mDirection == kVertical)
	 delta = ConvertMouseDeltaToNative(-dY);
	 else
	 delta = ConvertMouseDeltaToNative(dX);
	 
	 delta /= mGearing;
	 delta = round(delta);
	 value += delta;
	 value -= param->GetMin();
	 
	 mValue = value / param->GetRange();
	 }
	 
	 SetDirty(true);
	 }*/
	
	// Draw
	
	void Draw(IGraphics& pGraphics)
	{
		IParam *param = GetParam();
		double value, xIntersect, yIntersect;
		
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);

		// Background
		
		if (startBackground(pGraphics, mVecDraw, mRECT))
		{
			// Background Circles
			
			mVecDraw->startShadow(mOutlineSD);
			mVecDraw->setColor(mOutlineCS);
			mVecDraw->frameCircle(mCx, mCy, mR, mOutlineTK);
			mVecDraw->renderShadow();
			mVecDraw->setColor(mCircleFillCS);
			mVecDraw->fillCircle(mCx, mCy, mR);
			
			// Label
			
			mTextLabel->Draw(mVecDraw);
		}
				
		renderBackground(pGraphics, mVecDraw, mRECT);
		
		// Round positions for integer parameters
		
		if (param != NULL && param->Type() != IParam::kTypeDouble && param->Type() != IParam::kTypeNone)
			value = (param->Int() - param->GetMin()) / param->GetRange();
		else 
			value = mValue;

		// Calculate Angles
		
		double iBaseAng = mStartAngle + mRefValue * mThrowAngle;
		double iDiffAng = ((value - mRefValue) * mThrowAngle);
		double iPntrAng = iBaseAng + iDiffAng;
		
		// Indicator Arc
		
		mVecDraw->setColor(mIndicatorCS);
		mVecDraw->fillArc(mCx, mCy, mR, iBaseAng, iDiffAng);
		
		// Pointer Line
		
		mVecDraw->setColor(mOutlineCS);
		mVecDraw->circleIntersection(mCx, mCy, iPntrAng, mR, &xIntersect, &yIntersect);
		mVecDraw->line(mCx, mCy, xIntersect, yIntersect, mPointerTK);
		
		// Pointer
		
		mVecDraw->startShadow(mPointerSD);
		mVecDraw->setColor(mPointerFillCS);
		mVecDraw->fillCPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle);
		mVecDraw->setColor(mPointerOutlineCS);
		mVecDraw->frameCPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle, mPointerOutlineTK);
		mVecDraw->renderShadow();
        
		if (IsGrayed() == true)
		{
			// Inactive Overlay
			
			mVecDraw->setColor(mInactiveOverlayCS);
			mVecDraw->fillCircle(mCx, mCy, mR);
		}
		else
        {
            if (!mInEdit)
                mTextParam->Draw(mVecDraw);
        }
	}
	
	void setThrow(double refValue, double startAngle, double throwAngle)
	{
		mRefValue = refValue;
		mStartAngle = startAngle;
		mThrowAngle = throwAngle;
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// HISSTools Button /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// On/Off button with text on or off the handle

// FIX - Momentary action!!

class HISSTools_Button: public IControl, public HISSTools_Control_Layers
{
	
private:
		
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
	// Positioning / Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	double mTextPad;
	double mRoundness;
		
	// Line Thicknesses
	
	double mOutlineTK;
	
	// Shadow Spec
	
	HISSTools_Shadow *mShadow;
	
	// Text Spec
	
	HISSTools_Text *mTextStyle;
	
	// Color Specs
	
	HISSTools_Color_Spec *mOnCS;
	HISSTools_Color_Spec *mOffCS;
	HISSTools_Color_Spec *mOutlineCS;
	HISSTools_Color_Spec *mHandleLabelCS;
	HISSTools_Color_Spec *mBackgroundLabelCS;
	HISSTools_Color_Spec *mInactiveOverlayCS;
	
	// Label Mode
	
	bool mLabelMode;
    
protected:
    
    // Text
    
    const char *mName;
	
public:
	
	// Constructor
	
	HISSTools_Button(IPlugBaseGraphics* plug, int paramIdx, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(*plug, IRECT(), paramIdx), HISSTools_Control_Layers()
	{		
		mVecDraw = vecDraw;
        
		// Dimensions
		
        mX = x;
		mY = y;
        mW = w <= 0 ? designScheme->getDimension("ButtonWidth", type) : w;
        mH = h <= 0 ? designScheme->getDimension("ButtonHeight", type) : h;
		
		double roundness = designScheme->getDimension("ButtonRoundness", type);
		mRoundness = roundness < 0 ? mH / 2 : roundness;		
		
		mTextPad = designScheme->getDimension("ButtonTextPad", type);

		// Label Mode
		
		mLabelMode = designScheme->getFlag("ButtonLabelMode", type);
		
		// Get Appearance
		
		mOutlineTK = designScheme->getDimension("ButtonOutline", type);
		
		mShadow = designScheme->getShadow("Button", type);
		
		mTextStyle = designScheme->getTextStyle("Button", type);
		
		mOnCS = designScheme->getColorSpec("ButtonHandleOn", type);
		mOffCS = designScheme->getColorSpec("ButtonHandleOff", type);
		mHandleLabelCS = designScheme->getColorSpec("ButtonHandleLabel", type);
		mOutlineCS = designScheme->getColorSpec("ButtonOutline", type);
		mBackgroundLabelCS = designScheme->getColorSpec("ButtonBackgroundLabel", type);
		mInactiveOverlayCS = designScheme->getColorSpec("ButtonInactiveOverlay", type);
		
		// Calculate Areas (including shadows and thicknesses)
		
		HISSTools_Bounds handleBounds(mX, mY, mLabelMode ? mH : mW, mH);
		HISSTools_Bounds fullBounds(mX, mY, mW, mH);
		
		handleBounds.addThickness(mOutlineTK);
		
		fullBounds = mShadow->getBlurBounds(handleBounds);
		fullBounds.include(fullBounds);
		
		mRECT = (fullBounds.iBounds());
		SetTargetArea(handleBounds.iBounds());
		
        mName = GetParam() != NULL ? GetParam()->GetNameForHost() : "";
        
		mDblAsSingleClick = true;
	}
	
public:
	
	// Mousing Functions
	
	void OnMouseDown(int x, int y, const IMouseMod& pMod)
	{
		mValue += 1.0;
		mValue = mValue > 1.0 ? 0 : mValue;
		SetDirty();
	}	

	// Draw
	
	void Draw(IGraphics& pGraphics)
	{		
		// FIX - Support Label Colour States / Outline Color States? - Multiple States?
		
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);
        
		// Button Rectangle
		
		mVecDraw->startShadow(mShadow);
		mVecDraw->setColor(mValue > 0.5 ? mOnCS : mOffCS);
		mVecDraw->fillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
		mVecDraw->setColor(mOutlineCS);
		mVecDraw->frameRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness, mOutlineTK);
		mVecDraw->renderShadow();
		
		mVecDraw->setColor(mLabelMode == true ? mBackgroundLabelCS : mHandleLabelCS);
		mVecDraw->text(mTextStyle, mName, mLabelMode == true ? mX + mH + mTextPad : mX, mY, mLabelMode == true ? mW - (mH + mTextPad) : mW, mH, mLabelMode == true ?  kHAlignLeft : kHAlignCenter);

		// Inactive
		
		if (IsGrayed())
		{
			// Inactive Overlay
			
			mVecDraw->setColor(mInactiveOverlayCS);
			mVecDraw->fillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
		}
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// HISSTools Switch/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Switch - multi state control with a number of vertical or horizontal positions

class HISSTools_Switch : public IControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
	// Positioning / Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	double mS;
	double mRoundness;
	
	// Line Thicknesses
	
	double mHandleTK;
	double mBoxOutlineTK;
	
	// Shadow Spec
	
	HISSTools_Shadow *mShadow;
	
	// Color Specs
	
	HISSTools_Color_Spec *mHandleFillCS;
	HISSTools_Color_Spec *mHandleOutlineCS;
	HISSTools_Color_Spec *mBoxFillCS;
	HISSTools_Color_Spec *mBoxOutlineCS;
	HISSTools_Color_Spec *mInactiveOverlayCS;
	
	// Number of States
	
	int mNStates;
	
public:
	
	HISSTools_Switch(IPlugBaseGraphics* plug, int paramIdx, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w, double h, int nStates = 2, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(*plug, IRECT(), paramIdx), HISSTools_Control_Layers()
	{
		mVecDraw = vecDraw;
        
		// Dimensions
		
		mX = x;
		mY = y;
		mW = w < 0 ? designScheme->getDimension("SwitchWidth", type) : w;
		mH = h < 0 ? designScheme->getDimension("SwitchHeight", type) : h;
		mS = std::min(w, h);
	
		double roundness = designScheme->getDimension("SwitchRoundness", type);
		mRoundness = roundness < 0 ? mH / 2 : roundness;		
		
		// Number of States
		
		// FIX - get from parameters if <= 0 ??
		
		mNStates = nStates < 2 ? 2 : nStates;
		
		// Get Appearance
		
		mHandleTK = designScheme->getDimension("SwitchHandleOutline", type);
		mBoxOutlineTK = designScheme->getDimension("SwitchBoxOutline", type);
		
		mShadow = designScheme->getShadow("Switch", type);

		mHandleFillCS = designScheme->getColorSpec("SwitchHandleFill", type);
		mHandleOutlineCS = designScheme->getColorSpec("SwitchHandleOutline", type);
		mBoxFillCS = designScheme->getColorSpec("SwitchBoxFill", type);
		mBoxOutlineCS = designScheme->getColorSpec("SwitchOutline", type);
		mInactiveOverlayCS = designScheme->getColorSpec("SwitchInactiveOverlay", type);
		
		// Calculate Areas (including shadows and thicknesses)
		
		HISSTools_Bounds boxBounds(mX, mY, mW, mH);
		HISSTools_Bounds fullBounds(mX, mY, mW, mH);
		
		boxBounds.addThickness(mBoxOutlineTK);
		fullBounds.addThickness(mHandleTK);
		
		fullBounds = mShadow->getBlurBounds(fullBounds);
		fullBounds.include(boxBounds);
		
		mRECT = (fullBounds.iBounds());
		SetTargetArea(boxBounds.iBounds());
	}
	
public:
	
	// Mousing Functions
	
	void OnMouseDown(int x, int y, const IMouseMod& pMod)
	{
		OnMouseDrag(x, y, 0, 0, pMod);
	}
	
	void OnMouseDrag(int x, int y, int dX, int dY, const IMouseMod& pMod)
	{
        // FIX - retina support for position data!
        
		if (mW > mH)
			mValue = round(std::max(0.0, std::min(1.0, ((x - mX) / mW))) * (mNStates - 1)) / (mNStates - 1);
		else
			mValue = round(std::max(0.0, std::min(1.0, ((y - mY) / mH))) * (mNStates - 1)) / (mNStates - 1);
		
		SetDirty();
	}
	
	// Draw
	
	void Draw(IGraphics& pGraphics)
	{		
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);

		// Calculate position (according to orientation)
		
		double xPos, yPos;
		
		if (mW > mH)
		{
			xPos = mX + (mW - mS) * mValue;
			yPos = mY;
		}
		else 
		{
			xPos = mX;
			yPos = mY + (mH - mS) * mValue;
		}

		// Background
		
		if (startBackground(pGraphics, mVecDraw, mRECT))
		{
			// Background Rectangle
		
			mVecDraw->setColor(mBoxFillCS);
			mVecDraw->fillRoundRect(mX, mY, mW, mH, mRoundness);
			mVecDraw->setColor(mBoxOutlineCS);
			mVecDraw->frameRoundRect(mX, mY, mW, mH, mRoundness, mBoxOutlineTK);
		}
		
		renderBackground(pGraphics, mVecDraw, mRECT);
		
		// Handle
		
		mVecDraw->setColor(mHandleFillCS);
		mVecDraw->startShadow(mShadow);
		mVecDraw->fillRoundRect(xPos, yPos, mS, mS, mRoundness);
		mVecDraw->setColor(mHandleOutlineCS);
		mVecDraw->frameRoundRect(xPos, yPos, mS, mS, mRoundness, mHandleTK);
		mVecDraw->renderShadow();
		
		// Fix - Labels (Control Name and Value)
		// Label
		
		//mVecDraw->setColor(textColor);
		//mVecDraw->text(&txt, "Load", mRECT.L, mRECT.R, mRECT.T, mRECT.B);
		
		// Inactive
		
		if (IsGrayed() == true)
		{
			// Inactive Overlay
			
			mVecDraw->setColor(mInactiveOverlayCS);
			mVecDraw->fillRoundRect(mX, mY, mW, mH, mRoundness);
		}
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// HISSTools Matrix /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


class HISSTools_Matrix : public IControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
	// Size (Dimension)

	int mXDim;
	int mYDim;	
	
	// Positioning / Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	double mS;
	double mRoundness;
	double mGap;
	double mUnit;
	
	// Line Thicknesses
	
	double mHandleEmptyOutlineTK;
	double mHandleFilledOutlineTK;
	double mHiliteTK;
	
	// Shadow Spec
	
	HISSTools_Shadow *mShadow;
	
	// Color Specs
	
	HISSTools_Color_Spec *mStateCS[256];
	HISSTools_Color_Spec *mOutlineCS;
	HISSTools_Color_Spec *mHiliteCS;

	// States
	
	unsigned char *mStates;
	unsigned char mNStates;	
	
	// Hilite
	
	int mXHilite;
	int mYHilite;
	
public:
	
	// FIX - Consider making the variables private again
	
	// Mousing Info
	
	MousingAction mMousing;

	IMouseMod mPMod;
	
	int mXPos;
	int mYPos;

	int mMouseWheel;
	
	bool mValidReport;
	
private:
	
	bool coordsToIndices(double x, double y, int *xPos, int *yPos)
	{
		*xPos = -1;
		*yPos = -1;
	
		// FIX - Look at different thicknesses here
		
		double halfTK = mHandleFilledOutlineTK / 2.0;
		x += halfTK;
		y += halfTK;
		
		if (x < mX) 
			return false;
		if (x > mX + mW) 
			return false;
		if (y < mY) 
			return false;
		if (y > mY + mH) 
			return false;
		
		*xPos = (x - mX) / mUnit;
		*yPos = (y - mY) / mUnit;
		
		if (((*xPos + 1) * mUnit + mX - mGap + halfTK) > x && ((*yPos + 1) * mUnit + mY - mGap + halfTK) > y)
			return true;
		
		*xPos = -1;
		*yPos = -1;
		
		return false;
	}
	
	void reportToPlug()
	{
		if (mParamIdx >= 0)
		{
			mValidReport = true;
            mPlug.OnParamChange(mParamIdx);//, IPlugBaseGraphics::kGUI);
			mValidReport = false;
		}
	}
	
	
public:
	
	// Constructor and Destructor
	
	HISSTools_Matrix(IPlugBaseGraphics* plug, int paramIdx, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, int xDim, int yDim, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, HISSTools_Design_Scheme *stateScheme = 0)
	: IControl(*plug, IRECT(), paramIdx), HISSTools_Control_Layers()
	{
		mVecDraw = vecDraw;
        
		// Dimensions
		
		mXDim = xDim;
		mYDim = yDim;
		mX = x;
		mY = y;
		mS = designScheme->getDimension("MatrixHandleSize", type);
		mGap = designScheme->getDimension("MatrixHandleGap", type);
		mUnit = mS + mGap;
		mW = mXDim * mUnit - mGap;
		mH = mYDim * mUnit - mGap;
		mRoundness = designScheme->getDimension("MatrixHandleRoundness", type);
		mRoundness = mRoundness < 0. ? mS / 2 : mRoundness;
		
		// States
		
		HISSTools_Design_Scheme *currentStateScheme = stateScheme ? stateScheme : designScheme;
		
		mStates = new unsigned char[mXDim * mYDim];
		
		for (int i = 0; i < mXDim; i++)
			for (int j = 0; j < mYDim; j++)
				mStates[j * mXDim + i] = 0;
		
		// Get number of states that are supported with color specs

		for (int i = 0; i < 256; i++)
		{
			char testName[16];
			
			sprintf(testName, "MatrixState%d", i);
			mStateCS[i] = currentStateScheme->getColorSpec(testName, type);
			
			if (!mStateCS[i])
				break;
			else
				mNStates = i + 1;
		}
		
		// Mousing

		mXPos = mYPos = -1;
		
		// Hilite
		
		mXHilite = mYHilite = -1;
		
		// Get Appearance
		
		mHandleEmptyOutlineTK = designScheme->getDimension("MatrixShadowOutline", type);
		mHandleFilledOutlineTK = designScheme->getDimension("MatrixOutline", type);
		mHiliteTK = designScheme->getDimension("MatrixHilite", type);
				
		mShadow = designScheme->getShadow("Matrix", type);
		
		mOutlineCS = designScheme->getColorSpec("MatrixOutline", type);
		mHiliteCS = designScheme->getColorSpec("MatrixHilite", type);
		
		// Calculate Areas (including shadows and thicknesses)
		
		HISSTools_Bounds boxBoundsShadow(mX, mY, mW, mH);
		HISSTools_Bounds boxBoundsOutline(mX, mY, mW, mH);
		HISSTools_Bounds fullBoxBounds(mX, mY, mW, mH);
		
		boxBoundsShadow.addThickness(mHandleEmptyOutlineTK);
		boxBoundsShadow = mShadow->getBlurBounds(boxBoundsShadow);
		boxBoundsOutline.addThickness(mHandleFilledOutlineTK);
		fullBoxBounds = boxBoundsOutline;
		fullBoxBounds.include(boxBoundsShadow);
		
		mRECT = (fullBoxBounds.iBounds());
		SetTargetArea(boxBoundsOutline.iBounds());
		
		mValidReport = false;
	}
	
	~HISSTools_Matrix()
	{
		delete[] mStates;
	}
	
	 // Mousing Functions
	 
	bool OnMousing(int x, int y, const IMouseMod& pMod, MousingAction action)
	{
		mMousing = action;
		mPMod = pMod;
		
		if (coordsToIndices(x, y, &mXPos, &mYPos) == true)
		{
			reportToPlug();
			
			return true;
		}
		
		return false;
	}
	
	void OnMouseDown(int x, int y, const IMouseMod& pMod)
	{
		OnMousing(x, y, pMod, kMouseDown);
	}
	
	void OnMouseUp(int x, int y, const IMouseMod& pMod)
	{
		OnMousing(x, y, pMod, kMouseUp);
	}
	
	void OnMouseDblClick(int x, int y, const IMouseMod& pMod)
	{
		OnMousing(x, y, pMod, kMouseDblClick);
	}
	
	void OnMouseDrag(int x, int y, int dX, int dY, const IMouseMod& pMod)
	{
		OnMousing(x, y, pMod, kMouseDrag);
	}
	
	void OnMouseWheel(int x, int y, const IMouseMod& pMod, int d)
	{
        mMouseWheel = ConvertMouseDeltaToNative(d);
		OnMousing(x, y, pMod, kMouseWheel);
	}
	
	void OnMouseOver(int x, int y, const IMouseMod& pMod)
	{
		if (OnMousing(x, y, pMod, kMouseOver) == false)
			OnMouseOut();
		else
			SetDirty(false);
	}
	
	virtual void OnMouseOut()
	{
		mMousing = kMouseOut;
		mXPos = -1;
		mXPos = -1;
		
		reportToPlug();
		
		SetDirty(false);
	}
	
	// Draw
	
	void Draw(IGraphics& pGraphics)
	{				
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);

		// Background (shadow boxes)
		
		if (startBackground(pGraphics, mVecDraw, mRECT))
		{		
			mVecDraw->startShadow(mShadow);
			mVecDraw->setColor(mOutlineCS);
			
			for (int i = 0; i < mXDim; i++)
			{
				double sx = mX + i * mUnit;
				
				for (int j = 0; j < mYDim; j++) 
				{
					double sy = mY + j * mUnit;
					
					mVecDraw->frameRoundRect(sx, sy, mS, mS, mRoundness, mHandleEmptyOutlineTK);
				}
			}
			
			mVecDraw->renderShadow(false);
		}
		
		renderBackground(pGraphics, mVecDraw, mRECT);

		// Matrix fills
		
		for (int i = 0; i < mXDim; i++)
		{
			double sx = mX + i * mUnit;
			
			for (int j = 0; j < mYDim; j++) 
			{
				double sy = mY + j * mUnit;
								
				mVecDraw->setColor(mStateCS[mStates[j * mXDim + i] % mNStates]);   
				mVecDraw->fillRoundRect(sx, sy, mS, mS, mRoundness);
				mVecDraw->setColor(mOutlineCS);
				mVecDraw->frameRoundRect(sx, sy, mS, mS, mRoundness, mHandleFilledOutlineTK);
			}
		}

		if (mXHilite > -1 && mYHilite > -1)
		{
			mVecDraw->setColor(mHiliteCS);
			mVecDraw->frameRoundRect(mX + mXPos * mUnit, mY + mYPos * mUnit, mS, mS, mRoundness, mHiliteTK);
		}
	}
	
	void SetState(int x, int y, char state)
	{
		if (x >= 0 && x < mXDim && y >= 0  && y < mYDim)
			mStates[y * mXDim + x] = state;
		
		SetDirty(false);
	}
	
	unsigned char GetState(int x, int y)
	{
		if (x >= 0 && x < mXDim && y >= 0  && y < mYDim)
			return mStates[y * mXDim + x];
		
		return 0;
	}
	
	void SetHilite(bool on)
	{
		if (on == true)
		{
			mXHilite = mXPos;
			mYHilite = mYPos;
		}
		else 
		{
			mXHilite = -1;
			mXHilite = -1;
		}
		
		SetDirty(false);
	}
	
	bool validReport(bool reset = true)
	{
		bool validReport = mValidReport;
		if (reset)
			mValidReport = false;
		return validReport;
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// HISSTools TextBlock ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// A Text Block (Static or otherwise)

class HISSTools_TextBlock : public HISSTools_Text_Helper_Block, public IControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
public:
	
	// Constructors
	
	HISSTools_TextBlock(IPlugBaseGraphics* plug, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w, double h, const char* str = "", HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter, const char* type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: HISSTools_Text_Helper_Block(x, y, w, h, hAlign, vAlign, "TextBlock", type, designScheme), IControl(*plug, IRECT()), HISSTools_Control_Layers()
	{
		mVecDraw = vecDraw;
		setText(str);
		mRECT = bounds().iBounds();
	}
	
	void Draw(IGraphics& pGraphics)
	{
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);
        HISSTools_Text_Helper_Block::Draw(mVecDraw);
	}
	
	void setText(const char *str)
	{
		HISSTools_Text_Helper_Block::setText(str);
		SetDirty();
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// HISSTools Panel //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Panel with or without outline and with dropshadow

class HISSTools_Panel: public IControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;
	
	// Positioning / Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
	
	double mRoundnessTL;
	double mRoundnessTR;
	double mRoundnessBL;
	double mRoundnessBR;
	
	// Line Thicknesses
	
	double mOutlineTK;
	
	// Shadow Spec
	
	HISSTools_Shadow *mShadow;
	
	// Color Specs
	
	HISSTools_Color_Spec *mPanelCS;
	HISSTools_Color_Spec *mOutlineCS;
	
	bool mDrawOutline;
	
private:
	
	double getRoundness(const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{
		double roundness = designScheme->getDimension(name, type);
		
		// FIX - Why height?
		
		return roundness < 0 ? mH / 2 : roundness;	
	}
	
public:
	
	// Constructor
	
	HISSTools_Panel(IPlugBaseGraphics* plug, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(*plug, IRECT()), HISSTools_Control_Layers()
	{		
		mVecDraw = vecDraw;
        
		// Dimenasions
		
		mX = x;
		mY = y;
		mW = w <= 0 ? designScheme->getDimension("PanelWidth", type) : w;
		mH = h <= 0 ? designScheme->getDimension("PanelHeight", type) : h;
		
		mRoundnessTL = getRoundness("PanelRoundnessTL", type, designScheme);
		mRoundnessTR = getRoundness("PanelRoundnessTR", type, designScheme);
		mRoundnessBL = getRoundness("PanelRoundnessBL", type, designScheme);
		mRoundnessBR = getRoundness("PanelRoundnessBR", type, designScheme);
		
		// Get Appearance
		
		mOutlineTK = designScheme->getDimension("PanelOutline", type);
		
		mShadow = designScheme->getShadow("Panel", type);
			
		mDrawOutline = designScheme->getFlag("PanelDrawOutline", type);
		
		mPanelCS = designScheme->getColorSpec("PanelFill", type);
		mOutlineCS = designScheme->getColorSpec("PanelOutline", type);
			
		// Calculate Areas (including shadows and thicknesses)
		
		HISSTools_Bounds fullBounds(mX, mY, mW, mH);

		if (mDrawOutline)
			fullBounds.addThickness(mOutlineTK);
		
		fullBounds = mShadow->getBlurBounds(fullBounds);
		
		mRECT = (fullBounds.iBounds());
	}
	
public:
	
	// Draw
	
	void Draw(IGraphics& pGraphics)
	{				
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);

		if (startBackground(pGraphics, mVecDraw, mRECT))
		{
			mVecDraw->startShadow(mShadow);
			mVecDraw->setColor(mPanelCS);
			mVecDraw->fillRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR);
			if (mDrawOutline)
			{				
				mVecDraw->setColor(mOutlineCS);
				mVecDraw->frameRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR, mOutlineTK);
			}
			mVecDraw->renderShadow();
		}
		
		// Background
		
		renderBackground(pGraphics, mVecDraw, mRECT);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// HISSTools VU ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


class HISSTools_Progress : public IControl, public HISSTools_Control_Layers
{
	
private:
	
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;	
	
	// Positioning / Dimensions
	
	double mX;
	double mY;
	double mW;
	double mH;
		
	// Line Thicknesses
	
	double mOutlineTK;
	
	// Shadow Spec
	
	HISSTools_Shadow *mShadow;
		
	// Color Specs
	
	HISSTools_Color_Spec *mBackgroundCS;
	HISSTools_Color_Spec *mOutlineCS;
	HISSTools_Color_Spec *mProgressCS;
		
public:
	
	HISSTools_Progress(IPlugBaseGraphics* plug, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(*plug, IRECT()), HISSTools_Control_Layers(), mX(x), mY(y), mW(w), mH(h)
	{ 	
		mVecDraw = vecDraw;
        
		// Get Appearance
		
		mOutlineTK = designScheme->getDimension("ProgressOutline", type);
						
		mBackgroundCS = designScheme->getColorSpec("ProgressBackground", type);
		mOutlineCS = designScheme->getColorSpec("ProgressOutline", type);
		mProgressCS = designScheme->getColorSpec("Progress", type);
		
		mShadow  = designScheme->getShadow("Progress", type);
		
		// Area
		
		// FIX - Hack
		
		mRECT = HISSTools_Bounds(x, y, w, h).iBounds();
	}
	
	~HISSTools_Progress() {}
	
	
	void Draw(IGraphics& pGraphics)
	{				
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);
		mVecDraw->setColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);
        
		// Parameters
				
		if (startBackground(pGraphics, mVecDraw, mRECT))
		{
			mVecDraw->setColor(mBackgroundCS);
			mVecDraw->fillRect(mX, mY, mW, mH);
			
			// Frame Rectangle
			
			mVecDraw->startShadow(mShadow);
			mVecDraw->setColor(mOutlineCS);
			mVecDraw->frameRect(mX, mY, mW, mH, mOutlineTK);
			mVecDraw->renderShadow();
		}
		
		renderBackground(pGraphics, mVecDraw, mRECT);
		
		// Meter Rectangles
		
		mVecDraw->forceGradientBox(mX, mY, mX + mW, mY + mH);
		
		// Progress
		
		mVecDraw->setColor(mProgressCS);

		if (mW < mH)
			mVecDraw->fillRect(mX, mY + mH * (1 - mValue), mW, mH * mValue);
		else 
			mVecDraw->fillRect(mX, mY, mW * mValue, mH);
			
		mVecDraw->forceGradientBox();
		
		// Outline Again
		// FIX - Draw ALL PROPERLY
		
		mVecDraw->setColor(mOutlineCS);
		mVecDraw->frameRect(mX, mY, mW, mH, mOutlineTK);
		
		// Reset Orientation (default is always horizontal)
		
		mVecDraw->setColorOrientation(kCSOrientHorizontal);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// HISSTools VU ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// A VU meter display. Ballistics should be provided on the DSP side
// The meter supports two on meter levels (drawn in order) and a side value, typically intended for peak hold


class HISSTools_MeterTest : public IControl, public HISSTools_Control_Layers
{
	
private:
		
	// Drawing Object
	
	HISSTools_LICE_Vec_Lib *mVecDraw;	
	
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
		double db = linear == true ? 20. * log10(value) : value;
		double size = (db - mMinDB) / (mMaxDB - mMinDB);
		
		if (size != size)
			size = 0;
		
		size = size > 1 ? 1 : size;
		
		return size;
	}
	
	void horzTick(double x1, double x2, double y, double h, double normPosition, double thickness)
	{
		double yPos =  y + h * (1 - normPosition);
		
		mVecDraw->line(x1, yPos, x2, yPos, thickness);
	}
	
	void vertTick(double y1, double y2, double x, double w, double normPosition, double thickness)
	{
		double xPos =  x + w * normPosition;
		
		mVecDraw->line(xPos, y1, xPos, y2, thickness);
	}
	
public:
	
	HISSTools_MeterTest(IPlugBaseGraphics* plug, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w, double h, bool flip = false, double minDB = -60, double maxDB = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(*plug, IRECT()), HISSTools_Control_Layers()
	{ 	
		mVecDraw = vecDraw;
        
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
	
	~HISSTools_MeterTest() {}
	
	void setLevels(double VU1, double VU2, double side, bool peak, bool linear = true)
	{
		mVU1Size = getSize(VU1, linear);
		mVU2Size = getSize(VU2, linear);
		mSideSize = getSize(side, linear);
		
		mPeak = peak;
		
		SetDirty(false);
		Redraw();
	}
	
	void Draw(IGraphics& pGraphics)
	{				
		mVecDraw->setContext((cairo_t *)pGraphics.GetData(), pGraphics.GetDisplayScale());
        mVecDraw->setClip(mRECT);
		mVecDraw->setColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);

		// Parameters
		
		int nTicks = 10;
		
		if (startBackground(pGraphics, mVecDraw, mRECT))
		{
			mVecDraw->setColor(mBackgroundCS);
			mVecDraw->fillRect(mX, mY, mW, mH);
		
			// Frame Rectangle
		
			mVecDraw->startShadow(mShadow);
			mVecDraw->setColor(mOutlineCS);
			mVecDraw->frameRect(mX, mY, mW, mH, mOutlineTK);
			mVecDraw->renderShadow();
		}
		
		renderBackground(pGraphics, mVecDraw, mRECT);
		
		// Meter Rectangles
		
		mVecDraw->forceGradientBox(mX, mY, mX + mW, mY + mH);

		// VU 1
		
		if (mVU1Size > 0)
		{
			if (mPeak)
				mVecDraw->setColor(mVU1PeakCS);
			else
				mVecDraw->setColor(mVU1CS);
		
			if (mW < mH)
				mVecDraw->fillRect(mX, mY + mH * (1 - mVU1Size), mW, mH * mVU1Size);
			else 
				mVecDraw->fillRect(mX, mY, mW * mVU1Size, mH);
		}
		
		// VU 2
		
		if (mVU2Size > 0)
		{
			if (mOverlayFixedGradientBox == false)
				mVecDraw->forceGradientBox();
			
			if (mPeak)
				mVecDraw->setColor(mVU2PeakCS);
			else
				mVecDraw->setColor(mVU2CS);
					
			if (mW < mH)
				mVecDraw->fillRect(mX, mY + mH * (1 - mVU2Size),  mW, mH * mVU2Size);
			else
				mVecDraw->fillRect(mX, mY, mW * mVU2Size, mH);
		}
		
		// Ticks
		
		mVecDraw->setColor(mOutlineCS);
				
		if (mW < mH)
			for (int i = 0; i < nTicks; i++)
				horzTick(mTick1, mTick2, mY, mH, (i / (double) (nTicks - 1)), mTickTK);
		else
			for (int i = 0; i < nTicks; i++)
				vertTick (mTick1, mTick2, mX, mW, (i / (double) (nTicks - 1)), mTickTK);
			
		// Side VU
		
		if (mSideSize >= 0)
		{
			mVecDraw->forceGradientBox(mX, mY, mX + mW, mY + mH);
		
			if (mPeak)
				mVecDraw->setColor(mVUSidePeakCS);
			else
				mVecDraw->setColor(mVUSideCS);
		
			if (mW < mH)
				horzTick(mTick3, mTick4, mY, mH, mSideSize, mPeakHoldTK);
			else
				vertTick(mTick3, mTick4, mX, mW, mSideSize, mPeakHoldTK);
		}

		mVecDraw->forceGradientBox();
		
		// Outline Again
		// FIX - Draw ALL PROPERLY
		
		mVecDraw->setColor(mOutlineCS);
		mVecDraw->frameRect(mX, mY, mW, mH, mOutlineTK);

		// Reset Orientation (default is always horizontal)
		
		mVecDraw->setColorOrientation(kCSOrientHorizontal);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// HISSTools FileSelector //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	
	bool mValidReport;
	
private:

	void reportToPlug()
	{
		if (mParamIdx >= 0)
		{
			mValidReport = true;
            mPlug.OnParamChange(mParamIdx);//, IPlugBaseGraphics::kGUI);
			mValidReport = false;
		}
	}
	
public:
	
	// FIX - turn automation off (also for matrix)
		
	HISSTools_FileSelector(IPlugBaseGraphics* plug, int paramIdx, HISSTools_LICE_Vec_Lib *vecDraw, double x, double y, double w, double h, EFileAction action, char* dir = "", char* extensions = "", const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: HISSTools_Button(plug, paramIdx, vecDraw, x, y, w, h, type, designScheme) , mFileAction(action), mDir(dir), mExtensions(extensions)
	{
		mValidReport = false;
	}
		
	void OnMouseDown(int x, int y, const IMouseMod& pMod)
	{
		if (mPlug.GetGUI())
		{
			WDL_String tempFile;

			if (pMod.A == true)
			{
				mState = kFSDone;

				mFile.Set("");
				reportToPlug();
			}
			else 
			{
				mState = kFSSelecting;
				SetDirty(false);
				
				mPlug.GetGUI()->PromptForFile(tempFile, mFileAction, &mDir, mExtensions.Get());
				
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
	
	void Draw(IGraphics& pGraphics)
	{
		switch (mState)
		{
			case kFSDone:
			case kFSNone:
				mValue = 0;
				break;
				
			case kFSSelecting:
				mValue = 1;
				break;
		}
		
		HISSTools_Button::Draw(pGraphics);
	}
	
	void GetLastSelectedFileForPlug(WDL_String *pStr)
	{
		pStr->Set(mFile.Get());
	}
	
	void SetLastSelectedFileFromPlug(char* file)
	{
		mFile.Set(file);
	}
	
	bool validReport(bool reset = true)
	{
		bool valid = mValidReport;
		if (reset)
			mValidReport = false;
		return valid;
	}
    
    void setExtensions(char *extensions)
    {
        mExtensions.Set(extensions);
    }
};

#endif /* __HISSTOOLS_CONTROLS__ */
