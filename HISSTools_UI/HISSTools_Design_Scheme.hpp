

#ifndef __HISSTOOLS_DESIGN_SCHEME__
#define __HISSTOOLS_DESIGN_SCHEME__

#include "../HISSTools_Graphics/HISSTools_VecLib_Structs.hpp"
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

    HISSTools_Label(T value, const char *name, const char *subType, bool free)
        : TypeName(name), SubType(subType), mValue(value), mDelete(free)
    {}

    HISSTools_Label(T value, const char *name, const char *subType)
        : TypeName(name), SubType(subType), mValue(value), mDelete(false)
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
        if (defaultScheme)
            SetDefaults();
    }

    ~HISSTools_Design_Scheme()
    {
        DeletePointers(mColorSpecs);
        DeletePointers(mShadowSpecs);
        DeletePointers(mTextStyles);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////// Templates //////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////

private:

    // Storage for all named specificiations

    std::vector<HISSTools_Label<HISSTools_Text*>>           mTextStyles;
    std::vector<HISSTools_Label<HISSTools_Color_Spec*>>     mColorSpecs;
    std::vector<HISSTools_Label<HISSTools_Shadow*>>         mShadowSpecs;
    std::vector<HISSTools_Label<double>>                    mDimensions;
    std::vector<HISSTools_Label<bool>>                      mFlags;

    // Searching Template

    template <class T>
    T FindByName(std::vector<HISSTools_Label<T>>& searchSpace, const char *searchName, const char *searchSubTypes, T defaultValue)
    {
        if (searchSubTypes)
        {
            const char *searchType = 0;
            char searchTypesMutable[4096];

            strncpy(searchTypesMutable, searchSubTypes, 4095);

            for (searchType = strtok(searchTypesMutable, " "); searchType; searchType = strtok(0, " "))
            {
                for (auto it = searchSpace.begin(); it != searchSpace.end(); it++)
                    if (it->SubType.GetLength() && !strcmp(it->TypeName.Get(), searchName) && !strcmp(it->SubType.Get(), searchType))
                        return it->mValue;
            }
        }

        for (auto it = searchSpace.begin(); it != searchSpace.end(); it++)
            if (it->TypeName.GetLength() && !strcmp(it->TypeName.Get(), searchName))
                return it->mValue;

        return defaultValue;
    }

    // Addition Template For Pointers

    template <class T>
    void AddPointer(std::vector<HISSTools_Label<T*>>& searchSpace, const char *name, const char *subType, T *newValue)
    {
        for (auto it = searchSpace.begin(); it != searchSpace.end(); it++)
        {
            if (it->mValue == newValue)
            {
                searchSpace.push_back(HISSTools_Label<T*>(newValue, name, subType, false));
                return;
            }
        }

        searchSpace.push_back(HISSTools_Label<T*>(newValue, name, subType, true));
    }

    // Deletion Template for Pointers

    template <class T>
    void DeletePointers(std::vector<HISSTools_Label<T*>>& searchSpace)
    {
        for (auto it = searchSpace.begin(); it != searchSpace.end(); it++)
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

    void AddColorSpec(const char *name, const char *subType, HISSTools_Color_Spec *spec)
    {
        AddPointer(mColorSpecs, name, subType, spec);
    }

    void AddColorSpec(const char *name, HISSTools_Color_Spec *spec)
    {
        AddColorSpec(name, nullptr, spec);
    }

    HISSTools_Color_Spec *GetColorSpec(const char *name, const char *subType = 0)
    {
        return FindByName(mColorSpecs, name, subType, (HISSTools_Color_Spec *)NULL);
    }

    // Dimensions

    void AddDimension(const char *name, const char *subType, double thickness)
    {
        mDimensions.push_back(HISSTools_Label<double>(thickness, name, subType));
    }

    void AddDimension(const char *name, double thickness)
    {
        AddDimension(name, nullptr, thickness);
    }

    double GetDimension(const char *name, const char *subType = 0)
    {
        return FindByName(mDimensions, name, subType, 0.0);
    }

    // Text

    void AddTextStyle(const char *name, const char *subType, HISSTools_Text *spec)
    {
        AddPointer(mTextStyles, name, subType, spec);
    }

    void AddTextStyle(const char *name, HISSTools_Text *spec)
    {
        AddTextStyle(name, nullptr, spec);
    }

    HISSTools_Text *GetTextStyle(const char *name, const char *subType = 0)
    {
        return FindByName(mTextStyles, name, subType, (HISSTools_Text *)NULL);
    }

    // Shadows

    void AddShadow(const char *name, const char *subType, HISSTools_Shadow *spec)
    {
        AddPointer(mShadowSpecs, name, subType, spec);
    }

    void AddShadow(const char *name, HISSTools_Shadow *spec)
    {
        AddShadow(name, nullptr, spec);
    }

    HISSTools_Shadow *GetShadow(const char *name, const char *subType = 0)
    {
        return FindByName(mShadowSpecs, name, subType, (HISSTools_Shadow *)NULL);
    }

    // Flags

    void AddFlag(const char *name, const char *subType, bool flag)
    {
        mFlags.push_back(HISSTools_Label<bool>(flag, name, subType));
    }

    void AddFlag(const char *name, bool flag)
    {
        AddFlag(name, nullptr, flag);
    }

    bool GetFlag(const char *name, const char *subType = 0)
    {
        return FindByName(mFlags, name, subType, (bool)false);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// Default Scheme Values ///////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////

private:

    void SetDefaults()
    {
        // FIX - NEATEN THIS
        // FIX - default option for controls (so as to allow no scheme when no plugin....)

        // Dial Throw

        AddDimension("DialRefValue", 0.0);
        AddDimension("DialRefValue", "bipolar", 0.5);
        AddDimension("DialStartAngle", 0.33);
        AddDimension("DialThrowAngle", 0.84);

        // Dial Pointer Appearance

        AddDimension("DialPointerRatio", 0.60);
        AddDimension("DialTipRatio", 1.30);
        AddDimension("DialPointerAngle", 0.03);

        AddDimension("DialPointerAngle", "tiny", 0.1);

        // Dial Prompts

        AddDimension("DialPromptPadding", 1.0);
        AddDimension("DialValuePromptRatio", 0.5);
        AddDimension("ValuePromptRatio", 0.4);

        // Flags

        AddFlag("ButtonLabelMode", false);
        AddFlag("ButtonLabelMode", "label", true);

        AddFlag("ValueDrawTriangle", true);
        AddFlag("ValueDrawSeparator", true);

        AddFlag("DialBiPolar", false);
        AddFlag("DialBiPolar", "bipolar", true);

        AddFlag("ShowUnits", true);
        AddFlag("ShowUnits", "nounits", false);

        AddFlag("VUOverlayFixedGradientBox", false);
        AddFlag("PanelDrawOutline", false);

        AddFlag("ValueDrawLabel", true);

        // Shadows

        HISSTools_Color_Spec shadowCS1(HISSTools_Color(0.05, 0.05, 0.05, 0.63));
        HISSTools_Color_Spec shadowCS2(HISSTools_Color(0.05, 0.05, 0.05, 1.0));
        HISSTools_Color_Spec shadowCS3(HISSTools_Color(0.00, 0.00, 0.00, 0.70));
        HISSTools_Color_Spec shadowCS4(HISSTools_Color(0.00, 0.00, 0.00, 0.81));

        //HISSTools_Color_Spec *shadowCS1 = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 0.4));
        //HISSTools_Color_Spec *shadowCS2 = new HISSTools_Color_Spec(HISSTools_Color(0.05, 0.05, 0.05, 1.0));
        //HISSTools_Color_Spec *shadowCS3 = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.5));
        //HISSTools_Color_Spec *shadowCS4 = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.65));

        HISSTools_Shadow *shadowSpec1 = new HISSTools_Shadow(shadowCS1, 3, 3, 8);
        HISSTools_Shadow *shadowSpec2 = new HISSTools_Shadow(shadowCS3, 4, 4, 6);
        HISSTools_Shadow *shadowSpec3 = new HISSTools_Shadow(shadowCS4, 2, 3, 4);
        HISSTools_Shadow *shadowSpec4 = new HISSTools_Shadow(shadowCS3, 2, 2, 3);
        HISSTools_Shadow *shadowSpec5 = new HISSTools_Shadow(shadowCS2, 2, 2, 8);

        AddShadow("ValuePanel", shadowSpec1);

        AddShadow("DialOutline", shadowSpec1);
        AddShadow("DialPointer", shadowSpec2);
        AddShadow("Switch", shadowSpec3);
        AddShadow("Button", shadowSpec3);
        AddShadow("Matrix", shadowSpec4);
        AddShadow("Meter", shadowSpec5);
        AddShadow("Progress", shadowSpec5);
        AddShadow("Panel", shadowSpec3);
        AddShadow("TextBlock", 0);

        // Text Styles
        /*
        HISSTools_Text *defaultTxt = new HISSTools_Text(16, "Arial", HISSTools_Text::kStyleBold);
        HISSTools_Text *medTxt = new HISSTools_Text(13, "Arial", HISSTools_Text::kStyleBold);
        HISSTools_Text *smallTxt = new HISSTools_Text(11, "Arial", HISSTools_Text::kStyleBold);
        */

        HISSTools_Text *defaultTxt = new HISSTools_Text(14, "Arial Bold");
        HISSTools_Text *medTxt = new HISSTools_Text(12, "Arial Bold");
        HISSTools_Text *smallTxt = new HISSTools_Text(10, "Arial Bold");

        AddTextStyle("Value", defaultTxt);

        AddTextStyle("ValueLabel", defaultTxt);
        AddTextStyle("DialLabel", defaultTxt);
        AddTextStyle("DialValue", defaultTxt);
        AddTextStyle("DialLabel", "small", medTxt);
        AddTextStyle("DialValue", "small", medTxt);
        AddTextStyle("DialLabel", "tiny", smallTxt);
        AddTextStyle("DialValue", "tiny", smallTxt);
        AddTextStyle("TextBlock", defaultTxt);
        AddTextStyle("TextBlock", "small", medTxt);
        AddTextStyle("Button", defaultTxt);

        // Line Thicknesses

        AddDimension("SpectralDisplayFrame", 1);
        AddDimension("SpectralDisplayGrid", 1);
        AddDimension("SpectralDisplayTick", 1);

        AddDimension("ValuePanelOutline", 0.75);

        AddDimension("DialPointerOutline", 0.5);
        AddDimension("DialOutline", 1.5);
        AddDimension("DialIndicatorLineWidth", 0.4);

        AddDimension("SwitchHandleOutline", 0.6);
        AddDimension("SwitchBoxOutline", 0.9);

        AddDimension("ButtonOutline", 0.6);

        AddDimension("MatrixShadowOutline", 2);
        AddDimension("MatrixOutline", 1.2);
        AddDimension("MatrixHilite", 3);

        AddDimension("MatrixShadowOutline", "round", 0.75);
        AddDimension("MatrixOutline", "round", 0.5);

        AddDimension("ProgressOutline", 1.);

        AddDimension("VUOutline", 1.);
        AddDimension("VUTick", 0.5);
        AddDimension("VUPeakHold", 2);

        AddDimension("PanelOutline", 0.6);

        // Rounding 

        AddDimension("MatrixHandleRoundness", 3);
        AddDimension("MatrixHandleRoundness", "round", -1);

        AddDimension("ValuePanelRoundness", -1);

        AddDimension("ButtonRoundness", -1);
        AddDimension("ButtonRoundness", "tight", 2);

        AddDimension("SwitchRoundness", -1);

        AddDimension("PanelRoundnessTL", 60);
        AddDimension("PanelRoundnessTR", 60);
        AddDimension("PanelRoundnessBL", 40);
        AddDimension("PanelRoundnessBR", 40);
        AddDimension("PanelRoundnessTL", "tight", 10);
        AddDimension("PanelRoundnessTR", "tight", 10);
        AddDimension("PanelRoundnessBL", "tight", 10);
        AddDimension("PanelRoundnessBR", "tight", 10);

        // Sizes

        AddDimension("DialDiameter", 90);
        AddDimension("DialDiameter", "small", 60);
        AddDimension("DialDiameter", "tiny", 40);
        AddDimension("DialTextArea", 25);
        AddDimension("ValueTextArea", 20);
        
        AddDimension("ButtonWidth", 100);
        AddDimension("ButtonHeight", 30);
        AddDimension("ButtonTextPad", 10);
        
        AddDimension("SwitchWidth", 60);
        AddDimension("SwitchHeight", 20);
        
        AddDimension("MatrixHandleSize", 12);
        AddDimension("MatrixHandleGap", 6);
        
        AddDimension("MatrixHandleSize", "round", 9);
        AddDimension("MatrixHandleGap", "round", 9);
        
        AddDimension("PanelWidth", 200);
        AddDimension("PanelHeight", 200);

        // Other

        AddDimension("SpectralDisplayFreqMin", 20);
        AddDimension("SpectralDisplayFreqMax", 22050);
        AddDimension("SpectralDisplayDbMin", -100);
        AddDimension("SpectralDisplayDbMax", 20);

        AddDimension("SpectralDisplayGridOctaveSpacing", 1.0 / 3.0);
        AddDimension("SpectralDisplayGridFreqReference", 1000.0);
        AddDimension("SpectralDisplayGridDbSpacing", 10.0);
        AddDimension("SpectralDisplayGridDbReference", 0.0);

        AddDimension("MenuTriangleWidthRatio", 0.6);
        AddDimension("MenuTriangleHeightRatio", 0.5);

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

        matrixFillCS->addStop(HISSTools_Color(0.3, 0.3, 0.3, 1.0), 0.8);
        matrixFillCS->addStop(HISSTools_Color(0.1, 0.1, 0.1, 1.0), 1.0);

        matrixHalfFillCS->addStop(HISSTools_Color(0.3, 0.3, 0.3, 0.7), 0.5);
        matrixHalfFillCS->addStop(HISSTools_Color(0.1, 0.1, 0.1, 0.7), 1.0);

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

        boxFill->addStop(HISSTools_Color(0.76, 0.76, 0.76, 1.), 0);
        boxFill->addStop(HISSTools_Color(0.90, 0.90, 0.90, 1.), 1.);

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

        AddColorSpec("SpectralDisplayTick", buttonOutlineCS);
        AddColorSpec("SpectralDisplayFrame", buttonOutlineCS);
        AddColorSpec("SpectralDisplayGrid", spectralDisplayGridCS);
        AddColorSpec("SpectralDisplayBackground", spectralDisplayBackgroundCS);

        AddColorSpec("Value", handleTextCS);
        AddColorSpec("ValuePanel", valueFillCS);
        AddColorSpec("ValuePanelOutline", dialPointerOutlineCS);
        AddColorSpec("ValueHilite", valueAlterCS);

        AddColorSpec("DialPointerFill", handleFillCS);
        AddColorSpec("DialCircleFill", dialCircleFillGradient);
        AddColorSpec("DialValue", handleTextCS);
        AddColorSpec("DialLabel", labelTextCS);
        AddColorSpec("ValueLabel", labelTextCS);
        AddColorSpec("DialInactiveOverlay", inactiveOverlayCS);
        AddColorSpec("DialOutline", dialOutlineCS);
        AddColorSpec("DialPointerOutline", dialPointerOutlineCS);
        AddColorSpec("DialIndicator", dialIndicatorRed);
        AddColorSpec("DialIndicator", "red", dialIndicatorRed);
        AddColorSpec("DialIndicator", "green", dialIndicatorGreen);

        AddColorSpec("Progress", dialIndicatorRed);
        AddColorSpec("ProgressOutline", VUOutlineCS);
        AddColorSpec("ProgressBackground", VUBackground);

        AddColorSpec("TextBlock", labelTextCS);

        AddColorSpec("ButtonHandleOn", onHandleFillCS);
        AddColorSpec("ButtonHandleOff", offHandleFillCS);
        AddColorSpec("ButtonOutline", buttonOutlineCS);
        AddColorSpec("ButtonHandleLabel", handleTextCS);
        AddColorSpec("ButtonBackgroundLabel", labelTextCS);
        AddColorSpec("ButtonInactiveOverlay", inactiveOverlayCS);
        AddColorSpec("ButtonHandleLabelOff", 0);

        AddColorSpec("SwitchHandleFill", onHandleFillCS);
        AddColorSpec("SwitchHandleOutline", buttonOutlineCS);
        AddColorSpec("SwitchBoxFill", boxFill);
        AddColorSpec("SwitchOutline", buttonOutlineCS);
        AddColorSpec("SwitchInactiveOverlay", inactiveOverlayCS);

        AddColorSpec("MatrixOutline", matrixOutlineCS);
        AddColorSpec("MatrixHilite", matrixHiliteCS);
        AddColorSpec("MatrixState0", transparentCS);
        AddColorSpec("MatrixState1", greyishCS);
        AddColorSpec("MatrixState2", matrixFillCS);
        AddColorSpec("MatrixState3", matrixHalfFillCS);

        AddColorSpec("VUBackground", VUBackground);
        AddColorSpec("VUOutline", VUOutlineCS);
        AddColorSpec("VU1", VUGradient);
        AddColorSpec("VU2", VUOverlay);
        AddColorSpec("VUSide", VUGradient);
        AddColorSpec("VU1Peak", VUPeak);
        AddColorSpec("VU2Peak", VUOverlayPeak);
        AddColorSpec("VUSidePeak", VUPeak);

        AddColorSpec("PanelFill", PanelFillCS);
        AddColorSpec("PanelFill", "grey", PanelFillGreyCS);

        AddColorSpec("PanelOutline", buttonOutlineCS);

        // VU Leds

        HISSTools_Color_Spec *VULedCS0 = new HISSTools_Color_Spec(HISSTools_Color(0.000, 0.000, 0.000, 0.000));
        HISSTools_Color_Spec *VULedCS1 = new HISSTools_Color_Spec(HISSTools_Color(0.075, 0.267, 0.000, 0.275));
        HISSTools_Color_Spec *VULedCS2 = new HISSTools_Color_Spec(HISSTools_Color(0.075, 0.267, 0.000, 0.450));
        HISSTools_Color_Spec *VULedCS3 = new HISSTools_Color_Spec(HISSTools_Color(0.186, 0.650, 0.000, 0.650));
        HISSTools_Color_Spec *VULedCS4 = new HISSTools_Color_Spec(HISSTools_Color(0.500, 0.790, 0.000, 0.790));
        HISSTools_Color_Spec *VULedCS5 = new HISSTools_Color_Spec(HISSTools_Color(0.933, 0.800, 0.000, 0.940));
        HISSTools_Color_Spec *VULedCS6 = new HISSTools_Color_Spec(HISSTools_Color(0.769, 0.000, 0.000, 1.000));

        AddColorSpec("MatrixState0", "VU_Leds", VULedCS0);
        AddColorSpec("MatrixState1", "VU_Leds", VULedCS1);
        AddColorSpec("MatrixState2", "VU_Leds", VULedCS2);
        AddColorSpec("MatrixState3", "VU_Leds", VULedCS3);
        AddColorSpec("MatrixState4", "VU_Leds", VULedCS4);
        AddColorSpec("MatrixState5", "VU_Leds", VULedCS5);
        AddColorSpec("MatrixState6", "VU_Leds", VULedCS6);

        // TEMP

        HISSTools_Color_Spec *spectralCurveCS = new HISSTools_Color_Spec(HISSTools_Color(1., 1., 1., 1));
        AddColorSpec("SpectralCurve", spectralCurveCS);
        AddDimension("SpectralCurve", 3.0);
        AddDimension("SpectralCurveSubSample", 1.0);
    }
};

static HISSTools_Design_Scheme DefaultDesignScheme(true);

#endif /* __HISSTOOLS_DESIGN_SCHEME__ */
