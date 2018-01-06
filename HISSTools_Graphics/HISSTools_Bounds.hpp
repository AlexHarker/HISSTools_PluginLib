
#ifndef __HISSTOOLS_BOUNDS__
#define __HISSTOOLS_BOUNDS__

#include <IPlugStructs.h>

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
		if (inc.mX < mX)
		{
			mW = (mX + mW) - inc.mX;
			mX = inc.mX;
		}
		if (inc.mY < mY)
		{
			mH = (mY + mH) - inc.mY;
			mY = inc.mY;
		}
		if ((inc.mX + inc.mW) > (mX + mW))
			mW = (inc.mX + inc.mW) - mX;
		if ((inc.mY + inc.mH) > (mY + mH))
			mH = (inc.mY + inc.mH) - mY;
	}
	
	void addThickness(double thickness)
	{
		thickness = thickness > 0 ? thickness : 0;
		
		mX -= thickness * 0.5;
		mY -= thickness * 0.5;
		mW += thickness;
		mH += thickness;
	}
	
    IRECT iBounds() const       { return IRECT(floor(mX), floor(mY), ceil((mX + mW)), ceil((mY + mH))); }
    
	double getX() const         { return mX; }
	double getY() const         { return mY; }
	double getWidth() const     { return mW; }
	double getHeight() const    { return mH; }
    
private:
    
    // Note that mW and mH are always positive
    
    double mX;
    double mY;
    double mW;
    double mH;
};

#endif /* __HISSTOOLS_LICE_BOUNDS__ */
