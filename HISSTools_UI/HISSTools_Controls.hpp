
#ifndef __HISSTOOLS_CONTROLS__
#define __HISSTOOLS_CONTROLS__

#include "../HISSTools_Graphics/HISSTools_VecLib.hpp"
#include "HISSTools_Design_Scheme.hpp"
#include <vector>

#include <IPlugAPIBase.h>
#include <IControl.h>

enum MousingAction {kMouseDown, kMouseUp, kMouseDblClick, kMouseDrag, kMouseWheel, kMouseOver, kMouseOut};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

// A struct to bring iplug and igraphics items into scope

struct HISSTools_Graphics_Types
{    
    using IEditorDelegate = iplug::IEditorDelegate;
    template <typename T>
    using IPlugQueue = iplug::IPlugQueue<T>;
    using IParam = iplug::IParam;
    using IKeyPress = iplug::IKeyPress;

    using IGraphics = iplug::igraphics::IGraphics;
    using IPopupMenu = iplug::igraphics::IPopupMenu;
    using IBitmap = iplug::igraphics::IBitmap;
    using IRECT = iplug::igraphics::IRECT;
    using IPattern = iplug::igraphics::IPattern;
    using IMatrix = iplug::igraphics::IMatrix;
    using IColor = iplug::igraphics::IColor;
    using IText = iplug::igraphics::IText;
    using IShadow = iplug::igraphics::IShadow;
    using IFillOptions = iplug::igraphics::IFillOptions;
    using IStrokeOptions = iplug::igraphics::IStrokeOptions;
    using IMouseMod = iplug::igraphics::IMouseMod;
    using IActionFunction = iplug::igraphics::IActionFunction;
    using ILayerPtr = iplug::igraphics::ILayerPtr;
    
    using EBlend = iplug::igraphics::EBlend;
    using EAlign = iplug::igraphics::EAlign;
    using EVAlign = iplug::igraphics::EVAlign;
    using ETextStyle = iplug::igraphics::ETextStyle;
    using EDirection = iplug::igraphics::EDirection;
    using EFileAction = iplug::igraphics::EFileAction;
    using EWinding = iplug::igraphics::EWinding;
    using EFillRule = iplug::igraphics::EFillRule;
    using ELineCap = iplug::igraphics::ELineCap;
    using ELineJoin = iplug::igraphics::ELineJoin;
    using EPatternType = iplug::igraphics::EPatternType;
    using EPatternExtend = iplug::igraphics::EPatternExtend;
    using ECursor = iplug::igraphics::ECursor;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// A class to cache layers in control drawing (currently only supports a single background...)

class HISSTools_Control_Layers : protected virtual HISSTools_Graphics_Types
{

private:
	
    ILayerPtr mBackground;
    const static bool sNoCaching = false;
    
public:
	
    HISSTools_Control_Layers()
	{}
	
    bool startBackground(IGraphics& graphics, HISSTools_VecLib& vecDraw, IRECT area)
    {
        if (sNoCaching)
            return true;
        
        if (!graphics.CheckLayer(mBackground))
        {
            vecDraw.startGroup(area);
            return true;
        }
        
        return false;
    }
    
    void renderBackground(IGraphics& graphics, HISSTools_VecLib& vecDraw, IRECT area)
    {
        if (!sNoCaching)
        {
            if (!graphics.CheckLayer(mBackground))
                mBackground = vecDraw.endGroup();
            if (mBackground)
                vecDraw.renderGroup(mBackground);
        }
    }
   
	void redrawBackground()
	{
		if (mBackground)
            mBackground->Invalidate();
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

// An abstract class to control tabs in plug-ins

class HISSTools_Tabs : protected virtual HISSTools_Graphics_Types
{

private:
	
    struct TabItem
	{
		iplug::igraphics::IControl *mControl;
		int mTabNumber;
		
		TabItem(iplug::igraphics::IControl *control, int tabNumber) : mControl(control), mTabNumber(tabNumber)
		{}
	};
	
	const IParam *mParam;
	iplug::igraphics::IControl *mTabControl;
	std::vector<TabItem> mItems;
	
	int mCurrentTabNumber;
	int mMaxTabNumber;	
	
	void updateItems()
	{
		bool tabObjectHidden = mTabControl->IsHidden();
		
		for (auto it = mItems.begin(); it != mItems.end(); it++)
            it->mControl->Hide(tabObjectHidden || clipTabNumber(it->mTabNumber) != mCurrentTabNumber);
	}
	
	int clipTabNumber(int tabNumber)
	{
		tabNumber = tabNumber < 0 ? 0 : tabNumber;
		tabNumber = tabNumber > mMaxTabNumber ? mMaxTabNumber : tabNumber;

		return tabNumber;
	}
	
public:
	
	// You should pass the inheriting class here, after constructing the control, which must be mapped to a valid parameter of the plug - the tabs are tied to the parameter, rather than the control
	
    HISSTools_Tabs(iplug::igraphics::IControl *tabControl) : mTabControl(tabControl), mParam(nullptr)
	{}
	
    // Call this from OnInit in the inheriting class
    
    void init()
    {
        mParam = mTabControl->GetParam();
        mMaxTabNumber = mParam != nullptr ? round(mParam->GetRange()) : 0;
        tabSetDirty(false);
    }
    
	void attachControl(iplug::igraphics::IControl *control, int tabNumber)
	{
        // N.B. - mMaxTabNumber is one lass than the number of actual tabs (zero referenced)

		mItems.push_back(TabItem(control, tabNumber));
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
	/*
	void setTabFromPlug(int tabNumber)
	{
		if (mParam)
			mParam->Set(tabNumber + mParam->GetMin());
			
		tabSetDirty(false);
	}*/
	
	// These functions should be declared in any inheriting classes, and should call the related tab versions
	
	virtual void Hide(bool hide) = 0;
	virtual void SetDirty(bool pushParamToPlug, int) = 0;
};


// This class allows Live-style tabs that have no explicit selector, and instead are set from the plug via other related controls

class HISSTools_Invisible_Tabs : public iplug::igraphics::IControl, public HISSTools_Tabs
{
    
public:
	
	HISSTools_Invisible_Tabs(int paramIdx) : IControl(IRECT(), paramIdx), HISSTools_Tabs(this) {}
	
    void OnInit() override                              { init(); }
	void Draw(IGraphics& g) override                    { }
	void Hide(bool hide) override                       { tabHide(hide); }
	void SetDirty(bool pushParamToPlug, int) override   { tabSetDirty(pushParamToPlug); }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper class to deal with blocks of text 

class HISSTools_Text_Helper_Block : protected virtual HISSTools_Graphics_Types
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
	
	HISSTools_Text_Helper_Block(double x, double y, double w, double h,  HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{		
		setup(x, y, w, h, hAlign, vAlign, name, type, designScheme);
	}
	
	HISSTools_Text_Helper_Block(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	{
		setup(0, 0, 0, 0, hAlign, vAlign, name, type, designScheme);
	}
	
	void Draw(HISSTools_VecLib& vecDraw)
	{
		if (mTextSD)
			vecDraw.startShadow(mTextSD, bounds());
			
		vecDraw.setColor(mTextCS);
		vecDraw.text(mTextTS, mStr.Get(), mX, mY, mW, mH, mHAlign, mVAlign);

		if (mTextSD)
			vecDraw.renderShadow();
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
        return (mPanelOutlineTK && mPanelOutlineCS != nullptr);
    }
				
    bool doDrawPanel()
    {
        return ((mPanelFillCS != nullptr) || doDrawOutline());
    }

public:
	
	HISSTools_Text_Helper_Panel(double x, double y, double w, double h, double wPad, double hPad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	: HISSTools_Text_Helper_Block(hAlign, vAlign, name, type, designScheme)
	{
		setup(x, y, w, h, wPad, hPad, hAlign, vAlign, name, type, designScheme);
	}
	
	HISSTools_Text_Helper_Panel(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme) 
	: HISSTools_Text_Helper_Block(hAlign, vAlign, name, type, designScheme)
	{
		setup(0, 0, 0, 0, 0, 0, hAlign, vAlign, name, type, designScheme);
	}
	
	void Draw(HISSTools_VecLib& vecDraw, bool drawText)
	{	
		if (doDrawPanel())
		{
			if (mPanelSD)
				vecDraw.startShadow(mPanelSD, bounds());
		
			vecDraw.setColor(mPanelFillCS);
			vecDraw.fillRoundRect(mX, mY, mW, mH, mPanelRoundness);
			
			if (doDrawOutline())
			{	
				vecDraw.setColor(mPanelOutlineCS);
				vecDraw.frameRoundRect(mX, mY, mW, mH, mPanelRoundness, mPanelOutlineTK);
			}

			if (mPanelSD)
				vecDraw.renderShadow();
		}
		
        if (drawText)
            HISSTools_Text_Helper_Block::Draw(vecDraw);

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
		mX = w > 0 ? x : x + w;
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
	
	iplug::igraphics::IControl *mControl;
	
    double mPromptRatio;
	double mTextHPad;
	double mSeparatorX;
	
	bool mDrawSeparator;
	bool mDrawTriangle;
	bool mMenuFlipTriangle;
    bool mInEdit;
	
	double mMenuTriangleTop;
	double mMenuTriangleBtm;
	double mMenuTriangleL;
	double mMenuTriangleR;
	
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
		
		switch (mHAlign)
		{
			case kHAlignLeft:
                text.mAlign = EAlign::Near;
				break;
				
			case kHAlignCenter:
				text.mAlign = EAlign::Center;
				break;
				
			case kHAlignRight:
				text.mAlign = EAlign::Far;
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
	
	HISSTools_Text_Helper_Param(iplug::igraphics::IControl *control, double x, double y, double w, double h, double pad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *designScheme)
	: HISSTools_Text_Helper_Panel(x, y, w, h, 0, 0, hAlign, vAlign, name, type, designScheme), mInEdit(false)
	{			
        double textHeight = HISSTools_VecLib::getTextLineHeight(mTextTS);
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
        sprintf(concatenatedName, "%sPromptRatio", name);
        mPromptRatio = designScheme->getDimension(concatenatedName, type);
        sprintf(concatenatedName, "%sHilite", name);
		mTextHiliteCS = designScheme->getColorSpec(concatenatedName, type);
		sprintf(concatenatedName, "%sPanelHilite", name);
		mPanelHiliteCS = designScheme->getColorSpec(concatenatedName, type);
		sprintf(concatenatedName, "%sPanelOutlineHilite", name);
		mOutlineHiliteCS = designScheme->getColorSpec(concatenatedName, type);
		mShowUnits = designScheme->getFlag("ShowUnits", type);
		
		// FIX - Padding! (see Draw also)
    
		double wPad = 9;
		double hPad = 0;
		
		//changePadding(wPad, hPad);
		changePadding(0, hPad);
        
		if (drawMenuTriangle)
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
			mSeparatorX = mX + (mMenuFlipTriangle ? separatorWidth : mW - separatorWidth);

            mMenuTriangleTop = mY + (mH - menuTriangleHeight) / 2.0;
			mMenuTriangleBtm = mMenuTriangleTop + menuTriangleHeight;
			mMenuTriangleL = mSeparatorX + (mMenuFlipTriangle ? -(menuTriangleWidth + wPad) : wPad);
			mMenuTriangleR = mMenuTriangleL + menuTriangleWidth;
		}
	}
	
	bool menuParam()
	{
		if (mControl->GetParam() == nullptr)
			return false;
		
		if (mControl->GetParam()->NDisplayTexts())
			return true;
		
		return false;
	}
	
    void finishEdit() { mInEdit = false; }
    
	void promptUserInput()
	{
		HISSTools_Bounds entryBounds;
		IRECT iEntryBounds;
		
        mInEdit = menuParam() ? false : true;
        
		// FIX - Widths ??
		// FIX - Text Prompt Vertical Centering??
		// FIX - Text Prompt - proper matching font....
		// FIX - Prompt Menus - Centering for text and box
		// FIX - Prompt Menus - font....
		
		if (mControl->GetParam() == nullptr)
			return;
		
		double textHeight = mTextTS->mSize;
		double promptHeight = menuParam() ? 0 : textHeight + (2.0 * mTextHPad);

		// FIX - Set Padding, rather than variable???
		// FIX - is this one justified?
		
		double promptTop = mY - ((textHeight / 2.0) + mTextHPad + 1.0);
		double promptLeft = mX + mLPad;
		double promptWidth = mW - (mLPad + mRPad);

        promptLeft += 0.5 * promptWidth * (1.0 - mPromptRatio);
        promptWidth *= mPromptRatio;
        
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
		
		if (mDrawSeparator && menuParam() && doDrawOutline())
		{
			promptTop = mY + mH;
            promptLeft = mMenuFlipTriangle ? mX : mSeparatorX;
		}
		
		entryBounds = HISSTools_Bounds(promptLeft, promptTop, promptWidth, promptHeight);
		
		setControlText();
		mControl->DisablePrompt(false);
		mControl->PromptUserInput(entryBounds);
	}
	
	bool promptUserInput(float x, float y)
	{
		if (bounds().Contains(x, y))
		{
			if (!mDrawSeparator || (mMenuFlipTriangle == (x < mSeparatorX)))
			{
				promptUserInput();
				return true;
			}
		}
		
		return false;
	}
	
	void hilite(bool on)
	{
		if (on)
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
	
	void Draw(HISSTools_VecLib& vecDraw)
	{
        if (mDrawTriangle && menuParam())
        {
            // FIX - Padding! (see constructor also)

            double wPad = 9;
            double hPad = 0;
            double separatorWidth = mMenuFlipTriangle ? (mSeparatorX - mX) : mW + mX - mSeparatorX;

            changePadding(wPad + (mMenuFlipTriangle ? separatorWidth : 0), wPad + (mMenuFlipTriangle ? 0 : separatorWidth), hPad);
        }
        
		const IParam *param = mControl->GetParam();
		
        WDL_String str;
        
		if (param == nullptr)
			return;
		
		// Retrieve Value
		
		param->GetDisplay(str);
		
		// If label is nullptr don't add the space
		
		if (mShowUnits && *(param->GetLabel()))
		{
			str.Append(" ");
			str.Append(param->GetLabel());
		}
		
		// Draw Text (with Panel)
		
		setText(str.Get());
		HISSTools_Text_Helper_Panel::Draw(vecDraw, !mInEdit);
		
		// Menu Separator / Triangle
		
		if (mDrawTriangle && menuParam())
		{
            // Separator
            
            if (mDrawSeparator && doDrawOutline())
            {
                vecDraw.setColor(mPanelOutlineCS);
                vecDraw.line(mSeparatorX, mY, mSeparatorX, mY + mH, mPanelOutlineTK);
            }
            
            // Triangle
            
			if (mTextSD)
                vecDraw.startShadow(mTextSD, IRECT(mX, mY, mX + mW, mY + mH));
		 
            vecDraw.setColor(mTextCS);
            vecDraw.fillTriangle(mMenuTriangleL, mMenuTriangleTop, (mMenuTriangleL + mMenuTriangleR) / 2.0, mMenuTriangleBtm, mMenuTriangleR, mMenuTriangleTop);
		 
			if (mTextSD)
                vecDraw.renderShadow();
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// HISSTools Value //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Multipurpose numerical / menu control

// FIX - do your own mousing later...

class HISSTools_Value : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{
	
private:
	
	// Text
	
	HISSTools_Text_Helper_Param *mTextParam;
	HISSTools_Text_Helper_Block *mTextLabel;
	
    // Name
    
    WDL_String mDisplayName;

    // Mousing
    
    bool mDrag;
    
	// Stored Dimensions
	
	double mTextArea;
	
public:
	
	HISSTools_Value(int paramIdx, double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr)
	: IKnobControlBase(IRECT(), paramIdx), HISSTools_Control_Layers()
	{
		// FIX - perhaps just inherit these??

        bool labelBelow = designScheme->getFlag("ValueLabelBelow", type);
        bool label = designScheme->getFlag("ValueDrawLabel", type);
		mTextArea = designScheme->getDimension("ValueTextArea", type);
		        
		mTextParam = new HISSTools_Text_Helper_Param(this, x, y, w, h, 1, kHAlignCenter, kVAlignCenter, "Value", type, designScheme);
        
        if (label && labelBelow)
            mTextLabel = new HISSTools_Text_Helper_Block(x, y + h, w, mTextArea, kHAlignCenter, kVAlignBottom, "ValueLabel", type, designScheme);
        else if (label)
            mTextLabel = new HISSTools_Text_Helper_Block(x, y - mTextArea, w, mTextArea, kHAlignCenter, kVAlignTop, "ValueLabel", type, designScheme);
        else
            mTextLabel = nullptr;

		SetTargetRECT(HISSTools_Bounds(x, y, w, h));
		
		HISSTools_Bounds fullBoxBounds = mTextParam->bounds();
		if (mTextLabel)
            fullBoxBounds.include(mTextLabel->bounds());
		mRECT = fullBoxBounds;
        
        SetMouseOverWhenDisabled(true);
        SetMouseEventsWhenDisabled(true);
        
        if (name)
            mDisplayName.Set(name);
	}
	
	~HISSTools_Value()
	{
		delete mTextParam;
		delete mTextLabel;
	}
	
    void OnInit() override
    {
        if (mTextLabel)
        {
            if (mDisplayName.GetLength())
                mTextLabel->setText(mDisplayName.Get());
            else
                mTextLabel->setText((GetParam() != nullptr) ? GetParam()->GetName() : "");
        }
    }
    
	void OnMouseDown(float x, float y, const IMouseMod& pMod) override
	{
        mMouseDown = true;
        mMouseDragValue = GetValue();

		if (pMod.S)
		{
            SetValueToDefault();
            return;
		}

		if (mTextParam->menuParam())
		{
			if (mTextParam->promptUserInput(x, y) == false && GetParam())
			{
				double value = round(GetValue() * (GetParam()->GetRange()) + 1) / (GetParam()->GetRange());
                SetValue(value > 1.0 ? 0 : value);
			}
		}
        else
            mTextParam->hilite(true);
        
        mDrag = false;
		SetDirty();
	}
	
	void OnMouseUp(float x, float y, const IMouseMod& pMod) override
	{
        if (mDrag == false)
        {
            if (mTextParam->menuParam() == false)
                mTextParam->promptUserInput();
        }
        else
            mTextParam->hilite(false);
            
        mMouseDown = false;
        SetDirty(false);
	}
	
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override
    {
        mDrag = true;
    
        IKnobControlBase::OnMouseDrag(x, y, dX, dY, pMod);
    }
    
	void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override
	{
        OnMouseDown(x, y, pMod);
	}
    
    virtual void SetValueFromUserInput(double value, int valIdx) override
    {
        mDrag = false;
        mTextParam->finishEdit();
        mTextParam->hilite(false);
        IKnobControlBase::SetValueFromUserInput(value, valIdx);
    }
	
	void Draw(IGraphics& g) override
	{
        HISSTools_VecLib vecDraw(g);

        // Label
        
        if (mTextLabel)
        {
            if (startBackground(g, vecDraw, mRECT))
                mTextLabel->Draw(vecDraw);
            renderBackground(g, vecDraw, mRECT);
        }
		mTextParam->Draw(vecDraw);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// HISSTools Dial ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// Multipurpose dial with auto value display

// FIX - do your own mousing later...

class HISSTools_Dial : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{
	
private:
	
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
	
    // Values on when mouse is over
    
    bool mMouseOver;
    bool mDrawValOnlyOnMO;
    
    WDL_String mDisplayName;
    
public:

	// Constructor

	HISSTools_Dial(int paramIdx, double x, double y, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr)
	: IKnobControlBase(IRECT(), paramIdx), HISSTools_Control_Layers(), mMouseOver(false)
	{
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
		
        mDrawValOnlyOnMO = designScheme->getFlag("DialDrawValOnlyOnMO");
        
		// Text 
				
		mPromptHalfHeight = (0.5 * designScheme->getTextStyle("DialValue", type)->mSize) + designScheme->getDimension("DialPromptPadding", type);

		double textPad = designScheme->getDimension("DialPromptPadding", type);
        double halfWidth = mR;// * mPromptRatio;// * mPointerCircRadius;
        double labelHalfWidth = mR * 1.2;
        
		//FIX - above
		mTextParam = new HISSTools_Text_Helper_Param(this, mCx - halfWidth, mCy - mPromptHalfHeight, 2. * halfWidth, 2. * mPromptHalfHeight, textPad, kHAlignCenter, kVAlignCenter, "DialValue", type, designScheme);
		mTextLabel = new HISSTools_Text_Helper_Block(mCx - labelHalfWidth, mCy + mR, 2 * labelHalfWidth, mTextArea, kHAlignCenter, kVAlignBottom, "DialLabel", type, designScheme);
        
		// Calculate Areas (including shadows and thicknesses)
		
        HISSTools_Bounds labelBounds(mCx - labelHalfWidth, mCy + mR, 2 * labelHalfWidth, mTextArea);
        HISSTools_Bounds dialBoxBounds(x, y, d, d);
        HISSTools_Bounds fullBoxBounds = dialBoxBounds;
        fullBoxBounds.include(labelBounds);
		HISSTools_Bounds ptrBoxBounds(mCx - mPointerTipRadius, mCy - mPointerTipRadius, 2 * mPointerTipRadius, 2 * mPointerTipRadius);
		
		dialBoxBounds.addThickness(mOutlineTK);
		ptrBoxBounds.addThickness(mPointerOutlineTK);
		
		fullBoxBounds.include(mOutlineSD->getBlurBounds(dialBoxBounds));
		fullBoxBounds.include(mPointerSD->getBlurBounds(ptrBoxBounds));
		
		mRECT = fullBoxBounds;
		SetTargetRECT(dialBoxBounds);
        
        SetMouseOverWhenDisabled(true);
        SetMouseEventsWhenDisabled(true);
        
        if (name)
            mDisplayName.Set(name);
	}	
	
	~HISSTools_Dial()
	{
		delete mTextLabel;
		delete mTextParam;
	}
	
    void OnInit() override
    {
        if (mDisplayName.GetLength())
            mTextLabel->setText(mDisplayName.Get());
        else
            mTextLabel->setText(GetParam() != nullptr ? GetParam()->GetName() : "");
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
	
	void OnMouseDown(float x, float y, const IMouseMod& pMod) override
	{
        OnMouseOver(x, y, pMod);
        
        mMouseDown = true;
        mMouseDragValue = GetValue();
        
		if (pMod.S)
            SetValueToDefault();
        else
            GetUI()->HideMouseCursor();
	}
	
	void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override
	{
		// FIX - Confirm best key combo...
		
		if (pMod.S)
		{
			OnMouseDown(x, y, pMod);
			return;
		}
		
		mTextParam->promptUserInput();
        SetDirty(false);
	}
    
    void OnMouseOver(float x, float y, const IMouseMod& pMod) override
    {
        if (!mMouseOver)
        {
            mMouseOver = true;
            SetDirty(false);
        }
    }

    void OnMouseOut() override
    {
        if (mMouseOver)
        {
            mMouseOver = false;
            SetDirty(false);
        }
    }
    
    void SetDisabled(bool disable) override
    {
        bool old = mDisabled;
        mDisabled = disable;
        if (disable != old)
            SetDirty(false);
    }
    
    void SetValueFromUserInput(double value, int valIdx) override
    {
        mTextParam->finishEdit();
        IControl::SetValueFromUserInput(value, valIdx);
    }
    
	/*
	 // Mousing Functions
	 
	 // FIX - better gearing??
	 // FIX - click through options?
	 
	 void OnMouseDown(float x, float y, const IMouseMod& pMod) override
	 {
	 OnMouseDrag(x, y, 0, 0, pMod);
	 }
	 
	 void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override
	 {		
	 IParam *param = GetParam();
	 
	 IControl::OnMouseDrag(x, y, 0, 0, pMod);
	 
	 if (param && param->Type() != IParam::kTypeDouble && param->Type() != IParam::kTypeNone)
	 {
	 double value = param->Int();
	 double delta;
	 
	 if (mDirection == kVertical)
	 delta = -dY;
	 else
	 delta = dX;
	 
	 delta /= mGearing;
	 delta = round(delta);
	 value += delta;
	 value -= param->GetMin();
	 
	 mValue = value / param->GetRange();
	 }
	 
	 SetDirty(true);
	 }*/
	
	// Draw
	
	void Draw(IGraphics& g) override
	{
		const IParam *param = GetParam();
		double value, xIntersect, yIntersect;
		
        HISSTools_VecLib vecDraw(g);

		// Background
		
		if (startBackground(g, vecDraw, mRECT))
		{
			// Background Circles
			
			vecDraw.startShadow(mOutlineSD, mRECT);
			vecDraw.setColor(mOutlineCS);
			vecDraw.frameCircle(mCx, mCy, mR, mOutlineTK);
			vecDraw.renderShadow();
			vecDraw.setColor(mCircleFillCS);
			vecDraw.fillCircle(mCx, mCy, mR);
			
			// Label
			
			mTextLabel->Draw(vecDraw);
		}
				
		renderBackground(g, vecDraw, mRECT);
		
		// Round positions for integer parameters
		
		if (param != nullptr && param->Type() != IParam::kTypeDouble && param->Type() != IParam::kTypeNone)
			value = (param->Int() - param->GetMin()) / param->GetRange();
		else 
			value = GetValue();

		// Calculate Angles
		
		double iBaseAng = mStartAngle + mRefValue * mThrowAngle;
		double iDiffAng = ((value - mRefValue) * mThrowAngle);
		double iPntrAng = iBaseAng + iDiffAng;
		
		// Indicator Arc
		
		vecDraw.setColor(mIndicatorCS);
		vecDraw.fillArc(mCx, mCy, mR, iBaseAng, iDiffAng);
		
		// Pointer Line
		
		vecDraw.setColor(mOutlineCS);
		vecDraw.circleIntersection(mCx, mCy, iPntrAng, mR, &xIntersect, &yIntersect);
		vecDraw.line(mCx, mCy, xIntersect, yIntersect, mPointerTK);
		
		// Pointer
		
		vecDraw.startShadow(mPointerSD, mRECT);
		vecDraw.setColor(mPointerFillCS);
		vecDraw.fillCPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle);
		vecDraw.setColor(mPointerOutlineCS);
		vecDraw.frameCPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle, mPointerOutlineTK);
		vecDraw.renderShadow();
        
		if (IsDisabled())
		{
			// Inactive Overlay
			
			vecDraw.setColor(mInactiveOverlayCS);
            vecDraw.fillCPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle);
		}
		else
        {
            if (!(mDrawValOnlyOnMO && !mMouseOver))
                mTextParam->Draw(vecDraw);
        }
	}
	
	void setThrow(double refValue, double startAngle, double throwAngle)
	{
		mRefValue = refValue;
		mStartAngle = startAngle;
		mThrowAngle = throwAngle;
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// HISSTools Button //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// On/Off button with text on or off the handle

// FIX - Momentary action!!

class HISSTools_Button: public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
	
private:
	
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
    HISSTools_Color_Spec *mHandleLabelOffCS;
	HISSTools_Color_Spec *mBackgroundLabelCS;
	HISSTools_Color_Spec *mInactiveOverlayCS;
	
	// Label Mode
	
	bool mLabelMode;
    
protected:
    
    // Text
    
    WDL_String mDisplayName;
	
public:
	
	// Constructor
	
	HISSTools_Button(int paramIdx, double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char *name = "")
	: IControl(IRECT(), paramIdx), HISSTools_Control_Layers()
	{
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
        mHandleLabelOffCS = designScheme->getColorSpec("ButtonHandleLabelOff", type);
        mHandleLabelOffCS = mHandleLabelOffCS ? mHandleLabelOffCS : mHandleLabelCS;
		mOutlineCS = designScheme->getColorSpec("ButtonOutline", type);
		mBackgroundLabelCS = designScheme->getColorSpec("ButtonBackgroundLabel", type);
		mInactiveOverlayCS = designScheme->getColorSpec("ButtonInactiveOverlay", type);
		
		// Calculate Areas (including shadows and thicknesses)
		
		HISSTools_Bounds handleBounds(mX, mY, mLabelMode ? mH : mW, mH);
		HISSTools_Bounds fullBounds(mX, mY, mW, mH);
		
		handleBounds.addThickness(mOutlineTK);
		
		fullBounds = mShadow->getBlurBounds(handleBounds);
		fullBounds.include(fullBounds);
		
		mRECT = fullBounds;
		SetTargetRECT(handleBounds);
        
        if (name)
            mDisplayName.Set(name);
        
		mDblAsSingleClick = true;
	}
	
public:
	
    void OnInit() override
    {
        if (!mDisplayName.GetLength() && GetParam() != nullptr)
            mDisplayName.Set(GetParam()->GetName());
    }
    
	// Mousing Functions
	
	void OnMouseDown(float x, float y, const IMouseMod& pMod) override
	{
		SetValue(GetValue() ? 0 : 1.0);
		SetDirty();
	}	

	// Draw
	
	void Draw(IGraphics& g) override
	{
        HISSTools_VecLib vecDraw(g);
        
		// FIX - Support Label Colour States / Outline Color States? - Multiple States?
        
		// Button Rectangle
		
		vecDraw.startShadow(mShadow, mRECT);
		vecDraw.setColor(GetValue() > 0.5 ? mOnCS : mOffCS);
		vecDraw.fillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
		vecDraw.setColor(mOutlineCS);
		vecDraw.frameRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness, mOutlineTK);
		vecDraw.renderShadow();
		
        vecDraw.setColor(mLabelMode ? mBackgroundLabelCS : GetValue() > 0.5 ? mHandleLabelCS : mHandleLabelOffCS);
		vecDraw.text(mTextStyle, mDisplayName.Get(), mLabelMode ? mX + mH + mTextPad : mX, mY, mLabelMode ? mW - (mH + mTextPad) : mW, mH, mLabelMode ?  kHAlignLeft : kHAlignCenter);

		// Inactive
		
		if (IsDisabled())
		{
			// Inactive Overlay
			
			vecDraw.setColor(mInactiveOverlayCS);
			vecDraw.fillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
		}
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// HISSTools Switch//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// Switch - multi state control with a number of vertical or horizontal positions

class HISSTools_Switch : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
	
private:

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
	
	HISSTools_Switch(int paramIdx, double x, double y, double w, double h, int nStates = 2, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(IRECT(), paramIdx), HISSTools_Control_Layers()
	{
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
		
		mRECT = fullBounds;
		SetTargetRECT(boxBounds);
        
        SetMouseOverWhenDisabled(true);
        SetMouseEventsWhenDisabled(true);
	}
	
public:
	
	// Mousing Functions
	
	void OnMouseDown(float x, float y, const IMouseMod& pMod) override
	{
		OnMouseDrag(x, y, 0, 0, pMod);
	}
	
	void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override
	{
        // FIX - retina support for position data!
        
		if (mW > mH)
			SetValue(round(std::max(0.0, std::min(1.0, ((x - mX) / mW))) * (mNStates - 1)) / (mNStates - 1));
		else
			SetValue(round(std::max(0.0, std::min(1.0, ((y - mY) / mH))) * (mNStates - 1)) / (mNStates - 1));
		
		SetDirty();
	}
	
	// Draw
	
	void Draw(IGraphics& g) override
	{
        HISSTools_VecLib vecDraw(g);

		// Calculate position (according to orientation)
		
		double xPos, yPos;
		
		if (mW > mH)
		{
			xPos = mX + (mW - mS) * GetValue();
			yPos = mY;
		}
		else 
		{
			xPos = mX;
			yPos = mY + (mH - mS) * GetValue();
		}

		// Background
		
		if (startBackground(g, vecDraw, mRECT))
		{
			// Background Rectangle
		
			vecDraw.setColor(mBoxFillCS);
			vecDraw.fillRoundRect(mX, mY, mW, mH, mRoundness);
			vecDraw.setColor(mBoxOutlineCS);
			vecDraw.frameRoundRect(mX, mY, mW, mH, mRoundness, mBoxOutlineTK);
		}
		
		renderBackground(g, vecDraw, mRECT);
		
		// Handle
		
		vecDraw.setColor(mHandleFillCS);
		vecDraw.startShadow(mShadow, mRECT);
		vecDraw.fillRoundRect(xPos, yPos, mS, mS, mRoundness);
		vecDraw.setColor(mHandleOutlineCS);
		vecDraw.frameRoundRect(xPos, yPos, mS, mS, mRoundness, mHandleTK);
		vecDraw.renderShadow();
		
		// Fix - Labels (Control Name and Value)
		// Label
		
		//vecDraw.setColor(textColor);
		//vecDraw.text(&txt, "Load", mRECT.L, mRECT.R, mRECT.T, mRECT.B);
		
		// Inactive
		
		if (IsDisabled())
		{
			// Inactive Overlay
			
			vecDraw.setColor(mInactiveOverlayCS);
			vecDraw.fillRoundRect(mX, mY, mW, mH, mRoundness);
		}
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// HISSTools Matrix /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


class HISSTools_Matrix : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
	
private:
	
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
	
    // Mousing Info
    
    int mXPos;
    int mYPos;
    	
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
	
	virtual void reportToPlug(int xPos, int yPos, const IMouseMod& mod, MousingAction action, float wheel = 0.f) {}
	
public:
	
	// Constructor and Destructor
	
	HISSTools_Matrix(int paramIdx, double x, double y, int xDim, int yDim, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, HISSTools_Design_Scheme *stateScheme = 0)
	: IControl(IRECT(), paramIdx), HISSTools_Control_Layers()
	{
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
		
		mRECT = (fullBoxBounds);
		SetTargetRECT(boxBoundsOutline);
	}
	
	~HISSTools_Matrix()
	{
		delete[] mStates;
	}
	
    int getXPos() const { return mXPos; }
    int getYPos() const { return mYPos; }
    
    // Mousing Functions
	 
	bool OnMousing(float x, float y, const IMouseMod& mod, MousingAction action, float wheel = 0.f)
	{
		if (coordsToIndices(x, y, &mXPos, &mYPos))
		{
			reportToPlug(mXPos, mYPos, mod, action, wheel);
			
			return true;
		}
		
		return false;
	}
	
	void OnMouseDown(float x, float y, const IMouseMod& mod) override
	{
		OnMousing(x, y, mod, kMouseDown);
	}
	
	void OnMouseUp(float x, float y, const IMouseMod& mod) override
	{
		OnMousing(x, y, mod, kMouseUp);
	}
	
	void OnMouseDblClick(float x, float y, const IMouseMod& mod) override
	{
		OnMousing(x, y, mod, kMouseDblClick);
	}
	
	void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
	{
		OnMousing(x, y, mod, kMouseDrag);
	}
	
	void OnMouseWheel(float x, float y, const IMouseMod& pMod, float d) override
	{
		OnMousing(x, y, pMod, kMouseWheel, d);
	}
	
	void OnMouseOver(float x, float y, const IMouseMod& mod) override
	{
		if (OnMousing(x, y, mod, kMouseOver) == false)
			OnMouseOut();
		else
			SetDirty(false);
	}
	
	virtual void OnMouseOut() override
	{
        mXPos = -1;
        mXPos = -1;
        
		reportToPlug(-1, -1, IMouseMod(), kMouseOut);
		
		SetDirty(false);
	}
	
	// Draw
	
	void Draw(IGraphics& g) override
	{
        HISSTools_VecLib vecDraw(g);
        
		// Background (shadow boxes)
		
		if (startBackground(g, vecDraw, mRECT))
		{		
			vecDraw.startShadow(mShadow, mRECT);
			vecDraw.setColor(mOutlineCS);
			
			for (int i = 0; i < mXDim; i++)
			{
				double sx = mX + i * mUnit;
				
				for (int j = 0; j < mYDim; j++) 
				{
					double sy = mY + j * mUnit;
					
					vecDraw.frameRoundRect(sx, sy, mS, mS, mRoundness, mHandleEmptyOutlineTK);
				}
			}
			
			vecDraw.renderShadow(false);
		}
		
		renderBackground(g, vecDraw, mRECT);

		// Matrix fills
		
		for (int i = 0; i < mXDim; i++)
		{
			double sx = mX + i * mUnit;
			
			for (int j = 0; j < mYDim; j++) 
			{
				double sy = mY + j * mUnit;
								
				vecDraw.setColor(mStateCS[mStates[j * mXDim + i] % mNStates]);
				vecDraw.fillRoundRect(sx, sy, mS, mS, mRoundness);
				vecDraw.setColor(mOutlineCS);
				vecDraw.frameRoundRect(sx, sy, mS, mS, mRoundness, mHandleFilledOutlineTK);
			}
		}

		if (mXHilite > -1 && mYHilite > -1)
		{
			vecDraw.setColor(mHiliteCS);
			vecDraw.frameRoundRect(mX + mXPos * mUnit, mY + mYPos * mUnit, mS, mS, mRoundness, mHiliteTK);
		}
	}
	
	void SetState(int x, int y, char state)
	{
		if (x >= 0 && x < mXDim && y >= 0  && y < mYDim)
        {
            if (state != mStates[y * mXDim + x])
            {
                mStates[y * mXDim + x] = state;
                SetDirty(false);
            }
        }
	}
	
	unsigned char GetState(int x, int y)
	{
		if (x >= 0 && x < mXDim && y >= 0  && y < mYDim)
			return mStates[y * mXDim + x];
		
		return 0;
	}
	
	void SetHilite(bool on)
	{
		if (on)
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
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// HISSTools TextBlock ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// A Text Block (Static or otherwise)

class HISSTools_TextBlock : public HISSTools_Text_Helper_Block, public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
	
public:
	
	// Constructors
	
	HISSTools_TextBlock(double x, double y, double w, double h, const char* str = "", HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter, const char* type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: HISSTools_Text_Helper_Block(x, y, w, h, hAlign, vAlign, "TextBlock", type, designScheme), IControl(IRECT()), HISSTools_Control_Layers()
	{
		setText(str);
		mRECT = bounds();
	}
	
	void Draw(IGraphics& g)
	{
        HISSTools_VecLib vecDraw(g);
        HISSTools_Text_Helper_Block::Draw(vecDraw);
	}
	
	void setText(const char *str)
	{
		HISSTools_Text_Helper_Block::setText(str);
		SetDirty();
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// HISSTools Panel ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


// Panel with or without outline and with dropshadow

class HISSTools_Panel: public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
	
private:

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
	
	HISSTools_Panel(double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(IRECT()), HISSTools_Control_Layers()
	{
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
		
		mRECT = fullBounds;
	}
	
public:
	
	// Draw
	
	void Draw(IGraphics& g)
	{
        HISSTools_VecLib vecDraw(g);

		if (startBackground(g, vecDraw, mRECT))
		{
			vecDraw.startShadow(mShadow, mRECT);
			vecDraw.setColor(mPanelCS);
			vecDraw.fillRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR);
			if (mDrawOutline)
			{				
				vecDraw.setColor(mOutlineCS);
				vecDraw.frameRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR, mOutlineTK);
			}
			vecDraw.renderShadow();
		}
		
		// Background
		
		renderBackground(g, vecDraw, mRECT);
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// HISSTools VU ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


class HISSTools_Progress : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
	
private:
    
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
	
	HISSTools_Progress(double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
	: IControl(IRECT()), HISSTools_Control_Layers(), mX(x), mY(y), mW(w), mH(h)
	{
		// Get Appearance
		
		mOutlineTK = designScheme->getDimension("ProgressOutline", type);
						
		mBackgroundCS = designScheme->getColorSpec("ProgressBackground", type);
		mOutlineCS = designScheme->getColorSpec("ProgressOutline", type);
		mProgressCS = designScheme->getColorSpec("Progress", type);
		
		mShadow  = designScheme->getShadow("Progress", type);
		
		// Area
		
		// FIX - Hack
		
		mRECT = HISSTools_Bounds(x, y, w, h);
	}
	
	~HISSTools_Progress() {}
	
	
	void Draw(IGraphics& g)
	{
        HISSTools_VecLib vecDraw(g);

		vecDraw.setColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);
        
		// Parameters
				
		if (startBackground(g, vecDraw, mRECT))
		{
			vecDraw.setColor(mBackgroundCS);
			vecDraw.fillRect(mX, mY, mW, mH);
			
			// Frame Rectangle
			
			vecDraw.startShadow(mShadow, mRECT);
			vecDraw.setColor(mOutlineCS);
			vecDraw.frameRect(mX, mY, mW, mH, mOutlineTK);
			vecDraw.renderShadow();
		}
		
		renderBackground(g, vecDraw, mRECT);
		
		// Progress Rectangles
		
		vecDraw.forceGradientBox(mX, mY, mX + mW, mY + mH);
		
		// Progress
		
		vecDraw.setColor(mProgressCS);

		if (mW < mH)
			vecDraw.fillRect(mX, mY + mH * (1 - GetValue()), mW, mH * GetValue());
		else 
			vecDraw.fillRect(mX, mY, mW * GetValue(), mH);
			
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
		
		if (startBackground(g, vecDraw, mRECT))
		{
			vecDraw.setColor(mBackgroundCS);
			vecDraw.fillRect(mX, mY, mW, mH);
		
			// Frame Rectangle
		
			vecDraw.startShadow(mShadow, mRECT);
			vecDraw.setColor(mOutlineCS);
			vecDraw.frameRect(mX, mY, mW, mH, mOutlineTK);
			vecDraw.renderShadow();
		}
		
		renderBackground(g, vecDraw, mRECT);
		
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
