
#include "HISSTools_Controls.hpp"

bool HISSTools_Control_Layers::startBackground(IGraphics& g, HISSTools_VecLib& vecDraw, IRECT area)
{
    if (sNoCaching)
        return true;
    
    if (!g.CheckLayer(mBackground))
    {
        vecDraw.startGroup(area);
        return true;
    }
    
    return false;
}

void HISSTools_Control_Layers::renderBackground(IGraphics& g, HISSTools_VecLib& vecDraw, IRECT area)
{
    if (!sNoCaching)
    {
        if (!g.CheckLayer(mBackground))
            mBackground = vecDraw.endGroup();
        if (mBackground)
            vecDraw.renderGroup(mBackground);
    }
}

void HISSTools_Control_Layers::redrawBackground()
{
    if (mBackground)
        mBackground->Invalidate();
}

///////////////////////////////

HISSTools_Text_Helper_Block::HISSTools_Text_Helper_Block(double x, double y, double w, double h,  HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
{
    // Dimensions
    
    resizeText(x, y, w, h);
    
    // Get Appearance
    
    mTextTS = scheme->getTextStyle(name, type);
    mTextCS = scheme->getColorSpec(name, type);
    mTextSD = scheme->getShadow(name, type);
    mHAlign = hAlign;
    mVAlign = vAlign;
}

void HISSTools_Text_Helper_Block::Draw(HISSTools_VecLib& vecDraw)
{
    if (mTextSD)
        vecDraw.startShadow(mTextSD, IRECT(mX, mY, mX + mW, mY + mH));
    
    vecDraw.setColor(mTextCS);
    vecDraw.text(mTextTS, mStr.Get(), mX, mY, mW, mH, mHAlign, mVAlign);
    
    if (mTextSD)
        vecDraw.renderShadow();
}

void HISSTools_Text_Helper_Block::resizeText(double x, double y, double w, double h)
{
    mX = w > 0 ? x : x + w;
    mY = h > 0 ? y : y + h;
    mW = fabs(w);
    mH = fabs(h);
}

void HISSTools_Text_Helper_Block::setTextColor(HISSTools_Color_Spec *colorSpec)
{
    mTextCS = colorSpec;
}

void HISSTools_Text_Helper_Block::setText(const char *str)
{
    mStr.Set(str);
}

HISSTools_Bounds HISSTools_Text_Helper_Block::bounds()
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
    resize(x, y, w, h);
    
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

void HISSTools_Text_Helper_Panel::Draw(HISSTools_VecLib& vecDraw, bool drawText)
{
    if (doDrawPanel())
    {
        if (mPanelSD)
            vecDraw.startShadow(mPanelSD, IRECT(mX, mY, mX + mW, mY + mH));
        
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

void HISSTools_Text_Helper_Panel::changePadding(double lPad, double rPad, double hPad)
{
    mLPad = lPad;
    mRPad = rPad;
    mHPad = hPad;
    
    resize(mX, mY, mW, mH);
}

void HISSTools_Text_Helper_Panel::changePadding(double wPad, double hPad)
{
    changePadding(wPad, wPad, hPad);
}

void HISSTools_Text_Helper_Panel::resize(double x, double y, double w, double h)
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

void HISSTools_Text_Helper_Panel::setPanelColor(HISSTools_Color_Spec *colorSpec)
{
    mPanelFillCS = colorSpec;
}

void HISSTools_Text_Helper_Panel::setOutlineColor(HISSTools_Color_Spec *colorSpec)
{
    mPanelOutlineCS = colorSpec;
}

HISSTools_Bounds HISSTools_Text_Helper_Panel::bounds()
{
    HISSTools_Bounds boxBounds(mX, mY, mW, mH);
    
    boxBounds.addThickness(mPanelOutlineTK);
    
    if (mPanelSD)
        boxBounds.include(mPanelSD->getBlurBounds(boxBounds));
    
    boxBounds.include(HISSTools_Text_Helper_Block::bounds());
    
    return boxBounds;
}

bool HISSTools_Text_Helper_Panel::doDrawOutline()
{
    return (mPanelOutlineTK && mPanelOutlineCS != nullptr);
}

bool HISSTools_Text_Helper_Panel::doDrawPanel()
{
    return ((mPanelFillCS != nullptr) || doDrawOutline());
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
    
    //changePadding(wPad, hPad);
    changePadding(0, hPad);
    
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
        
        double separatorWidth = wPad * 2.0 + menuTriangleWidth + roundnessCompensate(menuTriangleHeight);
        mSeparatorX = mX + (mMenuFlipTriangle ? separatorWidth : mW - separatorWidth);
        
        mMenuTriangleTop = mY + (mH - menuTriangleHeight) / 2.0;
        mMenuTriangleBtm = mMenuTriangleTop + menuTriangleHeight;
        mMenuTriangleL = mSeparatorX + (mMenuFlipTriangle ? -(menuTriangleWidth + wPad) : wPad);
        mMenuTriangleR = mMenuTriangleL + menuTriangleWidth;
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

void HISSTools_Text_Helper_Param::finishEdit()
{
    mInEdit = false;
}

void HISSTools_Text_Helper_Param::promptUserInput()
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
    
    promptLeft += promptWidth * 0.2;
    promptWidth *= 0.6;
    
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
    iEntryBounds = entryBounds.iBounds();
    
    setControlText();
    mControl->DisablePrompt(false);
    mControl->PromptUserInput(iEntryBounds);
}

bool HISSTools_Text_Helper_Param::promptUserInput(float x, float y)
{
    if (bounds().iBounds().Contains(x, y))
    {
        if (!mDrawSeparator || (mMenuFlipTriangle == (x < mSeparatorX)))
        {
            promptUserInput();
            return true;
        }
    }
    
    return false;
}

void HISSTools_Text_Helper_Param::hilite(bool on)
{
    using Base = HISSTools_Text_Helper_Panel;
    
    Base::mTextCS = on && mTextHiliteCS ? mTextHiliteCS : mTextCS;
    Base::mPanelFillCS = on && mPanelHiliteCS ? mPanelHiliteCS : mPanelFillCS;
    Base::mPanelOutlineCS = on && mOutlineHiliteCS ? mOutlineHiliteCS : mPanelOutlineCS;
}

void HISSTools_Text_Helper_Param::Draw(HISSTools_VecLib& vecDraw)
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
    
    param->GetDisplayForHost(str);
    
    // If label is nullptr don't add the space
    
    if (mShowUnits && *(param->GetLabelForHost()))
    {
        str.Append(" ");
        str.Append(param->GetLabelForHost());
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

void HISSTools_Text_Helper_Param::setControlText()
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

double HISSTools_Text_Helper_Param::roundnessCompensate(double menuTriangleHeight)
{
    if (menuTriangleHeight > mH)
        return mPanelRoundness;
    if (menuTriangleHeight <= mH - (2.0 * mPanelRoundness))
        return 0;
    
    return mPanelRoundness - sqrt(mPanelRoundness * mPanelRoundness - (0.25 * menuTriangleHeight * menuTriangleHeight) - (0.5 * mH) - mPanelRoundness);
}
