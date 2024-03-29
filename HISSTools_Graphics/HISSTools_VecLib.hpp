
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
    
    void setClip()
    {
        mGraphics.PathClipRegion();
    }
    
    void setClip(HISSTools_Bounds clip)
    {
        mGraphics.PathClipRegion(clip);
    }
    
    void setClip(double xLo, double yLo, double xHi, double yHi)
    {
        setClip(HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo));
    }
    
    void setClip(const IRECT& rect)
    {
        setClip(rect.L, rect.T, rect.R, rect.B);
    }
    
    void startGroup(const IRECT& r)
    {
        mGraphics.StartLayer(nullptr, r);
    }
    
    ILayerPtr endGroup()
    {
        return mGraphics.EndLayer();
    }
    
    void renderGroup(const ILayerPtr& layer)
    {
        mGraphics.DrawLayer(layer);
    }
    
    void setColor(HISSTools_Color_Spec *color)
    {
        mColor = color;
    }
    
    // Orientation allows gradient rotation ONLY for relevant Color Specs
    
    void setColorOrientation(ColorOrientation CSOrientation)
    {
        mCSOrientation = CSOrientation;
    }
    
    void forceGradientBox()     { mForceGradientBox = false; }
    
    void forceGradientBox(double xLo, double yLo, double xHi, double yHi)
    {
        mGradientArea = HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo);
        mForceGradientBox = true;
    }
    
    void startMultiLine(double x, double y, double thickness)
    {
        mMultiLineThickness = thickness;
        mGraphics.PathMoveTo(x, y);
    }
    
    void continueMultiLine(double x, double y)
    {
        mGraphics.PathLineTo(x, y);
    }
    
    void finishMultiLine()
    {
        stroke(mMultiLineThickness);
    }
    
    void circleIntersection(double cx, double cy, double ang, double r, double *retX, double *retY)
    {
        *retX = cos(2.0 * ang * PI) * r + cx;
        *retY = sin(2.0 * ang * PI) * r + cy;
    }
    
    void frameArc(double cx, double cy, double r, double begAng, double arcAng, double thickness)
    {
        arc(cx, cy, r, begAng, arcAng);
        stroke(thickness);
    }
    
    void fillArc(double cx, double cy, double r, double begAng, double arcAng)
    {
        mGraphics.PathClear();
        arc(cx, cy, r, begAng, arcAng);
        mGraphics.PathLineTo(cx, cy);
        mGraphics.PathClose();
        fill();
    }
    
    void fillCircle(double cx, double cy, double r)
    {
        mGraphics.PathCircle(cx, cy, r);
        setShapeGradient(HISSTools_Bounds(cx - r, cy - r, 2.0 * r, 2.0 * r));
        fill();
    }
    
    void frameCircle(double cx, double cy, double r, double thickness)
    {
        frameArc(cx, cy, r, 0.0, 1.0, thickness);
    }
    
    void frameTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double thickness)
    {
        triangle(x1, y1, x2, y2, x3, y3);
        stroke(thickness);
    }
    
    void fillTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        triangle(x1, y1, x2, y2, x3, y3);
        fill();
    }
    
    void fillRect(double x, double y, double w, double h)
    {
        rectangle(x, y, w, h);
        fill();
    }
    
    void frameRect(double x, double y, double w, double h, double thickness)
    {
        rectangle(x, y, w, h);
        stroke(thickness);
    }
    
    void fillRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        roundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        fill();
    }
    
    void frameRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr, double thickness)
    {
        roundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        stroke(thickness);
    }
    
    void fillRoundRect(double x, double y, double w, double h, double r)
    {
        fillRoundRect(x, y, w, h, r, r, r, r);
    }
    
    void frameRoundRect(double x, double y, double w, double h, double r, double thickness)
    {
        frameRoundRect(x, y, w, h, r, r, r, r, thickness);
    }
    
    void fillCPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        cPointer(cx, cy, r, pr, ang, pAng);
        fill();
    }
    
    void frameCPointer(double cx, double cy, double r, double pr, double ang, double pAng, double thickness)
    {
        cPointer(cx, cy, r, pr, ang, pAng);
        stroke(thickness);
    }
    
    void line(double x1, double y1, double x2, double y2, double thickness)
    {
        mGraphics.PathLine(x1, y1, x2, y2);
        setShapeGradient(HISSTools_Bounds(std::min(x1, x2), std::min(y1, y2), std::abs(x1 - x2), std::abs(y1 - y2)));
        stroke(thickness);
    }
    
    void text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
        IText textSpec(pTxt->mSize, mColor->getColor(), pTxt->mFont, (EAlign) hAlign, (EVAlign) vAlign, 0);
        HISSTools_Bounds rect(x, y, w, h);
        mGraphics.DrawText(textSpec, str, rect);
        
        setShapeGradient(rect);
    }
    
    static double getTextLineHeight(HISSTools_Text *pTxt)
    {
        return ::getLineHeight(pTxt);
    }
    
    void startShadow(HISSTools_Shadow *shadow, const IRECT& rect)
    {
        mShadow = shadow;
        startGroup(rect);
    }
    
    void renderShadow(bool renderImage = true)
    {
        ILayerPtr shadowLayer = endGroup();
        
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
    
    void fill()
    {
        mGraphics.PathFill(mColor->getPattern());
    }
    
    void stroke(double thickness)
    {
        mGraphics.PathStroke(mColor->getPattern(), thickness);
    }
    
    double sanitizeRadius(double r, double w, double h)
    {
        r = r < 0 ? 0 : r;
        r = ((r * 2.0) > w) ? w / 2.0: r;
        r = ((r * 2.0) > h) ? h / 2.0: r;
        
        return r;
    }
    
    void arc(double cx, double cy, double r, double begAng, double arcAng)
    {
        begAng = begAng * 360.0 + 90.f;
        arcAng = begAng + (arcAng * 360.0);
        
        mGraphics.PathArc(cx, cy, r, std::min(begAng, arcAng), std::max(arcAng, begAng));
        setShapeGradient(HISSTools_Bounds(cx - r, cy - r, 2.0 * r, 2.0 * r));
    }
    
    void rectangle(double x, double y, double w, double h)
    {
        HISSTools_Bounds r(x, y, w, h);
        mGraphics.PathRect(r);
        setShapeGradient(r);
    }
    
    void roundedRectangle(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        rtl = sanitizeRadius(rtl, w, h);
        rtr = sanitizeRadius(rtr, w, h);
        rbl = sanitizeRadius(rbl, w, h);
        rbr = sanitizeRadius(rbr, w, h);
        
        HISSTools_Bounds r(x, y, w, h);
        mGraphics.PathRoundRect(r, rtl, rtr, rbl, rbr);
        setShapeGradient(r);
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
        setShapeGradient(HISSTools_Bounds(cx - pr, cy - pr, 2.0 * pr, 2.0 * pr));
    }
    
    void triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        mGraphics.PathTriangle(x1, y1, x2, y2, x3, y3);
        double l = std::min(x1, std::min(x2, x3));
        double r = std::max(x1, std::max(x2, x3));
        double t = std::min(y1, std::min(y2, y3));
        double b = std::max(y1, std::max(y2, y3));
        setShapeGradient(HISSTools_Bounds(l, t, r - l, b - t));
    }
    
    void setShapeGradient(const HISSTools_Bounds& r)
    {
        mColor->setRect(mForceGradientBox ? mGradientArea : r, mCSOrientation);
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
