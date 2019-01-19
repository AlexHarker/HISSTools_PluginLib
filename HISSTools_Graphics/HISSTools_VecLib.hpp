
#pragma once

#include "IGraphics.h"
#include "HISSTools_VecLib_Structs.hpp"
#include <algorithm>
#include <vector>
#include "cairo/cairo.h"
#include <cmath>

#define USE_IGRAPHICS_TEXT

#include "HISSTools_LICE_Text.hpp"

static HISSTools_Color_Spec defaultColor;

class HISSTools_VecLib
{
    
public:
    
    HISSTools_VecLib(IGraphics& graphics) : mGraphics(graphics), mShadow(nullptr), mForceGradientBox(false), mColor(&defaultColor), mCSOrientation(kCSOrientHorizontal)
    {
#ifndef USE_IGRAPHICS_TEXT
        mTextBitmap = nullptr;
#endif
    }
    
    ~HISSTools_VecLib()
    {
#ifndef USE_IGRAPHICS_TEXT
        delete mTextBitmap;
#endif
    }
    
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
    
    void setClip(IRECT rect)
    {
        setClip(rect.L, rect.T, rect.R, rect.B);
    }
    
    void startGroup(const IRECT& r)
    {
        mGraphics.StartLayer(r);
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
        mGradientArea = IRECT(xLo, xHi, yLo, yHi);
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
        fill(true);
    }
    
    void frameCircle(double cx, double cy, double r, double thickness)
    {
        frameArc(cx, cy, r, 0.0, 1.0, thickness);
    }
    
    void frameTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        triangle(x1, y1, x2, y2, x3, y3);
        stroke(true);
    }
    
    void fillTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        triangle(x1, y1, x2, y2, x3, y3);
        fill(true);
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
        stroke(thickness, true);
    }
    
    void text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
#ifdef USE_IGRAPHICS_TEXT
        IText textSpec(pTxt->mSize, mColor->getColor(), pTxt->mFont, (IText::EStyle) pTxt->mStyle, (IText::EAlign) hAlign, (IText::EVAlign) vAlign, 0, IText::kQualityAntiAliased);
        IRECT rect(x, y, x + w, y + h);
        mGraphics.DrawText(textSpec, str, rect);
        
        updateDrawBounds(floor(x), ceil(x + w) - 1, floor(y), ceil(y + h) - 1, true);
#else
        double scale = mGraphics->GetDisplayScale();
        LICE_IBitmap *bitmap = mTextBitmap;
        
        int width = mGraphics->GetWidth() * scale;
        int height = mGraphics->GetHeight() * scale;
        
        // This allows the window to be any size...
        
        width = (width + 3 ) &~ 3;
        
        if (!bitmap || bitmap->getWidth() != width || bitmap->getHeight() != height)
        {
            delete mTextBitmap;
            bitmap = mTextBitmap = new LICE_SysBitmap(width, height);
        }
        
        LICE_Clear(bitmap, 0);
        HISSTools_LICE_Text::text(bitmap, pTxt, str, x, y, w, h, scale, hAlign, vAlign);
        
        updateDrawBounds(floor(x), ceil(x + w) - 1, floor(y), ceil(y + h) - 1, true);
        
        mGraphics->PathStateSave();
        setClip(HISSTools_Bounds(x, y, w, h));
        int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
        cairo_surface_t *surface = cairo_image_surface_create_for_data((unsigned char *) bitmap->getBits(), CAIRO_FORMAT_ARGB32, width, height, stride);
        mGraphics->PathTransformScale(1.0/scale);
        cairo_mask_surface(getContext(), surface, 0, 0);
        mGraphics->PathStateRestore();
        cairo_surface_destroy(surface);
#endif
    }
    
    static double getTextLineHeight(HISSTools_Text *pTxt)
    {
#ifdef USE_IGRAPHICS_TEXT
        return pTxt->mSize;
#else
        return HISSTools_LICE_Text::getTextLineHeight(pTxt);
#endif
    }
    
    void startShadow(HISSTools_Shadow *shadow, IRECT rect)
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
    
    void fill(bool useExtents = false)
    {
        updateDrawBounds(true, useExtents);
        mGraphics.PathFill(mColor->getPattern());
    }
    
    void stroke(double thickness, bool useExtents = false)
    {
        updateDrawBounds(false, useExtents);
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
        setShapeGradient(cx - r, cx + r, cy - r, cy + r);
    }
    
    void rectangle(double x, double y, double w, double h)
    {
        mGraphics.PathRect(IRECT(x, y, x + w, y + h));
        setShapeGradient(x, x + w, y, y + h);
    }
    
    void roundedRectangle(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        rtl = sanitizeRadius(rtl, w, h);
        rtr = sanitizeRadius(rtr, w, h);
        rbl = sanitizeRadius(rbl, w, h);
        rbr = sanitizeRadius(rbr, w, h);
        
        mGraphics.PathMoveTo(x, y + rtl);
        mGraphics.PathArc(x + rtl, y + rtl, rtl, 270.0,  360.0);
        mGraphics.PathArc(x + w - rtr, y + rtr, rtr, 0.0,  90.0);
        mGraphics.PathArc(x + w - rbr, y + h - rbr, rbr, 90.0, 180.0);
        mGraphics.PathArc(x + rbl, y + h - rbl, rbl, 180.0, 270.0);
        mGraphics.PathClose();
        setShapeGradient(x, x + w, y, y + h);
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
        setShapeGradient(cx - pr, cx + pr, cy - pr, cy + pr);
    }
    
    void triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        mGraphics.PathTriangle(x1, y1, x2, y2, x3, y3);
    }
    
    void updateDrawBounds(bool fill, bool useExtents)
    {
        double xLo, xHi, yLo, yHi;
        
        if (fill)
            cairo_fill_extents(getContext(), &xLo, &yLo, &xHi, &yHi);
        else
            cairo_stroke_extents(getContext(), &xLo, &yLo, &xHi, &yHi);
        
        updateDrawBounds(xLo, xHi, yLo, yHi, useExtents);
    }
    
    void updateDrawBounds(double xLo, double xHi, double yLo, double yHi, bool useExtents)
    {
        if (useExtents)
            setShapeGradient(xLo, xHi, yLo, yHi);
    }
    
    void setShapeGradient(double xLo, double xHi, double yLo, double yHi)
    {
        if (mForceGradientBox)
            mColor->setRect(mGradientArea.L, mGradientArea.R, mGradientArea.T, mGradientArea.B, mCSOrientation);
        else
            mColor->setRect(xLo, xHi, yLo, yHi, mCSOrientation);
    }
    
    cairo_t *getContext() const { return (cairo_t *) mGraphics.GetDrawContext(); }
    
    IGraphics& mGraphics;
    
#ifndef USE_IGRAPHICS_TEXT
    LICE_SysBitmap *mTextBitmap;
#endif
    
    // Boundaries
    
    IRECT mGradientArea;
    
    // Forced Gradient Bounds Flag
    
    bool mForceGradientBox;
    ColorOrientation mCSOrientation;
    
    double mMultiLineThickness = 1.0;
    
    // Color
    
    HISSTools_Color_Spec *mColor;
    
    // Shadow
    
    HISSTools_Shadow *mShadow;
};
