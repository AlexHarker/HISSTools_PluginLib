
#ifndef __HISSTOOLS_BOUNDS__
#define __HISSTOOLS_BOUNDS__

#include <IGraphicsConstants.h>
#include <IGraphicsStructs.h>
#include <algorithm>

struct HISSTools_Bounds
{
    HISSTools_Bounds() {}
    
    HISSTools_Bounds(double x, double y, double w, double h) : mRECT(w > 0 ? x : x - w, h > 0 ? y : y - h, w > 0 ? x + w : x, h > 0 ? y + h : y) {}
	
    void include(HISSTools_Bounds inc)      { mRECT = mRECT.Union(inc.mRECT); }
	void addThickness(double thickness)     { mRECT.Pad(std::max(0.0, thickness) * 0.5); }
	
    IRECT iBounds() const       { return IRECT(floor(mRECT.L), floor(mRECT.T), ceil(mRECT.R), ceil(mRECT.B)); }
    
    IRECT mRECT;
};

#endif /* __HISSTOOLS_BOUNDS__ */
