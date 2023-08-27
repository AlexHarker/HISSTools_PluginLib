
#include "HISSTools_Controls.hpp"
#include "IControl.h"

bool HISSTools_Control_Layers::StartBackground(HISSTools_VecLib& vl, IRECT area)
{
    if (sNoCaching)
        return true;

    if (!vl.CheckGroup(mBackground))
    {
        vl.StartGroup(area);
        return true;
    }

    return false;
}

void HISSTools_Control_Layers::RenderBackground(HISSTools_VecLib& vl, IRECT area)
{
    if (!sNoCaching)
    {
        if (!vl.CheckGroup(mBackground))
            mBackground = vl.EndGroup();
        if (mBackground)
            vl.RenderGroup(mBackground);
    }
}

void HISSTools_Control_Layers::RedrawBackground()
{
    if (mBackground)
        mBackground->Invalidate();
}

///////////////////////////////

HISSTools_Text_Helper_Block::HISSTools_Text_Helper_Block(double x, double y, double w, double h, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
{
    // Dimensions

    ResizeText(x, y, w, h);

    // Get Appearance

    mTextTS = scheme->GetTextStyle(name, type);
    mTextCS = scheme->GetColorSpec(name, type);
    mTextSD = scheme->GetShadow(name, type);
    mHAlign = hAlign;
    mVAlign = vAlign;
}

void HISSTools_Text_Helper_Block::Draw(HISSTools_VecLib& vl)
{
    if (mTextSD)
        vl.StartShadow(mTextSD, Bounds());

    vl.SetColor(mTextCS);
    vl.Text(mTextTS, mStr.Get(), mX, mY, mW, mH, mHAlign, mVAlign);

    if (mTextSD)
        vl.RenderShadow();
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
        boxBounds.Include(mTextSD->GetBlurBounds(boxBounds));

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

    mPanelFillCS = scheme->GetColorSpec(concatenatedName, type);
    mPanelSD = scheme->GetShadow(concatenatedName, type);

    sprintf(concatenatedName, "%sPanelOutline", name);

    mPanelOutlineCS = scheme->GetColorSpec(concatenatedName, type);
    mPanelOutlineTK = scheme->GetDimension(concatenatedName, type);

    sprintf(concatenatedName, "%sPanelRoundness", name);

    double roundness = scheme->GetDimension(concatenatedName, type);
    roundness = ((roundness * 2) > mW) ? mW / 2. : roundness;
    roundness = ((roundness * 2) > mH) ? mH / 2. : roundness;
    mPanelRoundness = roundness < 0 ? mH / 2 : roundness;
}

void HISSTools_Text_Helper_Panel::Draw(HISSTools_VecLib& vl, bool drawText)
{
    if (DoDrawPanel())
    {
        if (mPanelSD)
            vl.StartShadow(mPanelSD, Bounds());

        vl.SetColor(mPanelFillCS);
        vl.FillRoundRect(mX, mY, mW, mH, mPanelRoundness);

        if (DoDrawOutline())
        {
            vl.SetColor(mPanelOutlineCS);
            vl.FrameRoundRect(mX, mY, mW, mH, mPanelRoundness, mPanelOutlineTK);
        }

        if (mPanelSD)
            vl.RenderShadow();
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

    boxBounds.AddThickness(mPanelOutlineTK);

    if (mPanelSD)
        boxBounds.Include(mPanelSD->GetBlurBounds(boxBounds));

    boxBounds.Include(HISSTools_Text_Helper_Block::Bounds());

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
    double textHeight = HISSTools_VecLib::GetTextLineHeight(mTextTS);
    char concatenatedName[256];

    mControl = control;
    mTextHPad = pad;

    mTextCS = HISSTools_Text_Helper_Panel::mTextCS;
    mPanelFillCS = HISSTools_Text_Helper_Panel::mPanelFillCS;
    mPanelOutlineCS = HISSTools_Text_Helper_Panel::mPanelOutlineCS;

    // Get Appearance

    bool drawMenuTriangle;

    sprintf(concatenatedName, "%sDrawTriangle", name);
    drawMenuTriangle = scheme->GetFlag(concatenatedName, type);
    sprintf(concatenatedName, "%sPromptRatio", name);
    mPromptRatio = scheme->GetDimension(concatenatedName, type);
    sprintf(concatenatedName, "%sHilite", name);
    mTextHiliteCS = scheme->GetColorSpec(concatenatedName, type);
    sprintf(concatenatedName, "%sPanelHilite", name);
    mPanelHiliteCS = scheme->GetColorSpec(concatenatedName, type);
    sprintf(concatenatedName, "%sPanelOutlineHilite", name);
    mOutlineHiliteCS = scheme->GetColorSpec(concatenatedName, type);
    mShowUnits = scheme->GetFlag("ShowUnits", type);

    // FIX - Padding! (see Draw also)

    double wPad = 9;
    double hPad = 0;

    //ChangePadding(wPad, hPad);
    ChangePadding(0, hPad);

    if (drawMenuTriangle)
    {
        mDrawTriangle = true;
        sprintf(concatenatedName, "%sDrawSeparator", name);
        mDrawSeparator = scheme->GetFlag(concatenatedName, type);
        sprintf(concatenatedName, "%sFlipTriangle", name);
        mMenuFlipTriangle = scheme->GetFlag(concatenatedName, type);

        double menuTriangleWidthRatio = scheme->GetDimension("MenuTriangleWidthRatio", type);
        double menuTriangleHeightRatio = scheme->GetDimension("MenuTriangleHeightRatio", type);
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

bool HISSTools_Text_Helper_Param::MenuParam()
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

    mInEdit = MenuParam() ? false : true;

    // FIX - Widths ??
    // FIX - Text Prompt Vertical Centering??
    // FIX - Text Prompt - proper matching font....
    // FIX - Prompt Menus - Centering for text and box
    // FIX - Prompt Menus - font....

    if (mControl->GetParam() == nullptr)
        return;

    double textHeight = mTextTS->mSize;
    double promptHeight = MenuParam() ? 0 : textHeight + (2.0 * mTextHPad);

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

    if (mDrawSeparator && MenuParam() && DoDrawOutline())
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
    if (mDrawTriangle && MenuParam())
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

    if (mDrawTriangle && MenuParam())
    {
        // Separator

        if (mDrawSeparator && DoDrawOutline())
        {
            vl.SetColor(mPanelOutlineCS);
            vl.Line(mSeparatorX, mY, mSeparatorX, mY + mH, mPanelOutlineTK);
        }

        // Triangle

        if (mTextSD)
            vl.StartShadow(mTextSD, IRECT(mX, mY, mX + mW, mY + mH));

        vl.SetColor(mTextCS);
        vl.FillTriangle(mMenuTriangleL, mMenuTriangleTop, (mMenuTriangleL + mMenuTriangleR) / 2.0, mMenuTriangleBtm, mMenuTriangleR, mMenuTriangleTop);

        if (mTextSD)
            vl.RenderShadow();
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
    double roundness = scheme->GetDimension(name, type);

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
    mW = w <= 0 ? scheme->GetDimension("PanelWidth", type) : w;
    mH = h <= 0 ? scheme->GetDimension("PanelHeight", type) : h;

    mRoundnessTL = GetRoundness("PanelRoundnessTL", type, scheme);
    mRoundnessTR = GetRoundness("PanelRoundnessTR", type, scheme);
    mRoundnessBL = GetRoundness("PanelRoundnessBL", type, scheme);
    mRoundnessBR = GetRoundness("PanelRoundnessBR", type, scheme);

    // Get Appearance

    mOutlineTK = scheme->GetDimension("PanelOutline", type);

    mShadow = scheme->GetShadow("Panel", type);

    mDrawOutline = scheme->GetFlag("PanelDrawOutline", type);

    mPanelCS = scheme->GetColorSpec("PanelFill", type);
    mOutlineCS = scheme->GetColorSpec("PanelOutline", type);

    // Calculate Areas (including shadows and thicknesses)

    HISSTools_Bounds fullBounds(mX, mY, mW, mH);

    if (mDrawOutline)
        fullBounds.AddThickness(mOutlineTK);

    fullBounds = mShadow->GetBlurBounds(fullBounds);

    mRECT = fullBounds;
}

// Draw

void HISSTools_Panel::Draw(IGraphics& g)
{
    HISSTools_VecLib vl(g);

    if (StartBackground(vl, mRECT))
    {
        vl.StartShadow(mShadow, mRECT);
        vl.SetColor(mPanelCS);
        vl.FillRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR);
        if (mDrawOutline)
        {
            vl.SetColor(mOutlineCS);
            vl.FrameRoundRect(mX, mY, mW, mH, mRoundnessTL, mRoundnessTR, mRoundnessBL, mRoundnessBR, mOutlineTK);
        }
        vl.RenderShadow();
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
    mW = w <= 0 ? scheme->GetDimension("ButtonWidth", type) : w;
    mH = h <= 0 ? scheme->GetDimension("ButtonHeight", type) : h;

    double roundness = scheme->GetDimension("ButtonRoundness", type);
    mRoundness = roundness < 0 ? mH / 2 : roundness;

    mTextPad = scheme->GetDimension("ButtonTextPad", type);

    // Label Mode

    mLabelMode = scheme->GetFlag("ButtonLabelMode", type);

    // Get Appearance

    mOutlineTK = scheme->GetDimension("ButtonOutline", type);

    mShadow = scheme->GetShadow("Button", type);

    mTextStyle = scheme->GetTextStyle("Button", type);

    mOnCS = scheme->GetColorSpec("ButtonHandleOn", type);
    mOffCS = scheme->GetColorSpec("ButtonHandleOff", type);
    mHandleLabelCS = scheme->GetColorSpec("ButtonHandleLabel", type);
    mHandleLabelOffCS = scheme->GetColorSpec("ButtonHandleLabelOff", type);
    mHandleLabelOffCS = mHandleLabelOffCS ? mHandleLabelOffCS : mHandleLabelCS;
    mOutlineCS = scheme->GetColorSpec("ButtonOutline", type);
    mBackgroundLabelCS = scheme->GetColorSpec("ButtonBackgroundLabel", type);
    mInactiveOverlayCS = scheme->GetColorSpec("ButtonInactiveOverlay", type);

    // Calculate Areas (including shadows and thicknesses)

    HISSTools_Bounds handleBounds(mX, mY, mLabelMode ? mH : mW, mH);
    HISSTools_Bounds fullBounds(mX, mY, mW, mH);

    handleBounds.AddThickness(mOutlineTK);

    fullBounds = mShadow->GetBlurBounds(handleBounds);
    fullBounds.Include(fullBounds);

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

    vl.StartShadow(mShadow, mRECT);
    vl.SetColor(GetValue() > 0.5 ? mOnCS : mOffCS);
    vl.FillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
    vl.SetColor(mOutlineCS);
    vl.FrameRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness, mOutlineTK);
    vl.RenderShadow();

    vl.SetColor(mLabelMode ? mBackgroundLabelCS : GetValue() > 0.5 ? mHandleLabelCS : mHandleLabelOffCS);
    vl.Text(mTextStyle, mDisplayName.Get(), mLabelMode ? mX + mH + mTextPad : mX, mY, mLabelMode ? mW - (mH + mTextPad) : mW, mH, mLabelMode ? kHAlignLeft : kHAlignCenter);

    // Inactive

    if (IsDisabled())
    {
        // Inactive Overlay

        vl.SetColor(mInactiveOverlayCS);
        vl.FillRoundRect(mX, mY, mLabelMode ? mH : mW, mH, mRoundness);
    }
}

// HISSTools_Tabs
// An abstract class to control tabs in plug-ins

void HISSTools_Tabs::Init()
{
    mParam = mTabControl->GetParam();
    mMaxTabNumber = mParam != nullptr ? round(mParam->GetRange()) : 0;
    TabSetDirty(false);
}

void HISSTools_Tabs::AttachControl(iplug::igraphics::IControl *control, int tabNumber)
{
    // N.B. - mMaxTabNumber is one lass than the number of actual tabs (zero referenced)

    mItems.push_back(TabItem(control, tabNumber));
    UpdateItems();
}

void HISSTools_Tabs::TabHide(bool hide)
{
    mTabControl->IControl::Hide(hide);
    UpdateItems();
}

void HISSTools_Tabs::TabSetDirty(bool pushParamToPlug)
{
    mTabControl->IControl::SetDirty(pushParamToPlug);
    mCurrentTabNumber = mParam ? ClipTabNumber(mParam->Int() - mParam->GetMin()) : 0;
    UpdateItems();
}

/*
void setTabFromPlug(int tabNumber)
{
  if (mParam)
    mParam->Set(tabNumber + mParam->GetMin());

  tabSetDirty(false);
}*/

void HISSTools_Tabs::UpdateItems()
{
    bool tabObjectHidden = mTabControl->IsHidden();

    for (auto it = mItems.begin(); it != mItems.end(); it++)
        it->mControl->Hide(tabObjectHidden || ClipTabNumber(it->mTabNumber) != mCurrentTabNumber);
}

int HISSTools_Tabs::ClipTabNumber(int tabNumber)
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

    bool labelBelow = designScheme->GetFlag("ValueLabelBelow", type);
    bool label = designScheme->GetFlag("ValueDrawLabel", type);
    mTextArea = designScheme->GetDimension("ValueTextArea", type);

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
        fullBoxBounds.Include(mTextLabel->Bounds());
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

    if (mTextParam->MenuParam())
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
        if (mTextParam->MenuParam() == false)
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

    double d = designScheme->GetDimension("DialDiameter", type);

    mTextArea = designScheme->GetDimension("DialTextArea", type);

    mR = d / 2.0;
    mCx = x + mR;
    mCy = y + mR;

    // Pointer Appearance

    double pointerCircRatio = designScheme->GetDimension("DialPointerRatio", type);
    double pointerTipRatio = designScheme->GetDimension("DialTipRatio", type);
    double pointerAngle = designScheme->GetDimension("DialPointerAngle", type);

    SetPointerAppearance(pointerCircRatio, pointerTipRatio, pointerAngle);

    // Set Throw

    mRefValue = designScheme->GetDimension("DialRefValue", type);
    mStartAngle = designScheme->GetDimension("DialStartAngle", type);
    mThrowAngle = designScheme->GetDimension("DialThrowAngle", type);

    // Get Appearance

    mOutlineTK = designScheme->GetDimension("DialOutline", type);
    mPointerTK = designScheme->GetDimension("DialIndicatorLineWidth", type);
    mPointerOutlineTK = designScheme->GetDimension("DialPointerOutline", type);

    mOutlineSD = designScheme->GetShadow("DialOutline", type);
    mPointerSD = designScheme->GetShadow("DialPointer", type);

    mIndicatorCS = designScheme->GetColorSpec("DialIndicator", type);
    mCircleFillCS = designScheme->GetColorSpec("DialCircleFill", type);
    mPointerFillCS = designScheme->GetColorSpec("DialPointerFill", type);
    mOutlineCS = designScheme->GetColorSpec("DialOutline", type);
    mPointerOutlineCS = designScheme->GetColorSpec("DialPointerOutline", type);
    mInactiveOverlayCS = designScheme->GetColorSpec("DialInactiveOverlay", type);

    mDrawValOnlyOnMO = designScheme->GetFlag("DialDrawValOnlyOnMO");

    // Text 

    mPromptHalfHeight = (0.5 * designScheme->GetTextStyle("DialValue", type)->mSize) + designScheme->GetDimension("DialPromptPadding", type);

    double textPad = designScheme->GetDimension("DialPromptPadding", type);
    double halfWidth = mR;// * mPromptRatio;// * mPointerCircRadius;
    double labelHalfWidth = mR * 1.2;

    //FIX - above
    mTextParam = new HISSTools_Text_Helper_Param(this, mCx - halfWidth, mCy - mPromptHalfHeight, 2. * halfWidth, 2. * mPromptHalfHeight, textPad, kHAlignCenter, kVAlignCenter, "DialValue", type, designScheme);
    mTextLabel = new HISSTools_Text_Helper_Block(mCx - labelHalfWidth, mCy + mR, 2 * labelHalfWidth, mTextArea, kHAlignCenter, kVAlignBottom, "DialLabel", type, designScheme);

    // Calculate Areas (including shadows and thicknesses)

    HISSTools_Bounds labelBounds(mCx - labelHalfWidth, mCy + mR, 2 * labelHalfWidth, mTextArea);
    HISSTools_Bounds dialBoxBounds(x, y, d, d);
    HISSTools_Bounds fullBoxBounds = dialBoxBounds;
    fullBoxBounds.Include(labelBounds);
    HISSTools_Bounds ptrBoxBounds(mCx - mPointerTipRadius, mCy - mPointerTipRadius, 2 * mPointerTipRadius, 2 * mPointerTipRadius);

    dialBoxBounds.AddThickness(mOutlineTK);
    ptrBoxBounds.AddThickness(mPointerOutlineTK);

    fullBoxBounds.Include(mOutlineSD->GetBlurBounds(dialBoxBounds));
    fullBoxBounds.Include(mPointerSD->GetBlurBounds(ptrBoxBounds));

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

        vecDraw.StartShadow(mOutlineSD, mRECT);
        vecDraw.SetColor(mOutlineCS);
        vecDraw.FrameCircle(mCx, mCy, mR, mOutlineTK);
        vecDraw.RenderShadow();
        vecDraw.SetColor(mCircleFillCS);
        vecDraw.FillCircle(mCx, mCy, mR);

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

    vecDraw.SetColor(mIndicatorCS);
    vecDraw.FillArc(mCx, mCy, mR, iBaseAng, iDiffAng);

    // Pointer Line

    vecDraw.SetColor(mOutlineCS);
    vecDraw.CircleIntersection(mCx, mCy, iPntrAng, mR, &xIntersect, &yIntersect);
    vecDraw.Line(mCx, mCy, xIntersect, yIntersect, mPointerTK);

    // Pointer

    vecDraw.StartShadow(mPointerSD, mRECT);
    vecDraw.SetColor(mPointerFillCS);
    vecDraw.FillCircularPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle);
    vecDraw.SetColor(mPointerOutlineCS);
    vecDraw.FrameCircularPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle, mPointerOutlineTK);
    vecDraw.RenderShadow();

    if (IsDisabled())
    {
        // Inactive Overlay

        vecDraw.SetColor(mInactiveOverlayCS);
        vecDraw.FillCircularPointer(mCx, mCy, mPointerCircRadius, mPointerTipRadius, iPntrAng, mPointerAngle);
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

void HISSTools_Dial::SetPointerAppearance(double pointerCircRatio, double pointerTipRatio, double pointerAngle)
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
    mW = w < 0 ? designScheme->GetDimension("SwitchWidth", type) : w;
    mH = h < 0 ? designScheme->GetDimension("SwitchHeight", type) : h;
    mS = std::min(w, h);

    double roundness = designScheme->GetDimension("SwitchRoundness", type);
    mRoundness = roundness < 0 ? mH / 2 : roundness;

    // Number of States

    // FIX - get from parameters if <= 0 ??

    mNStates = nStates < 2 ? 2 : nStates;

    // Get Appearance

    mHandleTK = designScheme->GetDimension("SwitchHandleOutline", type);
    mBoxOutlineTK = designScheme->GetDimension("SwitchBoxOutline", type);

    mShadow = designScheme->GetShadow("Switch", type);

    mHandleFillCS = designScheme->GetColorSpec("SwitchHandleFill", type);
    mHandleOutlineCS = designScheme->GetColorSpec("SwitchHandleOutline", type);
    mBoxFillCS = designScheme->GetColorSpec("SwitchBoxFill", type);
    mBoxOutlineCS = designScheme->GetColorSpec("SwitchOutline", type);
    mInactiveOverlayCS = designScheme->GetColorSpec("SwitchInactiveOverlay", type);

    // Calculate Areas (including shadows and thicknesses)

    HISSTools_Bounds boxBounds(mX, mY, mW, mH);
    HISSTools_Bounds fullBounds(mX, mY, mW, mH);

    boxBounds.AddThickness(mBoxOutlineTK);
    fullBounds.AddThickness(mHandleTK);

    fullBounds = mShadow->GetBlurBounds(fullBounds);
    fullBounds.Include(boxBounds);

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

        vecDraw.SetColor(mBoxFillCS);
        vecDraw.FillRoundRect(mX, mY, mW, mH, mRoundness);
        vecDraw.SetColor(mBoxOutlineCS);
        vecDraw.FrameRoundRect(mX, mY, mW, mH, mRoundness, mBoxOutlineTK);
    }

    RenderBackground(vecDraw, mRECT);

    // Handle

    vecDraw.SetColor(mHandleFillCS);
    vecDraw.StartShadow(mShadow, mRECT);
    vecDraw.FillRoundRect(xPos, yPos, mS, mS, mRoundness);
    vecDraw.SetColor(mHandleOutlineCS);
    vecDraw.FrameRoundRect(xPos, yPos, mS, mS, mRoundness, mHandleTK);
    vecDraw.RenderShadow();

    // Fix - Labels (Control Name and Value)
    // Label

    //vecDraw.setColor(textColor);
    //vecDraw.text(&txt, "Load", mRECT.L, mRECT.R, mRECT.T, mRECT.B);

    // Inactive

    if (IsDisabled())
    {
        // Inactive Overlay

        vecDraw.SetColor(mInactiveOverlayCS);
        vecDraw.FillRoundRect(mX, mY, mW, mH, mRoundness);
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
    mS = designScheme->GetDimension("MatrixHandleSize", type);
    mGap = designScheme->GetDimension("MatrixHandleGap", type);
    mUnit = mS + mGap;
    mW = mXDim * mUnit - mGap;
    mH = mYDim * mUnit - mGap;
    mRoundness = designScheme->GetDimension("MatrixHandleRoundness", type);
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
        mStateCS[i] = currentStateScheme->GetColorSpec(testName, type);

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

    mHandleEmptyOutlineTK = designScheme->GetDimension("MatrixShadowOutline", type);
    mHandleFilledOutlineTK = designScheme->GetDimension("MatrixOutline", type);
    mHiliteTK = designScheme->GetDimension("MatrixHilite", type);

    mShadow = designScheme->GetShadow("Matrix", type);

    mOutlineCS = designScheme->GetColorSpec("MatrixOutline", type);
    mHiliteCS = designScheme->GetColorSpec("MatrixHilite", type);

    // Calculate Areas (including shadows and thicknesses)

    HISSTools_Bounds boxBoundsShadow(mX, mY, mW, mH);
    HISSTools_Bounds boxBoundsOutline(mX, mY, mW, mH);
    HISSTools_Bounds fullBoxBounds(mX, mY, mW, mH);

    boxBoundsShadow.AddThickness(mHandleEmptyOutlineTK);
    boxBoundsShadow = mShadow->GetBlurBounds(boxBoundsShadow);
    boxBoundsOutline.AddThickness(mHandleFilledOutlineTK);
    fullBoxBounds = boxBoundsOutline;
    fullBoxBounds.Include(boxBoundsShadow);

    mRECT = fullBoxBounds;
    SetTargetRECT(boxBoundsOutline);
}

HISSTools_Matrix::~HISSTools_Matrix()
{
    delete[] mStates;
}

int HISSTools_Matrix::GetXPos() const
{
    return mXPos;
}
int HISSTools_Matrix::GetYPos() const
{
    return mYPos;
}

// Mousing Functions

bool HISSTools_Matrix::OnMousing(float x, float y, const IMouseMod& mod, MousingAction action, float wheel)
{
    if (CoordsToIndices(x, y, &mXPos, &mYPos))
    {
        ReportToPlug(mXPos, mYPos, mod, action, wheel);

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

    ReportToPlug(-1, -1, IMouseMod(), kMouseOut);

    SetDirty(false);
}

// Draw

void HISSTools_Matrix::Draw(IGraphics& g)
{
    HISSTools_VecLib vecDraw(g);

    // Background (shadow boxes)

    if (StartBackground(vecDraw, mRECT))
    {
        vecDraw.StartShadow(mShadow, mRECT);
        vecDraw.SetColor(mOutlineCS);

        for (int i = 0; i < mXDim; i++)
        {
            double sx = mX + i * mUnit;

            for (int j = 0; j < mYDim; j++)
            {
                double sy = mY + j * mUnit;

                vecDraw.FrameRoundRect(sx, sy, mS, mS, mRoundness, mHandleEmptyOutlineTK);
            }
        }

        vecDraw.RenderShadow(false);
    }

    RenderBackground(vecDraw, mRECT);

    // Matrix fills

    for (int i = 0; i < mXDim; i++)
    {
        double sx = mX + i * mUnit;

        for (int j = 0; j < mYDim; j++)
        {
            double sy = mY + j * mUnit;

            vecDraw.SetColor(mStateCS[mStates[j * mXDim + i] % mNStates]);
            vecDraw.FillRoundRect(sx, sy, mS, mS, mRoundness);
            vecDraw.SetColor(mOutlineCS);
            vecDraw.FrameRoundRect(sx, sy, mS, mS, mRoundness, mHandleFilledOutlineTK);
        }
    }

    if (mXHilite > -1 && mYHilite > -1)
    {
        vecDraw.SetColor(mHiliteCS);
        vecDraw.FrameRoundRect(mX + mXPos * mUnit, mY + mYPos * mUnit, mS, mS, mRoundness, mHiliteTK);
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

bool HISSTools_Matrix::CoordsToIndices(double x, double y, int *xPos, int *yPos)
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

    mOutlineTK = designScheme->GetDimension("ProgressOutline", type);

    mBackgroundCS = designScheme->GetColorSpec("ProgressBackground", type);
    mOutlineCS = designScheme->GetColorSpec("ProgressOutline", type);
    mProgressCS = designScheme->GetColorSpec("Progress", type);

    mShadow = designScheme->GetShadow("Progress", type);

    // Area

    // FIX - Hack

    mRECT = HISSTools_Bounds(x, y, w, h);
}

// Draw

void HISSTools_Progress::Draw(IGraphics& g)
{
    HISSTools_VecLib vecDraw(g);

    vecDraw.SetColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);

    // Parameters

    if (StartBackground(vecDraw, mRECT))
    {
        vecDraw.SetColor(mBackgroundCS);
        vecDraw.FillRect(mX, mY, mW, mH);

        // Frame Rectangle

        vecDraw.StartShadow(mShadow, mRECT);
        vecDraw.SetColor(mOutlineCS);
        vecDraw.FrameRect(mX, mY, mW, mH, mOutlineTK);
        vecDraw.RenderShadow();
    }

    RenderBackground(vecDraw, mRECT);

    // Progress Rectangles

    vecDraw.ForceGradientBox(mX, mY, mX + mW, mY + mH);

    // Progress

    vecDraw.SetColor(mProgressCS);

    if (mW < mH)
        vecDraw.FillRect(mX, mY + mH * (1 - GetValue()), mW, mH * GetValue());
    else
        vecDraw.FillRect(mX, mY, mW * GetValue(), mH);

    vecDraw.ForceGradientBox();

    // Outline Again
    // FIX - Draw ALL PROPERLY

    vecDraw.SetColor(mOutlineCS);
    vecDraw.FrameRect(mX, mY, mW, mH, mOutlineTK);

    // Reset Orientation (default is always horizontal)

    vecDraw.SetColorOrientation(kCSOrientHorizontal);
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

    mOutlineTK = designScheme->GetDimension("VUOutline", type);
    mTickTK = designScheme->GetDimension("VUTick", type);
    mPeakHoldTK = designScheme->GetDimension("VUPeakHold", type);

    mShadow = designScheme->GetShadow("Meter", type);

    mOverlayFixedGradientBox = designScheme->GetFlag("VUOverlayFixedGradientBox", type);

    mBackgroundCS = designScheme->GetColorSpec("VUBackground", type);
    mOutlineCS = designScheme->GetColorSpec("VUOutline", type);
    mVU1CS = designScheme->GetColorSpec("VU1", type);
    mVU1PeakCS = designScheme->GetColorSpec("VU1Peak", type);
    mVU2CS = designScheme->GetColorSpec("VU2", type);
    mVU2PeakCS = designScheme->GetColorSpec("VU2Peak", type);
    mVUSideCS = designScheme->GetColorSpec("VUSide", type);
    mVUSidePeakCS = designScheme->GetColorSpec("VUSidePeak", type);

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

        mVU1Size = GetSize(pTypedData->mVU1, pTypedData->mLinear);
        mVU2Size = GetSize(pTypedData->mVU2, pTypedData->mLinear);
        mSideSize = GetSize(pTypedData->mSide, pTypedData->mLinear);
        mPeak = pTypedData->mPeak;

        SetDirty(false);
    }
}

// Draw

void HISSTools_VUMeter::Draw(IGraphics& g)
{
    HISSTools_VecLib vecDraw(g);
    vecDraw.SetColorOrientation(mW < mH ? kCSOrientVertical : kCSOrientHorizontal);

    // Parameters

    int nTicks = 10;

    if (StartBackground(vecDraw, mRECT))
    {
        vecDraw.SetColor(mBackgroundCS);
        vecDraw.FillRect(mX, mY, mW, mH);

        // Frame Rectangle

        vecDraw.StartShadow(mShadow, mRECT);
        vecDraw.SetColor(mOutlineCS);
        vecDraw.FrameRect(mX, mY, mW, mH, mOutlineTK);
        vecDraw.RenderShadow();
    }

    RenderBackground(vecDraw, mRECT);

    // Meter Rectangles

    vecDraw.ForceGradientBox(mX, mY, mX + mW, mY + mH);

    // VU 1

    if (mVU1Size > 0)
    {
        if (mPeak)
            vecDraw.SetColor(mVU1PeakCS);
        else
            vecDraw.SetColor(mVU1CS);

        if (mW < mH)
            vecDraw.FillRect(mX, mY + mH * (1 - mVU1Size), mW, mH * mVU1Size);
        else
            vecDraw.FillRect(mX, mY, mW * mVU1Size, mH);
    }

    // VU 2

    if (mVU2Size > 0)
    {
        if (mOverlayFixedGradientBox == false)
            vecDraw.ForceGradientBox();

        if (mPeak)
            vecDraw.SetColor(mVU2PeakCS);
        else
            vecDraw.SetColor(mVU2CS);

        if (mW < mH)
            vecDraw.FillRect(mX, mY + mH * (1 - mVU2Size), mW, mH * mVU2Size);
        else
            vecDraw.FillRect(mX, mY, mW * mVU2Size, mH);
    }

    // Ticks

    vecDraw.SetColor(mOutlineCS);

    if (mW < mH)
        for (int i = 0; i < nTicks; i++)
            HorzTick(vecDraw, mTick1, mTick2, mY, mH, (i / (double)(nTicks - 1)), mTickTK);
    else
        for (int i = 0; i < nTicks; i++)
            VertTick(vecDraw, mTick1, mTick2, mX, mW, (i / (double)(nTicks - 1)), mTickTK);

    // Side VU

    if (mSideSize >= 0)
    {
        vecDraw.ForceGradientBox(mX, mY, mX + mW, mY + mH);

        if (mPeak)
            vecDraw.SetColor(mVUSidePeakCS);
        else
            vecDraw.SetColor(mVUSideCS);

        if (mW < mH)
            HorzTick(vecDraw, mTick3, mTick4, mY, mH, mSideSize, mPeakHoldTK);
        else
            VertTick(vecDraw, mTick3, mTick4, mX, mW, mSideSize, mPeakHoldTK);
    }

    vecDraw.ForceGradientBox();

    // Outline Again
    // FIX - Draw ALL PROPERLY

    vecDraw.SetColor(mOutlineCS);
    vecDraw.FrameRect(mX, mY, mW, mH, mOutlineTK);

    // Reset Orientation (default is always horizontal)

    vecDraw.SetColorOrientation(kCSOrientHorizontal);
}

// N.B. currently we linearly interpolate dB but we could do something nicer here later.....

double HISSTools_VUMeter::GetSize(double value, bool linear)
{
    double db = linear ? 20. * log10(value) : value;
    double size = (db - mMinDB) / (mMaxDB - mMinDB);

    if (size != size)
        size = 0;

    size = size > 1 ? 1 : size;

    return size;
}

void HISSTools_VUMeter::HorzTick(HISSTools_VecLib& vecDraw, double x1, double x2, double y, double h, double normPosition, double thickness)
{
    double yPos = y + h * (1 - normPosition);

    vecDraw.Line(x1, yPos, x2, yPos, thickness);
}

void HISSTools_VUMeter::VertTick(HISSTools_VecLib& vecDraw, double y1, double y2, double x, double w, double normPosition, double thickness)
{
    double xPos = x + w * normPosition;

    vecDraw.Line(xPos, y1, xPos, y2, thickness);
}

// HISSTools_FileSelector
//

// Mousing Functions

void HISSTools_FileSelector::OnMouseDown(float x, float y, const IMouseMod& pMod)
{
    mState = kFSSelecting;
    SetDirty(false);
}

void HISSTools_FileSelector::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod)
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

void HISSTools_FileSelector::OnMouseUp(float x, float y, const IMouseMod& pMod)
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
            ReportToPlug();
        }
        else
        {
            GetUI()->PromptForFile(tempFile, mDir, mFileAction, mExtensions.Get());

            mState = kFSDone;

            if (tempFile.GetLength())
            {
                mFile.Set(tempFile.Get());
                ReportToPlug();
            }
        }

        SetDirty(false);
    }
}

// Draw

void HISSTools_FileSelector::Draw(IGraphics& g)
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

// File Functions

const WDL_String& HISSTools_FileSelector::GetLastSelectedFileForPlug()
{
    return mFile;
}

void HISSTools_FileSelector::SetLastSelectedFileFromPlug(const char* file)
{
    mFile.Set(file);
}

void HISSTools_FileSelector::SetExtensions(char *extensions)
{
    mExtensions.Set(extensions);
}