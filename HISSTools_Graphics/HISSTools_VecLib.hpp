

#ifndef __HISSTOOLS_VECLIB__
#define __HISSTOOLS_VECLIB__

#include "IGraphics.h"
#include "HISSTools_Shadow.hpp"
#include "HISSTools_Color.hpp"
#include <algorithm>
#include <vector>
#include "cairo/cairo.h"
#include <cmath>

#define USE_IGRAPHICS_TEXT
#include "HISSTools_LICE_Text.hpp"

static HISSTools_Color_Spec defaultColor;

class HISSTools_VecLib
{
    double radToDeg(double radians) { return (180.0 * (radians / PI)) + 90.0; }
    
    struct Area
    {
        Area() : x1(0), x2(0), y1(0), y2(0) {}
        Area(double xL, double xH, double yL, double yH) : x1(xL), x2(xH), y1(yL), y2(yH) {}
    
        double x1, x2, y1, y2;
    };
    
public:
	
    HISSTools_VecLib() : mGraphics(nullptr), mShadow(NULL), mForceGradientBox(false), mCSOrientation(kCSOrientHorizontal)
    {
        setColor(&defaultColor);
#ifndef USE_IGRAPHICS_TEXT
        mTextBitmap = nullptr;
        mWidth = 0;
        mHeight = 0;
#endif
    }
    
    ~HISSTools_VecLib()
    {
#ifndef USE_IGRAPHICS_TEXT
        delete mTextBitmap;
#endif
    }
    
    void setIGraphics(IGraphics* graphics)
    {
        mGraphics = graphics;
        
        // FIX - clip and other state etc. when pushing and popping?
        
        cairo_set_operator(getContext(), CAIRO_OPERATOR_OVER);
        
#ifndef USE_IGRAPHICS_TEXT
        mWidth = graphics->Width();
        mHeight = graphics->Height();
#endif
    }
    
    void setClip()  { cairo_reset_clip(getContext()); }
    
    void setClip(HISSTools_Bounds clip)
    {
        //cairo_reset_clip(getContext());
        cairo_rectangle(getContext(), clip.mRECT.L, clip.mRECT.T, clip.mRECT.W(), clip.mRECT.H());
        cairo_clip(getContext());
    }
    
    void setClip(double xLo, double yLo, double xHi, double yHi)
    {
        setClip(HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo));
    }
    
    void setClip(IRECT rect)
    {
        setClip(rect.L, rect.T, rect.R, rect.B);
    }
    
    void startGroup()
    {
        cairo_push_group(getContext());
    }
    
    cairo_pattern_t *endGroup()
    {
        return cairo_pop_group(getContext());
    }
    
    void renderPattern(cairo_pattern_t *pattern)
    {
        cairo_set_source(getContext(), pattern);
        cairo_paint_with_alpha(getContext(), 1.0);
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
        mGradientArea = Area(xLo, xHi, yLo, yHi);
        mForceGradientBox = true;
    }
    /*
    double getX() const
    {
        double x, y;
        
        cairo_get_current_point(mContext, &x, &y);
        
        return x;
    }
    
    double getY() const
    {
        double x, y;
        
        cairo_get_current_point(mContext, &x, &y);
        
        return y;
    }*/
   
    void startMultiLine(double x, double y, double thickness)
    {
        mMultiLineThickness = thickness;
        mGraphics->PathStart();
        mGraphics->PathMoveTo(x, y);
    }
    
    void continueMultiLine(double x, double y)
    {
        mGraphics->PathLineTo(x, y);
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
        mGraphics->PathStart();
        arc(cx, cy, r, begAng, arcAng);
        mGraphics->PathLineTo(cx, cy);
        mGraphics->PathClose();
        fill();
    }

    void fillCircle(double cx, double cy, double r)
    {
        fillArc(cx, cy, r, 0.0, 1.0);
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
        mGraphics->PathLine(x1, y1, x2, y2);
        stroke(thickness, true);
    }
    
    void text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
#ifdef USE_IGRAPHICS_TEXT
        IText textSpec(pTxt->mSize, mColor->getColor(), pTxt->mFont, (IText::EStyle) pTxt->mStyle, (IText::EAlign) hAlign, (IText::EVAlign) vAlign, 0, IText::kQualityAntiAliased);
        IRECT rect(x, y, x + w, y + h);
        mGraphics->DrawText(textSpec, str, rect);
            
        updateDrawBounds(floor(x), ceil(x + w) - 1, floor(y), ceil(y + h) - 1, true);
#else
        double scale = mGraphics->GetDisplayScale();
        LICE_IBitmap *bitmap = mTextBitmap;
        
        int width = mWidth * scale;
        int height = mHeight * scale;
        
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

        HISSTools_Bounds clip(x, y, w, h);
        
        cairo_save(getContext());
        setClip(clip);
        int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
        cairo_surface_t *surface = cairo_image_surface_create_for_data((unsigned char *) bitmap->getBits(), CAIRO_FORMAT_ARGB32, width, height, stride);
        cairo_scale(getContext(), 1.0/scale, 1.0/scale);
        cairo_mask_surface(getContext(), surface, 0, 0);
        cairo_restore(getContext());
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
    
    void startShadow(HISSTools_Shadow *shadow)
    {
        double x1, x2, y1, y2;
        
        mShadow = shadow;
        mDrawArea = HISSTools_Bounds();
        cairo_save(getContext());
        cairo_clip_extents(getContext(), &x1, &y1, &x2, &y2);
        HISSTools_Bounds clip(x1, y1, x2 - x1, y2 - y1);
        double enlargeBy = (shadow->getBlurSize() + 2) * 2;
        clip.include(HISSTools_Bounds(clip.mRECT.L - shadow->getXOffset(), clip.mRECT.T - shadow->getYOffset(), clip.mRECT.W() + enlargeBy, clip.mRECT.W() + enlargeBy));
        setClip();
        setClip(clip);
        startGroup();
    }
    
    void renderShadow(bool renderImage = true)
    {
        // Sanity Check
        
        if (mDrawArea.mRECT.Empty())
            return;
        
        cairo_pattern_t *shadowRender = endGroup();
        cairo_restore(getContext());
        
        // Check there is a shadow specified (otherwise only render original image)
        
        if (mShadow)
        {
            double scale = mGraphics->GetDisplayScale();
            mShadow->setScaling(scale);
            
            int kernelSize = mShadow->getKernelSize();
            
            HISSTools_Bounds bounds = mDrawArea;
            bounds.mRECT.Scale(scale
                               );
            IRECT draw = bounds.iBounds();
           
            int width = (draw.R - draw.L) + (2 * kernelSize - 1);
            int height = (draw.B - draw.T) + (2 * kernelSize - 1);
            int alphaSurfaceStride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, width);
            
            // Alloc and zero blur memory
            
            mBlurTempAlpha1.resize(alphaSurfaceStride * height);
            mBlurTempAlpha2.resize(alphaSurfaceStride * height);
            std::fill(mBlurTempAlpha1.begin(), mBlurTempAlpha1.end(), 0);
            std::fill(mBlurTempAlpha2.begin(), mBlurTempAlpha2.end(), 0);
            
            // Copy alpha values from the temp bitmap
            
            cairo_surface_t *mask = cairo_image_surface_create_for_data(&mBlurTempAlpha1[0], CAIRO_FORMAT_A8, width, height, alphaSurfaceStride);
            cairo_t* maskContext = cairo_create(mask);
            cairo_scale(maskContext, scale, scale);
            cairo_translate(maskContext, (kernelSize - 1 - draw.L) / scale, (kernelSize - 1 - draw.T) / scale);
            cairo_set_source(maskContext, shadowRender);
            cairo_paint(maskContext);
            cairo_destroy(maskContext);
            
            // Do blur
            
            mShadow->blur(&mBlurTempAlpha1[0], &mBlurTempAlpha2[0], width, height, alphaSurfaceStride);
            
            // Draw shadow in correct place and color
            
            cairo_save(getContext());
            mShadow->getShadowColor()->setAsSource(getContext());
            cairo_scale(getContext(), 1.0/scale, 1.0/scale);
            cairo_mask_surface(getContext(), mask, mShadow->getXOffset() * scale + ((draw.L - (kernelSize - 1))), mShadow->getYOffset() * scale + ((draw.T - (kernelSize - 1))));
            cairo_restore(getContext());
            cairo_surface_destroy(mask);
        }
        
        // Render pattern
        
        if (renderImage)
            renderPattern(shadowRender);
        
        cairo_pattern_destroy(shadowRender);
    }
    
private:
    
    void fill(bool useExtents = false)
    {
        updateDrawBounds(true, useExtents);
        mGraphics->PathFill(mColor->getPattern());
    }
    
    void stroke(double thickness, bool useExtents = false)
    {
        updateDrawBounds(false, useExtents);
        mGraphics->PathStroke(mColor->getPattern(), thickness);
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
        begAng = begAng * 360.0;
        arcAng = begAng + (arcAng * 360.0);

        mGraphics->PathArc(cx, cy, r, std::min(begAng, arcAng), std::max(arcAng, begAng));
        setShapeGradient(cx - r, cx + r, cy - r, cy + r);
    }
    
    void rectangle(double x, double y, double w, double h)
    {
        mGraphics->PathRect(IRECT(x, y, x + w, y + h));
        setShapeGradient(x, x + w, y, y + h);
    }
    
    void roundedRectangle(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        rtl = sanitizeRadius(rtl, w, h);
        rtr = sanitizeRadius(rtr, w, h);
        rbl = sanitizeRadius(rbl, w, h);
        rbr = sanitizeRadius(rbr, w, h);
        
        mGraphics->PathStart();
        mGraphics->PathArc(x + rtl, y + rtl, rtl, 180.0,  270.0);
        mGraphics->PathArc(x + w - rtr, y + rtr, rtr, 270.0,  360.0);
        mGraphics->PathArc(x + w - rbr, y + h - rbr, rbr, 0.0, 90.0);
        mGraphics->PathArc(x + rbl, y + h - rbl, rbl, 90.0, 180.0);
        mGraphics->PathClose();
        setShapeGradient(x, x + w, y, y + h);
    }
    
    void cPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        double xx = cx + cos(2.0 * PI * ang) * pr;
        double yy = cy + sin(2.0 * PI * ang) * pr;
        
        double begAng = (ang - pAng) * 360.0;
        double arcAng = (pAng * 2.0 * 360.0) + begAng;
        
        mGraphics->PathStart();
        mGraphics->PathArc(cx, cy, r, arcAng, begAng);
        mGraphics->PathLineTo(xx, yy);
        mGraphics->PathClose();

        // FIX - revise...
        setShapeGradient(cx - pr, cx + pr, cy - pr, cy + pr);
    }
    
    void triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        mGraphics->PathTriangle(x1, y1, x2, y2, x3, y3);
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
        // FIX - account for clip!
        
        mDrawArea.include(HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo));
        
        if (useExtents)
            setShapeGradient(xLo, xHi, yLo, yHi);
    }
    
    void setShapeGradient(double xLo, double xHi, double yLo, double yHi)
    {
        if (mForceGradientBox)
            mColor->setRect(mGradientArea.x1, mGradientArea.x2, mGradientArea.y1, mGradientArea.y2, mCSOrientation);
        else
            mColor->setRect(xLo, xHi, yLo, yHi, mCSOrientation);
    }
    
    cairo_t *getContext() const { return mGraphics ? (cairo_t *) mGraphics->GetDrawContext() : nullptr; }
    
    IGraphics* mGraphics;
    
#ifndef USE_IGRAPHICS_TEXT
    int mWidth, mHeight;
    LICE_SysBitmap *mTextBitmap;
#endif
    
    // Boundaries
    
    HISSTools_Bounds mDrawArea;
    Area mGradientArea;
    
    // Forced Gradient Bounds Flag
    
    bool mForceGradientBox;
    ColorOrientation mCSOrientation;
    
    double mMultiLineThickness = 1.0;
    
    // Color
    
    HISSTools_Color_Spec *mColor;
    
    // Shadow
    
    HISSTools_Shadow *mShadow;
    
    std::vector <unsigned char> mBlurTempAlpha1;
    std::vector <unsigned char> mBlurTempAlpha2;
};

#endif /* __HISSTOOLS_VECLIB__ */
