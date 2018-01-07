

#ifndef __HISSTOOLS_VL__
#define __HISSTOOLS_VL__

#include "HISSTools_Raster.hpp"

#include "cairo/cairo.h"


class HISSTools_VecLib : private HISSTools_Raster
{

public:
	
	HISSTools_VecLib(cairo_t *cairo) : HISSTools_Raster(cairo), mScale(1.0)
	{}
    
    void setSize(int w, int h)
    {
        HISSTools_Raster::setSize(w, h);
    }
    
    void setClip(double xLo, double yLo, double xHi, double yHi)
    {
        HISSTools_Raster::setClip(xLo, yLo, xHi, yHi);
    }
    
    void setClip()
    {
        HISSTools_Raster::setClip();
    }
    
    void setClip(IRECT rect)
    {
        HISSTools_Raster::setClip(rect.L, rect.T, rect.R, rect.B);
    }
    
    cairo_t *getContext() const { return mContext; }
    
    void setContext(cairo_t *context, double scale)
    {
        HISSTools_Raster::setContext(context);
        mScale = scale;
    }
    
    void startGroup()
    {
        cairo_push_group(mContext);
    }
    
    cairo_pattern_t *endGroup()
    {
        return cairo_pop_group(mContext);
    }
    
    void renderPattern(cairo_pattern_t *pattern)
    {
        cairo_set_source(mContext, pattern);
        cairo_paint_with_alpha(mContext, 1.0);
    }
    
    double getScale() const { return mScale; }

    void setColor(HISSTools_Color_Spec * color)
    {
        HISSTools_Raster::setColor(color);
    }
    
    void setColorOrientation(ColorOrientation orientation)
    {
        HISSTools_Raster::setColorOrientation(orientation);
    }
    
    void forceGradientBox() { HISSTools_Raster::forceGradientBox(); }
    void forceGradientBox(double xLo, double yLo, double xHi, double yHi)
    {
        HISSTools_Raster::forceGradientBox(xLo, yLo, xHi, yHi);
    }
    
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
    }
    
    void moveTo(double x, double y)
    {
        cairo_move_to(mContext, x, y);
    }
    
    void lineTo(double x, double y)
    {
        cairo_line_to(mContext, x, y);
    }
    
    void startMultiLine(double x, double y, double thickness)
    {
        cairo_new_path(mContext);
        setLineThickness(thickness);
        moveTo(x, y);
    }
    
    void continueMultiLine(double x, double y)
    {
        lineTo(x, y);
    }
    
    void finishMultiLine()
    {
        stroke();
    }
    
    void circleIntersection(double cx, double cy, double ang, double r, double *retX, double *retY)
    {
        *retX = sin(2.0 * ang * PI) * r + cx;
        *retY = -cos(2.0 * ang * PI) * r + cy;
    }
    
    void frameArc(double cx, double cy, double r, double begAng, double arcAng, double thickness)
    {
        setLineThickness(thickness);
        arc(cx, cy, r, begAng, arcAng);
        stroke();
    }
    
    void fillArc(double cx, double cy, double r, double begAng, double arcAng)
    {
        cairo_new_path(mContext);
        arc(cx, cy, r, begAng, arcAng);
        cairo_line_to(mContext, cx, cy);
        cairo_close_path(mContext);
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
        setLineThickness(thickness);
        rectangle(x, y, w, h);
        stroke();
    }
    
    void fillRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        roundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        fill();
    }
    
    void frameRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr, double thickness)
    {
        setLineThickness(thickness);
        roundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        stroke();
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
        setLineThickness(thickness);
        cPointer(cx, cy, r, pr, ang, pAng);
        stroke();
    }
    
    void line(double x1, double y1, double x2, double y2, double thickness)
    {
        cairo_set_line_width(mContext, thickness);
        cairo_move_to(mContext, x1, y1);
        cairo_line_to(mContext, x2, y2);
        stroke(true);
    }
    
    void text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
        HISSTools_Raster::text(pTxt, str, x, y, w, h, mScale, hAlign, vAlign);
    }

    static double getTextLineHeight(HISSTools_Text *pTxt)
    {
        return HISSTools_LICE_Text::getTextLineHeight(pTxt);
    }
    
    void startShadow(HISSTools_Shadow *shadow)
    {
        HISSTools_Raster::startShadow(shadow);
    }
    
    void renderShadow(bool renderImage = true)
    {
        HISSTools_Raster::renderShadow(renderImage, getScale());
    }
    
private:
    
    void fill(bool useExtents = false)
    {
        updateDrawBounds(true, useExtents);
        cairo_fill(mContext);
    }
    
    void stroke(bool useExtents = false)
    {
        updateDrawBounds(false, useExtents);
        cairo_stroke(mContext);
    }
    
    double sanitizeRadius(double r, double w, double h)
    {
        r = r < 0 ? 0 : r;
        r = ((r * 2.0) > w) ? w / 2.0: r;
        r = ((r * 2.0) > h) ? h / 2.0: r;
        
        return r;
    }

    void setLineThickness(double thickness)
    {
        cairo_set_line_width(mContext, thickness);
    }

    void arc(double cx, double cy, double r, double begAng, double arcAng)
    {
        begAng = (begAng - 0.25) * 2.0 * M_PI;
        arcAng = (arcAng * 2.0 * M_PI) + begAng;
        
        cairo_arc(mContext, cx, cy, r, begAng, arcAng);
        setShapeGradient(cx - r, cx + r, cy - r, cy + r);
    }
    
    void rectangle(double x, double y, double w, double h)
    {
        cairo_rectangle(mContext, x, y, w, h);
        setShapeGradient(x, x + w, y, y + h);
    }
    
    void roundedRectangle(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        rtl = sanitizeRadius(rtl, w, h);
        rtr = sanitizeRadius(rtr, w, h);
        rbl = sanitizeRadius(rbl, w, h);
        rbr = sanitizeRadius(rbr, w, h);
        
        cairo_new_path(mContext);
        cairo_arc(mContext, x + rtl, y + rtl, rtl, M_PI, 3.0 * M_PI / 2.0);
        cairo_arc(mContext, x + w - rtr, y + rtr, rtr, 3.0 * M_PI / 2.0, 0.0);
        cairo_arc(mContext, x + w - rbr, y + h - rbr, rbr, 0.0, M_PI / 2.0);
        cairo_arc(mContext, x + rbl, y + h - rbl, rbl, M_PI / 2.0, M_PI);
        cairo_close_path(mContext);
        setShapeGradient(x, x + w, y, y + h);
    }
    
    void cPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        double xx = cx + sin(2.0 * PI * ang) * pr;
        double yy = cy - cos(2.0 * PI * ang) * pr;
        
        double begAng = (ang - pAng - 0.25) * 2.0 * M_PI;
        double arcAng = (pAng * 4.0 * M_PI) + begAng;
        
        cairo_new_path(mContext);
        cairo_arc_negative(mContext, cx, cy, r, begAng, arcAng);
        cairo_line_to(mContext, xx, yy);
        cairo_close_path(mContext);
        // FIX - revise...
        setShapeGradient(cx - pr, cx + pr, cy - pr, cy + pr);
    }
    
    void triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        cairo_move_to(mContext, x1, y1 );
        cairo_line_to(mContext, x2, y2);
        cairo_line_to(mContext, x3, y3);
        cairo_close_path(mContext);
    }
    
    double mScale;
};

#endif /* __HISSTOOLS_LICE_LVL_CAIRO__ */
