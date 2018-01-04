

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
	
    std::vector <double> mBlurTempAlpha1;
    std::vector <double> mBlurTempAlpha2;
    std::vector<unsigned char> mAlphaMask;
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
		
        mDrawArea = Area(getWidth(), 0, getHeight(), 0);
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
            double xOffset = mShadow->getXOffset();
            double yOffset = mShadow->getYOffset();
            
            int xLo = floor(mDrawArea.x1 * scale);
            int xHi = ceil(mDrawArea.x2 * scale);
            int yLo = floor(mDrawArea.y1 * scale);
            int yHi = ceil(mDrawArea.y2 * scale);
			int width = (xHi - xLo) + (2 * kernelSize - 1);		// Account for blur size and fractional shift
			int height = (yHi - yLo) + (2 * kernelSize - 1);	// Account for blur size and fractional shift
			int xShift = trunc(xOffset) - (kernelSize - 1);		
			int yShift = trunc(yOffset) - (kernelSize - 1);		
			
			double alphaRecip = 1.0 / 255.0;

			// Alloc and zero blur memory
			
			int sizeRequired = width * height;
			
            mBlurTempAlpha1.resize(sizeRequired);
            mBlurTempAlpha2.resize(sizeRequired);
			std::fill(mBlurTempAlpha1.begin(), mBlurTempAlpha1.end(), 0);
            std::fill(mBlurTempAlpha2.begin(), mBlurTempAlpha2.end(), 0);

			// Copy alpha values from the temp bitmap
				
            cairo_surface_t* shadowSurface = cairo_image_surface_create(CAIRO_FORMAT_A8, getWidth() * scale, getHeight() * scale);
            cairo_t* shadowContext = cairo_create(shadowSurface);
            cairo_scale(shadowContext, scale, scale);
            cairo_set_source(shadowContext, shadowRender);
            cairo_paint(shadowContext);
            
            unsigned char *data = cairo_image_surface_get_data(shadowSurface);
            int surfaceStride = cairo_image_surface_get_stride(shadowSurface);
            
			for (int i = kernelSize - 1; i < (height - kernelSize) + 1; i++)
				for (int j = kernelSize - 1; j < (width - kernelSize) + 1; j++)
					mBlurTempAlpha2[i * width + j] = getAlpha((j + xLo - (kernelSize - 1)), (i + yLo - (kernelSize - 1)), data, surfaceStride) * alphaRecip;
			
            cairo_surface_destroy(shadowSurface);
            cairo_destroy(shadowContext);
            
			// Do blur (row, column then alpha mask with offsets)
			
			mShadow->rowBlur(&mBlurTempAlpha1[0], &mBlurTempAlpha2[0], width, height);
            mShadow->colBlur(&mBlurTempAlpha2[0], &mBlurTempAlpha1[0], width, height);
			
			// FIX - Add Fractional offset - use linear interpolation on write
			
            surfaceStride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, getWidth() * scale);
            mAlphaMask.resize(surfaceStride * scale * getHeight());
            std::fill(mAlphaMask.begin(), mAlphaMask.end(), 0);
            data = &mAlphaMask[0];
            
            Area clipExtents;
            IRECT rect;
            
            cairo_clip_extents(mContext, &clipExtents.x1, &clipExtents.y1, &clipExtents.x2, &clipExtents.y2);
            
            clipExtents.x1 = std::max(ceil(clipExtents.x1), 0.0) * scale;
            clipExtents.x2 = std::min(floor(clipExtents.x2), (double) getWidth()) * scale;
            clipExtents.y1 = std::max(ceil(clipExtents.y1), 0.0) * scale;
            clipExtents.y2 = std::min(floor(clipExtents.y2), (double) getHeight()) * scale;

            rect.L = floor(clipExtents.clipX(xLo + xShift));
            rect.R = ceil(clipExtents.clipX(xLo + xShift + width));
            rect.T = floor(clipExtents.clipY(yLo + yShift));
            rect.B = ceil(clipExtents.clipY(yLo + yShift + height));
            
            for (int i = rect.T; i < rect.B; i++)
                for (int j = rect.L; j < rect.R; j++)
                    setAlpha(j, i, mBlurTempAlpha2[(i - yLo - yShift) * width + (j - xLo - xShift)], data, surfaceStride);
            
            // Draw Shadow in correct color
            
            cairo_t *cr = getContext();
            
            cairo_save(cr);
            mShadow->getShadowColor()->setAsSource(cr);
            cairo_surface_t *mask = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_A8, getWidth() * scale, getHeight() * scale, surfaceStride);
            cairo_scale(cr, 1.0/scale, 1.0/scale);
            cairo_mask_surface(cr, mask, 0, 0);
            cairo_restore(cr);
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
    
    void setAlpha(int x, int y, double alpha, unsigned char *data, int stride)
    {
        data[y * stride + x] = std::min(static_cast<int>(alpha * 255.0), 0xff);
    }
    
    unsigned char getAlpha(int x, int y, unsigned char *data, int stride)
    {
        return data[y * stride + x];
    }
    
    int getHeight() const   { return mHeight; }
    int getWidth() const    { return mWidth; }
    
};

#endif /* __HISSTOOLS_LICE_RASTER_CAIRO__ */
