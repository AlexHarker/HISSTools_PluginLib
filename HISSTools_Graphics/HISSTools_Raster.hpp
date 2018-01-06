

#ifndef __HISSTOOLS_RASTER__
#define __HISSTOOLS_RASTER__

#include "HISSTools_Color.hpp"
#include "HISSTools_Shadow.hpp"
#include "HISSTools_LICE_Text.hpp"

#include <algorithm>
#include <vector>
#include "cairo/cairo.h"

static HISSTools_Color_Spec defaultColor;

class HISSTools_Raster
{
    struct Area
    {
        Area() : x1(0), x2(0), y1(0), y2(0) {}
        Area(double xL, double xH, double yL, double yH) : x1(xL), x2(xH), y1(yL), y2(yH) {}
        
        double clipX(double x) { return std::min(x2, std::max(x1, x)); }
        double clipY(double y) { return std::min(y2, std::max(y1, y)); }
        
        double x1, x2, y1, y2;
    };
    
private:

    LICE_SysBitmap mTextBitmap;
    
    cairo_t *mContext;

	// Boundaries
	
    int mWidth, mHeight;
	Area mDrawArea;
	Area mGradientArea;
	
	// Forced Gradient Bounds Flag
	
	bool mForceGradientBox;
	ColorOrientation mCSOrientation; 
	
	// Color
	
	HISSTools_Color_Spec *mColor;
	
	// Shadow
	
	HISSTools_Shadow *mShadow;
	
    std::vector <unsigned char> mBlurTempAlpha1;
    std::vector <unsigned char> mBlurTempAlpha2;

protected:
	
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	// Request clipping with negative values is equivalent to turning clipping off for that edge
	
    HISSTools_Raster(cairo_t *cairo) : mContext(cairo), mShadow(NULL), mWidth(0), mHeight(0), mForceGradientBox(false), mCSOrientation(kCSOrientHorizontal)
	{
		setColor(&defaultColor);
	}
	
    void setSize(int w, int h)
    {
        mWidth = w;
        mHeight = h;
    }
    
    void setContext(cairo_t *context)
    {
        // FIX - clip and other state etc. when pushing and popping?
        
        mContext = context;
        cairo_set_operator(mContext, CAIRO_OPERATOR_OVER);
    }
		
    cairo_t *getContext() const { return mContext; }
    
    HISSTools_Color_Spec *getColorSpec() const { return mColor; }

    
    void setClip()  { cairo_reset_clip(mContext); }
    
	void setClip(double xLo, double yLo, double xHi, double yHi)
	{
        //cairo_reset_clip(mContext);
        cairo_rectangle(mContext, xLo, yLo, xHi - xLo, yHi - yLo);
        cairo_clip(mContext);
	}
	
	void setColor(HISSTools_Color_Spec *color)
	{
		mColor = color;
                
        if (getContext())
            mColor->setAsSource(getContext());
	}
	
	void forceGradientBox()     { mForceGradientBox = false; }
	
	void forceGradientBox(double xLo, double yLo, double xHi, double yHi)
	{
        mGradientArea = Area(xLo, xHi, yLo, yHi);
		mForceGradientBox = true;
	}
	
	// Orientation allows gradient rotation ONLY for relevant Color Specs
	
	void setColorOrientation(ColorOrientation CSOrientation)
	{
		mCSOrientation = CSOrientation;
	}
	
	void startShadow(HISSTools_Shadow *shadow)
	{
        mShadow = shadow;
        cairo_push_group(getContext());
        
		// Reset draw boundaries for shadow calculation
		
        mDrawArea = Area(std::numeric_limits<double>::infinity(), 0, numeric_limits<double>::infinity(), 0);
	}
    
	void renderShadow(bool renderImage, double scale)
	{
        // Sanity Check
        
        if (mDrawArea.x1 > mDrawArea.x2 || mDrawArea.y1 > mDrawArea.y2)
            return;
        
        cairo_pattern_t *shadowRender = cairo_pop_group(getContext());
        
        // Check there is a shadow specified (otherwise only render original image)

		if (mShadow)
		{
            mShadow->setScaling(scale);
            
            int kernelSize = mShadow->getKernelSize();
            
            int xLo = floor(mDrawArea.x1 * scale);
            int xHi = ceil(mDrawArea.x2 * scale);
            int yLo = floor(mDrawArea.y1 * scale);
            int yHi = ceil(mDrawArea.y2 * scale);
			int width = (xHi - xLo) + (2 * kernelSize - 1);
			int height = (yHi - yLo) + (2 * kernelSize - 1);
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
            cairo_translate(maskContext, (kernelSize - 1 - xLo) / scale, (kernelSize - 1 - yLo) / scale);
            cairo_set_source(maskContext, shadowRender);
            cairo_paint(maskContext);
            cairo_destroy(maskContext);
            
			// Do blur
			
			mShadow->blur(&mBlurTempAlpha1[0], &mBlurTempAlpha2[0], width, height, alphaSurfaceStride);

            // Draw shadow in correct place and color
            
            cairo_save(mContext);
            mShadow->getShadowColor()->setAsSource(mContext);
            cairo_scale(mContext, 1.0/scale, 1.0/scale);
            cairo_mask_surface(mContext, mask, mShadow->getXOffset() * scale + ((xLo - (kernelSize - 1))), mShadow->getYOffset() * scale + ((yLo - (kernelSize - 1))));
            cairo_restore(mContext);
            cairo_surface_destroy(mask);
		}
		
		// Render pattern
        
		if (renderImage)
        {
            cairo_set_source(getContext(), shadowRender);
            cairo_paint_with_alpha(getContext(), 1.0);
        }

        cairo_pattern_destroy(shadowRender);
	}
    
    void text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, double scale, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
        LICE_IBitmap *bitmap = &mTextBitmap;
        
        int width = getWidth() * scale;
        int height = getHeight() * scale;
        
        bitmap->resize(width, height);
        LICE_Clear(bitmap, 0);
        
        HISSTools_LICE_Text::text(bitmap,pTxt, str, x, y, w, h, scale, hAlign, vAlign);
        
        updateDrawBounds(floor(x), ceil(x + w) - 1, floor(y), ceil(y + h) - 1, true);
         
        Area clip(x, x + w, y, y + h);
        
        cairo_save(mContext);
        setClip(clip.x1, clip.y1, clip.x2, clip.y2);
        int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
        cairo_surface_t *surface = cairo_image_surface_create_for_data((unsigned char *) bitmap->getBits(), CAIRO_FORMAT_ARGB32, width, height, stride);
        cairo_scale(mContext, 1.0/scale, 1.0/scale);
        cairo_mask_surface(mContext, surface, 0, 0);
        cairo_restore(mContext);
        cairo_surface_destroy(surface);
    }
    
    void updateDrawBounds(bool fill, bool useExtents)
    {
        double xLo, xHi, yLo, yHi;
        
        if (fill)
            cairo_fill_extents(mContext, &xLo, &yLo, &xHi, &yHi);
        else
            cairo_stroke_extents(mContext, &xLo, &yLo, &xHi, &yHi);
        
        updateDrawBounds(xLo, xHi, yLo, yHi, useExtents);
    }
    
    void updateDrawBounds(double xLo, double xHi, double yLo, double yHi, bool useExtents)
    {
        mDrawArea.x1 = std::min(xLo, mDrawArea.x1);
        mDrawArea.x2 = std::max(xHi, mDrawArea.x2);
        mDrawArea.y1 = std::min(yLo, mDrawArea.y1);
        mDrawArea.y2 = std::max(yHi, mDrawArea.y2);
        
        if (useExtents)
            setShapeGradient(xLo, xHi, yLo, yHi);
    }
    
    void setShapeGradient(double xLo, double xHi, double yLo, double yHi)
    {
        if (mForceGradientBox == false)
            mColor->setRect(xLo, xHi, yLo, yHi, mCSOrientation);
        else
            mColor->setRect(mGradientArea.x1, mGradientArea.x2, mGradientArea.y1, mGradientArea.y2, mCSOrientation);
        
        setColor(mColor);
    }
    
private:
    
    // Drop Shadow
    
    unsigned char getAlpha(int x, int y, unsigned char *data, int stride)
    {
        return data[y * stride + x];
    }
    
    int getHeight() const   { return mHeight; }
    int getWidth() const    { return mWidth; }
    
};

#endif /* __HISSTOOLS_LICE_RASTER_CAIRO__ */
