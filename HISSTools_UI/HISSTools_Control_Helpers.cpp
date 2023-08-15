
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

// HISSTools_Value
// Multipurpose numerical / menu control
// FIX - do your own mousing later...

HISSTools_Value::HISSTools_Value(int paramIdx, double x, double y, double w, double h, const char *type, HISSTools_Design_Scheme *designScheme, const char* name)
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

  HISSTools_Bounds fullBoxBounds = mTextParam->Bounds();
  if (mTextLabel)
    fullBoxBounds.include(mTextLabel->Bounds());
  mRECT = fullBoxBounds;

  SetMouseOverWhenDisabled(true);
  SetMouseEventsWhenDisabled(true);

  if (name)
    mDisplayName.Set(name);
}

HISSTools_Value::~HISSTools_Value()
{
  delete mTextParam;
  delete mTextLabel;
}

void HISSTools_Value::OnInit()
{
  if (mTextLabel)
  {
    if (mDisplayName.GetLength())
      mTextLabel->SetText(mDisplayName.Get());
    else
      mTextLabel->SetText((GetParam() != nullptr) ? GetParam()->GetName() : "");
  }
}

void HISSTools_Value::OnMouseDown(float x, float y, const IMouseMod& pMod)
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
    if (mTextParam->PromptUserInput(x, y) == false && GetParam())
    {
      double value = round(GetValue() * (GetParam()->GetRange()) + 1) / (GetParam()->GetRange());
      SetValue(value > 1.0 ? 0 : value);
    }
  }
  else
    mTextParam->Hilite(true);

  mDrag = false;
  SetDirty();
}

void HISSTools_Value::OnMouseUp(float x, float y, const IMouseMod& pMod)
{
  if (mDrag == false)
  {
    if (mTextParam->menuParam() == false)
      mTextParam->PromptUserInput();
  }
  else
    mTextParam->Hilite(false);

  mMouseDown = false;
  SetDirty(false);
}

void HISSTools_Value::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod)
{
  mDrag = true;

  IKnobControlBase::OnMouseDrag(x, y, dX, dY, pMod);
}

void HISSTools_Value::OnMouseDblClick(float x, float y, const IMouseMod& pMod)
{
  OnMouseDown(x, y, pMod);
}

void HISSTools_Value::SetValueFromUserInput(double value, int valIdx)
{
  mDrag = false;
  mTextParam->FinishEdit();
  mTextParam->Hilite(false);
  IKnobControlBase::SetValueFromUserInput(value, valIdx);
}

void HISSTools_Value::Draw(IGraphics& g)
{
  HISSTools_VecLib vecDraw(g);

  // Label

  if (mTextLabel)
  {
    if (StartBackground(vecDraw, mRECT))
      mTextLabel->Draw(vecDraw);
    RenderBackground(vecDraw, mRECT);
  }
  mTextParam->Draw(vecDraw);
}

// HISSTools_Dial
// Multipurpose dial with auto value display
// FIX - do your own mousing later...

// Constructor

HISSTools_Dial::HISSTools_Dial(int paramIdx, double x, double y, const char *type, HISSTools_Design_Scheme *designScheme, const char* name)
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

HISSTools_Dial::~HISSTools_Dial()
{
  delete mTextLabel;
  delete mTextParam;
}

void HISSTools_Dial::OnInit()
{
  if (mDisplayName.GetLength())
    mTextLabel->SetText(mDisplayName.Get());
  else
    mTextLabel->SetText(GetParam() != nullptr ? GetParam()->GetName() : "");
}

void HISSTools_Dial::OnMouseDown(float x, float y, const IMouseMod& pMod)
{
  OnMouseOver(x, y, pMod);

  mMouseDown = true;
  mMouseDragValue = GetValue();

  if (pMod.S)
    SetValueToDefault();
  else
    GetUI()->HideMouseCursor();
}

void HISSTools_Dial::OnMouseDblClick(float x, float y, const IMouseMod& pMod)
{
  // FIX - Confirm best key combo...

  if (pMod.S)
  {
    OnMouseDown(x, y, pMod);
    return;
  }

  mTextParam->PromptUserInput();
  SetDirty(false);
}

void HISSTools_Dial::OnMouseOver(float x, float y, const IMouseMod& pMod)
{
  if (!mMouseOver)
  {
    mMouseOver = true;
    SetDirty(false);
  }
}

void HISSTools_Dial::OnMouseOut()
{
  if (mMouseOver)
  {
    mMouseOver = false;
    SetDirty(false);
  }
}

void HISSTools_Dial::SetDisabled(bool disable)
{
  bool old = mDisabled;
  mDisabled = disable;
  if (disable != old)
    SetDirty(false);
}

void HISSTools_Dial::SetValueFromUserInput(double value, int valIdx)
{
  mTextParam->FinishEdit();
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

void HISSTools_Dial::Draw(IGraphics& g)
{
  const IParam *param = GetParam();
  double value, xIntersect, yIntersect;

  HISSTools_VecLib vecDraw(g);

  // Background

  if (StartBackground(vecDraw, mRECT))
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

  RenderBackground(vecDraw, mRECT);

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

void HISSTools_Dial::setThrow(double refValue, double startAngle, double throwAngle)
{
  mRefValue = refValue;
  mStartAngle = startAngle;
  mThrowAngle = throwAngle;
}

void HISSTools_Dial::setPointerAppearance(double pointerCircRatio, double pointerTipRatio, double pointerAngle)
{
  pointerCircRatio = pointerCircRatio > 0.99 ? 0.99 : pointerCircRatio;

  pointerTipRatio = pointerTipRatio > 1.0 / pointerCircRatio ? 1.0 / pointerCircRatio : pointerTipRatio;
  pointerTipRatio = pointerTipRatio < 1.01 ? 1.01 : pointerTipRatio;

  mPointerCircRadius = mR * pointerCircRatio;
  mPointerTipRadius = mPointerCircRadius * pointerTipRatio;
  mPointerAngle = pointerAngle;
}

// HISSTools_Switch
// Multi state control with a number of vertical or horizontal positions

HISSTools_Switch::HISSTools_Switch(int paramIdx, double x, double y, double w, double h, int nStates, const char *type, HISSTools_Design_Scheme *designScheme)
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

// Mousing Functions

void HISSTools_Switch::OnMouseDown(float x, float y, const IMouseMod& pMod)
{
  OnMouseDrag(x, y, 0, 0, pMod);
}

void HISSTools_Switch::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod)
{
  // FIX - retina support for position data!

  if (mW > mH)
    SetValue(round(std::max(0.0, std::min(1.0, ((x - mX) / mW))) * (mNStates - 1)) / (mNStates - 1));
  else
    SetValue(round(std::max(0.0, std::min(1.0, ((y - mY) / mH))) * (mNStates - 1)) / (mNStates - 1));

  SetDirty();
}

// Draw

void HISSTools_Switch::Draw(IGraphics& g)
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

  if (StartBackground(vecDraw, mRECT))
  {
    // Background Rectangle

    vecDraw.setColor(mBoxFillCS);
    vecDraw.fillRoundRect(mX, mY, mW, mH, mRoundness);
    vecDraw.setColor(mBoxOutlineCS);
    vecDraw.frameRoundRect(mX, mY, mW, mH, mRoundness, mBoxOutlineTK);
  }

  RenderBackground(vecDraw, mRECT);

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

// HISSTools_Matrix
//

// Constructor and Destructor

HISSTools_Matrix::HISSTools_Matrix(int paramIdx, double x, double y, int xDim, int yDim, const char *type, HISSTools_Design_Scheme *designScheme, HISSTools_Design_Scheme *stateScheme)
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

  mRECT = fullBoxBounds;
  SetTargetRECT(boxBoundsOutline);
}

HISSTools_Matrix::~HISSTools_Matrix()
{
  delete[] mStates;
}

int HISSTools_Matrix::getXPos() const
{
  return mXPos;
}
int HISSTools_Matrix::getYPos() const
{
  return mYPos;
}

// Mousing Functions

bool HISSTools_Matrix::OnMousing(float x, float y, const IMouseMod& mod, MousingAction action, float wheel)
{
  if (coordsToIndices(x, y, &mXPos, &mYPos))
  {
    reportToPlug(mXPos, mYPos, mod, action, wheel);

    return true;
  }

  return false;
}

void HISSTools_Matrix::OnMouseDown(float x, float y, const IMouseMod& mod)
{
  OnMousing(x, y, mod, kMouseDown);
}

void HISSTools_Matrix::OnMouseUp(float x, float y, const IMouseMod& mod)
{
  OnMousing(x, y, mod, kMouseUp);
}

void HISSTools_Matrix::OnMouseDblClick(float x, float y, const IMouseMod& mod)
{
  OnMousing(x, y, mod, kMouseDblClick);
}

void HISSTools_Matrix::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod)
{
  OnMousing(x, y, mod, kMouseDrag);
}

void HISSTools_Matrix::OnMouseWheel(float x, float y, const IMouseMod& pMod, float d)
{
  OnMousing(x, y, pMod, kMouseWheel, d);
}

void HISSTools_Matrix::OnMouseOver(float x, float y, const IMouseMod& mod)
{
  if (OnMousing(x, y, mod, kMouseOver) == false)
    OnMouseOut();
  else
    SetDirty(false);
}

void HISSTools_Matrix::OnMouseOut()
{
  mXPos = -1;
  mXPos = -1;

  reportToPlug(-1, -1, IMouseMod(), kMouseOut);

  SetDirty(false);
}

// Draw

void HISSTools_Matrix::Draw(IGraphics& g)
{
  HISSTools_VecLib vecDraw(g);

  // Background (shadow boxes)

  if (StartBackground(vecDraw, mRECT))
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

  RenderBackground(vecDraw, mRECT);

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

void HISSTools_Matrix::SetState(int x, int y, char state)
{
  if (x >= 0 && x < mXDim && y >= 0 && y < mYDim)
  {
    if (state != mStates[y * mXDim + x])
    {
      mStates[y * mXDim + x] = state;
      SetDirty(false);
    }
  }
}

unsigned char HISSTools_Matrix::GetState(int x, int y)
{
  if (x >= 0 && x < mXDim && y >= 0 && y < mYDim)
    return mStates[y * mXDim + x];

  return 0;
}

void HISSTools_Matrix::SetHilite(bool on)
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

bool HISSTools_Matrix::coordsToIndices(double x, double y, int *xPos, int *yPos)
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

// HISSTools_Progress
//

// Constructor

HISSTools_Progress::HISSTools_Progress(double x, double y, double w, double h, const char *type, HISSTools_Design_Scheme *designScheme)
  : IControl(IRECT()), HISSTools_Control_Layers(), mX(x), mY(y), mW(w), mH(h)
{
  // Get Appearance

  mOutlineTK = designScheme->getDimension("ProgressOutline", type);

  mBackgroundCS = designScheme->getColorSpec("ProgressBackground", type);
  mOutlineCS = designScheme->getColorSpec("ProgressOutline", type);
  mProgressCS = designScheme->getColorSpec("Progress", type);

  mShadow = designScheme->getShadow("Progress", type);

  // Area

  // FIX - Hack

  mRECT = HISSTools_Bounds(x, y, w, h);
}

// Draw

void HISSTools_Progress::Draw(IGraphics& g)
{
  HISSTools_VecLib vecDraw(g);

  vecDraw.setColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);

  // Parameters

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

// HISSTools_VUMeter
// A VU meter display. Ballistics should be provided on the DSP side
// The meter supports two on meter levels (drawn in order) and a side value, typically intended for peak hold

// Sender

void HISSTools_VUMeter::Sender::Set(double VU1, double VU2, double side, bool peak, bool linear)
{
  mQueue.Push(MeterValues{ VU1, VU2, side, peak, linear });
}

void HISSTools_VUMeter::Sender::UpdateControl(IEditorDelegate& dlg)
{
  while (mQueue.ElementsAvailable())
  {
    MeterValues v;
    mQueue.Pop(v);
    dlg.SendControlMsgFromDelegate(mControlTag, kUpdateTag, sizeof(MeterValues), (void*)&v);
  }
}

void HISSTools_VUMeter::Sender::Reset()
{
  MeterValues v;

  while (mQueue.ElementsAvailable())
    mQueue.Pop(v);
}

// Constructor

HISSTools_VUMeter::HISSTools_VUMeter(double x, double y, double w, double h, bool flip, double minDB, double maxDB, const char *type, HISSTools_Design_Scheme *designScheme)
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

void HISSTools_VUMeter::OnMsgFromDelegate(int messageTag, int dataSize, const void* pData)
{
  if (messageTag == kUpdateTag && dataSize == sizeof(MeterValues))
  {
    MeterValues* pTypedData = (MeterValues*)pData;

    mVU1Size = getSize(pTypedData->mVU1, pTypedData->mLinear);
    mVU2Size = getSize(pTypedData->mVU2, pTypedData->mLinear);
    mSideSize = getSize(pTypedData->mSide, pTypedData->mLinear);
    mPeak = pTypedData->mPeak;

    SetDirty(false);
  }
}

// Draw

void HISSTools_VUMeter::Draw(IGraphics& g)
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
      vecDraw.fillRect(mX, mY + mH * (1 - mVU2Size), mW, mH * mVU2Size);
    else
      vecDraw.fillRect(mX, mY, mW * mVU2Size, mH);
  }

  // Ticks

  vecDraw.setColor(mOutlineCS);

  if (mW < mH)
    for (int i = 0; i < nTicks; i++)
      horzTick(vecDraw, mTick1, mTick2, mY, mH, (i / (double)(nTicks - 1)), mTickTK);
  else
    for (int i = 0; i < nTicks; i++)
      vertTick(vecDraw, mTick1, mTick2, mX, mW, (i / (double)(nTicks - 1)), mTickTK);

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

// N.B. currently we linearly interpolate dB but we could do something nicer here later.....

double HISSTools_VUMeter::getSize(double value, bool linear)
{
  double db = linear ? 20. * log10(value) : value;
  double size = (db - mMinDB) / (mMaxDB - mMinDB);

  if (size != size)
    size = 0;

  size = size > 1 ? 1 : size;

  return size;
}

void HISSTools_VUMeter::horzTick(HISSTools_VecLib& vecDraw, double x1, double x2, double y, double h, double normPosition, double thickness)
{
  double yPos = y + h * (1 - normPosition);

  vecDraw.line(x1, yPos, x2, yPos, thickness);
}

void HISSTools_VUMeter::vertTick(HISSTools_VecLib& vecDraw, double y1, double y2, double x, double w, double normPosition, double thickness)
{
  double xPos = x + w * normPosition;

  vecDraw.line(xPos, y1, xPos, y2, thickness);
}

