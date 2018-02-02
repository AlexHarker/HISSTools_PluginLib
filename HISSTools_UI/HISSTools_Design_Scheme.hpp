

#ifndef __HISSTOOLS_DESIGN_SCHEME__
#define __HISSTOOLS_DESIGN_SCHEME__

#include "../HISSTools_Graphics/HISSTools_Color.hpp"
#include <wdlstring.h>
#include <vector>
#include <iostream>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Label Template Class //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> 
class HISSTools_Label
{
public:
		WDL_String TypeName;
		WDL_String SubType;
		T mValue;
		bool mDelete;

		HISSTools_Label (T value, const char *name, const char *subType, bool free)
		: TypeName(name), SubType(subType), mValue (value), mDelete(free)
		{}
	
		HISSTools_Label (T value, const char *name, const char *subType)
		: TypeName(name), SubType(subType), mValue (value), mDelete(false)
		{}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Design Scheme Class ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


class HISSTools_Design_Scheme
{
	
public:
	
	HISSTools_Design_Scheme(bool defaultScheme = false)
	{
		if (defaultScheme == true)
			setDefaults();
	}
	
	~HISSTools_Design_Scheme()
	{
		deletePointers(&mColorSpecs);
		deletePointers(&mShadowSpecs);
		deletePointers(&mTextStyles);
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////// Templates //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	// Storage for all named specificiations
	
	std::vector < HISSTools_Label< HISSTools_Text *> >			mTextStyles;
	std::vector < HISSTools_Label< HISSTools_Color_Spec *> >	mColorSpecs;
	std::vector < HISSTools_Label < HISSTools_Shadow *> >		mShadowSpecs;
	std::vector < HISSTools_Label <double> >					mDimensions;
	std::vector < HISSTools_Label <bool> >						mFlags;
	
	// Searching Template
	
	template <class T> T findByName(std::vector < HISSTools_Label <T> > *searchSpace, const char *searchName, const char *searchSubTypes, T defaultValue)
	{
		typename std::vector < HISSTools_Label <T> >::iterator it;
		
		if (searchSubTypes)
		{
			const char *searchType = 0;
			char searchTypesMutable[4096];
			
			strncpy(searchTypesMutable, searchSubTypes, 4095);
			
			for (searchType = strtok(searchTypesMutable, " "); searchType; searchType = strtok(0, " "))
			{
				for (it = searchSpace->begin(); it != searchSpace->end(); it++)
					if (it->SubType.GetLength() && !strcmp(it->TypeName.Get(), searchName) && !strcmp(it->SubType.Get(), searchType))
						return it->mValue;			
			}
		}
		
		for (it = searchSpace->begin(); it != searchSpace->end(); it++)
			if (it->TypeName.GetLength() && !strcmp(it->TypeName.Get(), searchName))
				return it->mValue;
		
		return defaultValue;
	}
	
	// Addition Template For Pointers
	
	template <class T> void addPointer(std::vector < HISSTools_Label < T *> > *searchSpace, const char *name, const char *subType, T *newValue)
	{		
		for (typename std::vector< HISSTools_Label < T *> >::iterator it = searchSpace->begin(); it != searchSpace->end(); it++)
		{
			if (it->mValue == newValue)
			{
				searchSpace->push_back(HISSTools_Label < T *> (newValue, name, subType, false));
				return;
			}
		}
		
		searchSpace->push_back(HISSTools_Label < T *> (newValue, name, subType, true));
	}
	
	// Deletion Template for Pointers
	
	template <class T> void deletePointers(std::vector < HISSTools_Label < T *> > *searchSpace)
	{
		for (typename std::vector< HISSTools_Label < T *> >::iterator it = searchSpace->begin(); it != searchSpace->end(); it++)
		{
			if (it->mDelete)
				delete it->mValue;
		}
	}
		
	/////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////// Public Methods Per Type //////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

public:

	// Color Specs

	void addColorSpec(const char *name, const char *subType, HISSTools_Color_Spec *spec)
	{
		addPointer(&mColorSpecs, name, subType, spec);
	}
	
	void addColorSpec(const char *name, HISSTools_Color_Spec *spec)
	{
		addColorSpec(name, 0, spec);
	}
	
	HISSTools_Color_Spec *getColorSpec(const char *name, const char *subType = 0)
	{
		return findByName(&mColorSpecs, name, subType, (HISSTools_Color_Spec *) NULL);
	}

	// Dimensions

	void addDimension(const char *name, const char *subType, double thickness)
	{
		mDimensions.push_back(HISSTools_Label <double> (thickness, name, subType));
	}
	
	void addDimension(const char *name, double thickness)
	{
		addDimension(name, 0, thickness);
	}
	
	double getDimension(const char *name, const char *subType = 0)
	{	
        return findByName(&mDimensions, name, subType, 0.0);
	}
	
	// Text
		
	void addTextStyle(const char *name, const char *subType, HISSTools_Text *spec)
	{
		addPointer(&mTextStyles, name, subType, spec);
	}
	
	void addTextStyle(const char *name, HISSTools_Text *spec)
	{
		addTextStyle(name, 0, spec);
	}
	
	HISSTools_Text *getTextStyle(const char *name, const char *subType = 0)
	{
		return findByName(&mTextStyles, name, subType, (HISSTools_Text *) NULL);
	}
	
	// Shadows
		
	void addShadow(const char *name, const char *subType, HISSTools_Shadow *spec)
	{
		addPointer(&mShadowSpecs, name, subType, spec);
	}
	
	void addShadow(const char *name, HISSTools_Shadow *spec)
	{
		addShadow(name, 0, spec);
	}
	
	HISSTools_Shadow *getShadow(const char *name, const char *subType = 0)
	{
		return findByName(&mShadowSpecs, name, subType, (HISSTools_Shadow *)NULL);
	}	
	
	// Flags
	
	void addFlag(const char *name, const char *subType, bool flag)
	{
		mFlags.push_back(HISSTools_Label <bool> (flag, name, subType));
	}
	
	void addFlag(const char *name, bool flag)
	{
		addFlag(name, 0, flag);
	}
	
	bool getFlag(const char *name, const char *subType = 0)
	{
		return findByName(&mFlags, name, subType, (bool) false);
	}
		
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////// Default Scheme Values ///////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	void setDefaults()
	{
		// FIX - NEATEN THIS
		// FIX - default option for controls (so as to allow no scheme when no plugin....)
		
		// Dial Throw
		
		addDimension("DialRefValue", 0.0);
		addDimension("DialRefValue", "bipolar", 0.5);
		addDimension("DialStartAngle", 0.58);
		addDimension("DialThrowAngle", 0.84);
		
		// Dial Pointer Appearance
		
		addDimension("DialPointerRatio", 0.60);
		addDimension("DialTipRatio", 1.30);
		addDimension("DialPointerAngle", 0.03);
		
		addDimension("DialPointerAngle", "tiny", 0.1);
		
		// Dial Prompts
		
		addDimension("DialPromptPadding", 1.0);	
		addDimension("DialPromptRatio", 0.8);
		
		// Flags
		
		addFlag("ButtonLabelMode", false);
		addFlag("ButtonLabelMode", "label", true);
		
		addFlag("ValueDrawTriangle", true);
		addFlag("ValueDrawSeparator", true);
		
		addFlag("DialBiPolar", false);
		addFlag("DialBiPolar", "bipolar", true);
		
		addFlag("ShowUnits", true);
		addFlag("ShowUnits", "nounits", false);
		
		addFlag("VUOverlayFixedGradientBox", false);
		addFlag("PanelDrawOutline", false);
		
        addFlag("ValueDrawLabel", true);

		// Shadows
		
        HISSTools_Color_Spec *shadowCS1 = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 0.63));
        HISSTools_Color_Spec *shadowCS2 = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 1.0));
        HISSTools_Color_Spec *shadowCS3 = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.70));
        HISSTools_Color_Spec *shadowCS4 = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.81));
        
		//HISSTools_Color_Spec *shadowCS1 = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 0.4));
		//HISSTools_Color_Spec *shadowCS2 = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 1.0));
		//HISSTools_Color_Spec *shadowCS3 = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.5));
		//HISSTools_Color_Spec *shadowCS4 = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.65));
		
		HISSTools_Shadow *shadowSpec1 = new HISSTools_Shadow(shadowCS1, 3, 3, 8);
		HISSTools_Shadow *shadowSpec2 = new HISSTools_Shadow(shadowCS3, 4, 4, 6);
		HISSTools_Shadow *shadowSpec3 = new HISSTools_Shadow(shadowCS4, 2, 3, 4);
		HISSTools_Shadow *shadowSpec4 = new HISSTools_Shadow(shadowCS3, 2, 2, 3);
		HISSTools_Shadow *shadowSpec5 = new HISSTools_Shadow(shadowCS2, 2, 2, 8);
		
		addShadow("ValuePanel", shadowSpec1);
		
		addShadow("DialOutline", shadowSpec1);
		addShadow("DialPointer", shadowSpec2);
		addShadow("Switch", shadowSpec3);
		addShadow("Button", shadowSpec3);
		addShadow("Matrix", shadowSpec4);
		addShadow("Meter", shadowSpec5);
		addShadow("Progress", shadowSpec5);
		addShadow("Panel", shadowSpec3);
		addShadow("TextBlock", 0);
		
		// Text Styles
		
		HISSTools_Text *defaultTxt = new HISSTools_Text(16, "Arial", HISSTools_Text::kStyleBold);
		HISSTools_Text *medTxt = new HISSTools_Text(13, "Arial", HISSTools_Text::kStyleBold);
		HISSTools_Text *smallTxt = new HISSTools_Text(11, "Arial", HISSTools_Text::kStyleBold);
		
		addTextStyle("Value", defaultTxt);

		addTextStyle("ValueLabel", defaultTxt);
		addTextStyle("DialLabel", defaultTxt);
		addTextStyle("DialValue", defaultTxt);
		addTextStyle("DialLabel", "small", medTxt);
        addTextStyle("DialValue", "small", medTxt);
        addTextStyle("DialLabel", "tiny", smallTxt);
		addTextStyle("DialValue", "tiny", smallTxt);
		addTextStyle("TextBlock", defaultTxt);
		addTextStyle("TextBlock", "small", medTxt);
		addTextStyle("Button", defaultTxt);
		
		// Line Thicknesses
		
		addDimension("SpectralDisplayFrame", 1);
		addDimension("SpectralDisplayGrid", 1);
        addDimension("SpectralDisplayTick", 1);
		
		addDimension("ValuePanelOutline", 0.75);
		
		addDimension("DialPointerOutline", 0.5);
		addDimension("DialOutline", 1.5);
		addDimension("DialIndicatorLineWidth", 0.4);
		
		addDimension("SwitchHandleOutline", 0.6);
		addDimension("SwitchBoxOutline", 0.9);
		
		addDimension("ButtonOutline", 0.6);
		
		addDimension("MatrixShadowOutline", 2);
		addDimension("MatrixOutline", 1.2);
		addDimension("MatrixHilite", 3);
		
		addDimension("MatrixShadowOutline", "round", 0.75);
		addDimension("MatrixOutline", "round", 0.5);
		
		addDimension("ProgressOutline", 1.);

		addDimension("VUOutline", 1.);
		addDimension("VUTick", 0.5);
		addDimension("VUPeakHold", 2);
		
		addDimension("PanelOutline", 0.6);
		
		// Rounding 
		
		addDimension("MatrixHandleRoundness", 3);
		addDimension("MatrixHandleRoundness", "round", -1);
		
		addDimension("ValuePanelRoundness", -1);		
		
		addDimension("ButtonRoundness", -1);
		addDimension("ButtonRoundness", "tight", 2);
		
		addDimension("SwitchRoundness", -1);
		
		addDimension("PanelRoundnessTL", 60);
		addDimension("PanelRoundnessTR", 60);
		addDimension("PanelRoundnessBL", 40);
		addDimension("PanelRoundnessBR", 40);
		addDimension("PanelRoundnessTL","tight", 10);
		addDimension("PanelRoundnessTR","tight", 10);
		addDimension("PanelRoundnessBL","tight", 10);
		addDimension("PanelRoundnessBR","tight", 10);
		
		// Sizes
		
		addDimension("DialDiameter", 90);
		addDimension("DialDiameter", "small", 60);
		addDimension("DialDiameter", "tiny", 40);
		addDimension("DialTextArea", 25);
		addDimension("ValueTextArea", 20);
		
		addDimension("ButtonWidth", 100);
		addDimension("ButtonHeight", 30);
		addDimension("ButtonTextPad", 10);
		
		addDimension("SwitchWidth", 60);
		addDimension("SwitchHeight", 20);
		
		addDimension("MatrixHandleSize", 12);
		addDimension("MatrixHandleGap", 6);
		
		addDimension("MatrixHandleSize", "round", 9);
		addDimension("MatrixHandleGap", "round",  9);
		
		addDimension("PanelWidth", 200);
		addDimension("PanelHeight", 200);
		
		// Other
		
		addDimension("SpectralDisplayFreqMin", 20);
		addDimension("SpectralDisplayFreqMax", 22050);
		addDimension("SpectralDisplayDbMin", -100);
		addDimension("SpectralDisplayDbMax", 20);
		
		addDimension("SpectralDisplayGridOctaveSpacing", 1.0 / 3.0);
		addDimension("SpectralDisplayGridFreqReference", 1000.0);
		addDimension("SpectralDisplayGridDbSpacing", 10.0);
		addDimension("SpectralDisplayGridDbReference", 0.0);
		
		addDimension("MenuTriangleWidthRatio", 0.6);
		addDimension("MenuTriangleHeightRatio", 0.5);
		
		// Color Specs
		
		// Gradients
		
		HISSTools_LICE_VGradient *handleFillCS = new HISSTools_LICE_VGradient;
		HISSTools_LICE_VGradient *valueFillCS = new HISSTools_LICE_VGradient;
		HISSTools_LICE_VGradient *onHandleFillCS = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *offHandleFillCS = new HISSTools_LICE_VGradient(); 
		HISSTools_LICE_VGradient *dialCircleFillGradient = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *boxFill = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *dialIndicatorRed = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *dialIndicatorGreen = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *matrixFillCS = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *matrixHalfFillCS = new HISSTools_LICE_VGradient();
		HISSTools_LICE_HVGradient *VUOverlay = new HISSTools_LICE_HVGradient();	
		HISSTools_LICE_HVGradient *VUGradient = new HISSTools_LICE_HVGradient();
		HISSTools_LICE_VGradient *PanelFillCS = new HISSTools_LICE_VGradient();
		HISSTools_LICE_VGradient *PanelFillGreyCS = new HISSTools_LICE_VGradient();
		
		// Gradient Specs
		
		PanelFillCS->addStop(HISSTools_Color(0.7, 0.7, 0.7, 0.4), 0.0);   // SUPER LUMINESCENT    (USED in shadow bitmap)
		PanelFillCS->addStop(HISSTools_Color(0.2, 0.2, 0.2, 0.5), 0.9);
		PanelFillCS->addStop(HISSTools_Color(0.075, 0.075, 0.075, 0.6), 1.0);
		
		PanelFillGreyCS->addStop(HISSTools_Color(0.05, 0.05, 0.08, 0.3), 0.0);
		PanelFillGreyCS->addStop(HISSTools_Color(0.05, 0.05, 0.08, 0.4), 1.0);
		
		matrixFillCS->addStop(HISSTools_Color(0.1, 0.1, 0.1, 1.0), 1.0);
		matrixFillCS->addStop(HISSTools_Color(0.3, 0.3, 0.3, 1.0), 0.8);
		
		matrixHalfFillCS->addStop(HISSTools_Color(0.1, 0.1, 0.1, 0.4), 1.0);
		matrixHalfFillCS->addStop(HISSTools_Color(0.3, 0.3, 0.3, 0.4), 0.5);
		
		handleFillCS->addStop(HISSTools_Color(0.6, 0.6, 0.6, 1.0), 0.0);
		handleFillCS->addStop(HISSTools_Color(0.15, 0.15, 0.15, 1.0), 0.9);
		
		valueFillCS->addStop(HISSTools_Color(0.3, 0.3, 0.3, 0.75), 0.0);
		valueFillCS->addStop(HISSTools_Color(0.15, 0.15, 0.15, 0.75), 0.9);
		
		offHandleFillCS->addStop(HISSTools_Color(0.415, 0.415, 0.415, 1.0), 0.0);
		offHandleFillCS->addStop(HISSTools_Color(0.169, 0.169, 0.169, 1.0), 1.0);
		
		//onHandleFillCS->addStop(HISSTools_Color(0.66, 0.00, 0.00, 1.0), 0);
		//onHandleFillCS->addStop(HISSTools_Color(0.33, 0.00, 0.00, 1.0), 1.);
		
		onHandleFillCS->addStop(HISSTools_Color(0.66, 0.66, 0.66, 1.0), 0);
		onHandleFillCS->addStop(HISSTools_Color(0.33, 0.33, 0.33, 1.0), 1.);
		
		dialCircleFillGradient->addStop(HISSTools_Color(0.3, 0.3, 0.3, 0.5), 0.0);
		dialCircleFillGradient->addStop(HISSTools_Color(0.86, 0.86, 0.86, 0.5), 0.6);   // SUPER LUMINESCENT    (USED in shadow bitmap)
		
		boxFill->addStop(HISSTools_Color(0.90, 0.90, 0.90, 1.), 1.);
		boxFill->addStop(HISSTools_Color(0.76, 0.76, 0.76, 1.), 0);
		
		dialIndicatorRed->addStop(HISSTools_Color(0.54, 0, 0, 1), 0.);
		dialIndicatorRed->addStop(HISSTools_Color(0.91, 0, 0, 1), 0.6);
		
		dialIndicatorGreen->addStop(HISSTools_Color(0., 0.42, 0.0, 1.0), 0.);
		dialIndicatorGreen->addStop(HISSTools_Color(0., 0.70, 0.0, 1.0), 0.6);
		
		VUOverlay->addStop(HISSTools_Color(0.6, 0.6, 0.6, 0.4), 0.05);  // SL    (never used on shadow bitmap)
		VUOverlay->addStop(HISSTools_Color(0.7, 0.7, 0.7, 0.4), 0.6);   // SL    (never used on shadow bitmap)
		VUOverlay->addStop(HISSTools_Color(0.4, 0.4, 0.4, 0.4), 0.9);
		
        //VUOverlay->addStop(HISSTools_Color(0.075, 0.267, 0.000, 0.3), 0.000);
        //VUOverlay->addStop(HISSTools_Color(0.220, 0.769, 0.000, 0.3), 0.549);
        //VUOverlay->addStop(HISSTools_Color(1.000, 0.882, 0.169, 0.3), 0.730);
        //VUOverlay->addStop(HISSTools_Color(0.933, 0.800, 0.000, 0.3), 0.789);
        //VUOverlay->addStop(HISSTools_Color(0.769, 0.000, 0.000, 0.3), 0.996);
        
		VUGradient->addStop(HISSTools_Color(0.075, 0.267, 0.000, 1.0), 0.000);
		VUGradient->addStop(HISSTools_Color(0.220, 0.769, 0.000, 1.0), 0.549);		
		VUGradient->addStop(HISSTools_Color(1.000, 0.882, 0.169, 1.0), 0.730);
		VUGradient->addStop(HISSTools_Color(0.933, 0.800, 0.000, 1.0), 0.789);
		VUGradient->addStop(HISSTools_Color(0.769, 0.000, 0.000, 1.0), 0.996);
		
		// Static Colors
		
        HISSTools_Color_Spec *spectralDisplayBackgroundCS = new HISSTools_Color_Spec(HISSTools_Color(0.1, 0.1, 0.1, 1.0));
        HISSTools_Color_Spec *spectralDisplayGridCS = new HISSTools_Color_Spec(HISSTools_Color(0.2, 0.2, 0.2, 1.0));
        
        //HISSTools_Color_Spec *spectralDisplayBackgroundCS = new HISSTools_Color_Spec(HISSTools_Color(1.0, 1.0, 1.0, 1.0));
        //HISSTools_Color_Spec *spectralDisplayGridCS = new HISSTools_Color_Spec(HISSTools_Color(0.9, 0.9, 0.9, 1.0));
        
		HISSTools_Color_Spec *handleTextCS = new HISSTools_Color_Spec(HISSTools_Color(0.9, 0.9, 0.9, 1));
		HISSTools_Color_Spec *labelTextCS = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 1));
		
		HISSTools_Color_Spec *valueAlterCS = new HISSTools_Color_Spec(HISSTools_Color(0.75, 0.82, 0.6, 1.0));
		
		HISSTools_Color_Spec *inactiveOverlayCS = new HISSTools_Color_Spec(HISSTools_Color(0.7, 0.7, 0.7, 0.6));   // SL    (never used on shadow bitmap)
		HISSTools_Color_Spec *dialOutlineCS = new HISSTools_Color_Spec(HISSTools_Color(0.0, 0.0, 0.0, 0.7));
		HISSTools_Color_Spec *dialPointerOutlineCS = new HISSTools_Color_Spec(HISSTools_Color(0.17, 0.17, 0.17, 1.0));
		HISSTools_Color_Spec *buttonOutlineCS = new HISSTools_Color_Spec(HISSTools_Color(0.0, 0.0, 0.0, 1.0));
		HISSTools_Color_Spec *VUOutlineCS = new HISSTools_Color_Spec(HISSTools_Color(0.0, 0.0, 0.0, 1.0));
		HISSTools_Color_Spec *greyishCS = new HISSTools_Color_Spec(HISSTools_Color(0.4, 0.55, 0.35, 0.64));
		HISSTools_Color_Spec *transparentCS = new HISSTools_Color_Spec(HISSTools_Color(0.0, 0.0, 0.0, 0.0));
		HISSTools_Color_Spec *matrixOutlineCS = new HISSTools_Color_Spec(HISSTools_Color(0.0, 0.0, 0.0, 1.0));
		HISSTools_Color_Spec *matrixHiliteCS = new HISSTools_Color_Spec(HISSTools_Color(1.0, 1.0, 1.0, 0.5));   // SL    (never used on shadow bitmap)
		HISSTools_Color_Spec *VUBackground = new HISSTools_Color_Spec(HISSTools_Color(0.9, 0.9, 0.9, 0.8));   // SL   (never used on shadow bitmap)
		HISSTools_Color_Spec *VUPeak = new HISSTools_Color_Spec(HISSTools_Color(0.9, 0.0, 0.0, 1.0));
		HISSTools_Color_Spec *VUOverlayPeak = new HISSTools_Color_Spec(HISSTools_Color(0.4, 0.4, 0.4, 0.4));
		
		addColorSpec("SpectralDisplayTick", buttonOutlineCS);
		addColorSpec("SpectralDisplayFrame", buttonOutlineCS);
		addColorSpec("SpectralDisplayGrid", spectralDisplayGridCS);
		addColorSpec("SpectralDisplayBackground", spectralDisplayBackgroundCS);
		
		addColorSpec("Value", handleTextCS);
		addColorSpec("ValuePanel", valueFillCS);		
		addColorSpec("ValuePanelOutline", dialPointerOutlineCS);
		addColorSpec("ValueHilite", valueAlterCS);
		
		addColorSpec("DialPointerFill", handleFillCS);
		addColorSpec("DialCircleFill", dialCircleFillGradient);
		addColorSpec("DialValue", handleTextCS);
		addColorSpec("DialLabel", labelTextCS);
		addColorSpec("ValueLabel", labelTextCS);
		addColorSpec("DialInactiveOverlay", inactiveOverlayCS);
		addColorSpec("DialOutline", dialOutlineCS);
		addColorSpec("DialPointerOutline", dialPointerOutlineCS);
		addColorSpec("DialIndicator", dialIndicatorRed);
		addColorSpec("DialIndicator", "red", dialIndicatorRed);
		addColorSpec("DialIndicator", "green", dialIndicatorGreen);
		
		addColorSpec("Progress", dialIndicatorRed);
		addColorSpec("ProgressOutline", VUOutlineCS);
		addColorSpec("ProgressBackground", VUBackground);

		addColorSpec("TextBlock", labelTextCS);
		
		addColorSpec("ButtonHandleOn", onHandleFillCS);
		addColorSpec("ButtonHandleOff", offHandleFillCS);
		addColorSpec("ButtonOutline", buttonOutlineCS);
		addColorSpec("ButtonHandleLabel", handleTextCS);
		addColorSpec("ButtonBackgroundLabel", labelTextCS);
		addColorSpec("ButtonInactiveOverlay", inactiveOverlayCS);
		
		addColorSpec("SwitchHandleFill", onHandleFillCS);
		addColorSpec("SwitchHandleOutline", buttonOutlineCS);
		addColorSpec("SwitchBoxFill", boxFill);
		addColorSpec("SwitchOutline", buttonOutlineCS);
		addColorSpec("SwitchInactiveOverlay", inactiveOverlayCS);
		
		addColorSpec("MatrixOutline", matrixOutlineCS);
		addColorSpec("MatrixHilite", matrixHiliteCS);
		addColorSpec("MatrixState0", transparentCS);
		addColorSpec("MatrixState1", greyishCS);
		addColorSpec("MatrixState2", matrixFillCS);
		addColorSpec("MatrixState3", matrixHalfFillCS);
		
		addColorSpec("VUBackground", VUBackground);
		addColorSpec("VUOutline", VUOutlineCS);
		addColorSpec("VU1", VUGradient);
		addColorSpec("VU2", VUOverlay);
		addColorSpec("VUSide", VUGradient);
		addColorSpec("VU1Peak", VUPeak);
		addColorSpec("VU2Peak", VUOverlayPeak);
		addColorSpec("VUSidePeak", VUPeak);
				
		addColorSpec("PanelFill", PanelFillCS);
		addColorSpec("PanelFill", "grey", PanelFillGreyCS);

		addColorSpec("PanelOutline", buttonOutlineCS);
		
		// VU Leds
		
		HISSTools_Color_Spec *VULedCS0 = new HISSTools_Color_Spec(HISSTools_Color(0.000, 0.000, 0.000, 0.000));
		HISSTools_Color_Spec *VULedCS1 = new HISSTools_Color_Spec(HISSTools_Color(0.075, 0.267, 0.000, 0.250));   // SUPER LUMINESCENT
		HISSTools_Color_Spec *VULedCS2 = new HISSTools_Color_Spec(HISSTools_Color(0.075, 0.267, 0.000, 0.450));
		HISSTools_Color_Spec *VULedCS3 = new HISSTools_Color_Spec(HISSTools_Color(0.220, 0.769, 0.000, 0.600));   // SUPER LUMINESCENT
		HISSTools_Color_Spec *VULedCS4 = new HISSTools_Color_Spec(HISSTools_Color(0.500, 0.790, 0.000, 0.750));   // SUPER LUMINESCENT
		HISSTools_Color_Spec *VULedCS5 = new HISSTools_Color_Spec(HISSTools_Color(0.933, 0.800, 0.000, 0.900));   // SUPER LUMINESCENT
		HISSTools_Color_Spec *VULedCS6 = new HISSTools_Color_Spec(HISSTools_Color(0.769, 0.000, 0.000, 1.000));
		
		addColorSpec("MatrixState0", "VU_Leds", VULedCS0);
		addColorSpec("MatrixState1", "VU_Leds", VULedCS1);
		addColorSpec("MatrixState2", "VU_Leds", VULedCS2);
		addColorSpec("MatrixState3", "VU_Leds", VULedCS3);
		addColorSpec("MatrixState4", "VU_Leds", VULedCS4);
		addColorSpec("MatrixState5", "VU_Leds", VULedCS5);
		addColorSpec("MatrixState6", "VU_Leds", VULedCS6);
		
		// TEMP
		
		HISSTools_Color_Spec *spectralCurveCS = new HISSTools_Color_Spec(HISSTools_Color(1., 1., 1., 1));
		addColorSpec("SpectralCurve", spectralCurveCS);
		addDimension("SpectralCurve", 3.0);
		addDimension("SpectralCurveSubSample", 1.0);
	}	
};

static HISSTools_Design_Scheme DefaultDesignScheme(true);

#endif /* __HISSTOOLS_DESIGN_SCHEME__ */
