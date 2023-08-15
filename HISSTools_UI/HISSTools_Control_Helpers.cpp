
#include "HISSTools_Controls.hpp"
#include "IControl.h"

bool HISSTools_Control_Layers::StartBackground(HISSTools_VecLib& vl, IRECT area)
{
    if (sNoCaching)
        return true;
    
    if (!vl.checkGroup(mBackground))
    {
        vl.startGroup(area);
        return true;
    }
    
    return false;
}

void HISSTools_Control_Layers::RenderBackground(HISSTools_VecLib& vl, IRECT area)
{
    if (!sNoCaching)
    {
        if (!vl.checkGroup(mBackground))
            mBackground = vl.endGroup();
        if (mBackground)
            vl.renderGroup(mBackground);
    }
}

void HISSTools_Control_Layers::RedrawBackground()
{
    if (mBackground)
        mBackground->Invalidate();
}

///////////////////////////////

HISSTools_Text_Helper_Block::HISSTools_Text_Helper_Block(double x, double y, double w, double h,  HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
{
    // Dimensions
    
    ResizeText(x, y, w, h);
    
    // Get Appearance
    
    mTextTS = scheme->getTextStyle(name, type);
    mTextCS = scheme->getColorSpec(name, type);
    mTextSD = scheme->getShadow(name, type);
    mHAlign = hAlign;
    mVAlign = vAlign;
}

void HISSTools_Text_Helper_Block::Draw(HISSTools_VecLib& vl)
{
    if (mTextSD)
        vl.startShadow(mTextSD, Bounds());
    
    vl.setColor(mTextCS);
    vl.text(mTextTS, mStr.Get(), mX, mY, mW, mH, mHAlign, mVAlign);
    
    if (mTextSD)
        vl.renderShadow();
}

void HISSTools_Text_Helper_Block::ResizeText(double x, double y, double w, double h)
{
    mX = w > 0 ? x : x + w;
    mY = h > 0 ? y : y + h;
    mW = fabs(w);
    mH = fabs(h);
}

void HISSTools_Text_Helper_Block::SetTextColor(HISSTools_Color_Spec *colorSpec)
{
    mTextCS = colorSpec;
}

void HISSTools_Text_Helper_Block::SetText(const char *str)
{
    mStr.Set(str);
}

HISSTools_Bounds HISSTools_Text_Helper_Block::Bounds()
{
    HISSTools_Bounds boxBounds(mX, mY, mW, mH);
    
    if (mTextSD)
        boxBounds.include(mTextSD->getBlurBounds(boxBounds));
    
    return boxBounds;
}

///////////////////////////////

HISSTools_Text_Helper_Panel::HISSTools_Text_Helper_Panel(double x, double y, double w, double h, double wPad, double hPad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
: HISSTools_Text_Helper_Block(hAlign, vAlign, name, type, scheme)
{
    // FIX - padding
    
    char concatenatedName[256];
    
    // Dimensions
    
    mLPad = wPad;
    mRPad = wPad;
    mHPad = hPad;
    Resize(x, y, w, h);
    
    // Get Appearance
    
    sprintf(concatenatedName, "%sPanel", name);
    
    mPanelFillCS = scheme->getColorSpec(concatenatedName, type);
    mPanelSD = scheme->getShadow(concatenatedName, type);
    
    sprintf(concatenatedName, "%sPanelOutline", name);
    
    mPanelOutlineCS = scheme->getColorSpec(concatenatedName, type);
    mPanelOutlineTK = scheme->getDimension(concatenatedName, type);
    
    sprintf(concatenatedName, "%sPanelRoundness", name);
    
    double roundness = scheme->getDimension(concatenatedName, type);
    roundness = ((roundness * 2) > mW) ? mW / 2.: roundness;
    roundness = ((roundness * 2) > mH) ? mH / 2.: roundness;
    mPanelRoundness = roundness < 0 ? mH / 2 : roundness;
}

void HISSTools_Text_Helper_Panel::Draw(HISSTools_VecLib& vl, bool drawText)
{
    if (DoDrawPanel())
    {
        if (mPanelSD)
            vl.startShadow(mPanelSD, Bounds());
        
        vl.setColor(mPanelFillCS);
        vl.fillRoundRect(mX, mY, mW, mH, mPanelRoundness);
        
        if (DoDrawOutline())
        {
            vl.setColor(mPanelOutlineCS);
            vl.frameRoundRect(mX, mY, mW, mH, mPanelRoundness, mPanelOutlineTK);
        }
        
        if (mPanelSD)
            vl.renderShadow();
    }
    
    if (drawText)
        HISSTools_Text_Helper_Block::Draw(vl);
    
}

void HISSTools_Text_Helper_Panel::ChangePadding(double lPad, double rPad, double hPad)
{
    mLPad = lPad;
    mRPad = rPad;
    mHPad = hPad;
    
    Resize(mX, mY, mW, mH);
}

void HISSTools_Text_Helper_Panel::ChangePadding(double wPad, double hPad)
{
    ChangePadding(wPad, wPad, hPad);
}

void HISSTools_Text_Helper_Panel::Resize(double x, double y, double w, double h)
{
    mX = w > 0 ? x : x + w;
    mY = h > 0 ? y : y + h;
    mW = fabs(w);
    mH = fabs(h);
    
    double lPad = mLPad > mW / 2.0 ? mW / 2.0 : mLPad;
    double rPad = mRPad > mW / 2.0 ? mW / 2.0 : mRPad;
    double hPad = mHPad > mH / 2.0 ? mH / 2.0 : mHPad;
    
    ResizeText(x + lPad, y + hPad, w - (lPad + rPad), h - (2.0 * hPad));
}

void HISSTools_Text_Helper_Panel::SetPanelColor(HISSTools_Color_Spec *colorSpec)
{
    mPanelFillCS = colorSpec;
}

void HISSTools_Text_Helper_Panel::SetOutlineColor(HISSTools_Color_Spec *colorSpec)
{
    mPanelOutlineCS = colorSpec;
}

HISSTools_Bounds HISSTools_Text_Helper_Panel::Bounds()
{
    HISSTools_Bounds boxBounds(mX, mY, mW, mH);
    
    boxBounds.addThickness(mPanelOutlineTK);
    
    if (mPanelSD)
        boxBounds.include(mPanelSD->getBlurBounds(boxBounds));
    
    boxBounds.include(HISSTools_Text_Helper_Block::Bounds());
    
    return boxBounds;
}

bool HISSTools_Text_Helper_Panel::DoDrawOutline()
{
    return (mPanelOutlineTK && mPanelOutlineCS != nullptr);
}

bool HISSTools_Text_Helper_Panel::DoDrawPanel()
{
    return ((mPanelFillCS != nullptr) || DoDrawOutline());
}

///////////////////////////////


HISSTools_Text_Helper_Param::HISSTools_Text_Helper_Param(iplug::igraphics::IControl *control, double x, double y, double w, double h, double pad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
: HISSTools_Text_Helper_Panel(x, y, w, h, 0, 0, hAlign, vAlign, name, type, scheme), mInEdit(false)
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
    drawMenuTriangle = scheme->getFlag(concatenatedName, type);
    sprintf(concatenatedName, "%sPromptRatio", name);
    mPromptRatio = scheme->getDimension(concatenatedName, type);
    sprintf(concatenatedName, "%sHilite", name);
    mTextHiliteCS = scheme->getColorSpec(concatenatedName, type);
    sprintf(concatenatedName, "%sPanelHilite", name);
    mPanelHiliteCS = scheme->getColorSpec(concatenatedName, type);
    sprintf(concatenatedName, "%sPanelOutlineHilite", name);
    mOutlineHiliteCS = scheme->getColorSpec(concatenatedName, type);
    mShowUnits = scheme->getFlag("ShowUnits", type);
    
    // FIX - Padding! (see Draw also)
    
    double wPad = 9;
    double hPad = 0;
    
    //ChangePadding(wPad, hPad);
    ChangePadding(0, hPad);
    
    if (drawMenuTriangle)
    {
        mDrawTriangle = true;
        sprintf(concatenatedName, "%sDrawSeparator", name);
        mDrawSeparator = scheme->getFlag(concatenatedName, type);
        sprintf(concatenatedName, "%sFlipTriangle", name);
        mMenuFlipTriangle = scheme->getFlag(concatenatedName, type);
        
        double menuTriangleWidthRatio = scheme->getDimension("MenuTriangleWidthRatio", type);
        double menuTriangleHeightRatio = scheme->getDimension("MenuTriangleHeightRatio", type);
        double menuTriangleWidth = textHeight * menuTriangleWidthRatio;
        double menuTriangleHeight = textHeight * menuTriangleHeightRatio;
        
        double separatorWidth = wPad * 2.0 + menuTriangleWidth + RoundnessCompensate(menuTriangleHeight);
        mSeparatorX = mX + (mMenuFlipTriangle ? separatorWidth : mW - separatorWidth);
        
        mMenuTriangleTop = mY + (mH - menuTriangleHeight) / 2.0;
        mMenuTriangleBtm = mMenuTriangleTop + menuTriangleHeight;
        mMenuTriangleL = mSeparatorX + (mMenuFlipTriangle ? -(menuTriangleWidth + wPad) : wPad);
        mMenuTriangleR = mMenuTriangleL + menuTriangleWidth;
    }
    else
    {
        mDrawTriangle = false;
        mDrawSeparator = false;
        mMenuFlipTriangle = false;
        mSeparatorX = mX + mW;
        
        mMenuTriangleTop = 0;
        mMenuTriangleBtm = 0;
        mMenuTriangleL = 0;
        mMenuTriangleR = 0;
    }
}

bool HISSTools_Text_Helper_Param::menuParam()
{
    if (mControl->GetParam() == nullptr)
        return false;
    
    if (mControl->GetParam()->NDisplayTexts())
        return true;
    
    return false;
}

void HISSTools_Text_Helper_Param::FinishEdit()
{
    mInEdit = false;
}

void HISSTools_Text_Helper_Param::PromptUserInput()
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
    
    if (mDrawSeparator && menuParam() && DoDrawOutline())
    {
        promptTop = mY + mH;
        promptLeft = mMenuFlipTriangle ? mX : mSeparatorX;
    }
    
    entryBounds = HISSTools_Bounds(promptLeft, promptTop, promptWidth, promptHeight);
    
    SetControlText();
    mControl->DisablePrompt(false);
    mControl->PromptUserInput(entryBounds);
}

bool HISSTools_Text_Helper_Param::PromptUserInput(float x, float y)
{
    if (Bounds().Contains(x, y))
    {
        if (!mDrawSeparator || (mMenuFlipTriangle == (x < mSeparatorX)))
        {
            PromptUserInput();
            return true;
        }
    }
    
    return false;
}

void HISSTools_Text_Helper_Param::Hilite(bool on)
{
    using Base = HISSTools_Text_Helper_Panel;
    
    Base::mTextCS = on && mTextHiliteCS ? mTextHiliteCS : mTextCS;
    Base::mPanelFillCS = on && mPanelHiliteCS ? mPanelHiliteCS : mPanelFillCS;
    Base::mPanelOutlineCS = on && mOutlineHiliteCS ? mOutlineHiliteCS : mPanelOutlineCS;
}

void HISSTools_Text_Helper_Param::Draw(HISSTools_VecLib& vl)
{
    if (mDrawTriangle && menuParam())
    {
        // FIX - Padding! (see constructor also)
        
        double wPad = 9;
        double hPad = 0;
        double separatorWidth = mMenuFlipTriangle ? (mSeparatorX - mX) : mW + mX - mSeparatorX;
        
        ChangePadding(wPad + (mMenuFlipTriangle ? separatorWidth : 0), wPad + (mMenuFlipTriangle ? 0 : separatorWidth), hPad);
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
    
    SetText(str.Get());
    HISSTools_Text_Helper_Panel::Draw(vl, !mInEdit);
    
    // Menu Separator / Triangle
    
    if (mDrawTriangle && menuParam())
    {
        // Separator
        
        if (mDrawSeparator && DoDrawOutline())
        {
            vl.setColor(mPanelOutlineCS);
            vl.line(mSeparatorX, mY, mSeparatorX, mY + mH, mPanelOutlineTK);
        }
        
        // Triangle
        
        if (mTextSD)
            vl.startShadow(mTextSD, IRECT(mX, mY, mX + mW, mY + mH));
        
        vl.setColor(mTextCS);
        vl.fillTriangle(mMenuTriangleL, mMenuTriangleTop, (mMenuTriangleL + mMenuTriangleR) / 2.0, mMenuTriangleBtm, mMenuTriangleR, mMenuTriangleTop);
        
        if (mTextSD)
            vl.renderShadow();
    }
}

void HISSTools_Text_Helper_Param::SetControlText()
{
    IText text;
    
    strcpy(text.mFont, mTextTS->mFont);
    text.mSize = mTextTS->mSize;
    
    switch (mHAlign)
    {
        case kHAlignLeft:       text.mAlign = EAlign::Near;     break;
        case kHAlignCenter:     text.mAlign = EAlign::Center;   break;
        case kHAlignRight:      text.mAlign = EAlign::Far;      break;
    }
    
    mControl->SetText(text);
}

double HISSTools_Text_Helper_Param::RoundnessCompensate(double menuTriangleHeight)
{
    if (menuTriangleHeight > mH)
        return mPanelRoundness;
    if (menuTriangleHeight <= mH - (2.0 * mPanelRoundness))
        return 0;
    
    return mPanelRoundness - sqrt(mPanelRoundness * mPanelRoundness - (0.25 * menuTriangleHeight * menuTriangleHeight) - (0.5 * mH) - mPanelRoundness);
}

// **** Controls **** //

// HISSTools_TextBlock
// A Text Block 

HISSTools_TextBlock::HISSTools_TextBlock(double x, double y, double w, double h, const char* str, HTextAlign hAlign, VTextAlign vAlign, const char* type, HISSTools_Design_Scheme *scheme)
: HISSTools_Text_Helper_Block(x, y, w, h, hAlign, vAlign, "TextBlock", type, scheme), IControl(IRECT()), HISSTools_Control_Layers()
{
    SetText(str);
    mRECT = Bounds();
}

void HISSTools_TextBlock::Draw(IGraphics& g)
{
    HISSTools_VecLib vl(g);
    HISSTools_Text_Helper_Block::Draw(vl);
}

void HISSTools_TextBlock::SetText(const char *str)
{
    HISSTools_Text_Helper_Block::SetText(str);
    SetDirty();
}

// HISSTools_Panel
// Panel with or without outline and with dropshadow

double HISSTools_Panel::GetRoundness(const char *name, const char *type, HISSTools_Design_Scheme *scheme)
{
    double roundness = scheme->getDimension(name, type);
    
    // FIX - Why height?
    
    return roundness < 0 ? mH / 2 : roundness;
}

// Constructor

HISSTools_Panel::HISSTools_Panel(double x, double y, double w, double h, const char *type, HISSTools_Design_Scheme *scheme)
: IControl(IRECT()), HISSTools_Control_Layers()
{
    // Dimenasions
    
    mX = x;
    mY = y;
    mW = w <= 0 ? scheme->getDimension("PanelWidth", type) : w;
    mH = h <= 0 ? scheme->getDimension("PanelHeight", type) : h;
    
    mRoundnessTL = GetRoundness("PanelRoundnessTL", type, scheme);
    mRoundnessTR = GetRoundness("PanelRoundnessTR", type, scheme);
    mRoundnessBL = GetRoundness("PanelRoundnessBL", type, scheme);
    mRoundnessBR = GetRoundness("PanelRoundnessBR", type, scheme);
    
    // Get Appearance
    
    mOutlineTK = scheme->getDimension("PanelOutline", type);
    
    mShadow = scheme->getShadow("Panel", type);
    
    mDrawOutline = scheme->getFlag("PanelDrawOutline", type);
    
    mPanelCS = scheme->getColorSpec("PanelFill", type);
    mOutlineCS = scheme->getColorSpec("PanelOutline", type);
    
    // Calculate Areas (including shadows and thicknesses)
    
    HISSTools_Bounds fullBounds(mX, mY, mW, mH);
    
    if (mDrawOutline)
        fullBounds.addThickness(mOutlineTK);
    
    fullBounds = mShadow->getBlurBounds(fullBounds);
    
    mRECT = fullBounds;
}

// Draw

void HISSTools_Panel::Draw(IGraphics& g)
{
    HISSTools_VecLib vl(g);
    
    if (StartBackground(vl, mRECT))
    {
        vl.startShadow(mShadow, mRECT);
        vl.setColor(mPanelCS);
        vl.fillRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR);
        if (mDrawOutline)
        {
            vl.setColor(mOutlineCS);
            vl.frameRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR, mOutlineTK);
        }
        vl.renderShadow();
    }
    
    // Background
    
    RenderBackground(vl, mRECT);
}

// HISSTools_Button
// On/Off button with text on or off the handle
// FIX - Momentary action and extensibility!!

// Constructor
    
HISSTools_Button::HISSTools_Button(int paramIdx, double x, double y, double w, double h, const char *type, HISSTools_Design_Scheme *scheme, const char *name)
: IControl(IRECT(), paramIdx), HISSTools_Control_Layers()
{
    // Dimensions
    
    mX = x;
    mY = y;
    mW = w <= 0 ? scheme->getDimension("ButtonWidth", type) : w;
    mH = h <= 0 ? scheme->getDimension("ButtonHeight", type) : h;
    
    double roundness = scheme->getDimension("ButtonRoundness", type);
    mRoundness = roundness < 0 ? mH / 2 : roundness;
    
    mTextPad = scheme->getDimension("ButtonTextPad", type);
    
    // Label Mode
    
    mLabelMode = scheme->getFlag("ButtonLabelMode", type);
    
    // Get Appearance
    
    mOutlineTK = scheme->getDimension("ButtonOutline", type);
    
    mShadow = scheme->getShadow("Button", type);
    
    mTextStyle = scheme->getTextStyle("Button", type);
    
    mOnCS = scheme->getColorSpec("ButtonHandleOn", type);
    mOffCS = scheme->getColorSpec("ButtonHandleOff", type);
    mHandleLabelCS = scheme->getColorSpec("ButtonHandleLabel", type);
    mHandleLabelOffCS = scheme->getColorSpec("ButtonHandleLabelOff", type);
    mHandleLabelOffCS = mHandleLabelOffCS ? mHandleLabelOffCS : mHandleLabelCS;
    mOutlineCS = scheme->getColorSpec("ButtonOutline", type);
    mBackgroundLabelCS = scheme->getColorSpec("ButtonBackgroundLabel", type);
    mInactiveOverlayCS = scheme->getColorSpec("ButtonInactiveOverlay", type);
    
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

void HISSTools_Button::OnInit()
{
    if (!mDisplayName.GetLength() && GetParam() != nullptr)
        mDisplayName.Set(GetParam()->GetName());
}

// Mousing Functions

void HISSTools_Button::OnMouseDown(float x, float y, const IMouseMod& pMod)
{
    SetValue(GetValue() ? 0 : 1.0);
    SetDirty();
}

// Draw

void HISSTools_Button::Draw(IGraphics& g)
{
    HISSTools_VecLib vl(g);
    
    // FIX - Support Label Colour States / Outline Color States? - Multiple States?
    
    // Button Rectangle
    
    vl.startShadow(mShadow, mRECT);
    vl.setColor(GetValue() > 0.5 ? mOnCS : mOffCS);
    vl.fillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
    vl.setColor(mOutlineCS);
    vl.frameRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness, mOutlineTK);
    vl.renderShadow();
    
    vl.setColor(mLabelMode ? mBackgroundLabelCS : GetValue() > 0.5 ? mHandleLabelCS : mHandleLabelOffCS);
    vl.text(mTextStyle, mDisplayName.Get(), mLabelMode ? mX + mH + mTextPad : mX, mY, mLabelMode ? mW - (mH + mTextPad) : mW, mH, mLabelMode ?  kHAlignLeft : kHAlignCenter);
    
    // Inactive
    
    if (IsDisabled())
    {
        // Inactive Overlay
        
        vl.setColor(mInactiveOverlayCS);
        vl.fillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
    }
}

// HISSTools_Tabs
// An abstract class to control tabs in plug-ins

void HISSTools_Tabs::init()
{
  mParam = mTabControl->GetParam();
  mMaxTabNumber = mParam != nullptr ? round(mParam->GetRange()) : 0;
  tabSetDirty(false);
}

void HISSTools_Tabs::attachControl(iplug::igraphics::IControl *control, int tabNumber)
{
  // N.B. - mMaxTabNumber is one lass than the number of actual tabs (zero referenced)

  mItems.push_back(TabItem(control, tabNumber));
  updateItems();
}

void HISSTools_Tabs::tabHide(bool hide)
{
  mTabControl->IControl::Hide(hide);
  updateItems();
}

void HISSTools_Tabs::tabSetDirty(bool pushParamToPlug)
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

void HISSTools_Tabs::updateItems()
{
  bool tabObjectHidden = mTabControl->IsHidden();

  for (auto it = mItems.begin(); it != mItems.end(); it++)
    it->mControl->Hide(tabObjectHidden || clipTabNumber(it->mTabNumber) != mCurrentTabNumber);
}

int HISSTools_Tabs::clipTabNumber(int tabNumber)
{
  tabNumber = tabNumber < 0 ? 0 : tabNumber;
  tabNumber = tabNumber > mMaxTabNumber ? mMaxTabNumber : tabNumber;

  return tabNumber;
}