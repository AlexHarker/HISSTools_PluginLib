
#ifndef __HISSTOOLS_LICE_BOUNDS__
#define __HISSTOOLS_LICE_BOUNDS__

#include <IPlugStructs.h>

class HISSTools_Bounds
{
		
public:
		
    HISSTools_Bounds() : mX(0.0), mY(0.0), mW(0.0), mH(0.0) {}
	
	HISSTools_Bounds(double v1, double v2, double v3, double v4, bool absolute = false)
	{
		if (absolute)
		{
			mX = v1 < v3 ? v1 : v3;
			mY = v2 < v4 ? v2 : v4;
			mW = v1 < v3 ? v3 : v1;
			mH = v2 < v4 ? v4 : v2;
		}
		else 
		{
			mX = v3 > 0 ? v1 : v1 - v3;
			mY = v4 > 0 ? v2 : v2 - v4;
			mW = fabs(v3);
			mH = fabs(v4);
		}
	}
	
	IRECT iBounds(double scale)
	{
		return IRECT(floor(mX * scale), floor(mY * scale), ceil((mX + mW) * scale), ceil((mY + mH) * scale));
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
		double halfThickness = thickness / 2.0;
		
		mX -= halfThickness;
		mY -= halfThickness;
		mW += thickness;
		mH += thickness;
	}
	
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
