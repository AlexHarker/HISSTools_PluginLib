

#ifndef __HISSTOOLS_SHADOW__
#define __HISSTOOLS_SHADOW__

#include "HISSTools_Color.hpp"
#include "HISSTools_Bounds.hpp"

#include <vector>

class HISSTools_Shadow
{

private:
	
    std::vector<double> mBlurKernel;
	
	double mXOffset;
	double mYOffset;
    double mBlurSize;
    
	HISSTools_Color_Spec *mShadowColor;
	
public:
		
	HISSTools_Shadow(HISSTools_Color_Spec *shadowColor, double xOffset, double yOffset, double blurSize)
    : mXOffset(xOffset), mYOffset(yOffset), mBlurSize(blurSize), mShadowColor(shadowColor)
	{
        setScaling(1.0);
    }
	
	void rowBlur(double *oRow, double *iRow, long width, long height)
	{
        int kernelSize = getKernelSize();
        
		if (width < kernelSize || kernelSize < 2)
			return;
		
		for (long i = 0; i < height; i++)
		{
			for (long j = 0; j < kernelSize - 1; j++)
			{
				double accum = iRow[j] * mBlurKernel[0];
				for (long k = 1; k < j + 1; k++)
					accum += mBlurKernel[k] * iRow[j - k];
				for (long k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iRow[j + k];
				oRow[j] = accum;
			}
			for (long j = kernelSize - 1; j < (width - kernelSize) + 1; j++)
			{
				double accum = iRow[j] * mBlurKernel[0];
				for (long k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * (iRow[j - k] + iRow[j + k]);
				oRow[j] = accum;
			}
			for (long j = (width - kernelSize) + 1; j < width; j++)
			{
				double accum = iRow[j] * mBlurKernel[0];
				for (long k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iRow[j - k];
				for (long k = 1; k < width - j; k++)
					accum += mBlurKernel[k] * iRow[j + k];
				oRow[j] = accum;			
			}
			
			iRow += width;
			oRow += width;
		}
	}
	
	void colBlur(double *oCol, double *iCol, int width, int height)
	{
        int kernelSize = getKernelSize();

		if (height < kernelSize || kernelSize < 2)
			return;
		
		for (long i = 0; i < width; i++)
		{
			for (long j = 0; j < kernelSize - 1; j++)
			{
				double accum = iCol[j * width] * mBlurKernel[0];
				for (long k = 1; k < j + 1; k++)
					accum += mBlurKernel[k] * iCol[(j - k) * width];
				for (long k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iCol[(j + k) * width];
				oCol[j * width] = accum;
			}
			for (long j = kernelSize - 1; j < (height - kernelSize) + 1; j++)
			{
				double accum = iCol[j * width] * mBlurKernel[0];
				for (long k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * (iCol[(j - k) * width] + iCol[(j + k) * width]);
				oCol[j * width] = accum;
			}
			for (long j = (height - kernelSize) + 1; j < height; j++)
			{
				double accum = iCol[j * width] * mBlurKernel[0];
				for (long k = 1; k < kernelSize; k++)
					accum += mBlurKernel[k] * iCol[(j - k) * width];
				for (long k = 1; k < height - j; k++)
					accum += mBlurKernel[k] * iCol[(j + k) * width];
				oCol[j * width] = accum;			
			}
			
			iCol++;
			oCol++;
		}
	}
	
	int getKernelSize() const   { return mBlurKernel.size(); }
	
	double getXOffset() const   { return mXOffset; }
	double getYOffset() const   { return mYOffset; }
	
	HISSTools_Color_Spec *getShadowColor()
	{
		return mShadowColor;
	}
			
	HISSTools_Bounds getBlurBounds(HISSTools_Bounds currentBounds)
	{        
        int kernelSize = getKernelSize();
        
		double x = currentBounds.getX() + mXOffset - (kernelSize - 1);
		double y = currentBounds.getY() + mYOffset - (kernelSize - 1);
		double width = currentBounds.getWidth() + 2 * (kernelSize - 1);
		double height = currentBounds.getHeight() + 2 * (kernelSize - 1);
		
		currentBounds.include(HISSTools_Bounds(x, y, width, height));
		
		return currentBounds;
	}
    
    void setScaling(double scale)
    {
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
            
            double blurConst = 4.5 / (kernelSize * kernelSize);
                
            for (int i = 0; i < kernelSize; i++)
                mBlurKernel[i] = exp(-(i * i) * blurConst);
                
            double accum =  mBlurKernel[0];
            for (int i = 1; i < kernelSize; i++)
                accum += mBlurKernel[i] + mBlurKernel[i];
                
            double normalise = 1.0 / accum;
                
            for (int i = 0; i < kernelSize; i++)
                mBlurKernel[i] *= normalise;
        }
    }
};

#endif /* __HISSTOOLS_LICE_SHADOW__ */
