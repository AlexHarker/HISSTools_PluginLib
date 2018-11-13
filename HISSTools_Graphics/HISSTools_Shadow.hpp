

#ifndef __HISSTOOLS_SHADOW__
#define __HISSTOOLS_SHADOW__

#include "HISSTools_Color.hpp"
#include "HISSTools_Bounds.hpp"

#include <vector>
#include <algorithm>

class HISSTools_Shadow
{
    
public:
    
    HISSTools_Shadow(HISSTools_Color_Spec *shadowColor, double xOffset, double yOffset, double blurSize)
    : mXOffset(xOffset), mYOffset(yOffset), mBlurSize(blurSize), mShadowColor(shadowColor)
    {
        setScaling(1.0);
    }
    
    void blur(unsigned char *io, unsigned char *temp, int width, int height, int stride)
    {
        int kernelSize = getKernelSize();
        
        if (height < kernelSize || width < kernelSize || kernelSize < 2)
            return;
        
        blurSwap(temp, io, width, height, stride, height, kernelSize, mNorm);
        blurSwap(io, temp, height, width, height, stride, kernelSize, mNorm);
    }
    
    int getKernelSize() const   { return (int) mBlurKernel.size(); }
    
    double getXOffset() const   { return mXOffset; }
    double getYOffset() const   { return mYOffset; }
    double getBlurSize() const  { return mBlurSize; }
    
    HISSTools_Color_Spec *getShadowColor()
    {
        return mShadowColor;
    }
    
    HISSTools_Bounds getBlurBounds(HISSTools_Bounds currentBounds)
    {
        double x = currentBounds.mRECT.L + mXOffset - (mBlurSize - 1);
        double y = currentBounds.mRECT.T + mYOffset - (mBlurSize - 1);
        double width = currentBounds.mRECT.W() + 2 * (mBlurSize - 1);
        double height = currentBounds.mRECT.H() + 2 * (mBlurSize - 1);
        
        currentBounds.include(HISSTools_Bounds(x, y, width, height));
        
        return currentBounds;
    }
    
    void setScaling(double scale)
    {
        makeKernel(scale);
    }
    
private:
    
    void blurSwap(unsigned char *output, unsigned char *input, int width, int height, int inStride, int outStride, int kernelSize, unsigned long norm)
    {
        for (int i = 0; i < height; i++, input += inStride)
        {
            for (int j = 0; j < kernelSize - 1; j++)
            {
                unsigned long accum = input[j] * mBlurKernel[0];
                for (int k = 1; k < j + 1; k++)
                    accum += mBlurKernel[k] * input[j - k];
                for (int k = 1; k < kernelSize; k++)
                    accum += mBlurKernel[k] * input[j + k];
                output[j * outStride + i] = std::min(static_cast<unsigned long>(255), accum / norm);
            }
            for (int j = kernelSize - 1; j < (width - kernelSize) + 1; j++)
            {
                unsigned long accum = input[j] * mBlurKernel[0];
                for (int k = 1; k < kernelSize; k++)
                    accum += mBlurKernel[k] * (input[j - k] + input[j + k]);
                output[j * outStride + i] = std::min(static_cast<unsigned long>(255), accum / norm);
            }
            for (int j = (width - kernelSize) + 1; j < width; j++)
            {
                unsigned long accum = input[j] * mBlurKernel[0];
                for (int k = 1; k < kernelSize; k++)
                    accum += mBlurKernel[k] * input[j - k];
                for (int k = 1; k < width - j; k++)
                    accum += mBlurKernel[k] * input[j + k];
                output[j * outStride + i] = std::min(static_cast<unsigned long>(255), accum / norm);
            }
        }
    }
    
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
                mBlurKernel[i] = round(255.0 * exp(-(i * i) * blurConst));
            
            mNorm = mBlurKernel[0];
            for (int i = 1; i < kernelSize; i++)
                mNorm += mBlurKernel[i] + mBlurKernel[i];
        }
    }
    
    std::vector<unsigned char> mBlurKernel;
    unsigned long mNorm;
    
    double mXOffset;
    double mYOffset;
    double mBlurSize;
    
    HISSTools_Color_Spec *mShadowColor;
};

#endif /* __HISSTOOLS_LICE_SHADOW__ */

