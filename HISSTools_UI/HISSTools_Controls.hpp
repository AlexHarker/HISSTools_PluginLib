
#ifndef __HISSTOOLS_CONTROLS__
#define __HISSTOOLS_CONTROLS__

#include "../HISSTools_Graphics/HISSTools_VecLib.hpp"
#include "HISSTools_Design_Scheme.hpp"
#include <vector>

#include <IPlugAPIBase.h>
#include <IPlugQueue.h>
#include <IControl.h>

enum MousingAction { kMouseDown, kMouseUp, kMouseDblClick, kMouseDrag, kMouseWheel, kMouseOver, kMouseOut };

// HISSTools_Graphics_Types
// A base struct to bring iplug and igraphics items into scope

struct HISSTools_Graphics_Types
{
    using IEditorDelegate = iplug::IEditorDelegate;
    template <typename T>
    using IPlugQueue = iplug::IPlugQueue<T>;
    using IParam = iplug::IParam;
    using IKeyPress = iplug::IKeyPress;

    using IGraphics = iplug::igraphics::IGraphics;
    using IPopupMenu = iplug::igraphics::IPopupMenu;
    using IBitmap = iplug::igraphics::IBitmap;
    using IRECT = iplug::igraphics::IRECT;
    using IPattern = iplug::igraphics::IPattern;
    using IMatrix = iplug::igraphics::IMatrix;
    using IColor = iplug::igraphics::IColor;
    using IText = iplug::igraphics::IText;
    using IShadow = iplug::igraphics::IShadow;
    using IFillOptions = iplug::igraphics::IFillOptions;
    using IStrokeOptions = iplug::igraphics::IStrokeOptions;
    using IMouseMod = iplug::igraphics::IMouseMod;
    using IActionFunction = iplug::igraphics::IActionFunction;
    using ILayerPtr = iplug::igraphics::ILayerPtr;

    using EBlend = iplug::igraphics::EBlend;
    using EAlign = iplug::igraphics::EAlign;
    using EVAlign = iplug::igraphics::EVAlign;
    using ETextStyle = iplug::igraphics::ETextStyle;
    using EDirection = iplug::igraphics::EDirection;
    using EFileAction = iplug::igraphics::EFileAction;
    using EWinding = iplug::igraphics::EWinding;
    using EFillRule = iplug::igraphics::EFillRule;
    using ELineCap = iplug::igraphics::ELineCap;
    using ELineJoin = iplug::igraphics::ELineJoin;
    using EPatternType = iplug::igraphics::EPatternType;
    using EPatternExtend = iplug::igraphics::EPatternExtend;
    using ECursor = iplug::igraphics::ECursor;
};


// HISSTools_Control_Layers
// A class to cache layers in control drawing (currently only supports a single background...)

class HISSTools_Control_Layers : protected virtual HISSTools_Graphics_Types
{
public:

    bool StartBackground(HISSTools_VecLib& vl, IRECT area);
    void RenderBackground(HISSTools_VecLib& vl, IRECT area);
    void RedrawBackground();

private:

    ILayerPtr mBackground;
    const static bool sNoCaching = false;
};


// HISSTools_Text_Helper_Block
// Helper class to deal with blocks of text 

class HISSTools_Text_Helper_Block : protected virtual HISSTools_Graphics_Types
{
public:

    // Constructors

    HISSTools_Text_Helper_Block(double x, double y, double w, double h, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);

    HISSTools_Text_Helper_Block(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
        : HISSTools_Text_Helper_Block(0, 0, 0, 0, hAlign, vAlign, name, type, scheme)
    {}

    void Draw(HISSTools_VecLib& vl);

    void ResizeText(double x, double y, double w, double h);

    void SetTextColor(HISSTools_Color_Spec *colorSpec);
    void SetText(const char *str);

    HISSTools_Bounds Bounds();

protected:

    // Dimensions

    double mX;
    double mY;
    double mW;
    double mH;

    // Appearance

    HISSTools_Text *mTextTS;
    HISSTools_Color_Spec *mTextCS;
    HISSTools_Shadow *mTextSD;
    HTextAlign mHAlign;
    VTextAlign mVAlign;

    // String

    WDL_String mStr;
};


// HISSTools_Text_Helper_Panel
// Helper class to deal with blocks of text with a panel

class HISSTools_Text_Helper_Panel : public HISSTools_Text_Helper_Block
{
public:

    HISSTools_Text_Helper_Panel(double x, double y, double w, double h, double wPad, double hPad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);

    HISSTools_Text_Helper_Panel(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
        : HISSTools_Text_Helper_Panel(0, 0, 0, 0, 0, 0, hAlign, vAlign, name, type, scheme)
    {}

    void Draw(HISSTools_VecLib& vl, bool drawText);

    void ChangePadding(double lPad, double rPad, double hPad);
    void ChangePadding(double wPad, double hPad);

    void Resize(double x, double y, double w, double h);

    void SetPanelColor(HISSTools_Color_Spec *colorSpec);
    void SetOutlineColor(HISSTools_Color_Spec *colorSpec);

    HISSTools_Bounds Bounds();

protected:

    bool DoDrawOutline();
    bool DoDrawPanel();

    // Dimensions

    double mX;
    double mY;
    double mW;
    double mH;

    double mLPad;
    double mRPad;
    double mHPad;

    // Appearance

    HISSTools_Color_Spec *mPanelFillCS;
    HISSTools_Color_Spec *mPanelOutlineCS;
    HISSTools_Shadow *mPanelSD;
    double mPanelRoundness;
    double mPanelOutlineTK;
};


// HISSTools_Text_Helper_Param
// Helper class to deal with blocks of text displaying a parameter

class HISSTools_Text_Helper_Param : public HISSTools_Text_Helper_Panel
{
public:

    // Constructors

    HISSTools_Text_Helper_Param(iplug::igraphics::IControl *control, double x, double y, double w, double h, double pad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);

    bool menuParam();

    void FinishEdit();
    void Hilite(bool on);

    void PromptUserInput();
    bool PromptUserInput(float x, float y);

    void Draw(HISSTools_VecLib& vl);

private:

    void SetControlText();

    double RoundnessCompensate(double menuTriangleHeight);

    iplug::igraphics::IControl *mControl;

    double mPromptRatio;
    double mTextHPad;
    double mSeparatorX;

    bool mDrawSeparator;
    bool mDrawTriangle;
    bool mMenuFlipTriangle;
    bool mInEdit;

    double mMenuTriangleTop;
    double mMenuTriangleBtm;
    double mMenuTriangleL;
    double mMenuTriangleR;

    HISSTools_Color_Spec *mTextHiliteCS;
    HISSTools_Color_Spec *mPanelHiliteCS;
    HISSTools_Color_Spec *mOutlineHiliteCS;
    HISSTools_Color_Spec *mTextCS;
    HISSTools_Color_Spec *mPanelFillCS;
    HISSTools_Color_Spec *mPanelOutlineCS;

    // Show Units

    bool mShowUnits;
};

// **** Controls **** //

// HISSTools_TextBlock
// A Text Block

struct HISSTools_TextBlock : public HISSTools_Text_Helper_Block, public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    HISSTools_TextBlock(double x, double y, double w, double h, const char* str = "", HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter, const char* type = 0, HISSTools_Design_Scheme *scheme = &DefaultDesignScheme);

    void Draw(IGraphics& g) override;

    void SetText(const char *str);
};


// HISSTools_Panel
// Panel with or without outline and with dropshadow

class HISSTools_Panel : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    double GetRoundness(const char *name, const char *type, HISSTools_Design_Scheme *scheme);

public:

    // Constructor

    HISSTools_Panel(double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *scheme = &DefaultDesignScheme);

public:

    // Draw

    void Draw(IGraphics& g) override;

private:

    // Positioning / Dimensions

    double mX;
    double mY;
    double mW;
    double mH;

    double mRoundnessTL;
    double mRoundnessTR;
    double mRoundnessBL;
    double mRoundnessBR;

    // Line Thicknesses

    double mOutlineTK;

    // Shadow Spec

    HISSTools_Shadow *mShadow;

    // Color Specs

    HISSTools_Color_Spec *mPanelCS;
    HISSTools_Color_Spec *mOutlineCS;

    bool mDrawOutline;
};

// HISSTools_Button
// On/Off button with text on or off the handle
// FIX - Momentary action and extensibility!!

class HISSTools_Button : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor

    HISSTools_Button(int paramIdx, double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *scheme = &DefaultDesignScheme, const char *name = "");

public:

    void OnInit() override;

    // Mousing Functions

    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;

    // Draw

    void Draw(IGraphics& g) override;

protected:

    // Text

    WDL_String mDisplayName;

private:

    // Positioning / Dimensions

    double mX;
    double mY;
    double mW;
    double mH;
    double mTextPad;
    double mRoundness;

    // Line Thicknesses

    double mOutlineTK;

    // Shadow Spec

    HISSTools_Shadow *mShadow;

    // Text Spec

    HISSTools_Text *mTextStyle;

    // Color Specs

    HISSTools_Color_Spec *mOnCS;
    HISSTools_Color_Spec *mOffCS;
    HISSTools_Color_Spec *mOutlineCS;
    HISSTools_Color_Spec *mHandleLabelCS;
    HISSTools_Color_Spec *mHandleLabelOffCS;
    HISSTools_Color_Spec *mBackgroundLabelCS;
    HISSTools_Color_Spec *mInactiveOverlayCS;

    // Label Mode

    bool mLabelMode;
};

// HISSTools_Tabs
// An abstract class to control tabs in plug-ins

class HISSTools_Tabs : protected virtual HISSTools_Graphics_Types
{
public:

    // Constructor

    // You should pass the inheriting class here, after constructing the control, which must be mapped to a valid parameter of the plug - the tabs are tied to the parameter, rather than the control

    HISSTools_Tabs(iplug::igraphics::IControl *tabControl) : mTabControl(tabControl), mParam(nullptr) {}

    // Call this from OnInit in the inheriting class
    void init();

    void attachControl(iplug::igraphics::IControl *control, int tabNumber);
    void tabHide(bool hide);
    void tabSetDirty(bool pushPararmToPlug);

    //void setTabFromPlug(int tabNumber);

    // These functions should be declared in any inheriting classes, and should call the related tab versions

    virtual void Hide(bool hide) = 0;
    virtual void SetDirty(bool pushParamToPlug, int) = 0;

private:

    struct TabItem
    {
        iplug::igraphics::IControl *mControl;
        int mTabNumber;

        TabItem(iplug::igraphics::IControl *control, int tabNumber) : mControl(control), mTabNumber(tabNumber) {}
    };

    const IParam* mParam;
    iplug::igraphics::IControl *mTabControl;
    std::vector<TabItem> mItems;

    int mCurrentTabNumber;
    int mMaxTabNumber;

    void updateItems();

    int clipTabNumber(int tabNumber);

};

// HISSTools_Invisible_Tabs
// This class allows Live-style tabs that have no explicit selector, and instead are set from the plug via other related controls

class HISSTools_Invisible_Tabs : public iplug::igraphics::IControl, public HISSTools_Tabs
{
public:

    HISSTools_Invisible_Tabs(int paramIdx) : IControl(IRECT(), paramIdx), HISSTools_Tabs(this) {}
    void OnInit() override { init(); }
    void Draw(IGraphics& g) override {}
    void Hide(bool hide) override { tabHide(hide); }
    void SetDirty(bool pushParamToPlug, int) override { tabSetDirty(pushParamToPlug); }
};

// HISSTools_Value
// Multipurpose numerical / menu control
// FIX - do your own mousing later...

class HISSTools_Value : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    HISSTools_Value(int paramIdx, double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr);
    ~HISSTools_Value();

    void OnInit() override;

    // Mousing Functions

    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    void OnMouseUp(float x, float y, const IMouseMod& pMod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;
    void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override;
    virtual void SetValueFromUserInput(double value, int valIdx) override;

    // Draw

    void Draw(IGraphics& g) override;

private:

    // Text

    HISSTools_Text_Helper_Param *mTextParam;
    HISSTools_Text_Helper_Block *mTextLabel;

    // Name

    WDL_String mDisplayName;

    // Mousing

    bool mDrag;

    // Stored Dimensions

    double mTextArea;
};

// HISSTools_Dial
// Multipurpose dial with auto value display
// FIX - do your own mousing later...

class HISSTools_Dial : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    HISSTools_Dial(int paramIdx, double x, double y, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr);
    ~HISSTools_Dial();

    void OnInit() override;

    // Mousing Functions

    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override;
    void OnMouseOver(float x, float y, const IMouseMod& pMod) override;
    void OnMouseOut() override;
    void SetDisabled(bool disable) override;
    void SetValueFromUserInput(double value, int valIdx) override;

    // Draw

    void Draw(IGraphics& g) override;

    void setThrow(double refValue, double startAngle, double throwAngle);

private:

    void setPointerAppearance(double pointerCircRatio, double pointerTipRatio, double pointerAngle);

    // Positioning / Dimensions

    double mCx;
    double mCy;
    double mR;

    // Pointer Appearance

    double mPointerCircRadius;
    double mPointerTipRadius;
    double mPointerAngle;

    // Display Angles

    double mRefValue;
    double mStartAngle;
    double mThrowAngle;

    // Text Area

    double mTextArea;

    // Text Prompt

    double mPromptHalfHeight;

    // Line Thicknesses

    double mOutlineTK;
    double mPointerTK;
    double mPointerOutlineTK;

    // Shadow Specs

    HISSTools_Shadow *mOutlineSD;
    HISSTools_Shadow *mPointerSD;

    // Color Specs

    HISSTools_Color_Spec *mIndicatorCS;
    HISSTools_Color_Spec *mCircleFillCS;
    HISSTools_Color_Spec *mPointerFillCS;
    HISSTools_Color_Spec *mOutlineCS;
    HISSTools_Color_Spec *mPointerOutlineCS;
    HISSTools_Color_Spec *mInactiveOverlayCS;

    // Text Blocks

    HISSTools_Text_Helper_Block *mTextLabel;
    HISSTools_Text_Helper_Param *mTextParam;

    // Values on when mouse is over

    bool mMouseOver;
    bool mDrawValOnlyOnMO;

    WDL_String mDisplayName;
};

// HISSTools_Switch
// Switch - multi state control with a number of vertical or horizontal positions

class HISSTools_Switch : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor

    HISSTools_Switch(int paramIdx, double x, double y, double w, double h, int nStates = 2, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme);

    // Mousing Functions

    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;

    // Draw

    void Draw(IGraphics& g) override;

private:

    // Positioning / Dimensions

    double mX;
    double mY;
    double mW;
    double mH;
    double mS;
    double mRoundness;

    // Line Thicknesses

    double mHandleTK;
    double mBoxOutlineTK;

    // Shadow Spec

    HISSTools_Shadow *mShadow;

    // Color Specs

    HISSTools_Color_Spec *mHandleFillCS;
    HISSTools_Color_Spec *mHandleOutlineCS;
    HISSTools_Color_Spec *mBoxFillCS;
    HISSTools_Color_Spec *mBoxOutlineCS;
    HISSTools_Color_Spec *mInactiveOverlayCS;

    // Number of States

    int mNStates;
};

// HISSTools_Matrix
//

class HISSTools_Matrix : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    HISSTools_Matrix(int paramIdx, double x, double y, int xDim, int yDim, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, HISSTools_Design_Scheme *stateScheme = 0);

    ~HISSTools_Matrix();

    int getXPos() const;
    int getYPos() const;

    // Mousing Functions

    bool OnMousing(float x, float y, const IMouseMod& mod, MousingAction action, float wheel = 0.f);
    void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    void OnMouseUp(float x, float y, const IMouseMod& mod) override;
    void OnMouseDblClick(float x, float y, const IMouseMod& mod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
    void OnMouseWheel(float x, float y, const IMouseMod& pMod, float d) override;
    void OnMouseOver(float x, float y, const IMouseMod& mod) override;
    virtual void OnMouseOut() override;

    // Draw

    void Draw(IGraphics& g) override;

    void SetState(int x, int y, char state);
    unsigned char GetState(int x, int y);

    void SetHilite(bool on);

private:

    bool coordsToIndices(double x, double y, int *xPos, int *yPos);
    virtual void reportToPlug(int xPos, int yPos, const IMouseMod& mod, MousingAction action, float wheel = 0.f)
    {}

    // Size (Dimension)

    int mXDim;
    int mYDim;

    // Positioning / Dimensions

    double mX;
    double mY;
    double mW;
    double mH;
    double mS;
    double mRoundness;
    double mGap;
    double mUnit;

    // Line Thicknesses

    double mHandleEmptyOutlineTK;
    double mHandleFilledOutlineTK;
    double mHiliteTK;

    // Shadow Spec

    HISSTools_Shadow *mShadow;

    // Color Specs

    HISSTools_Color_Spec *mStateCS[256];
    HISSTools_Color_Spec *mOutlineCS;
    HISSTools_Color_Spec *mHiliteCS;

    // States

    unsigned char *mStates;
    unsigned char mNStates;

    // Hilite

    int mXHilite;
    int mYHilite;

    // Mousing Info

    int mXPos;
    int mYPos;
};

// HISSTools_Progress
//

class HISSTools_Progress : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    HISSTools_Progress(double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme);

    ~HISSTools_Progress() {}

    // Draw

    void Draw(IGraphics& g);

private:

    // Positioning / Dimensions

    double mX;
    double mY;
    double mW;
    double mH;

    // Line Thicknesses

    double mOutlineTK;

    // Shadow Spec

    HISSTools_Shadow *mShadow;

    // Color Specs

    HISSTools_Color_Spec *mBackgroundCS;
    HISSTools_Color_Spec *mOutlineCS;
    HISSTools_Color_Spec *mProgressCS;
};

// HISSTools_VUMeter
// A VU meter display. Ballistics should be provided on the DSP side
// The meter supports two on meter levels (drawn in order) and a side value, typically intended for peak hold

class HISSTools_VUMeter : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    enum { kUpdateTag = 0, };

    struct MeterValues
    {
        double mVU1;
        double mVU2;
        double mSide;
        bool mPeak;
        bool mLinear;
    };

public:

    class Sender
    {
    public:

        Sender(int controlTag) : mControlTag(controlTag), mQueue(32) {}

        void Set(double VU1, double VU2, double side, bool peak, bool linear = true);
        void UpdateControl(IEditorDelegate& dlg);
        void Reset();

    private:

        int mControlTag;
        IPlugQueue<MeterValues> mQueue;
    };

    HISSTools_VUMeter(double x, double y, double w, double h, bool flip = false, double minDB = -60, double maxDB = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme);

    ~HISSTools_VUMeter() {}

    void OnMsgFromDelegate(int messageTag, int dataSize, const void* pData) override;

    // Draw

    void Draw(IGraphics& g) override;

private:

    // N.B. currently we linearly interpolate dB but we could do something nicer here later.....

    double getSize(double value, bool linear);

    void horzTick(HISSTools_VecLib& vecDraw, double x1, double x2, double y, double h, double normPosition, double thickness);

    void vertTick(HISSTools_VecLib& vecDraw, double y1, double y2, double x, double w, double normPosition, double thickness);

    // Positioning / Dimensions

    double mX;
    double mY;
    double mW;
    double mH;
    double mTick1;
    double mTick2;
    double mTick3;
    double mTick4;

    // Line Thicknesses

    double mOutlineTK;
    double mTickTK;
    double mPeakHoldTK;

    // Shadow Spec

    HISSTools_Shadow *mShadow;

    // Drawing parameters

    double mVU1Size;
    double mVU2Size;
    double mSideSize;

    bool mPeak;

    // Values

    double mMinDB;
    double mMaxDB;

    // Color Specs

    HISSTools_Color_Spec *mBackgroundCS;
    HISSTools_Color_Spec *mOutlineCS;
    HISSTools_Color_Spec *mVU1CS;
    HISSTools_Color_Spec *mVU2CS;
    HISSTools_Color_Spec *mVUSideCS;
    HISSTools_Color_Spec *mVU1PeakCS;
    HISSTools_Color_Spec *mVU2PeakCS;
    HISSTools_Color_Spec *mVUSidePeakCS;

    bool mOverlayFixedGradientBox;

};

// HISSTools_FileSelector
// 

class HISSTools_FileSelector : public HISSTools_Button
{
public:

    enum EFileSelectorState { kFSNone, kFSSelecting, kFSDone };

    // FIX - turn automation off (also for matrix)

    // Constructor

    HISSTools_FileSelector(int paramIdx, double x, double y, double w, double h, EFileAction action, char* dir = "", char* extensions = "", const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char *label = "")
        : HISSTools_Button(paramIdx, x, y, w, h, type, designScheme, label), mState(kFSNone), mFileAction(action), mDir(dir), mExtensions(extensions)
    {}

    // Mousing Functions

    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;
    void OnMouseUp(float x, float y, const IMouseMod& pMod) override;

    // Draw

    void Draw(IGraphics& g) override;

    // File Functions

    const WDL_String& GetLastSelectedFileForPlug();
    void SetLastSelectedFileFromPlug(const char* file);
    void SetExtensions(char *extensions);

private:

    virtual void reportToPlug() {}

    // Strings

    WDL_String mDir;
    WDL_String mFile;
    WDL_String mExtensions;

    // States

    EFileAction mFileAction;
    EFileSelectorState mState;

};

#ifndef NO_HISSTOOLS_CONTROL_HELPERS_COMPILE
#include "HISSTools_Controls.cpp"
#endif

#endif /* __HISSTOOLS_CONTROLS__ */
