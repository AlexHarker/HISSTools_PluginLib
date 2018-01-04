

#ifndef __HISSTOOLS_LICE_RASTER__
#define __HISSTOOLS_LICE_RASTER__

#include "HISSTools_LICE_Color_Cairo.hpp"
#include "HISSTools_LICE_Shadow.hpp"

#ifndef WDL_NO_DEFINE_MINMAX
#define WDL_NO_DEFINE_MINMAX
#endif

#include <lice.h>
#include <algorithm>
#include <vector>
#include "cairo/cairo.h"

static HISSTools_Color_Spec defaultColor;

class HISSTools_LICE_Raster
{
    
private:

    cairo_t *mCairoDraw;

	// Boundaries
	
    int mWidth, mHeight;
	int mDrawXLo, mDrawXHi, mDrawYLo, mDrawYHi;
	double mGradientXLo, mGradientXHi, mGradientYLo, mGradientYHi;
	
	// Forced Gradient Bounds Flag
	
	bool mForceGradientBox;
	ColorOrientation mCSOrientation; 
	
	// Color
	
	HISSTools_Color_Spec *mColor;
	
	// Shadow
	
	HISSTools_Shadow *mShadow;
	
    std::vector <double> mBlurTempAlpha1;
    std::vector <double> mBlurTempAlpha2;
	
protected:
	
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	// Request clipping with negative values is equivalent to turning clipping off for that edge
	
    HISSTools_LICE_Raster(cairo_t *cairo) : mCairoDraw(NULL), mShadow(NULL), mWidth(0), mHeight(0)
	{
        mCairoDraw = cairo;
		setColor(&defaultColor);
		mForceGradientBox = false;
		mCSOrientation = kCSOrientHorizontal;
	}
	
	~HISSTools_LICE_Raster()
	{
    }
	
    void setSize(int w, int h)
    {
        mWidth = w;
        mHeight = h;
    }
    
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

    void setContext(cairo_t *context)
    {
        // FIX - clip and other state etc. etc..
        
        mCairoDraw = context;
        cairo_set_operator(mCairoDraw, CAIRO_OPERATOR_OVER);
    }
		
    /*
     void setBitmap(LICE_IBitmap *bitmap)
     {
     double x1, x2, y1, y2;
     
     mMainBitmap = bitmap;
     
     cairo_clip_extents(mCairoMain, &x1, &y1, &x2, &y2);
     
     destroyCairoResources(mCairoMain, mCairoSurfaceMain);
     createCairoResources(bitmap, mCairoMain, mCairoSurfaceMain);
     
     updateDrawBitmap(mMainBitmap, mCairoMain);
     
     setClip(x1, y1, x2, y2);
     }*/
    
private:
	
    int getHeight() const   { return mHeight; }
    int getWidth() const    { return mWidth; }
    
public:
	
    cairo_t *getContext() const { return mCairoDraw; }
    
    void setClip()
    {
        cairo_reset_clip(mCairoDraw);
    }
    
	void setClip(double xLo, double yLo, double xHi, double yHi)
	{
        //cairo_reset_clip(mCairoDraw);
        cairo_rectangle(mCairoDraw, xLo, yLo, xHi - xLo, yHi - yLo);
        cairo_clip(mCairoDraw);
	}
	
	void setColor(HISSTools_Color_Spec *color)
	{
		mColor = color;
                
        if (getContext())
            mColor->setAsSource(getContext());
	}
	
	void forceGradientBox()
	{
		mForceGradientBox = false;
	}
	
	void forceGradientBox(double xLo, double yLo, double xHi, double yHi)
	{
		mGradientXLo = xLo;
		mGradientXHi = xHi;
		mGradientYLo = yLo;
		mGradientYHi = yHi;
		
		mForceGradientBox = true;
	}
	
	// Orientation allows gradient rotation ONLY for relevant Color Specs
	
	void setColorOrientation(ColorOrientation CSOrientation)
	{
		mCSOrientation = CSOrientation;
	}
	
protected:
    
    void setShapeGradient(double xLo, double xHi, double yLo, double yHi)
    {
        if (mForceGradientBox == false)
            mColor->setRect(xLo, xHi, yLo, yHi, mCSOrientation);
        else
            mColor->setRect(mGradientXLo, mGradientXHi, mGradientYLo, mGradientYHi, mCSOrientation);
        
        setColor(mColor);
    }
    
	HISSTools_Color_Spec *getColorSpec()
	{
		return mColor;
	}
	
    void updateDrawBounds(int xLo, int xHi, int yLo, int yHi)
    {
        mDrawXLo = std::min(xLo, mDrawXLo);
        mDrawXHi = std::max(xHi, mDrawXHi);
        mDrawYLo = std::min(yLo, mDrawYLo);
        mDrawYHi = std::max(yHi, mDrawYHi);
    }
    
    void fill(bool useExtents = false)
    {
        double xLo, xHi, yLo, yHi;
     
        cairo_fill_extents(mCairoDraw, &xLo, &yLo, &xHi, &yHi);
        
        updateDrawBounds(round(xLo), round(xHi), round(yLo), round(yHi));
        
        if (useExtents)
            setShapeGradient(xLo, xHi, yLo, yHi);
        cairo_fill(mCairoDraw);
    }
    
    void stroke(bool useExtents = false)
    {
        double xLo, xHi, yLo, yHi;
        
        cairo_stroke_extents(mCairoDraw, &xLo, &yLo, &xHi, &yHi);
        
        updateDrawBounds(round(xLo), round(xHi), round(yLo), round(yHi));
        
        if (useExtents)
            setShapeGradient(xLo, xHi, yLo, yHi);
        cairo_stroke(mCairoDraw);
    }
    
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	// N.B. clip will clip to the range [lo-hi] (inclusive) - suitable for choosing loop points
	
	int clipX(int x, double scale)
	{
        x /= scale;
        double x1, x2, y1, y2;
        
        cairo_clip_extents(mCairoDraw, &x1, &y1, &x2, &y2);
        
        x1 = std::max(ceil(x1), 0.0);
        x2 = std::min(floor(x2), (double) getWidth());
        
		return std::min((int) x2, std::max((int) x1, x)) * scale;
	}
	
	int clipY(int y, double scale)
	{
        y /= scale;
        double x1, x2, y1, y2;
        
        cairo_clip_extents(mCairoDraw, &x1, &y1, &x2, &y2);
        
        y1 = std::max(ceil(y1), 0.0);
        y2 = std::min(floor(y2), (double) getHeight());
        
        return std::min((int) y2, std::max((int) y1, y)) * scale;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	// Drop Shadow
    
    void setPixel(int x, int y, double alpha, unsigned char *data, int stride)
    {
        data[y * stride + x] = std::min(static_cast<int>(alpha * 255.0), 0xff);
    }
    
public:
	
	void startShadow(HISSTools_Shadow *shadow, double scale)
	{
        mShadow = shadow;
        shadow->setScaling(scale);

        cairo_push_group(getContext());
        
		// Reset draw boundaries for shadow calculation
		
		mDrawXLo = getWidth();
		mDrawXHi = 0;
		mDrawYLo = getHeight();
		mDrawYHi = 0;
	}
	
	unsigned char getAlpha(int x, int y, unsigned char *data, int stride)
    {
        unsigned char *pixel = data + y * stride + x;
        
        return ((*pixel) >> 0) & 0xff;
       /*
        
        // FIX - check that these are the alphas!
        
        return data[y * stride + (x * 4)];*/
    }
    
    
	void renderShadow(bool renderImage, double scale)
	{
		int i, j;
		int xLo = round(mDrawXLo * scale);
		int xHi = round(mDrawXHi * scale);
		int yLo = round(mDrawYLo * scale);
		int yHi = round(mDrawYHi * scale);
		
		// Check there is a shadow specified (otherwise only render original image)
		
        cairo_pattern_t *shadowRender = cairo_pop_group(getContext());
        
		if (mShadow)
		{
            // FIX - kernel size issue!
            
			int kernelSize = mShadow->getKernelSize();
			double xOffset = mShadow->getXOffset();
			double yOffset = mShadow->getYOffset();
			
			int width = (xHi - xLo) + (2 * kernelSize - 1);		// Account for blur size and fractional shift
			int height = (yHi - yLo) + (2 * kernelSize - 1);	// Account for blur size and fractional shift
			int xShift = trunc(xOffset) - (kernelSize - 1);		
			int yShift = trunc(yOffset) - (kernelSize - 1);		
			
			double maxChanRecip = 1.0 / 255.0;
			
			// Sanity Checks
			
			if (mDrawXLo > mDrawXHi || mDrawYLo > mDrawYHi)
				return;
			
			// Alloc Blur Memory
			
			int sizeRequired = width * height;
			
			if ((mBlurTempAlpha1.size() < sizeRequired) || (mBlurTempAlpha2.size() < sizeRequired))
			{
				mBlurTempAlpha1.resize(sizeRequired);
				mBlurTempAlpha2.resize(sizeRequired);
			}

			// Zero temp values
			
			for (i = 0; i < sizeRequired; i++)
				mBlurTempAlpha1[i] = mBlurTempAlpha2[i] = 0.0;
			
			// Copy alpha values from the temp bitmap
				
            cairo_surface_t* shadowSurface = cairo_image_surface_create(CAIRO_FORMAT_A8, getWidth() * scale, getHeight() * scale);
            cairo_t* shadowContext = cairo_create(shadowSurface);
            cairo_scale(shadowContext, scale, scale);
            cairo_set_source(shadowContext, shadowRender);
            cairo_paint(shadowContext);
            
            unsigned char *data = cairo_image_surface_get_data(shadowSurface);
            int surfaceStride = cairo_image_surface_get_stride(shadowSurface);
            
			for (i = kernelSize - 1; i < (height - kernelSize) + 1; i++)
				for (j = kernelSize - 1; j < (width - kernelSize) + 1; j++)
					mBlurTempAlpha2[i * width + j] = getAlpha((j + xLo - (kernelSize - 1)), (i + yLo - (kernelSize - 1)), data, surfaceStride) * maxChanRecip;
			
            cairo_surface_destroy(shadowSurface);
            cairo_destroy(shadowContext);
            
            
			// Do blur (row, column then alpha mask with offsets)
			
			if (kernelSize > 1)
			{
				mShadow->rowBlur(&mBlurTempAlpha1[0], &mBlurTempAlpha2[0], width, height);
				mShadow->colBlur(&mBlurTempAlpha2[0], &mBlurTempAlpha1[0], width, height);
			}
			
			// FIX - Add Fractional offset - use linear interpolation on write
			
            surfaceStride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, getWidth() * scale);
            data = (unsigned char *)calloc(surfaceStride * scale * getHeight(), 1);
            
            for (i = clipY(yLo + yShift, scale); i < clipY(yLo + yShift + height, scale); i++)
                for (j =  clipX(xLo + xShift, scale); j < clipX(xLo + xShift + width, scale); j++)
                    setPixel(j, i, mBlurTempAlpha2[(i - yLo - yShift) * width + (j - xLo - xShift)], data, surfaceStride);
            
            // Draw Shadow in correct color
            
            cairo_t *cr = getContext();
            
            cairo_save(cr);
            mShadow->getShadowColor()->setAsSource(cr);
            cairo_surface_t *mask = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_A8, getWidth() * scale, getHeight() * scale, surfaceStride);
            cairo_scale(cr, 1.0/scale, 1.0/scale);
            cairo_mask_surface(cr, mask, 0, 0);
            cairo_restore(cr);
            cairo_surface_destroy(mask);
            free(data);
		}
		
		// Merge from temp bitmap

        // FIX - this looks inefficient - ensure better clipping!
         
		if (renderImage)
        {
            cairo_set_source(getContext(), shadowRender);
            cairo_paint_with_alpha(getContext(), 1.0);
        }

        cairo_pattern_destroy(shadowRender);
	}
};

#endif /* __HISSTOOLS_LICE_RASTER_CAIRO__ */
