
#ifndef __HISSTOOLS_COLOR_SPEC__
#define __HISSTOOLS_COLOR_SPEC__

#include <lice.h>
#include "cairo/cairo.h"

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
    
    double r;
    double g;
    double b;
    double a;
};

class HISSTools_Color_Spec
{
    
public:
    
    HISSTools_Color_Spec()
    {
        mColor.r = 0;
        mColor.g = 0;
        mColor.b = 0;
        mColor.a = 1;
    }
    
    HISSTools_Color_Spec (HISSTools_Color color)
    {
        mColor = clampColor(color);
    }
    
    HISSTools_Color_Spec(double r, double g, double b, double a)
    {
        mColor = clampColor(HISSTools_Color(r, g, b, a));
    }
    
    virtual ~HISSTools_Color_Spec(){}
    
    
    virtual void setAsSource(cairo_t *cr)
    {
        cairo_set_source_rgba(cr, mColor.r, mColor.g, mColor.b, mColor.a);
    }
    
    virtual void setRect(double xLo, double xHi, double yLo, double yHi, ColorOrientation CSOrientation)
    {
        mXLo = xLo;
        mXHi = xHi;
        mYLo = yLo;
        mYHi = yHi;
        mOrientation = CSOrientation;
    }
    
protected:
    
    HISSTools_Color mColor;
    
    double mXLo, mXHi, mYLo, mYHi;
    
    ColorOrientation mOrientation;
    
    double clampValue(double x)
    {
        x = x < 0.0 ? 0.0 : x;
        x = x > 1.0 ? 1.0 : x;
        
        return x;
    }
    
    HISSTools_Color clampColor(HISSTools_Color color)
    {
        color.r = clampValue(color.r);
        color.g = clampValue(color.g);
        color.b = clampValue(color.b);
        color.a = clampValue(color.a);
        
        return color;
    }
};


class HISSTools_LICE_HVGradient : public HISSTools_Color_Spec
{
	
public:
    
    HISSTools_LICE_HVGradient(bool flipVertical = true)
    {
        mPattern = cairo_pattern_create_linear(0.0, 0.0, 1.0, 0.0);
        cairo_pattern_set_extend(mPattern, CAIRO_EXTEND_PAD);
        mFlipVertical = flipVertical;
    }
    
    ~HISSTools_LICE_HVGradient()
    {
        cairo_pattern_destroy(mPattern);
    }
	
    virtual void setAsSource(cairo_t *cr)
    {
        cairo_set_source(cr, mPattern);
    }
    
	void addStop(HISSTools_Color color, double stop)
	{
        color = clampColor(color);
        
        cairo_pattern_add_color_stop_rgba(mPattern, stop, color.r, color.g, color.b, color.a);
    }
    
    virtual void setRect(double xLo, double xHi, double yLo, double yHi, ColorOrientation CSOrientation)
    {
        HISSTools_Color_Spec::setRect(xLo, xHi, yLo, yHi, CSOrientation);
        cairo_matrix_t matrix;

        if (CSOrientation == kCSOrientHorizontal)
        {
            cairo_matrix_init_translate(&matrix, -xLo, 0.0);
            cairo_matrix_scale(&matrix, 1.0 / (xHi - xLo), 1.0);
        }
        else
        {
            cairo_matrix_init_rotate(&matrix, -M_PI / 2.0);

            if (mFlipVertical)
            {
                cairo_matrix_scale(&matrix, 1.0, -1.0 / (yHi - yLo));
                cairo_matrix_translate(&matrix, 0.0, -yHi);
            }
            else
            {
                cairo_matrix_scale(&matrix, 1.0, 1.0 / (yHi - yLo));
                cairo_matrix_translate(&matrix, 0.0, -yLo);
            }
        }
        
        cairo_pattern_set_matrix(mPattern, &matrix);
    }
    
private:
    
    cairo_pattern_t *mPattern;
    bool mFlipVertical;
};

struct HISSTools_LICE_HGradient : public HISSTools_LICE_HVGradient
{
    HISSTools_LICE_HGradient() : HISSTools_LICE_HVGradient() {}
    
    virtual void setRect(double xLo, double xHi, double yLo, double yHi, ColorOrientation CSOrientation)
    {
        HISSTools_LICE_HVGradient::setRect(xLo, xHi, yLo, yHi, kCSOrientHorizontal);
    }
};


struct HISSTools_LICE_VGradient : public HISSTools_LICE_HVGradient
{    
    HISSTools_LICE_VGradient() : HISSTools_LICE_HVGradient(false) {}

    virtual void setRect(double xLo, double xHi, double yLo, double yHi, ColorOrientation CSOrientation)
    {
        HISSTools_LICE_HVGradient::setRect(xLo, xHi, yLo, yHi, kCSOrientVertical);
    }
};


#endif /* __HISSTools_Color_Spec__ */
