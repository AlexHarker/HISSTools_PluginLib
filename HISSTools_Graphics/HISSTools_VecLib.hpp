
#pragma once

#include "IGraphics.h"
#include "HISSTools_VecLib_Structs.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

static HISSTools_Color_Spec defaultColor;

class HISSTools_VecLib
{
    using IGraphics = iplug::igraphics::IGraphics;
    using IRECT = iplug::igraphics::IRECT;
    using IText = iplug::igraphics::IText;
    using IShadow = iplug::igraphics::IShadow;
    using ILayerPtr = iplug::igraphics::ILayerPtr;
    using EAlign = iplug::igraphics::EAlign;
    using EVAlign = iplug::igraphics::EVAlign;
    
    const double PI = iplug::PI;
    
public:
    
    HISSTools_VecLib(IGraphics& graphics) : mGraphics(graphics), mShadow(nullptr), mForceGradientBox(false), mColor(&defaultColor), mCSOrientation(kCSOrientHorizontal)
    {}
    
    void SetClip()
    {
        mGraphics.PathClipRegion();
    }
    
    void SetClip(HISSTools_Bounds clip)
    {
        mGraphics.PathClipRegion(clip);
    }
    
    void SetClip(double xLo, double yLo, double xHi, double yHi)
    {
        SetClip(HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo));
    }
    
    void SetClip(const IRECT& rect)
    {
        SetClip(rect.L, rect.T, rect.R, rect.B);
    }
    
    void StartGroup(const IRECT& r)
    {
        mGraphics.StartLayer(nullptr, r);
    }
    
    ILayerPtr EndGroup()
    {
        return mGraphics.EndLayer();
    }
    
    void RenderGroup(const ILayerPtr& layer)
    {
        mGraphics.DrawLayer(layer);
    }
    
    bool CheckGroup(const ILayerPtr& layer)
    {
        return mGraphics.CheckLayer(layer);
    }
    
    void SetColor(HISSTools_Color_Spec *color)
    {
        mColor = color;
    }
    
    // Orientation allows gradient rotation ONLY for relevant Color Specs
    
    void SetColorOrientation(ColorOrientation CSOrientation)
    {
        mCSOrientation = CSOrientation;
    }
    
    void ForceGradientBox()     { mForceGradientBox = false; }
    
    void ForceGradientBox(double xLo, double yLo, double xHi, double yHi)
    {
        mGradientArea = HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo);
        mForceGradientBox = true;
    }
    
    void StartMultiLine(double x, double y, double thickness)
    {
        mMultiLineThickness = thickness;
        mGraphics.PathMoveTo(x, y);
    }
    
    void ContinueMultiLine(double x, double y)
    {
        mGraphics.PathLineTo(x, y);
    }
    
    void FinishMultiLine()
    {
        Stroke(mMultiLineThickness);
    }
    
    void CircleIntersection(double cx, double cy, double ang, double r, double *retX, double *retY)
    {
        *retX = cos(2.0 * ang * PI) * r + cx;
        *retY = sin(2.0 * ang * PI) * r + cy;
    }
    
    void FrameArc(double cx, double cy, double r, double begAng, double arcAng, double thickness)
    {
        Arc(cx, cy, r, begAng, arcAng);
        Stroke(thickness);
    }
    
    void FillArc(double cx, double cy, double r, double begAng, double arcAng)
    {
        mGraphics.PathClear();
        Arc(cx, cy, r, begAng, arcAng);
        mGraphics.PathLineTo(cx, cy);
        mGraphics.PathClose();
        Fill();
    }
    
    void FillCircle(double cx, double cy, double r)
    {
        mGraphics.PathCircle(cx, cy, r);
        SetShapeGradient(HISSTools_Bounds(cx - r, cy - r, 2.0 * r, 2.0 * r));
        Fill();
    }
    
    void FrameCircle(double cx, double cy, double r, double thickness)
    {
        FrameArc(cx, cy, r, 0.0, 1.0, thickness);
    }
    
    void FrameTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double thickness)
    {
        Triangle(x1, y1, x2, y2, x3, y3);
        Stroke(thickness);
    }
    
    void FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        Triangle(x1, y1, x2, y2, x3, y3);
        Fill();
    }
    
    void FillRect(double x, double y, double w, double h)
    {
        Rectangle(x, y, w, h);
        Fill();
    }
    
    void FrameRect(double x, double y, double w, double h, double thickness)
    {
        Rectangle(x, y, w, h);
        Stroke(thickness);
    }
    
    void FillRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        RoundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        Fill();
    }
    
    void FrameRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr, double thickness)
    {
        RoundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        Stroke(thickness);
    }
    
    void FillRoundRect(double x, double y, double w, double h, double r)
    {
        FillRoundRect(x, y, w, h, r, r, r, r);
    }
    
    void FrameRoundRect(double x, double y, double w, double h, double r, double thickness)
    {
        FrameRoundRect(x, y, w, h, r, r, r, r, thickness);
    }
    
    void FillCPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        cPointer(cx, cy, r, pr, ang, pAng);
        Fill();
    }
    
    void FrameCPointer(double cx, double cy, double r, double pr, double ang, double pAng, double thickness)
    {
        cPointer(cx, cy, r, pr, ang, pAng);
        Stroke(thickness);
    }
    
    void Line(double x1, double y1, double x2, double y2, double thickness)
    {
        mGraphics.PathLine(x1, y1, x2, y2);
        SetShapeGradient(HISSTools_Bounds(std::min(x1, x2), std::min(y1, y2), std::abs(x1 - x2), std::abs(y1 - y2)));
        Stroke(thickness);
    }
    
    void Text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
        IText textSpec(pTxt->mSize, mColor->GetColor(), pTxt->mFont, (EAlign) hAlign, (EVAlign) vAlign, 0);
        HISSTools_Bounds rect(x, y, w, h);
        mGraphics.DrawText(textSpec, str, rect);
        
        SetShapeGradient(rect);
    }
    
    static double GetTextLineHeight(HISSTools_Text *pTxt)
    {
        return pTxt->mSize;
    }
    
    void StartShadow(HISSTools_Shadow *shadow, const IRECT& rect)
    {
        mShadow = shadow;
        StartGroup(rect);
    }
    
    void RenderShadow(bool renderImage = true)
    {
        ILayerPtr shadowLayer = EndGroup();
        
        // Check there is a shadow specified (otherwise only render original image)
        
        if (mShadow)
        {
            IShadow shadow(*mShadow);
            shadow.mDrawForeground = renderImage;
            mGraphics.ApplyLayerDropShadow(shadowLayer, shadow);
        }
        
        mGraphics.DrawLayer(shadowLayer);
    }
    
private:
    
    void Fill()
    {
        mGraphics.PathFill(mColor->GetPattern());
    }
    
    void Stroke(double thickness)
    {
        mGraphics.PathStroke(mColor->GetPattern(), thickness);
    }
    
    double SanitizeRadius(double r, double w, double h)
    {
        r = r < 0 ? 0 : r;
        r = ((r * 2.0) > w) ? w / 2.0: r;
        r = ((r * 2.0) > h) ? h / 2.0: r;
        
        return r;
    }
    
    void Arc(double cx, double cy, double r, double begAng, double arcAng)
    {
        begAng = begAng * 360.0 + 90.f;
        arcAng = begAng + (arcAng * 360.0);
        
        mGraphics.PathArc(cx, cy, r, std::min(begAng, arcAng), std::max(arcAng, begAng));
        SetShapeGradient(HISSTools_Bounds(cx - r, cy - r, 2.0 * r, 2.0 * r));
    }
    
    void Rectangle(double x, double y, double w, double h)
    {
        HISSTools_Bounds r(x, y, w, h);
        mGraphics.PathRect(r);
        SetShapeGradient(r);
    }
    
    void RoundedRectangle(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        rtl = SanitizeRadius(rtl, w, h);
        rtr = SanitizeRadius(rtr, w, h);
        rbl = SanitizeRadius(rbl, w, h);
        rbr = SanitizeRadius(rbr, w, h);
        
        HISSTools_Bounds r(x, y, w, h);
        mGraphics.PathRoundRect(r, rtl, rtr, rbl, rbr);
        SetShapeGradient(r);
    }
    
    void cPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        double xx = cx + cos(2.0 * PI * ang) * pr;
        double yy = cy + sin(2.0 * PI * ang) * pr;
        
        double begAng = (ang - pAng) * 360.0 + 90.f;
        double arcAng = (pAng * 2.0 * 360.0) + begAng;
        
        mGraphics.PathClear();
        mGraphics.PathArc(cx, cy, r, arcAng, begAng);
        mGraphics.PathLineTo(xx, yy);
        mGraphics.PathClose();
        
        // FIX - revise...
        SetShapeGradient(HISSTools_Bounds(cx - pr, cy - pr, 2.0 * pr, 2.0 * pr));
    }
    
    void Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        mGraphics.PathTriangle(x1, y1, x2, y2, x3, y3);
        double l = std::min(x1, std::min(x2, x3));
        double r = std::max(x1, std::max(x2, x3));
        double t = std::min(y1, std::min(y2, y3));
        double b = std::max(y1, std::max(y2, y3));
        SetShapeGradient(HISSTools_Bounds(l, t, r - l, b - t));
    }
    
    void SetShapeGradient(const HISSTools_Bounds& r)
    {
        mColor->SetRect(mForceGradientBox ? mGradientArea : r, mCSOrientation);
    }
    
    // IGraphics
    
    IGraphics& mGraphics;
    
    // Gradients
    
    HISSTools_Bounds mGradientArea;
    bool mForceGradientBox;
    ColorOrientation mCSOrientation;
    
    // Other State (line thickness, color and shadow
    
    double mMultiLineThickness = 1.0;
    HISSTools_Color_Spec *mColor;
    HISSTools_Shadow *mShadow;
};

