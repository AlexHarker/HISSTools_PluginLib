
#ifndef __HISSTOOLS_BOUNDS__
#define __HISSTOOLS_BOUNDS__

#include <IGraphicsConstants.h>
#include <IGraphicsStructs.h>
#include <algorithm>

class HISSTools_Bounds
{
		
public:
		
    HISSTools_Bounds() : mX(0.0), mY(0.0), mW(0.0), mH(0.0) {}
	
	HISSTools_Bounds(double x, double y, double w, double h)
	{
        mX = w > 0 ? x : x - w;
        mY = h > 0 ? y : y - h;
        mW = fabs(w);
        mH = fabs(h);
	}
	
	void include(HISSTools_Bounds inc)
	{
        if (!isEmpty())
        {
            double x = std::max(mX + mW, inc.mX + inc.mW);
            double y = std::max(mY + mH, inc.mY + inc.mH);
        
            mX = std::min(mX, inc.mX);
            mY = std::min(mY, inc.mY);
            mW = x - mX;
            mH = y - mY;
        }
        else
            *this = inc;
	}
	
	void addThickness(double thickness)
	{
        thickness = std::max(0.0, thickness);
		
		mX -= thickness * 0.5;
		mY -= thickness * 0.5;
		mW += thickness;
		mH += thickness;
	}
	
    IRECT iBounds(double scale = 1.0) const       { return IRECT(floor(mX * scale), floor(mY * scale), ceil((mX + mW) * scale), ceil((mY + mH) * scale)); }
    
	double getX() const         { return mX; }
	double getY() const         { return mY; }
	double getWidth() const     { return mW; }
	double getHeight() const    { return mH; }
    bool isEmpty() const        { return !mW || !mH; }
    
private:
    
    // Note that mW and mH are always positive
    
    double mX;
    double mY;
    double mW;
    double mH;
};

#endif /* __HISSTOOLS_LICE_BOUNDS__ */
