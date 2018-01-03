

#ifndef __HISSTOOLS_LICE_SHADOW__
#define __HISSTOOLS_LICE_SHADOW__

#include "HISSTools_LICE_Color_Cairo.hpp"
#include "HISSTools_LICE_Bounds.hpp"

#ifndef WDL_NO_DEFINE_MINMAX
#define WDL_NO_DEFINE_MINMAX
#endif

#include <lice.h>
#include <vector>

class HISSTools_Shadow
{

private:
	
    std::vector<double> mBlurKernel;
	
	double mXOffset;
	double mYOffset;
    double mBlurSize;
    double mScale;
    
	HISSTools_Color_Spec *mShadowColor;
	
public:
		
	HISSTools_Shadow(HISSTools_Color_Spec *shadowColor, double xOffset, double yOffset, double blurSize)
    : mXOffset(xOffset), mYOffset(yOffset), mBlurSize(blurSize), mShadowColor(shadowColor)
	{
        setScaling(1.0);
    }
	
	void rowBlur(double *oRow, double *iRow, long width, long height)
	{
		double accum;
		long i, j, k;
        int kernelSize = getKernelSize();
        
		if (width < kernelSize)
			return;
		
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < kernelSize - 1; j++)
			{
				accum = iRow[j] * mBlurKernel[0];
				for (k = 1; k < j + 1; k++)
					accum += mBlurKernel[k] * iRow[j - k];
				for (k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iRow[j + k];
				oRow[j] = accum;
			}
			for (j = kernelSize - 1; j < (width - kernelSize) + 1; j++)
			{
				accum = iRow[j] * mBlurKernel[0];
				for (k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * (iRow[j - k] + iRow[j + k]);
				oRow[j] = accum;
			}
			for (j = (width - kernelSize) + 1; j < width; j++)
			{
				accum = iRow[j] * mBlurKernel[0];
				for (k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iRow[j - k];
				for (k = 1; k < width - j; k++)
					accum += mBlurKernel[k] * iRow[j + k];
				oRow[j] = accum;			
			}
			
			iRow += width;
			oRow += width;
		}
	}
	
	void colBlur(double *oCol, double *iCol, int width, int height)
	{
		double accum;
		long i, j, k;
        int kernelSize = getKernelSize();

		if (height < kernelSize)
			return;
		
		for (i = 0; i < width; i++)
		{
			for (j = 0; j < kernelSize - 1; j++)
			{
				accum = iCol[j * width] * mBlurKernel[0];
				for (k = 1; k < j + 1; k++)
					accum += mBlurKernel[k] * iCol[(j - k) * width];
				for (k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iCol[(j + k) * width];
				oCol[j * width] = accum;
			}
			for (; j < (height - kernelSize) + 1; j++)
			{
				accum = iCol[j * width] * mBlurKernel[0];
				for (k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * (iCol[(j - k) * width] + iCol[(j + k) * width]);
				oCol[j * width] = accum;
			}
			for (; j < height; j++)
			{
				accum = iCol[j * width] * mBlurKernel[0];
				for (k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iCol[(j - k) * width];
				for (k = 1; k < height - j; k++)
					accum += mBlurKernel[k] * iCol[(j + k) * width];
				oCol[j * width] = accum;			
			}
			
			iCol++;
			oCol++;
		}
	}
	
	int getKernelSize() const   { return mBlurKernel.size(); }
	
	double getXOffset() const   { return mXOffset * mScale; }
	double getYOffset() const   { return mYOffset * mScale; }
	
	HISSTools_Color_Spec *getShadowColor()
	{
		return mShadowColor;
	}
			
	HISSTools_Bounds getBlurBounds(HISSTools_Bounds currentBounds, double scale)
	{
        setScaling(scale);
        
        int kernelSize = getKernelSize();
        
		double x = currentBounds.getX() + mXOffset * scale - (kernelSize - 1);
		double y = currentBounds.getY() + mYOffset * scale - (kernelSize - 1);
		double width = currentBounds.getWidth() + 2 * (kernelSize - 1);
		double height = currentBounds.getHeight() + 2 * (kernelSize - 1);
		
		currentBounds.include(HISSTools_Bounds(x, y, width, height));
		
		return currentBounds;
	}
    
    void setScaling(double scale)
    {
        mScale = scale;
        makeKernel(scale);
    }
    
private:
    
    void makeKernel(double scale)
    {
        // Reference blurSize to zero (which will be no blur)
        
        double blurSize = mBlurSize * scale + 1.0;
        blurSize = blurSize <  1.0 ? 1.0 : blurSize;
        int kernelSize = ceil(blurSize);
        
        if (kernelSize != getKernelSize())
        {
            mBlurKernel.resize(kernelSize);
            
            if (kernelSize > 1)
            {
                double blurRatio = kernelSize / 3.0;
                double blurConst = 1.0 / (2 * blurRatio * blurRatio);
                double accum = 0.;
                
                for (int i = 0; i < kernelSize; i++)
                    mBlurKernel[i] = exp(-(i * i) * blurConst);
                
                accum = mBlurKernel[0];
                
                for (int i = 1; i < kernelSize; i++)
                    accum += mBlurKernel[i] + mBlurKernel[i];
                
                double normalise = 1.0 / accum;
                
                for (int i = 0; i < kernelSize; i++)
                    mBlurKernel[i] *= normalise;
            }
            else
                mBlurKernel[0] = 1.0;
        }
    }
};

#endif /* __HISSTOOLS_LICE_SHADOW__ */
