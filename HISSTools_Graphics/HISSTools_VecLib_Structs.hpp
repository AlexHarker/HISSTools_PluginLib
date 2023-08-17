
#pragma once

#include <IGraphicsStructs.h>
#include <algorithm>

// Bounds

struct HISSTools_Bounds : public iplug::igraphics::IRECT
{
    using IRECT = iplug::igraphics::IRECT;
    
    HISSTools_Bounds() {}
    HISSTools_Bounds(const IRECT rect) : IRECT(rect) {}
    HISSTools_Bounds(double x, double y, double w, double h)
    : IRECT(static_cast<float>(w > 0 ? x : x - w),
            static_cast<float>(h > 0 ? y : y - h),
            static_cast<float>(w > 0 ? x + w : x),
            static_cast<float>(h > 0 ? y + h : y))
    {}
    
    void include(HISSTools_Bounds inc)      { *this = Union(inc); }
    void addThickness(double thickness)     { Pad(static_cast<float>(std::max(0.0, thickness) * 0.5)); }
};

// Colors and Color Specs

enum ColorOrientation { kCSOrientHorizontal, kCSOrientVertical };

struct HISSTools_Color
{
    HISSTools_Color() : r(0), g(0), b(0), a(0) {}
    
    HISSTools_Color(double R, double G, double B, double A) : r(R), g(G), b(B), a(A)
    {
        // FIX - issues with super-luminescence - where does this happen?
        
        if (a)
        {
            r /= a;
            g /= a;
            b /= a;
            a *= a;
        }
        
        if (r > 1.0 || g > 1.0 || b > 1.0)
            r = g = b = 1.0;
    }
    
    double r, g, b, a;
};

class HISSTools_Color_Spec
{
protected:
    
    using IRECT = iplug::igraphics::IRECT;
    using IPattern = iplug::igraphics::IPattern;
    using IColor = iplug::igraphics::IColor;
    using IColorStop = iplug::igraphics::IColorStop;
    using EPatternType = iplug::igraphics::EPatternType;
    
public:
    
    HISSTools_Color_Spec()
    : mPattern(iplug::igraphics::COLOR_BLACK)
    {}
    
    HISSTools_Color_Spec(HISSTools_Color color)
    : mPattern(clampColor(color))
    {}
    
    HISSTools_Color_Spec(double r, double g, double b, double a)
    : HISSTools_Color_Spec(HISSTools_Color(r, g, b, a))
    {}
    
    virtual ~HISSTools_Color_Spec(){}
    
    const IColor& getColor() const { return mPattern.GetStop(0).mColor; }
    
    virtual IPattern getPattern() const { return IPattern(getColor()); }
    
    virtual void setRect(const IRECT r, ColorOrientation CSOrientation) {}
    
private:
    
    int clampToInt(double x)
    {
        return std::min(255, std::max(0, static_cast<int>(x * 255.0)));
    }
    
    IColor clampColor(HISSTools_Color c)
    {
        return IColor(clampToInt(c.a), clampToInt(c.r), clampToInt(c.g), clampToInt(c.b));
    }
    
protected:
    
    IPattern mPattern;
};


class HISSTools_LICE_HVGradient : public HISSTools_Color_Spec
{
protected:
    
    enum EMode { kModeHorzVert, kModeHVFlip, kModeHorz, kModeVert };
    
    void setMode(EMode mode) { mMode = mode; }
    
public:
    
    HISSTools_LICE_HVGradient(bool flipVertical = true) : mMode(flipVertical ? kModeHVFlip : kModeHorzVert)
    {
        mPattern = IPattern(EPatternType::Linear);
    }
    
    void addStop(HISSTools_Color color, double offset)
    {
        HISSTools_Color_Spec colorClamp = HISSTools_Color_Spec(color);
        mPattern.AddStop(colorClamp.getColor(), static_cast<float>(offset));
    }
    
    virtual void setRect(const IRECT r, ColorOrientation CSOrientation) override
    {
        if ((CSOrientation == kCSOrientHorizontal && mMode != kModeVert) || mMode == kModeHorz)
            mBox = IRECT(r.L, r.T, r.R, r.T);
        else
        {
            if (mMode == kModeHVFlip)
                mBox = IRECT(r.L, r.B, r.L, r.T);
            else
                mBox = IRECT(r.L, r.T, r.L, r.B);
        }
    }
    
    IPattern getPattern() const override
    {
        IPattern pattern = IPattern::CreateLinearGradient(mBox.L, mBox.T, mBox.R, mBox.B);
        
        for (int i = 0; i < mPattern.NStops(); i++)
        {
            const IColorStop& stop = mPattern.GetStop(i);
            pattern.AddStop(stop.mColor, stop.mOffset);
        }
        
        return pattern;
    }
    
private:
    
    EMode mMode;
    IRECT mBox;
};

struct HISSTools_LICE_HGradient : public HISSTools_LICE_HVGradient
{
    HISSTools_LICE_HGradient() : HISSTools_LICE_HVGradient() { setMode(kModeHorz); }
};


struct HISSTools_LICE_VGradient : public HISSTools_LICE_HVGradient
{
    HISSTools_LICE_VGradient() : HISSTools_LICE_HVGradient(false) { setMode(kModeVert); }
};

// Shadows

struct HISSTools_Shadow : public iplug::igraphics::IShadow
{
    using IRECT = iplug::igraphics::IRECT;
    
    HISSTools_Shadow(HISSTools_Color_Spec& shadowColor, double xOffset, double yOffset, double blurSize)
    : IShadow(shadowColor.getPattern(), static_cast<float>(blurSize), static_cast<float>(xOffset), static_cast<float>(yOffset), 1.f, true)
    {}
    
    HISSTools_Bounds getBlurBounds(HISSTools_Bounds currentBounds)
    {
        IRECT rect = currentBounds.GetPadded(mBlurSize - 1);
        currentBounds.include(rect.GetTranslated(mXOffset, mYOffset));
        
        return currentBounds;
    }
};

// Text

// Defaults

#if defined __APPLE__
const char* const HT_DEFAULT_FONT = "Monaco";
const int HT_DEFAULT_TEXT_SIZE = 10;
#else
const char* const HT_DEFAULT_FONT = "Verdana";
const int HT_DEFAULT_TEXT_SIZE = 12;
#endif

const int HT_FONT_LEN = 32;

// Alignment

enum HTextAlign { kHAlignLeft, kHAlignCenter, kHAlignRight };
enum VTextAlign { kVAlignTop, kVAlignCenter, kVAlignBottom };

struct HISSTools_Text
{
    char mFont[HT_FONT_LEN];
    int mSize;
    
    HISSTools_Text(int size = HT_DEFAULT_TEXT_SIZE, char* font = 0)
    : mSize(size)
    {
        strcpy(mFont, (font ? font : HT_DEFAULT_FONT));
    }
    
    HISSTools_Text() : mSize(HT_DEFAULT_TEXT_SIZE)
    {
        strcpy(mFont, HT_DEFAULT_FONT);
    }
};
