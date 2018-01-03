

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

    cairo_t *mCairoRestore;

    cairo_t *mCairoDraw;
    
    cairo_t *mCairoShadow;
    cairo_surface_t *mCairoSurfaceShadow;
    
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
	
    HISSTools_LICE_Raster(cairo_t *cairo) : mCairoDraw(NULL), mCairoShadow(NULL), mCairoSurfaceShadow(NULL), mShadow(NULL), mCairoRestore(NULL), mWidth(0), mHeight(0)
	{
        mCairoDraw = cairo;
		setColor(&defaultColor);
		mForceGradientBox = false;
		mCSOrientation = kCSOrientHorizontal;
	}
	
	~HISSTools_LICE_Raster()
	{
        destroyCairoResources(mCairoShadow, mCairoSurfaceShadow);
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
    
    void destroyCairoResources(cairo_t *cairoContext, cairo_surface_t *cairoSurface)
    {
        if (cairoSurface)
            cairo_surface_destroy(cairoSurface);
        if (cairoContext)
            cairo_destroy(cairoContext);
    }
    
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

    void mergeBitmap(cairo_surface_t *source, int writeXOffset, int writeYOffset, int readXOffset, int readYOffset, int width, int height)
    {
        cairo_t *cr = getContext();
        
        cairo_save(cr);
        cairo_rectangle(cr, writeXOffset, writeYOffset, width, height);
        cairo_clip(cr);
        cairo_set_source_surface(cr, source, writeXOffset - readXOffset, writeYOffset - readYOffset);
        cairo_paint_with_alpha(cr, 1.0);
        cairo_restore(cr);
    }
    
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
	
	int clipX(int x)
	{
        double x1, x2, y1, y2;
        
        cairo_clip_extents(mCairoDraw, &x1, &y1, &x2, &y2);
        
        x1 = std::max(ceil(x1), 0.0);
        x2 = std::min(floor(x2), (double) getWidth());
        
		return std::min((int) x2, std::max((int) x1, x));
	}
	
	int clipY(int y)
	{
        double x1, x2, y1, y2;
        
        cairo_clip_extents(mCairoDraw, &x1, &y1, &x2, &y2);
        
        y1 = std::max(ceil(y1), 0.0);
        y2 = std::min(floor(y2), (double) getHeight());
        
        return std::min((int) y2, std::max((int) y1, y));
	}
	
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	// Drop Shadow
	
    void drawPixel(double x, double y, double alpha)
    {
        HISSTools_Color srcColour = mColor->getColor(x, y);
        
        cairo_set_source_rgba(getContext(), srcColour.r, srcColour.g, srcColour.b, srcColour.a * alpha);
        cairo_move_to(getContext(), x - 0.5, y - 0.5);
        cairo_line_to(getContext(), x + 0.5, y + 0.5);
        cairo_set_line_width(getContext(), 1);
        cairo_stroke(getContext());
    }
    
public:
	
	void startShadow(HISSTools_Shadow *shadow, double scale)
	{
        mShadow = shadow;
        shadow->setScaling(1.0);
        //shadow->setScaling(scale);

        int width = getWidth();// * scale;
        int height = getHeight();// * scale;

		// Make new temp image (inefficient!!)
		
        destroyCairoResources(mCairoShadow, mCairoSurfaceShadow);
        
        mCairoSurfaceShadow = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
        mCairoShadow = cairo_create(mCairoSurfaceShadow);
        //cairo_scale(mCairoShadow, scale, scale);
        
        mCairoRestore = mCairoDraw;
        setContext(mCairoShadow);
        
		// Reset draw boundaries for shadow calculation
		
		mDrawXLo = width;
		mDrawXHi = 0;
		mDrawYLo = height;
		mDrawYHi = 0;
	}
	
	unsigned char getAlpha(int x, int y, unsigned char *data, int stride)
    {
        unsigned int *pixel = (unsigned int*)(data + y * stride);
        pixel += x;
        
        return ((*pixel) >> 0) & 0xff;
       /*
        
        // FIX - check that these are the alphas!
        
        return data[y * stride + (x * 4)];*/
    }
    
	void renderShadow(bool renderImage, double scale)
	{
        if (mCairoRestore)
            setContext(mCairoRestore);
        mCairoRestore = NULL;
        
		int i, j;
		int xLo = mDrawXLo;
		int xHi = mDrawXHi;
		int yLo = mDrawYLo;
		int yHi = mDrawYHi;
		
		// Check there is a shadow specified (otherwise only render original image)
		
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
			
			for (i = 0; i < width * height; i++)
				mBlurTempAlpha1[i] = mBlurTempAlpha2[i] = 0.0;
			
			// Copy alpha values from the temp bitmap
				
            unsigned char *data = cairo_image_surface_get_data(mCairoSurfaceShadow);
            int surface_stride = cairo_image_surface_get_stride(mCairoSurfaceShadow);
            
			for (i = kernelSize - 1; i < (height - kernelSize) + 1; i++)
				for (j = kernelSize - 1; j < (width - kernelSize) + 1; j++)
					mBlurTempAlpha2[i * width + j] = getAlpha((j + xLo - (kernelSize - 1)), (i + yLo - (kernelSize - 1)), data, surface_stride) * maxChanRecip;
			
			// Do blur
			
			if (kernelSize > 1)
			{
				mShadow->rowBlur(&mBlurTempAlpha1[0], &mBlurTempAlpha2[0], width, height);
				mShadow->colBlur(&mBlurTempAlpha2[0], &mBlurTempAlpha1[0], width, height);
			}
			
			// Set to shadow color
			
			setColor(mShadow->getShadowColor(mColor));
			
			// Draw Shadow (add offsets here)
			// FIX - Add Fractional offset - use linear interpolation on write
			/*
            for (i = clipY(yLo + yShift); i < clipY(yLo + yShift + height); i++)
                for (j =  clipX(xLo + xShift); j < clipX(xLo + xShift + width); j++)
                    drawPixel(j, i, mBlurTempAlpha2[(i - yLo - yShift) * width + (j - xLo - xShift)]);
			*/
			// Set back to standard color spec
			
			setColor(mShadow->getPrevColor());
		}
		
		// Merge from temp bitmap
	
		if (renderImage)
		{
            // FIX - this looks inefficient - ensure better clipping!
            
            mergeBitmap(mCairoSurfaceShadow, 0, 0, 0, 0, getWidth(), getHeight());
            /*for (i = clipY(yLo); i < clipY(yHi + 1); i++)
                for (j = clipX(xLo); j < clipX(xHi + 1); j++)
                    mergePixel(mBits + i * mRowSpan + j, inPixels[i * rowSpan + j]);*/
		}
	}
};

#endif /* __HISSTOOLS_LICE_RASTER_CAIRO__ */
