
#ifndef __HISSTOOLS_CONTROL_HELPERS__
#define __HISSTOOLS_CONTROL_HELPERS__

#include "../HISSTools_Graphics/HISSTools_VecLib.hpp"
#include "HISSTools_Design_Scheme.hpp"

#include <IPlugQueue.h>
#include <IControl.h>

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
	
    HISSTools_Text_Helper_Block(double x, double y, double w, double h,  HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);
	
    HISSTools_Text_Helper_Block(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
    : HISSTools_Text_Helper_Block(0, 0, 0, 0 , hAlign, vAlign, name, type, scheme)
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

class HISSTools_Panel: public iplug::igraphics::IControl, public HISSTools_Control_Layers
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

class HISSTools_Button: public iplug::igraphics::IControl, public HISSTools_Control_Layers
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

public:

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
  void OnInit() override                              { init(); }
  void Draw(IGraphics& g) override                    { }
  void Hide(bool hide) override                       { tabHide(hide); }
  void SetDirty(bool pushParamToPlug, int) override   { tabSetDirty(pushParamToPlug); }
};

// HISSTools_Value
// Multipurpose numerical / menu control
// FIX - do your own mousing later...

class HISSTools_Value : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{

public:

  HISSTools_Value(int paramIdx, double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr)
    : IKnobControlBase(IRECT(), paramIdx), HISSTools_Control_Layers();

  ~HISSTools_Value();

  void OnInit() override;

  void OnMouseDown(float x, float y, const IMouseMod& pMod) override;

  void OnMouseUp(float x, float y, const IMouseMod& pMod) override;

  void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;

  void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override;

  virtual void SetValueFromUserInput(double value, int valIdx) override;

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

#ifndef NO_HISSTOOLS_CONTROL_HELPERS_COMPILE
#include "HISSTools_Control_Helpers.cpp"
#endif

#endif /* __HISSTOOLS_CONTROL_HELPERS__ */
