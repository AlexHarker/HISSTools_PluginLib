
/**
 * @file HISSTools_Controls.hpp
 * @brief Header file containing the declaration of various UI controls and graphical utilities for HISSTools.
 *
 * This file provides the definitions and declarations of the key classes and utilities used to build the user interface
 * components in the HISSTools plugin framework. The controls defined here, such as buttons, sliders, VU meters, and file selectors,
 * handle user interaction, graphics rendering, and visual customization for audio and plugin development environments.
 *
 * The file includes:
 * - UI control classes such as `HISSTools_Button`, `HISSTools_VUMeter`, `HISSTools_FileSelector`, `HISSTools_Dial`, and more.
 * - Graphics utilities for handling color specifications (`HISSTools_Color_Spec`), shadow rendering (`HISSTools_Shadow`), and layout management.
 * - Utility classes for handling text rendering, alignment, and interactive elements like sliders and knobs.
 *
 * @details
 * The controls in this file are designed to work with the iPlug graphical framework and provide flexible ways
 * to customize and extend the appearance and functionality of user interface components. The file also includes
 * various enums and type definitions that define behaviors such as alignment (`EAlign`), file actions (`EFileAction`),
 * and graphical extensions (`EPatternExtend`).
 *
 * @note This file should be included in any project that uses the HISSTools UI framework for building custom graphical controls.
 *
 * @see HISSTools_Button
 * @see HISSTools_VUMeter
 * @see iplug::igraphics::IGraphics
 */

#ifndef __HISSTOOLS_CONTROLS__
#define __HISSTOOLS_CONTROLS__

#include "../HISSTools_Graphics/HISSTools_VecLib.hpp"
#include "HISSTools_Design_Scheme.hpp"
#include <vector>

#include <IPlugAPIBase.h>
#include <IPlugQueue.h>
#include <IControl.h>

/**
 * @enum MousingAction
 * @brief Represents the different mouse actions that can be performed.
 *
 * This enumeration defines the various types of mouse actions that can
 * be detected and handled within the system.
 *
 * @var MousingAction::kMouseDown
 * Represents a mouse button press action.
 *
 * @var MousingAction::kMouseUp
 * Represents a mouse button release action.
 *
 * @var MousingAction::kMouseDblClick
 * Represents a double-click mouse action.
 *
 * @var MousingAction::kMouseDrag
 * Represents a mouse drag action.
 *
 * @var MousingAction::kMouseWheel
 * Represents a mouse wheel scroll action.
 *
 * @var MousingAction::kMouseOver
 * Represents a mouse hover action.
 *
 * @var MousingAction::kMouseOut
 * Represents a mouse out (mouse leaves an area) action.
 */

enum MousingAction { kMouseDown, kMouseUp, kMouseDblClick, kMouseDrag, kMouseWheel, kMouseOver, kMouseOut };

// HISSTools_Graphics_Types
// A base struct to bring iplug and igraphics items into scope

/**
 * @struct HISSTools_Graphics_Types
 * @brief Contains types and structures used for graphics operations in HISSTools.
 *
 * This structure holds various types and definitions related to the HISSTools graphics system.
 * It is used to define and manage graphical components and their properties within the system.
 *
 * Detailed information about the fields and methods (if any) within this structure can be added
 * as part of the struct definition.
 */

struct HISSTools_Graphics_Types
{
    
    /**
     * @brief Alias for the `iplug::IEditorDelegate` type.
     *
     * This line creates a type alias for the `iplug::IEditorDelegate` class, simplifying its usage within the code.
     * By using `IEditorDelegate`, developers can reference the `iplug::IEditorDelegate` type without needing to fully qualify
     * the namespace each time. The `IEditorDelegate` is typically responsible for handling communication between the user interface
     * and the audio processing engine or other plugin components.
     */
    
    using IEditorDelegate = iplug::IEditorDelegate;
    
    /**
     * @brief Template declaration for a generic type `T`.
     *
     * This line defines a template, allowing the following class or function to accept a generic type `T`.
     * Templates enable code to be written in a type-independent manner, allowing the same class or function
     * to work with various data types. The specific type `T` will be determined when the template is instantiated.
     *
     * @tparam T A placeholder for a data type that will be specified when the template is used.
     */
    
    template <typename T>
    
    /**
     * @brief Alias for the `iplug::IPlugQueue` template class with type `T`.
     *
     * This line creates a type alias for the `iplug::IPlugQueue` template class, where `T` is the type specified when the alias is used.
     * The `IPlugQueue` is a thread-safe queue class from the iPlug framework, typically used for handling real-time data,
     * such as audio processing or UI updates. By using this alias, the code can refer to `IPlugQueue` more concisely.
     *
     * @tparam T The data type that the queue will hold, which is specified when the alias is instantiated.
     */
    
    using IPlugQueue = iplug::IPlugQueue<T>;
    
    /**
     * @brief Alias for the `iplug::IParam` type.
     *
     * This line creates a type alias for the `iplug::IParam` class, simplifying its usage within the code.
     * The `IParam` class in the iPlug framework is used to represent a parameter, such as a control parameter in an audio plugin.
     * By using this alias, developers can reference `iplug::IParam` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IParam = iplug::IParam;
    
    /**
     * @brief Alias for the `iplug::IKeyPress` type.
     *
     * This line creates a type alias for the `iplug::IKeyPress` class, simplifying its usage within the code.
     * The `IKeyPress` class in the iPlug framework is used to represent key press events, capturing information about
     * keyboard input in a plugin or user interface. By using this alias, developers can refer to `iplug::IKeyPress`
     * more concisely without needing to fully qualify the namespace each time.
     */
    
    using IKeyPress = iplug::IKeyPress;

    /**
     * @brief Alias for the `iplug::igraphics::IGraphics` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IGraphics` class, simplifying its usage within the code.
     * The `IGraphics` class in the iPlug framework is used to handle graphical rendering, including drawing user interface elements
     * for audio plugins. By using this alias, developers can refer to `iplug::igraphics::IGraphics` more concisely without needing
     * to fully qualify the namespace each time.
     */
    
    using IGraphics = iplug::igraphics::IGraphics;
    
    /**
     * @brief Alias for the `iplug::igraphics::IPopupMenu` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IPopupMenu` class, simplifying its usage within the code.
     * The `IPopupMenu` class in the iPlug framework is used to create and manage popup menus in the graphical user interface
     * of audio plugins. By using this alias, developers can refer to `iplug::igraphics::IPopupMenu` more concisely
     * without needing to fully qualify the namespace each time.
     */
    
    using IPopupMenu = iplug::igraphics::IPopupMenu;
    
    /**
     * @brief Alias for the `iplug::igraphics::IBitmap` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IBitmap` class, simplifying its usage within the code.
     * The `IBitmap` class in the iPlug framework is used to represent bitmap images in the graphical user interface
     * of audio plugins. By using this alias, developers can refer to `iplug::igraphics::IBitmap` more concisely
     * without needing to fully qualify the namespace each time.
     */
    
    using IBitmap = iplug::igraphics::IBitmap;
    
    /**
     * @brief Alias for the `iplug::igraphics::IRECT` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IRECT` class, simplifying its usage within the code.
     * The `IRECT` class in the iPlug framework is used to represent rectangular areas in the graphical user interface,
     * defining regions for drawing or positioning elements. By using this alias, developers can refer to `iplug::igraphics::IRECT`
     * more concisely without needing to fully qualify the namespace each time.
     */
    
    using IRECT = iplug::igraphics::IRECT;
    
    /**
     * @brief Alias for the `iplug::igraphics::IPattern` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IPattern` class, simplifying its usage within the code.
     * The `IPattern` class in the iPlug framework is used to represent color patterns or gradients that can be applied
     * to graphical elements in the user interface, such as backgrounds, fills, or shading. By using this alias, developers
     * can refer to `iplug::igraphics::IPattern` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IPattern = iplug::igraphics::IPattern;
    
    /**
     * @brief Alias for the `iplug::igraphics::IMatrix` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IMatrix` class, simplifying its usage within the code.
     * The `IMatrix` class in the iPlug framework is used for representing transformation matrices, which can be applied
     * to graphical elements to perform operations such as scaling, rotation, and translation. By using this alias, developers
     * can refer to `iplug::igraphics::IMatrix` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IMatrix = iplug::igraphics::IMatrix;
    
    /**
     * @brief Alias for the `iplug::igraphics::IColor` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IColor` class, simplifying its usage within the code.
     * The `IColor` class in the iPlug framework is used to represent and manage color values, including RGB and alpha (transparency)
     * channels, for drawing and styling graphical elements in the plugin's user interface. By using this alias, developers
     * can refer to `iplug::igraphics::IColor` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IColor = iplug::igraphics::IColor;
    
    /**
     * @brief Alias for the `iplug::igraphics::IText` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IText` class, simplifying its usage within the code.
     * The `IText` class in the iPlug framework is used to represent text styling properties, such as font, size, alignment,
     * and color, for rendering text in the graphical user interface. By using this alias, developers can refer to
     * `iplug::igraphics::IText` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IText = iplug::igraphics::IText;
    
    /**
     * @brief Alias for the `iplug::igraphics::IShadow` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IShadow` class, simplifying its usage within the code.
     * The `IShadow` class in the iPlug framework is used to define shadow properties, such as offset, blur, and color,
     * for rendering shadows behind graphical elements in the user interface. By using this alias, developers can refer
     * to `iplug::igraphics::IShadow` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IShadow = iplug::igraphics::IShadow;
    
    /**
     * @brief Alias for the `iplug::igraphics::IFillOptions` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IFillOptions` class, simplifying its usage within the code.
     * The `IFillOptions` class in the iPlug framework is used to define various options for filling graphical elements,
     * such as fill type (solid, gradient), winding rules, and other properties that control how shapes are filled.
     * By using this alias, developers can refer to `iplug::igraphics::IFillOptions` more concisely without needing to
     * fully qualify the namespace each time.
     */
    
    using IFillOptions = iplug::igraphics::IFillOptions;
    
    /**
     * @brief Alias for the `iplug::igraphics::IStrokeOptions` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IStrokeOptions` class, simplifying its usage within the code.
     * The `IStrokeOptions` class in the iPlug framework is used to define options for stroking graphical elements,
     * such as line width, line joins, and end caps. These options control how the outlines or borders of shapes and paths
     * are drawn in the graphical user interface. By using this alias, developers can refer to `iplug::igraphics::IStrokeOptions`
     * more concisely without needing to fully qualify the namespace each time.
     */
    
    using IStrokeOptions = iplug::igraphics::IStrokeOptions;
    
    /**
     * @brief Alias for the `iplug::igraphics::IMouseMod` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IMouseMod` class, simplifying its usage within the code.
     * The `IMouseMod` class in the iPlug framework is used to represent the state of the mouse modifiers, such as
     * whether the control, shift, or alt keys are pressed, and the state of the mouse buttons during an interaction.
     * It helps manage and interpret mouse events in the graphical user interface. By using this alias, developers can
     * refer to `iplug::igraphics::IMouseMod` more concisely without needing to fully qualify the namespace each time.
     */
    
    using IMouseMod = iplug::igraphics::IMouseMod;
    
    /**
     * @brief Alias for the `iplug::igraphics::IActionFunction` type.
     *
     * This line creates a type alias for the `iplug::igraphics::IActionFunction` type, simplifying its usage within the code.
     * The `IActionFunction` is a function type in the iPlug framework that is used to define callback functions
     * or actions triggered by certain events, such as user interactions with controls in the graphical user interface.
     * It allows developers to define custom behaviors that execute in response to specific user actions.
     *
     * By using this alias, developers can refer to `iplug::igraphics::IActionFunction` more concisely without needing
     * to fully qualify the namespace each time.
     */
    
    using IActionFunction = iplug::igraphics::IActionFunction;
    
    /**
     * @brief Alias for the `iplug::igraphics::ILayerPtr` type.
     *
     * This line creates a type alias for the `iplug::igraphics::ILayerPtr`, which is a pointer to a graphical layer in the iPlug framework.
     * A layer represents a cached or off-screen drawing surface, which can be used to optimize rendering by reducing the need to
     * redraw elements repeatedly. The alias simplifies the use of `ILayerPtr` when dealing with layers in the graphical user interface.
     *
     * By using this alias, developers can refer to `iplug::igraphics::ILayerPtr` more concisely without needing
     * to fully qualify the namespace each time.
     */
    
    using ILayerPtr = iplug::igraphics::ILayerPtr;

    /**
     * @brief Alias for the `iplug::igraphics::EBlend` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EBlend` enumeration, which defines different blending modes
     * used for drawing graphical elements in the iPlug framework. Blending modes control how colors from different layers
     * or objects are combined, allowing for effects like transparency, multiplication, and screen blending.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EBlend` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EBlend = iplug::igraphics::EBlend;
    
    /**
     * @brief Alias for the `iplug::igraphics::EAlign` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EAlign` enumeration, which defines different alignment options
     * used for positioning graphical elements or text in the iPlug framework. These alignment options control how content is
     * aligned horizontally and vertically within a defined space, such as left, center, or right alignment.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EAlign` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EAlign = iplug::igraphics::EAlign;
    
    /**
     * @brief Alias for the `iplug::igraphics::EVAlign` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EVAlign` enumeration, which defines vertical alignment options
     * used for positioning graphical elements or text in the iPlug framework. The vertical alignment options control how content
     * is aligned within a vertical space, such as top, middle, or bottom alignment.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EVAlign` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EVAlign = iplug::igraphics::EVAlign;
    
    /**
     * @brief Alias for the `iplug::igraphics::ETextStyle` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::ETextStyle` enumeration, which defines various text styles
     * used for rendering text in the iPlug framework. The text styles include options such as regular, bold, italic, and underline,
     * which control the appearance of the text in the graphical user interface.
     *
     * By using this alias, developers can refer to `iplug::igraphics::ETextStyle` more concisely without needing to fully qualify the namespace each time.
     */
    
    using ETextStyle = iplug::igraphics::ETextStyle;
    
    /**
     * @brief Alias for the `iplug::igraphics::EDirection` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EDirection` enumeration, which defines different directional options
     * used for various graphical operations in the iPlug framework. These options typically control the orientation or flow of elements,
     * such as sliders, meters, or animations, in directions like horizontal, vertical, or bi-directional.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EDirection` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EDirection = iplug::igraphics::EDirection;
    
    /**
     * @brief Alias for the `iplug::igraphics::EFileAction` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EFileAction` enumeration, which defines the different actions
     * that can be performed with a file selector in the iPlug framework. These actions typically include opening a file or saving a file,
     * allowing the `HISSTools_FileSelector` or similar controls to manage file input and output behavior in the graphical user interface.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EFileAction` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EFileAction = iplug::igraphics::EFileAction;
    
    /**
     * @brief Alias for the `iplug::igraphics::EWinding` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EWinding` enumeration, which defines different winding rules
     * used for filling paths or shapes in the iPlug framework. Winding rules determine how the interior of a shape is defined,
     * affecting how complex shapes with overlapping paths are filled. The common winding rules include non-zero and even-odd.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EWinding` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EWinding = iplug::igraphics::EWinding;
    
    /**
     * @brief Alias for the `iplug::igraphics::EFillRule` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EFillRule` enumeration, which defines the rules used for filling shapes
     * in the iPlug framework. The fill rules determine how the interior of complex shapes is rendered when paths intersect or overlap.
     * Common fill rules include the "non-zero" rule and the "even-odd" rule, which influence how the shape's inside is calculated.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EFillRule` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EFillRule = iplug::igraphics::EFillRule;
    
    /**
     * @brief Alias for the `iplug::igraphics::ELineCap` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::ELineCap` enumeration, which defines the different styles
     * used to render the ends of lines in the iPlug framework. Line caps determine how the endpoints of a stroke are drawn,
     * with common options including butt, round, and square caps, affecting the visual appearance of lines in the graphical interface.
     *
     * By using this alias, developers can refer to `iplug::igraphics::ELineCap` more concisely without needing to fully qualify the namespace each time.
     */
    
    using ELineCap = iplug::igraphics::ELineCap;
    
    /**
     * @brief Alias for the `iplug::igraphics::ELineJoin` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::ELineJoin` enumeration, which defines the different styles
     * used to render the joints between connected line segments in the iPlug framework. Line joins determine how the corners
     * between two line segments are drawn, with common options including miter, round, and bevel joins. These options affect
     * the appearance of sharp or smooth transitions between line segments in the graphical user interface.
     *
     * By using this alias, developers can refer to `iplug::igraphics::ELineJoin` more concisely without needing to fully qualify the namespace each time.
     */
    
    using ELineJoin = iplug::igraphics::ELineJoin;
    
    /**
     * @brief Alias for the `iplug::igraphics::EPatternType` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EPatternType` enumeration, which defines different types of patterns
     * used in the iPlug framework for graphical rendering. Patterns can be solid colors, gradients, or other visual effects that
     * can be applied to fills, strokes, and backgrounds in the graphical user interface.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EPatternType` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EPatternType = iplug::igraphics::EPatternType;
    
    /**
     * @brief Alias for the `iplug::igraphics::EPatternExtend` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::EPatternExtend` enumeration, which defines the different extension modes
     * for patterns in the iPlug framework. These modes control how patterns (such as gradients) are extended beyond their boundaries,
     * with options like repeating, reflecting, or clamping the pattern to fit the drawing area.
     *
     * By using this alias, developers can refer to `iplug::igraphics::EPatternExtend` more concisely without needing to fully qualify the namespace each time.
     */
    
    using EPatternExtend = iplug::igraphics::EPatternExtend;
    
    /**
     * @brief Alias for the `iplug::igraphics::ECursor` enumeration.
     *
     * This line creates a type alias for the `iplug::igraphics::ECursor` enumeration, which defines the different types of mouse cursors
     * that can be used in the iPlug framework. The cursor types include standard options such as arrow, pointer, text, resize, and others,
     * allowing for a customized user experience based on interaction context.
     *
     * By using this alias, developers can refer to `iplug::igraphics::ECursor` more concisely without needing to fully qualify the namespace each time.
     */
    
    using ECursor = iplug::igraphics::ECursor;
};


// HISSTools_Control_Layers
// A class to cache layers in control drawing (currently only supports a single background...)

/**
 * @class HISSTools_Control_Layers
 * @brief Manages control layers for the HISSTools graphics system.
 *
 * This class handles the various control layers used in HISSTools,
 * providing functionalities to manage, manipulate, and interact with graphical layers.
 * It inherits from `HISSTools_Graphics_Types` to gain access to various
 * graphical type definitions and functionality.
 *
 * Inheritance:
 * - Inherits protected members from `HISSTools_Graphics_Types`.
 * - Inherits virtually to support multiple inheritance without ambiguity.
 *
 * This class is designed to be used internally and provides functionality
 * for managing graphical elements in a layered system, which can be customized
 * or extended in derived classes.
 */

class HISSTools_Control_Layers : protected virtual HISSTools_Graphics_Types
{
public:

    /**
     * @brief Initiates the background process for a specified area using vector graphics.
     *
     * This function begins a background process, likely for rendering or other graphical
     * operations, within a specified area using the provided vector library.
     *
     * @param vl A reference to the `HISSTools_VecLib` object used for vector graphics operations.
     * @param area An `IRECT` object defining the rectangular area where the background process will take place.
     *
     * @return Returns `true` if the background process was successfully started, otherwise returns `false`.
     */
    
    bool StartBackground(HISSTools_VecLib& vl, IRECT area);
    
    /**
     * @brief Renders the background for a specified area using vector graphics.
     *
     * This function is responsible for rendering the background within a given area
     * using the provided vector graphics library. It is likely used to handle the visual
     * rendering of a graphical component's background.
     *
     * @param vl A reference to the `HISSTools_VecLib` object used for vector graphics operations.
     * @param area An `IRECT` object defining the rectangular area where the background will be rendered.
     */
    
    void RenderBackground(HISSTools_VecLib& vl, IRECT area);
    
    /**
     * @brief Triggers a redraw of the background.
     *
     * This function initiates the process of redrawing the background. It is typically called
     * when the background needs to be refreshed or updated due to changes in the graphical
     * content or other events.
     *
     * The method does not take any parameters and operates on the current background context.
     */
    
    void RedrawBackground();

private:

    /**
     * @brief Pointer to the background layer.
     *
     * This member variable holds a pointer to the background layer, represented by
     * an `ILayerPtr`. It is used to manage and manipulate the graphical background
     * layer, which is rendered and redrawn as needed.
     *
     * The background layer typically contains graphical elements that are drawn
     * behind other content in the control layers.
     */
    
    ILayerPtr mBackground;
    
    /**
     * @brief Flag to control caching behavior.
     *
     * This static constant is used to indicate whether caching is disabled.
     *
     * @note Setting this value to `false` means that caching is enabled by default.
     * The `sNoCaching` flag can be used in methods or operations that involve
     * caching mechanisms, allowing control over whether or not to use cached data.
     */
    
    const static bool sNoCaching = false;
};


// HISSTools_Text_Helper_Block
// Helper class to deal with blocks of text 

/**
 * @class HISSTools_Text_Helper_Block
 * @brief A helper class for managing blocks of text in the HISSTools graphics system.
 *
 * This class provides functionalities to handle and manipulate blocks of text
 * within the HISSTools graphics framework. It offers methods for managing text
 * layout, rendering, and interaction with other graphical components.
 *
 * Inherits from `HISSTools_Graphics_Types` to gain access to graphical types and utilities.
 *
 * Inheritance:
 * - Protected and virtual inheritance from `HISSTools_Graphics_Types` to allow for integration
 *   with other components of the graphics system, supporting multiple inheritance where necessary.
 */

class HISSTools_Text_Helper_Block : protected virtual HISSTools_Graphics_Types
{
public:

    // Constructors

    /**
     * @brief Constructs a `HISSTools_Text_Helper_Block` object with specified parameters.
     *
     * This constructor initializes a text helper block with specified position, dimensions,
     * alignment, and other properties necessary for text rendering and layout in the HISSTools
     * graphics system.
     *
     * @param x The x-coordinate of the top-left corner of the text block.
     * @param y The y-coordinate of the top-left corner of the text block.
     * @param w The width of the text block.
     * @param h The height of the text block.
     * @param hAlign Horizontal text alignment, defined by the `HTextAlign` enum (e.g., left, center, right).
     * @param vAlign Vertical text alignment, defined by the `VTextAlign` enum (e.g., top, middle, bottom).
     * @param name A C-string representing the name of the text block, typically used for identification.
     * @param type A C-string representing the type of text block, which could define its usage or category.
     * @param scheme A pointer to the `HISSTools_Design_Scheme`, which holds design parameters such as colors and fonts used in the text block.
     */
    
    HISSTools_Text_Helper_Block(double x, double y, double w, double h, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);

    /**
     * @brief Constructs a `HISSTools_Text_Helper_Block` object with default dimensions and specified alignment, name, type, and design scheme.
     *
     * This constructor initializes a text helper block with default dimensions (x = 0, y = 0, width = 0, height = 0)
     * while allowing the horizontal and vertical alignment, name, type, and design scheme to be specified.
     *
     * It internally delegates the initialization to another constructor of the same class that takes full
     * parameters for position and dimensions.
     *
     * @param hAlign Horizontal text alignment, defined by the `HTextAlign` enum (e.g., left, center, right).
     * @param vAlign Vertical text alignment, defined by the `VTextAlign` enum (e.g., top, middle, bottom).
     * @param name A C-string representing the name of the text block, typically used for identification.
     * @param type A C-string representing the type of text block, which could define its usage or category.
     * @param scheme A pointer to the `HISSTools_Design_Scheme`, which holds design parameters such as colors and fonts used in the text block.
     *
     * @note This constructor is useful when specific dimensions are not needed at initialization time.
     */
    
    HISSTools_Text_Helper_Block(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
        : HISSTools_Text_Helper_Block(0, 0, 0, 0, hAlign, vAlign, name, type, scheme)
    {}

    /**
     * @brief Draws the text block using the specified vector graphics library.
     *
     * This method renders the text block using the provided `HISSTools_VecLib` instance.
     * It handles the drawing of text and any associated graphical elements within the block,
     * applying the current properties such as alignment, design scheme, and position.
     *
     * @param vl A reference to the `HISSTools_VecLib` object used to perform vector-based drawing operations.
     */
    
    void Draw(HISSTools_VecLib& vl);

    /**
     * @brief Resizes and repositions the text block.
     *
     * This method adjusts the position and dimensions of the text block. It updates the
     * coordinates and size of the text area based on the provided parameters, allowing
     * the text to be resized and repositioned dynamically.
     *
     * @param x The new x-coordinate for the top-left corner of the text block.
     * @param y The new y-coordinate for the top-left corner of the text block.
     * @param w The new width of the text block.
     * @param h The new height of the text block.
     */
    
    void ResizeText(double x, double y, double w, double h);

    /**
     * @brief Sets the color of the text block.
     *
     * This method updates the color of the text block by applying the specified color
     * scheme. The `HISSTools_Color_Spec` provides the color specification, which is used to
     * change the appearance of the text within the block.
     *
     * @param colorSpec A pointer to a `HISSTools_Color_Spec` object that contains the color
     * information to be applied to the text.
     */
    
    void SetTextColor(HISSTools_Color_Spec *colorSpec);
    
    /**
     * @brief Sets the text content of the text block.
     *
     * This method updates the content of the text block by assigning a new string.
     * The provided C-string is used as the new text that will be displayed within the text block.
     *
     * @param str A pointer to a C-string representing the new text to be set in the text block.
     */
    
    void SetText(const char *str);

    /**
     * @brief Retrieves the bounds of the text block.
     *
     * This method returns the current bounding box of the text block, represented
     * by a `HISSTools_Bounds` object. The bounds define the position and size of the
     * text block within the graphical context.
     *
     * @return A `HISSTools_Bounds` object representing the current position and size of the text block.
     */
    
    HISSTools_Bounds Bounds();

protected:

    // Dimensions

    /**
     * @brief The x-coordinate of the text block's position.
     *
     * This member variable stores the x-coordinate of the top-left corner of the
     * text block, determining its horizontal position within the graphical context.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate of the text block's position.
     *
     * This member variable stores the y-coordinate of the top-left corner of the
     * text block, determining its vertical position within the graphical context.
     */
    
    double mY;
    
    /**
     * @brief The width of the text block.
     *
     * This member variable stores the width of the text block, determining how
     * wide the block will be rendered within the graphical context.
     */
    
    double mW;
    
    /**
     * @brief The height of the text block.
     *
     * This member variable stores the height of the text block, determining how tall
     * the block will be rendered within the graphical context.
     */
    
    double mH;

    // Appearance

    /**
     * @brief Pointer to the `HISSTools_Text` object representing the text content.
     *
     * This member variable holds a pointer to a `HISSTools_Text` object, which manages
     * the text content displayed within the text block. The `HISSTools_Text` object
     * is responsible for handling the text's properties, such as font, size, and content.
     */
    
    HISSTools_Text *mTextTS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the text color specification.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines
     * the color properties of the text within the text block. The color specification includes
     * details such as the text color and any associated design elements that affect how the text is displayed.
     */
    
    HISSTools_Color_Spec *mTextCS;
    
    /**
     * @brief Pointer to the `HISSTools_Shadow` object representing the text shadow.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which defines
     * the shadow properties applied to the text within the text block. The shadow can enhance
     * the visibility and aesthetic of the text by adding depth and contrast.
     */
    
    HISSTools_Shadow *mTextSD;
    
    /**
     * @brief Specifies the horizontal alignment of the text.
     *
     * This member variable holds the horizontal alignment setting for the text block,
     * which is defined by the `HTextAlign` enum. It determines how the text is aligned
     * horizontally within the block, such as left-aligned, center-aligned, or right-aligned.
     */
    
    HTextAlign mHAlign;
    
    /**
     * @brief Specifies the vertical alignment of the text.
     *
     * This member variable holds the vertical alignment setting for the text block,
     * which is defined by the `VTextAlign` enum. It determines how the text is aligned
     * vertically within the block, such as top-aligned, middle-aligned, or bottom-aligned.
     */
    
    VTextAlign mVAlign;

    // String

    /**
     * @brief Stores the text content as a string.
     *
     * This member variable holds the text content for the text block using a `WDL_String` object.
     * It is responsible for managing the string data, which represents the actual text
     * displayed within the block.
     */
    
    WDL_String mStr;
};


// HISSTools_Text_Helper_Panel
// Helper class to deal with blocks of text with a panel

/**
 * @class HISSTools_Text_Helper_Panel
 * @brief A specialized class for managing text panels in the HISSTools graphics system.
 *
 * This class is derived from `HISSTools_Text_Helper_Block` and extends its functionality to
 * handle more complex text panels. A text panel may include additional features or behaviors
 * beyond those provided by a simple text block, such as interactive elements or specialized rendering.
 *
 * Inheritance:
 * - Publicly inherits from `HISSTools_Text_Helper_Block`, gaining access to all the text block's
 *   methods and properties while adding or overriding functionalities specific to a panel.
 */

class HISSTools_Text_Helper_Panel : public HISSTools_Text_Helper_Block
{
public:

    /**
     * @brief Constructs a `HISSTools_Text_Helper_Panel` object with specified parameters.
     *
     * This constructor initializes a text panel with the given position, dimensions, padding,
     * alignment, and other properties necessary for rendering and layout in the HISSTools
     * graphics system.
     *
     * @param x The x-coordinate of the top-left corner of the text panel.
     * @param y The y-coordinate of the top-left corner of the text panel.
     * @param w The width of the text panel.
     * @param h The height of the text panel.
     * @param wPad The horizontal padding for the text panel.
     * @param hPad The vertical padding for the text panel.
     * @param hAlign Horizontal text alignment, defined by the `HTextAlign` enum (e.g., left, center, right).
     * @param vAlign Vertical text alignment, defined by the `VTextAlign` enum (e.g., top, middle, bottom).
     * @param name A C-string representing the name of the text panel, typically used for identification.
     * @param type A C-string representing the type of text panel, which could define its usage or category.
     * @param scheme A pointer to the `HISSTools_Design_Scheme`, which holds design parameters such as colors and fonts used in the text panel.
     */
    
    HISSTools_Text_Helper_Panel(double x, double y, double w, double h, double wPad, double hPad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);

    /**
     * @brief Constructs a `HISSTools_Text_Helper_Panel` object with default dimensions and padding, but specified alignment, name, type, and design scheme.
     *
     * This constructor initializes a text panel with default values for position, dimensions, and padding
     * (x = 0, y = 0, width = 0, height = 0, horizontal padding = 0, vertical padding = 0), while allowing
     * the horizontal and vertical alignment, name, type, and design scheme to be specified.
     *
     * It delegates to the main constructor that handles all the parameters, setting defaults for those
     * that are not provided.
     *
     * @param hAlign Horizontal text alignment, defined by the `HTextAlign` enum (e.g., left, center, right).
     * @param vAlign Vertical text alignment, defined by the `VTextAlign` enum (e.g., top, middle, bottom).
     * @param name A C-string representing the name of the text panel, typically used for identification.
     * @param type A C-string representing the type of text panel, which could define its usage or category.
     * @param scheme A pointer to the `HISSTools_Design_Scheme`, which holds design parameters such as colors and fonts used in the text panel.
     *
     * @note This constructor is useful when specific dimensions or padding are not needed at initialization.
     */
    
    HISSTools_Text_Helper_Panel(HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme)
        : HISSTools_Text_Helper_Panel(0, 0, 0, 0, 0, 0, hAlign, vAlign, name, type, scheme)
    {}

    /**
     * @brief Draws the text panel, with an option to include or exclude the text.
     *
     * This method renders the panel using the provided `HISSTools_VecLib` object. It can also optionally draw
     * the text depending on the `drawText` flag. If `drawText` is set to true, the text content is rendered
     * along with the panel; if false, only the panel itself is drawn without the text.
     *
     * @param vl A reference to the `HISSTools_VecLib` object used for vector-based drawing operations.
     * @param drawText A boolean flag indicating whether the text should be drawn. If true, the text is rendered;
     *                 if false, the text is omitted from the drawing.
     */
    
    void Draw(HISSTools_VecLib& vl, bool drawText);

    /**
     * @brief Adjusts the padding of the text panel.
     *
     * This method changes the left, right, and horizontal padding for the text panel.
     * Padding affects the space between the text content and the edges of the panel,
     * allowing for custom spacing around the text.
     *
     * @param lPad The new left padding.
     * @param rPad The new right padding.
     * @param hPad The new vertical padding.
     */
    
    void ChangePadding(double lPad, double rPad, double hPad);
    
    /**
     * @brief Adjusts the horizontal and vertical padding of the text panel.
     *
     * This method changes the horizontal and vertical padding for the text panel.
     * Padding affects the space between the text content and the edges of the panel,
     * allowing for custom spacing around the text.
     *
     * @param wPad The new horizontal padding (applies equally to both left and right padding).
     * @param hPad The new vertical padding.
     */
    
    void ChangePadding(double wPad, double hPad);

    /**
     * @brief Resizes and repositions the text panel.
     *
     * This method adjusts the position and dimensions of the text panel by setting a new
     * x and y position as well as width and height. It allows for dynamic resizing and repositioning
     * of the panel within the graphical context.
     *
     * @param x The new x-coordinate for the top-left corner of the panel.
     * @param y The new y-coordinate for the top-left corner of the panel.
     * @param w The new width of the panel.
     * @param h The new height of the panel.
     */
    
    void Resize(double x, double y, double w, double h);

    /**
     * @brief Sets the color of the panel.
     *
     * This method updates the color of the text panel using the specified color specification.
     * The `HISSTools_Color_Spec` provides the necessary color information, which is applied to
     * the panel background or other graphical elements within the panel.
     *
     * @param colorSpec A pointer to a `HISSTools_Color_Spec` object that defines the color to be applied to the panel.
     */
    
    void SetPanelColor(HISSTools_Color_Spec *colorSpec);
    
    /**
     * @brief Sets the outline color of the panel.
     *
     * This method updates the color of the panel's outline using the specified color specification.
     * The `HISSTools_Color_Spec` provides the color information that is applied to the outline or border
     * of the panel, allowing for customization of its appearance.
     *
     * @param colorSpec A pointer to a `HISSTools_Color_Spec` object that defines the color to be applied to the panel's outline.
     */
    
    void SetOutlineColor(HISSTools_Color_Spec *colorSpec);

    /**
     * @brief Retrieves the bounds of the panel.
     *
     * This method returns the current bounding box of the panel, represented by a
     * `HISSTools_Bounds` object. The bounds define the panel's position and size within
     * the graphical context, including its width, height, and location.
     *
     * @return A `HISSTools_Bounds` object representing the current position and dimensions of the panel.
     */
    
    HISSTools_Bounds Bounds();

protected:

    /**
     * @brief Determines whether the panel's outline should be drawn.
     *
     * This method checks if the panel's outline is set to be drawn and returns a boolean
     * value indicating the result. It is used to conditionally enable or disable the rendering
     * of the panel's outline.
     *
     * @return `true` if the outline should be drawn, `false` otherwise.
     */
    
    bool DoDrawOutline();
    
    /**
     * @brief Determines whether the panel itself should be drawn.
     *
     * This method checks if the panel is set to be drawn and returns a boolean
     * value indicating the result. It is used to conditionally enable or disable
     * the rendering of the panel's background or content.
     *
     * @return `true` if the panel should be drawn, `false` otherwise.
     */
    
    bool DoDrawPanel();

    // Dimensions

    /**
     * @brief The x-coordinate of the panel's position.
     *
     * This member variable stores the x-coordinate of the top-left corner of the panel,
     * determining its horizontal position within the graphical context.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate of the panel's position.
     *
     * This member variable stores the y-coordinate of the top-left corner of the panel,
     * determining its vertical position within the graphical context.
     */
    
    double mY;
    
    /**
     * @brief The width of the panel.
     *
     * This member variable stores the width of the panel, determining how wide the panel will be
     * rendered within the graphical context.
     */
    
    double mW;
    
    /**
     * @brief The height of the panel.
     *
     * This member variable stores the height of the panel, determining how tall the panel will be
     * rendered within the graphical context.
     */
    
    double mH;

    /**
     * @brief The left padding of the panel.
     *
     * This member variable stores the amount of padding applied to the left side of the panel.
     * Padding defines the space between the panel's content (e.g., text) and its left edge,
     * allowing for custom spacing within the panel.
     */
    
    double mLPad;
    
    /**
     * @brief The right padding of the panel.
     *
     * This member variable stores the amount of padding applied to the right side of the panel.
     * Padding defines the space between the panel's content (e.g., text) and its right edge,
     * allowing for custom spacing within the panel.
     */
    
    double mRPad;
    
    /**
     * @brief The vertical padding of the panel.
     *
     * This member variable stores the amount of padding applied vertically to the panel.
     * It defines the space between the panel's content (e.g., text) and the top and bottom edges,
     * allowing for custom vertical spacing within the panel.
     */
    
    double mHPad;

    // Appearance

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the fill color of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the
     * fill color of the panel. The fill color is applied to the background of the panel,
     * determining the visual appearance of the panel's interior.
     */
    
    HISSTools_Color_Spec *mPanelFillCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the outline color of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the
     * color of the panel's outline. The outline color is applied to the border of the panel,
     * controlling the appearance of the panel's edges.
     */
    
    HISSTools_Color_Spec *mPanelOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Shadow` object representing the shadow effect of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object that defines the
     * shadow properties applied to the panel. The shadow effect can enhance the panel's
     * visual depth by adding a shadow behind it, improving contrast and aesthetics.
     */
    
    HISSTools_Shadow *mPanelSD;
    
    /**
     * @brief Specifies the roundness of the panel's corners.
     *
     * This member variable stores the value representing how rounded the corners of the panel are.
     * A higher value results in more rounded corners, while a lower value produces sharper, less rounded corners.
     *
     * @note The roundness is typically applied to the panel's border and affects its overall shape.
     */
    
    double mPanelRoundness;
    
    /**
     * @brief Specifies the thickness of the panel's outline.
     *
     * This member variable stores the value representing the thickness of the panel's outline or border.
     * A higher value results in a thicker outline, while a lower value produces a thinner, more subtle outline.
     *
     * @note The thickness affects the visual appearance of the panel's border and can enhance the panel's prominence.
     */
    
    double mPanelOutlineTK;
};


// HISSTools_Text_Helper_Param
// Helper class to deal with blocks of text displaying a parameter

/**
 * @class HISSTools_Text_Helper_Param
 * @brief A specialized class for managing parameter display panels in the HISSTools graphics system.
 *
 * This class is derived from `HISSTools_Text_Helper_Panel` and extends its functionality to handle
 * parameter-related panels. It provides additional features to manage, display, and interact with
 * graphical elements that represent parameter values, controls, or settings within the HISSTools system.
 *
 * Inheritance:
 * - Publicly inherits from `HISSTools_Text_Helper_Panel`, gaining access to all the text panel's
 *   methods and properties while adding or overriding functionalities specific to parameter management.
 */

class HISSTools_Text_Helper_Param : public HISSTools_Text_Helper_Panel
{
public:

    // Constructors

    /**
     * @brief Constructs a `HISSTools_Text_Helper_Param` object with the specified control, position, dimensions, padding, alignment, and design scheme.
     *
     * This constructor initializes a parameter display panel, associating it with a specific control and configuring
     * its position, size, padding, alignment, and visual properties based on the provided design scheme.
     *
     * @param control A pointer to an `iplug::igraphics::IControl` object that the parameter panel is associated with.
     *                This control is typically used to manage interactions or updates related to the parameter.
     * @param x The x-coordinate of the top-left corner of the parameter panel.
     * @param y The y-coordinate of the top-left corner of the parameter panel.
     * @param w The width of the parameter panel.
     * @param h The height of the parameter panel.
     * @param pad The padding applied within the panel to space its contents from the edges.
     * @param hAlign Horizontal text alignment, defined by the `HTextAlign` enum (e.g., left, center, right).
     * @param vAlign Vertical text alignment, defined by the `VTextAlign` enum (e.g., top, middle, bottom).
     * @param name A C-string representing the name of the parameter panel, typically used for identification.
     * @param type A C-string representing the type of parameter panel, which may indicate its specific use or purpose.
     * @param scheme A pointer to the `HISSTools_Design_Scheme`, which holds design parameters such as colors and fonts used in the parameter panel.
     */
    
    HISSTools_Text_Helper_Param(iplug::igraphics::IControl *control, double x, double y, double w, double h, double pad, HTextAlign hAlign, VTextAlign vAlign, const char *name, const char *type, HISSTools_Design_Scheme *scheme);

    /**
     * @brief Checks if the parameter is associated with a menu.
     *
     * This method determines whether the current parameter is a menu-type parameter.
     * It returns a boolean value indicating if the parameter is represented or controlled
     * through a menu interface.
     *
     * @return `true` if the parameter is a menu-type parameter, `false` otherwise.
     */
    
    bool MenuParam();

    /**
     * @brief Finalizes the editing process for the parameter.
     *
     * This method is called to signal the end of an editing session for a parameter.
     * It performs any necessary actions to finalize and apply changes made during
     * the editing process, ensuring the parameter's new value is set and any related
     * updates are processed.
     */
    
    void FinishEdit();
    
    /**
     * @brief Highlights or unhighlights the parameter panel.
     *
     * This method enables or disables the highlight effect on the parameter panel.
     * When highlighting is turned on, the panel may display visual changes (such as color or brightness)
     * to indicate that it is selected or in focus.
     *
     * @param on A boolean flag indicating whether to enable (`true`) or disable (`false`) the highlight effect.
     */
    
    void Hilite(bool on);

    /**
     * @brief Prompts the user for input related to the parameter.
     *
     * This method initiates a user input prompt, allowing the user to provide input or make changes
     * to the parameter's value. It may open a dialog, menu, or other interface elements to gather
     * user input.
     */
    
    void PromptUserInput();
    
    /**
     * @brief Prompts the user for input related to the parameter at a specified screen location.
     *
     * This method prompts the user for input, allowing them to adjust the parameter's value,
     * at a specified location on the screen. It may open a dialog, menu, or other input interface
     * elements at the given coordinates.
     *
     * @param x The x-coordinate on the screen where the input prompt should appear.
     * @param y The y-coordinate on the screen where the input prompt should appear.
     *
     * @return `true` if the input prompt was successfully initiated, `false` otherwise.
     */
    
    bool PromptUserInput(float x, float y);

    /**
     * @brief Renders the parameter panel using the specified vector graphics library.
     *
     * This method draws the parameter panel using the provided `HISSTools_VecLib` instance.
     * It handles the rendering of the panel, including its background, borders, text, and
     * other graphical elements, based on the current parameter state and design settings.
     *
     * @param vl A reference to the `HISSTools_VecLib` object used for vector-based drawing operations.
     */
        
    void Draw(HISSTools_VecLib& vl);

private:

    /**
     * @brief Updates the text displayed on the control associated with the parameter.
     *
     * This method sets or updates the text on the parameter's associated control.
     * It ensures that the displayed text reflects the current state, value, or label
     * of the parameter in the control interface.
     */
    
    void SetControlText();

    /**
     * @brief Adjusts the roundness of the panel's corners based on a given height.
     *
     * This method compensates for the roundness of the panel's corners by adjusting the corner curvature
     * based on the specified height, typically used for menu triangles or similar elements.
     * It ensures that the visual roundness matches the panel's dimensions appropriately.
     *
     * @param menuTriangleHeight The height of the menu triangle or related graphical element
     *                           used to calculate the required roundness compensation.
     *
     * @return The adjusted roundness value based on the provided height.
     */
    
    double RoundnessCompensate(double menuTriangleHeight);

    /**
     * @brief Pointer to the associated `IControl` object.
     *
     * This member variable holds a pointer to an `iplug::igraphics::IControl` object,
     * which represents the control element associated with this parameter panel.
     * The control manages interactions and updates related to the parameter, such as
     * receiving user input or displaying the parameter's value.
     */
    
    iplug::igraphics::IControl *mControl;

    /**
     * @brief The ratio used to scale the user input prompt area.
     *
     * This member variable stores the ratio that determines the scaling factor for
     * the prompt area when prompting the user for input. It adjusts the size or placement
     * of the input prompt relative to the control or panel, ensuring a suitable interaction zone.
     */
    
    double mPromptRatio;
    
    /**
     * @brief The horizontal padding for the text within the panel.
     *
     * This member variable stores the amount of horizontal padding applied to the text
     * within the panel. It defines the space between the text content and the left and right
     * edges of the panel, allowing for custom spacing and layout adjustments.
     */
    
    double mTextHPad;
    
    /**
     * @brief The x-coordinate position of the separator line within the panel.
     *
     * This member variable stores the x-coordinate for the separator line inside the panel.
     * The separator is typically used to divide or visually distinguish different sections
     * of the panel, such as between the label and the parameter value.
     */
    
    double mSeparatorX;

    /**
     * @brief Determines whether the separator line should be drawn.
     *
     * This member variable holds a boolean value that specifies whether the separator line
     * inside the panel should be rendered. If `true`, the separator is drawn to visually divide
     * sections of the panel; if `false`, the separator is not drawn.
     */
    
    bool mDrawSeparator;
    
    /**
     * @brief Determines whether the triangle indicator should be drawn.
     *
     * This member variable holds a boolean value that specifies whether a triangle indicator
     * should be rendered on the panel. The triangle is typically used to signify a menu, dropdown,
     * or another interactive element. If `true`, the triangle is drawn; if `false`, it is not rendered.
     */
    
    bool mDrawTriangle;
    
    /**
     * @brief Determines whether the triangle indicator should be flipped.
     *
     * This member variable holds a boolean value that specifies whether the triangle indicator
     * (typically used to represent a dropdown or menu) should be flipped. When `true`, the triangle is flipped,
     * which could indicate a different state, such as an open or closed menu. If `false`, the triangle remains in its default orientation.
     */
    
    bool mMenuFlipTriangle;
    
    /**
     * @brief Indicates whether the parameter is currently being edited.
     *
     * This member variable holds a boolean value that represents whether the parameter
     * is in an editing state. When `true`, the parameter is actively being edited by the user
     * (e.g., through a user input prompt or control). If `false`, the parameter is not in an editing state.
     */
    
    bool mInEdit;

    /**
     * @brief The top position of the menu triangle indicator.
     *
     * This member variable stores the vertical coordinate representing the top position
     * of the menu triangle indicator within the panel. It defines where the top of the triangle
     * is drawn, typically used to position the triangle relative to other panel elements.
     */
    
    double mMenuTriangleTop;
    
    /**
     * @brief The bottom position of the menu triangle indicator.
     *
     * This member variable stores the vertical coordinate representing the bottom position
     * of the menu triangle indicator within the panel. It defines where the bottom of the triangle
     * is drawn, typically used to position the triangle relative to other panel elements.
     */
    
    double mMenuTriangleBtm;
    
    /**
     * @brief The left position of the menu triangle indicator.
     *
     * This member variable stores the horizontal coordinate representing the left position
     * of the menu triangle indicator within the panel. It defines where the left side of the triangle
     * is drawn, typically used to position the triangle relative to other panel elements.
     */
    
    double mMenuTriangleL;
    
    /**
     * @brief The right position of the menu triangle indicator.
     *
     * This member variable stores the horizontal coordinate representing the right position
     * of the menu triangle indicator within the panel. It defines where the right side of the triangle
     * is drawn, typically used to position the triangle relative to other panel elements.
     */
    
    double mMenuTriangleR;

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the highlight color of the text.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the highlight color
     * for the text within the panel. The highlight color is applied when the text is selected or focused,
     * providing a visual indication of the highlighted state.
     */
    
    HISSTools_Color_Spec *mTextHiliteCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the highlight color of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the highlight color
     * for the panel. The highlight color is applied when the panel is selected, focused, or otherwise needs to
     * visually indicate a highlighted state.
     */
    
    HISSTools_Color_Spec *mPanelHiliteCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the highlight color of the panel's outline.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the highlight color
     * for the panel's outline. The highlight color is applied to the outline when the panel is selected, focused,
     * or needs to visually indicate a highlighted state, providing emphasis on the border of the panel.
     */
    
    HISSTools_Color_Spec *mOutlineHiliteCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the text color specification.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the color of the text
     * displayed within the panel. The color specification includes details such as the font color and any
     * associated design elements that affect the appearance of the text.
     */
    
    HISSTools_Color_Spec *mTextCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the fill color of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the fill color
     * of the panel. The fill color is applied to the background of the panel, determining the visual appearance
     * of the panel's interior.
     */
    
    HISSTools_Color_Spec *mPanelFillCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the outline color of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object that defines the color
     * of the panel's outline. The outline color is applied to the border of the panel, controlling
     * the appearance of the panel's edges.
     */
    
    HISSTools_Color_Spec *mPanelOutlineCS;

    // Show Units

    /**
     * @brief Indicates whether to display units alongside the parameter value.
     *
     * This member variable holds a boolean value that specifies whether the units
     * of the parameter should be shown next to the parameter value. If `true`, the units are displayed;
     * if `false`, the units are hidden.
     */
    
    bool mShowUnits;
};

// **** Controls **** //

// HISSTools_TextBlock
// A Text Block

/**
 * @struct HISSTools_TextBlock
 * @brief A structure for managing text blocks that integrate with the HISSTools graphics system and control layers.
 *
 * This struct combines functionalities from `HISSTools_Text_Helper_Block`, `iplug::igraphics::IControl`,
 * and `HISSTools_Control_Layers` to manage text blocks that interact with the graphics system and control layers.
 * It provides capabilities for rendering text, responding to control input, and handling layered graphics
 * for advanced interaction and customization.
 *
 * Inheritance:
 * - Inherits from `HISSTools_Text_Helper_Block` for managing text block properties and layout.
 * - Inherits from `iplug::igraphics::IControl` to enable interaction and control functionalities.
 * - Inherits from `HISSTools_Control_Layers` for layered graphical rendering and control.
 */

struct HISSTools_TextBlock : public HISSTools_Text_Helper_Block, public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    
    /**
     * @brief Constructs a `HISSTools_TextBlock` object with specified dimensions, text, alignment, and design scheme.
     *
     * This constructor initializes a text block at the specified position and size, with optional text content,
     * alignment settings, and design scheme. It allows customization of the text block's appearance and behavior
     * in the HISSTools graphics system.
     *
     * @param x The x-coordinate of the top-left corner of the text block.
     * @param y The y-coordinate of the top-left corner of the text block.
     * @param w The width of the text block.
     * @param h The height of the text block.
     * @param str A C-string representing the initial text to be displayed in the text block (default is an empty string).
     * @param hAlign Horizontal text alignment, defined by the `HTextAlign` enum (default is `kHAlignCenter` for center alignment).
     * @param vAlign Vertical text alignment, defined by the `VTextAlign` enum (default is `kVAlignCenter` for center alignment).
     * @param type A C-string representing the type of text block (default is `nullptr`).
     * @param scheme A pointer to the `HISSTools_Design_Scheme` that defines the design parameters such as colors and fonts (default is `&DefaultDesignScheme`).
     */
    
    HISSTools_TextBlock(double x, double y, double w, double h, const char* str = "", HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter, const char* type = 0, HISSTools_Design_Scheme *scheme = &DefaultDesignScheme);

    /**
     * @brief Renders the text block using the specified graphics context.
     *
     * This method overrides the base `Draw` function to render the text block using the provided `IGraphics`
     * object. It handles the drawing of text, background, and any other graphical elements, applying the
     * current design scheme and layout settings.
     *
     * @param g A reference to the `IGraphics` object used for rendering the text block.
     */
    
    void Draw(IGraphics& g) override;

    /**
     * @brief Sets or updates the text content of the text block.
     *
     * This method updates the text displayed within the text block. The provided C-string
     * replaces the current text, allowing the text block to reflect new content.
     *
     * @param str A pointer to a C-string representing the new text to be set in the text block.
     */
    
    void SetText(const char *str);
};


// HISSTools_Panel
// Panel with or without outline and with dropshadow

/**
 * @class HISSTools_Panel
 * @brief A panel control class in the HISSTools graphics system.
 *
 * This class represents a graphical panel within the HISSTools system. It combines
 * the functionalities of `iplug::igraphics::IControl` for user interaction and control
 * with `HISSTools_Control_Layers` for managing and rendering layered graphical elements.
 *
 * The panel can be used to display background graphics, interactive elements, or other
 * content, with support for multiple layers to organize visual components.
 *
 * Inheritance:
 * - Inherits from `iplug::igraphics::IControl` to handle user interaction and input.
 * - Inherits from `HISSTools_Control_Layers` to manage and render multiple layers of graphical elements.
 */

class HISSTools_Panel : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    
    /**
     * @brief Retrieves the roundness value for a panel based on its name, type, and design scheme.
     *
     * This method calculates and returns the roundness (corner curvature) of a panel
     * based on the specified name, type, and the provided design scheme. The roundness
     * value determines how rounded the corners of the panel will appear.
     *
     * @param name A C-string representing the name of the panel, which may influence the roundness.
     * @param type A C-string representing the type of panel, which may determine specific design characteristics.
     * @param scheme A pointer to the `HISSTools_Design_Scheme` object that defines the design parameters, including roundness settings.
     *
     * @return A double value representing the calculated roundness of the panel.
     */
    
    double GetRoundness(const char *name, const char *type, HISSTools_Design_Scheme *scheme);

public:

    // Constructor

    /**
     * @brief Constructs a `HISSTools_Panel` object with specified position, dimensions, type, and design scheme.
     *
     * This constructor initializes a panel at the given position and size, with an optional type and design scheme.
     * The panel can be customized to fit different graphical and functional purposes within the HISSTools system.
     *
     * @param x The x-coordinate of the top-left corner of the panel.
     * @param y The y-coordinate of the top-left corner of the panel.
     * @param w The width of the panel (default is 0).
     * @param h The height of the panel (default is 0).
     * @param type A C-string representing the type of the panel, which can influence its design or behavior (default is `nullptr`).
     * @param scheme A pointer to the `HISSTools_Design_Scheme` that defines the design parameters such as colors, fonts, and roundness (default is `&DefaultDesignScheme`).
     */
    
    HISSTools_Panel(double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *scheme = &DefaultDesignScheme);

public:

    // Draw

    /**
     * @brief Renders the panel using the specified graphics context.
     *
     * This method overrides the base `Draw` function to render the panel using the provided `IGraphics` object.
     * It handles drawing the panel's background, borders, and any other graphical elements, applying the current
     * design scheme and properties such as roundness and color.
     *
     * @param g A reference to the `IGraphics` object used to render the panel.
     */
    
    void Draw(IGraphics& g) override;

private:

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate of the panel's position.
     *
     * This member variable stores the x-coordinate of the top-left corner of the panel,
     * determining its horizontal position within the graphical context.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate of the panel's position.
     *
     * This member variable stores the y-coordinate of the top-left corner of the panel,
     * determining its vertical position within the graphical context.
     */
    
    double mY;
    
    /**
     * @brief The width of the panel.
     *
     * This member variable stores the width of the panel, determining how wide the panel will
     * be rendered within the graphical context.
     */
    
    double mW;
    
    /**
     * @brief The height of the panel.
     *
     * This member variable stores the height of the panel, determining how tall the panel will
     * be rendered within the graphical context.
     */
    
    double mH;

    /**
     * @brief The roundness of the top-left corner of the panel.
     *
     * This member variable stores the roundness value for the top-left corner of the panel.
     * It determines how rounded the corner will appear, with higher values resulting in more curved corners.
     */
    
    double mRoundnessTL;
    
    /**
     * @brief The roundness of the top-right corner of the panel.
     *
     * This member variable stores the roundness value for the top-right corner of the panel.
     * It determines how rounded the corner will appear, with higher values resulting in more curved corners.
     */
    
    double mRoundnessTR;
    
    /**
     * @brief The roundness of the bottom-left corner of the panel.
     *
     * This member variable stores the roundness value for the bottom-left corner of the panel.
     * It determines how rounded the corner will appear, with higher values resulting in more curved corners.
     */
    
    double mRoundnessBL;
    
    /**
     * @brief The roundness of the bottom-right corner of the panel.
     *
     * This member variable stores the roundness value for the bottom-right corner of the panel.
     * It determines how rounded the corner will appear, with higher values resulting in more curved corners.
     */
    
    double mRoundnessBR;

    // Line Thicknesses

    /**
     * @brief The thickness of the panel's outline.
     *
     * This member variable stores the thickness of the outline (border) of the panel.
     * It controls how thick the border around the panel will be rendered, with higher values
     * resulting in a thicker outline.
     */
    
    double mOutlineTK;

    // Shadow Spec

    /**
     * @brief Pointer to the `HISSTools_Shadow` object representing the shadow effect of the panel.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which defines the shadow properties
     * applied to the panel. The shadow enhances the panel's appearance by adding depth and contrast, giving the panel
     * a more three-dimensional look.
     */
    
    HISSTools_Shadow *mShadow;

    // Color Specs

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the panel's color specification.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color of the panel.
     * It controls the appearance of the panel's background or main fill color, contributing to the overall design
     * and look of the panel.
     */
    
    HISSTools_Color_Spec *mPanelCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the color of the panel's outline.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color of the panel's outline.
     * The outline color controls the appearance of the border surrounding the panel, contributing to its overall design and emphasis.
     */
    
    HISSTools_Color_Spec *mOutlineCS;

    /**
     * @brief Indicates whether the panel's outline should be drawn.
     *
     * This member variable holds a boolean value that specifies whether the panel's outline should be rendered.
     * If `true`, the outline is drawn; if `false`, the outline is not displayed, leaving the panel without a border.
     */
    
    bool mDrawOutline;
};

// HISSTools_Button
// On/Off button with text on or off the handle
// FIX - Momentary action and extensibility!!

/**
 * @class HISSTools_Button
 * @brief A button control class in the HISSTools graphics system.
 *
 * This class represents a graphical button within the HISSTools system, providing both interactive
 * and graphical functionality. It inherits from `iplug::igraphics::IControl` to handle user input
 * and interaction, and from `HISSTools_Control_Layers` to manage layered graphical elements.
 *
 * The `HISSTools_Button` class allows for customization of button appearance, behavior, and interaction,
 * making it suitable for various control scenarios within the graphical interface.
 *
 * Inheritance:
 * - Inherits from `iplug::igraphics::IControl` for user interaction and control functionality.
 * - Inherits from `HISSTools_Control_Layers` to manage and render layered graphics for advanced visual effects.
 */

class HISSTools_Button : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor

    /**
     * @brief Constructs a `HISSTools_Button` object with specified parameters for position, size, type, and design scheme.
     *
     * This constructor initializes a button control within the HISSTools system, assigning it a specific parameter index,
     * position, size, type, and design scheme. It allows customization of the button's appearance and behavior.
     *
     * @param paramIdx The parameter index associated with this button, typically used for parameter control and interaction.
     * @param x The x-coordinate of the top-left corner of the button.
     * @param y The y-coordinate of the top-left corner of the button.
     * @param w The width of the button (default is 0).
     * @param h The height of the button (default is 0).
     * @param type A C-string representing the type of the button, which can influence its design or behavior (default is `nullptr`).
     * @param scheme A pointer to the `HISSTools_Design_Scheme` that defines the button's design parameters such as colors, fonts, and appearance (default is `&DefaultDesignScheme`).
     * @param name A C-string representing the name of the button, typically used for identification (default is an empty string).
     */
    
    HISSTools_Button(int paramIdx, double x, double y, double w = 0, double h = 0, const char *type = 0, HISSTools_Design_Scheme *scheme = &DefaultDesignScheme, const char *name = "");

public:

    /**
     * @brief Initializes the button control after it is created.
     *
     * This method overrides the base `OnInit` function and is called when the button is initialized.
     * It performs any setup tasks or resource allocations necessary for the button to function correctly
     * within the HISSTools system, such as setting default states or loading graphical resources.
     */
    
    void OnInit() override;

    // Mousing Functions

    /**
     * @brief Handles the mouse down event for the button.
     *
     * This method overrides the base `OnMouseDown` function and is called when the user presses the mouse button
     * while hovering over the button. It processes the mouse down event, capturing the x and y coordinates of
     * the click, as well as any modifier keys or mouse buttons that were pressed.
     *
     * @param x The x-coordinate where the mouse button was pressed.
     * @param y The y-coordinate where the mouse button was pressed.
     * @param pMod A reference to an `IMouseMod` object that contains information about the mouse modifiers,
     *             such as whether control, shift, or other keys were held down during the click.
     */
    
    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;

    // Draw

    /**
     * @brief Renders the button using the specified graphics context.
     *
     * This method overrides the base `Draw` function to render the button using the provided `IGraphics` object.
     * It handles the drawing of the button's background, outline, text, and any other graphical elements,
     * applying the current design scheme and visual properties to create a visually appealing button.
     *
     * @param g A reference to the `IGraphics` object used to render the button.
     */
    
    void Draw(IGraphics& g) override;

protected:

    // Text

    /**
     * @brief Stores the display name of the button.
     *
     * This member variable holds a `WDL_String` object representing the display name of the button.
     * The display name is typically used for identifying or labeling the button visually within the user interface.
     * It can be rendered on the button or used for internal reference.
     */
    
    WDL_String mDisplayName;

private:

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate of the button's position.
     *
     * This member variable stores the x-coordinate of the top-left corner of the button,
     * determining its horizontal position within the graphical interface.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate of the button's position.
     *
     * This member variable stores the y-coordinate of the top-left corner of the button,
     * determining its vertical position within the graphical interface.
     */
    
    double mY;
    
    /**
     * @brief The width of the panel.
     *
     * This member variable stores the width of the panel, determining how wide the panel will
     * be rendered within the graphical context.
     */
    
    double mW;
    
    /**
     * @brief The height of the panel.
     *
     * This member variable stores the height of the panel, determining how tall the panel will
     * be rendered within the graphical context.
     */
    
    double mH;
    
    /**
     * @brief The padding applied around the button's text.
     *
     * This member variable stores the amount of padding applied between the text and the edges of the button.
     * It defines the space surrounding the text, helping to adjust the layout and ensure proper spacing
     * within the button's graphical context.
     */
    
    double mTextPad;
    
    /**
     * @brief The roundness of the button's corners.
     *
     * This member variable stores the value that defines the roundness of the button's corners.
     * A higher value results in more curved corners, while a lower value produces sharper corners,
     * affecting the overall shape and appearance of the button.
     */
    
    double mRoundness;

    // Line Thicknesses

    /**
     * @brief The thickness of the button's outline.
     *
     * This member variable stores the thickness of the button's outline or border.
     * A higher value results in a thicker outline, while a lower value produces a thinner outline,
     * affecting the visual prominence of the button's border.
     */
    
    double mOutlineTK;

    // Shadow Spec

    /**
     * @brief Pointer to the `HISSTools_Shadow` object representing the shadow effect of the button.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which defines the shadow properties
     * applied to the button. The shadow enhances the visual appearance of the button by adding depth and contrast,
     * giving the button a three-dimensional look.
     */
    
    HISSTools_Shadow *mShadow;

    // Text Spec

    /**
     * @brief Pointer to the `HISSTools_Text` object representing the text style of the button.
     *
     * This member variable holds a pointer to a `HISSTools_Text` object, which defines the style and formatting
     * of the text displayed on the button. It manages text properties such as font, size, alignment, and other
     * visual attributes that affect how the button's label is rendered.
     */
    
    HISSTools_Text *mTextStyle;

    // Color Specs

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the color when the button is in the "on" state.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * that the button should display when it is in the "on" or active state. This color is typically
     * used to visually indicate that the button is pressed or toggled on.
     */
    
    HISSTools_Color_Spec *mOnCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the color when the button is in the "off" state.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * that the button should display when it is in the "off" or inactive state. This color is typically
     * used to visually indicate that the button is unpressed or toggled off.
     */
    
    HISSTools_Color_Spec *mOffCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the outline color of the button.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * of the button's outline or border. The outline color is used to visually define the edges of the
     * button, enhancing its appearance and making it stand out.
     */
    
    HISSTools_Color_Spec *mOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the color of the handle label.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * of the label associated with the button's handle or any related interactive element. The handle label
     * color is typically used to enhance visibility and differentiate the label from other button elements.
     */
    
    HISSTools_Color_Spec *mHandleLabelCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the color of the handle label when the button is in the "off" state.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * of the handle label when the button is in the "off" or inactive state. This color is typically
     * used to visually indicate that the button is not pressed or toggled off, providing a contrast
     * to the active state label color.
     */
    
    HISSTools_Color_Spec *mHandleLabelOffCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the background label color.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * of the background label associated with the button or control. The background label color helps
     * distinguish the label from the button's other visual elements, ensuring proper visibility and design consistency.
     */
    
    HISSTools_Color_Spec *mBackgroundLabelCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object representing the color of the inactive overlay.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which defines the color
     * of the overlay applied when the button or control is in an inactive state. The inactive overlay
     * visually indicates that the button is disabled or not currently interactive, typically by dimming
     * or changing its appearance to reflect its inactive status.
     */
    
    HISSTools_Color_Spec *mInactiveOverlayCS;

    // Label Mode

    /**
     * @brief Indicates whether the button is in label mode.
     *
     * This member variable holds a boolean value that specifies whether the button is operating in label mode.
     * In label mode, the button may prioritize the display of text or labels over other visual elements,
     * such as icons or graphics. If `true`, the button is in label mode; if `false`, it operates in its default mode.
     */
    
    bool mLabelMode;
};

// HISSTools_Tabs
// An abstract class to control tabs in plug-ins

/**
 * @class HISSTools_Tabs
 * @brief A class for managing tabbed interfaces within the HISSTools graphics system.
 *
 * This class provides functionality for creating and managing tabs in a graphical interface.
 * It is designed to allow switching between different views or sections by interacting with tab controls.
 *
 * Inherits from `HISSTools_Graphics_Types` to access various graphical types and utilities necessary for
 * rendering and managing the tabs' graphical elements.
 *
 * Inheritance:
 * - Protected and virtual inheritance from `HISSTools_Graphics_Types` to support multiple inheritance
 *   while managing graphical resources and tab control rendering.
 */

class HISSTools_Tabs : protected virtual HISSTools_Graphics_Types
{
public:

    // Constructor

    // You should pass the inheriting class here, after constructing the control, which must be mapped to a valid parameter of the plug - the tabs are tied to the parameter, rather than the control

    /**
     * @brief Constructs a `HISSTools_Tabs` object with the specified tab control.
     *
     * This constructor initializes a `HISSTools_Tabs` object by associating it with a specific `IControl` object
     * that manages the tabs. The `IControl` object is responsible for handling user interaction and rendering
     * related to the tabs. The constructor also initializes the parameter pointer (`mParam`) to `nullptr`.
     *
     * @param tabControl A pointer to an `iplug::igraphics::IControl` object that manages the tab control interactions and rendering.
     */
    
    HISSTools_Tabs(iplug::igraphics::IControl *tabControl) : mTabControl(tabControl), mParam(nullptr) {}

    // Call this from OnInit in the inheriting class
    
    /**
     * @brief Initializes the tabs and prepares them for use.
     *
     * This method sets up the necessary elements and resources required for the tabs to function.
     * It is typically called after the `HISSTools_Tabs` object is constructed and is used to initialize
     * the tabs' layout, appearance, and interaction logic.
     */
    
    void Init();

    /**
     * @brief Attaches a control to a specific tab.
     *
     * This method associates a given control with a specific tab, allowing the control to be displayed and
     * interacted with when the corresponding tab is active. Each tab can have one or more controls attached to it.
     *
     * @param control A pointer to an `iplug::igraphics::IControl` object representing the control to be attached to the tab.
     * @param tabNumber The index of the tab to which the control should be attached. Tabs are typically numbered starting from 0.
     */
    
    void AttachControl(iplug::igraphics::IControl *control, int tabNumber);
    
    /**
     * @brief Hides or shows the active tab.
     *
     * This method controls the visibility of the currently active tab. When `hide` is set to `true`, the active tab is hidden.
     * When `hide` is set to `false`, the active tab is shown. It allows for dynamic control over whether the tab and its
     * associated content are visible or hidden.
     *
     * @param hide A boolean value that determines whether the active tab should be hidden (`true`) or shown (`false`).
     */
    
    void TabHide(bool hide);
    
    /**
     * @brief Marks the active tab as dirty and optionally pushes parameter changes to the plugin.
     *
     * This method marks the active tab as "dirty," meaning it requires a visual update or redraw.
     * If `pushParamToPlug` is set to `true`, the method also pushes any parameter changes made in the tab
     * to the plugin, ensuring that the plugin reflects the latest updates.
     *
     * @param pushParamToPlug A boolean value that indicates whether to push parameter changes to the plugin
     *                        (`true` to push changes, `false` to only mark the tab as dirty without pushing changes).
     */

    void TabSetDirty(bool pushPararmToPlug);

    //void setTabFromPlug(int tabNumber);

    // These functions should be declared in any inheriting classes, and should call the related tab versions

    /**
     * @brief Pure virtual function to hide or show the control.
     *
     * This pure virtual method must be implemented by derived classes. It controls the visibility of the control,
     * hiding it when `hide` is set to `true` and showing it when `hide` is set to `false`. This function is typically
     * used to manage the visibility of graphical components in the user interface.
     *
     * @param hide A boolean value that determines whether the control should be hidden (`true`) or shown (`false`).
     */
    
    virtual void Hide(bool hide) = 0;
    
    /**
     * @brief Pure virtual function to mark the control as dirty and optionally push parameter changes to the plugin.
     *
     * This pure virtual method must be implemented by derived classes. It marks the control as "dirty,"
     * meaning that it requires an update or redraw. Additionally, if `pushParamToPlug` is set to `true`,
     * the method also pushes any parameter changes made in the control to the plugin.
     *
     * @param pushParamToPlug A boolean value that determines whether to push parameter changes to the plugin
     *                        (`true` to push changes, `false` to only mark the control as dirty).
     * @param An unnamed integer parameter, which may be used for additional functionality by derived classes.
     */
    
    virtual void SetDirty(bool pushParamToPlug, int) = 0;

private:

    /**
     * @struct TabItem
     * @brief A structure representing an individual tab item.
     *
     * This structure holds the data and properties associated with a single tab in a tabbed interface.
     * It can store details such as the tab's label, content, or state, and is used to manage and display
     * multiple tabs within a user interface.
     */
    
    struct TabItem
    {
        
        /**
         * @brief Pointer to the associated `IControl` object.
         *
         * This member variable holds a pointer to an `iplug::igraphics::IControl` object,
         * which represents the control associated with a tab or graphical element. The control
         * is responsible for managing interactions, rendering, and responding to user inputs.
         */
        
        iplug::igraphics::IControl *mControl;
        
        /**
         * @brief The tab number associated with this item.
         *
         * This member variable stores the index or number of the tab within the tabbed interface.
         * It identifies which tab this item corresponds to and is used to manage the selection, display,
         * or interaction of the tab.
         */
        
        int mTabNumber;

        /**
         * @brief Constructs a `TabItem` with the specified control and tab number.
         *
         * This constructor initializes a `TabItem` object by associating it with a specific `IControl` object
         * and a tab number. The control manages the interaction and rendering for the tab, while the tab number
         * identifies the position of the tab within the interface.
         *
         * @param control A pointer to an `iplug::igraphics::IControl` object that handles interaction and rendering for the tab.
         * @param tabNumber An integer representing the index or number of the tab within the tabbed interface.
         */
        
        TabItem(iplug::igraphics::IControl *control, int tabNumber) : mControl(control), mTabNumber(tabNumber) {}
    };

    /**
     * @brief Pointer to the associated `IParam` object representing a parameter.
     *
     * This member variable holds a constant pointer to an `IParam` object, which represents a parameter
     * associated with the tab or control. The parameter typically stores information such as the current value,
     * range, and type of the parameter that the control is managing or displaying.
     */
    
    const IParam* mParam;
    
    /**
     * @brief Pointer to the tab control associated with the tabbed interface.
     *
     * This member variable holds a pointer to an `iplug::igraphics::IControl` object that manages the tab control.
     * The tab control is responsible for handling user interactions, such as switching between tabs, and for rendering
     * the tabbed interface. It serves as the main interface element that facilitates tab management.
     */
    
    iplug::igraphics::IControl *mTabControl;
    
    /**
     * @brief A vector containing the list of tab items.
     *
     * This member variable holds a `std::vector` of `TabItem` objects, representing all the tabs
     * in the tabbed interface. Each `TabItem` in the vector corresponds to a specific tab, storing
     * information about the tab's control and tab number. This vector allows dynamic management of
     * the tabs, such as adding, removing, or accessing individual tabs.
     */
    
    std::vector<TabItem> mItems;

    /**
     * @brief The index of the currently active tab.
     *
     * This member variable stores the tab number (or index) of the currently active or selected tab
     * within the tabbed interface. It is used to track which tab is currently displayed and interacted with,
     * allowing the system to manage tab switching and content updates.
     */
    
    int mCurrentTabNumber;
    
    /**
     * @brief The maximum number of tabs available in the tabbed interface.
     *
     * This member variable stores the highest index or total number of tabs that can be created or displayed
     * within the tabbed interface. It is used to manage the upper limit for tab creation, ensuring that
     * the interface does not exceed the specified number of tabs.
     */
    
    int mMaxTabNumber;

    /**
     * @brief Updates the list of tab items.
     *
     * This method refreshes or updates the `mItems` vector, which contains the list of `TabItem` objects.
     * It may be called to add, remove, or modify the tabs in the interface, ensuring that the list of tab
     * items reflects the current state of the tabbed interface.
     *
     * @note This method is typically invoked when tabs are dynamically added or removed, or when changes are made to the tab controls.
     */
    
    void UpdateItems();

    /**
     * @brief Ensures the provided tab number is within valid bounds.
     *
     * This method checks if the given `tabNumber` is within the valid range of available tabs.
     * If the `tabNumber` is below the minimum allowed value (typically 0) or exceeds the maximum
     * number of tabs (`mMaxTabNumber`), it adjusts the `tabNumber` to fit within the valid range.
     *
     * @param tabNumber The tab number to be validated and potentially adjusted.
     * @return The valid tab number, clipped to the allowed range of tabs.
     */
    
    int ClipTabNumber(int tabNumber);
};

// HISSTools_Invisible_Tabs
// This class allows Live-style tabs that have no explicit selector, and instead are set from the plug via other related controls

/**
 * @class HISSTools_Invisible_Tabs
 * @brief A class for managing invisible tab controls in the HISSTools graphics system.
 *
 * This class represents a tab control system where the tabs themselves are not visible, but their functionality
 * (such as switching between different views or sections) is still available. It inherits from both
 * `iplug::igraphics::IControl` for handling user interaction and `HISSTools_Tabs` for managing the tab logic.
 *
 * The class allows for advanced control over multiple views or sections without rendering a visible tab interface.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IControl` to manage user interaction and control logic.
 * - Publicly inherits from `HISSTools_Tabs` to handle the tab switching logic and management of tab content.
 */

class HISSTools_Invisible_Tabs : public iplug::igraphics::IControl, public HISSTools_Tabs
{
public:

    /**
     * @brief Constructs a `HISSTools_Invisible_Tabs` object with a specified parameter index.
     *
     * This constructor initializes the `HISSTools_Invisible_Tabs` object by linking it to a parameter index.
     * The `IControl` base class is initialized with an empty rectangle (`IRECT()`) and the given `paramIdx`.
     * The `HISSTools_Tabs` base class is initialized by passing a pointer to the current instance (`this`),
     * associating the tab logic with the invisible tab control.
     *
     * @param paramIdx The index of the parameter associated with the invisible tabs. This is used to manage control interactions.
     */
    
    HISSTools_Invisible_Tabs(int paramIdx) : IControl(IRECT(), paramIdx), HISSTools_Tabs(this) {}
    
    /**
     * @brief Initializes the `HISSTools_Invisible_Tabs` control.
     *
     * This method overrides the base `OnInit` function and is called when the control is first initialized.
     * It invokes the `Init()` function to set up the necessary components and logic for the invisible tabs,
     * ensuring that the control is ready for use.
     */
    
    void OnInit() override { Init(); }
    
    /**
     * @brief Renders the invisible tabs control.
     *
     * This method overrides the base `Draw` function but does not perform any drawing,
     * as the `HISSTools_Invisible_Tabs` class represents a tab control without a visible interface.
     * The function is intentionally left empty, as the tabs are meant to be managed and interacted with
     * without rendering any graphical elements.
     *
     * @param g A reference to the `IGraphics` object used for rendering. Since this control is invisible, no drawing is done.
     */
    
    void Draw(IGraphics& g) override {}
    
    /**
     * @brief Hides or shows the invisible tabs control.
     *
     * This method overrides the base `Hide` function and calls the `TabHide` method to control the visibility of the tabs.
     * When `hide` is set to `true`, the active tab and its contents are hidden; when `hide` is set to `false`,
     * the active tab is shown. Although the tabs themselves are invisible, this controls the visibility of their content.
     *
     * @param hide A boolean value that determines whether the tabs' content should be hidden (`true`) or shown (`false`).
     */
    
    void Hide(bool hide) override { TabHide(hide); }
    
    /**
     * @brief Marks the invisible tabs control as dirty and optionally pushes parameter changes to the plugin.
     *
     * This method overrides the base `SetDirty` function and calls the `TabSetDirty` method to mark the control
     * as "dirty," meaning it requires an update or redraw. If `pushParamToPlug` is set to `true`, the method also
     * pushes parameter changes to the plugin, ensuring the plugin is updated with any changes made in the tab control.
     *
     * @param pushParamToPlug A boolean value that determines whether to push parameter changes to the plugin (`true`)
     *                        or simply mark the control as dirty without pushing changes (`false`).
     * @param An unnamed integer parameter, which may be used for additional functionality (ignored in this implementation).
     */
    
    void SetDirty(bool pushParamToPlug, int) override { TabSetDirty(pushParamToPlug); }
};

// HISSTools_Value
// Multipurpose numerical / menu control
// FIX - do your own mousing later...

/**
 * @class HISSTools_Value
 * @brief A control class for displaying and adjusting values in the HISSTools graphics system.
 *
 * This class represents a value control, which allows users to view and adjust numerical values
 * using a knob interface. It inherits from `iplug::igraphics::IKnobControlBase` for handling
 * knob-style input and from `HISSTools_Control_Layers` for managing layered graphical elements.
 *
 * The `HISSTools_Value` class provides the functionality to visually represent a value and allows
 * users to adjust it through interaction with the knob control.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IKnobControlBase` to manage knob-based interaction.
 * - Publicly inherits from `HISSTools_Control_Layers` to handle graphical layering for visual elements.
 */

class HISSTools_Value : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    /**
     * @brief Constructs a `HISSTools_Value` object with specified parameters for position, dimensions, and design scheme.
     *
     * This constructor initializes a `HISSTools_Value` object, which represents a value control in the HISSTools system.
     * The control is positioned at the specified coordinates with a given width and height. It also allows for an optional
     * type, design scheme, and name for further customization of the control's appearance and behavior.
     *
     * @param paramIdx The index of the parameter associated with this value control, used for parameter handling.
     * @param x The x-coordinate of the top-left corner of the value control.
     * @param y The y-coordinate of the top-left corner of the value control.
     * @param w The width of the value control.
     * @param h The height of the value control.
     * @param type A C-string representing the type of value control, which may influence its behavior or appearance (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object, which defines the control's design elements, such as colors and fonts (default is `&DefaultDesignScheme`).
     * @param name A C-string representing the name of the value control (default is `nullptr`).
     */
    
    HISSTools_Value(int paramIdx, double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr);
    
    /**
     * @brief Destructor for the `HISSTools_Value` class.
     *
     * The destructor cleans up any resources or memory associated with the `HISSTools_Value` control.
     */
    
    ~HISSTools_Value();

    /**
     * @brief Initializes the `HISSTools_Value` control after it is created.
     *
     * This method overrides the base `OnInit` function and is called when the `HISSTools_Value` control is initialized.
     * It performs any necessary setup, such as initializing the graphical elements, setting default values, or
     * configuring the control based on the provided parameters or design scheme.
     */
    
    void OnInit() override;

    // Mousing Functions

    /**
     * @brief Handles the mouse down event for the `HISSTools_Value` control.
     *
     * This method overrides the base `OnMouseDown` function and is called when the user presses the mouse button
     * while interacting with the `HISSTools_Value` control. It processes the mouse down event, capturing the x and y
     * coordinates of the click and any modifier keys or mouse buttons that were pressed.
     *
     * @param x The x-coordinate where the mouse button was pressed.
     * @param y The y-coordinate where the mouse button was pressed.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons.
     */
    
    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse up event for the `HISSTools_Value` control.
     *
     * This method overrides the base `OnMouseUp` function and is called when the user releases the mouse button
     * while interacting with the `HISSTools_Value` control. It processes the mouse up event, capturing the x and y
     * coordinates where the mouse was released and any modifier keys or mouse buttons that were active during the event.
     *
     * @param x The x-coordinate where the mouse button was released.
     * @param y The y-coordinate where the mouse button was released.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons at the time of release.
     */
    
    void OnMouseUp(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse drag event for the `HISSTools_Value` control.
     *
     * This method overrides the base `OnMouseDrag` function and is called when the user drags the mouse
     * while interacting with the `HISSTools_Value` control. It processes the mouse drag event by capturing
     * the new x and y coordinates as well as the change in position (dX, dY) during the drag, along with
     * any modifier keys or mouse buttons that were active.
     *
     * @param x The current x-coordinate of the mouse during the drag.
     * @param y The current y-coordinate of the mouse during the drag.
     * @param dX The change in the x-coordinate (delta) during the drag.
     * @param dY The change in the y-coordinate (delta) during the drag.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the drag.
     */
    
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the double-click event for the `HISSTools_Value` control.
     *
     * This method overrides the base `OnMouseDblClick` function and is called when the user performs a double-click
     * while interacting with the `HISSTools_Value` control. It processes the double-click event by capturing the x and y
     * coordinates where the click occurred and any modifier keys or mouse buttons that were active during the event.
     *
     * @param x The x-coordinate where the double-click occurred.
     * @param y The y-coordinate where the double-click occurred.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the double-click.
     */
    
    void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Sets the value of the control based on user input.
     *
     * This method overrides the base `SetValueFromUserInput` function and is called when the user provides
     * input to update the control's value. It processes the new value and sets the control's parameter accordingly.
     * The method also handles which specific value to update if the control manages multiple values.
     *
     * @param value The new value provided by the user input.
     * @param valIdx The index of the value to be updated, used when the control manages multiple values (default is 0 for single-value controls).
     */
    
    virtual void SetValueFromUserInput(double value, int valIdx) override;

    // Draw

    /**
     * @brief Renders the `HISSTools_Value` control using the specified graphics context.
     *
     * This method overrides the base `Draw` function and is responsible for rendering the visual representation
     * of the `HISSTools_Value` control. It draws the control's elements, such as the knob, value, labels, and any
     * other graphical features, using the provided `IGraphics` object. The method applies the current design scheme
     * and control settings to create a visually appealing and interactive control.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g) override;

private:

    // Text

    /**
     * @brief Pointer to a `HISSTools_Text_Helper_Param` object representing the text helper for the parameter.
     *
     * This member variable holds a pointer to a `HISSTools_Text_Helper_Param` object, which assists in managing
     * and displaying the text related to the parameter. It is responsible for handling the formatting, rendering,
     * and interaction of parameter-related text within the control, ensuring proper display and user interaction.
     */
    
    HISSTools_Text_Helper_Param *mTextParam;
    
    /**
     * @brief Pointer to a `HISSTools_Text_Helper_Block` object representing the label for the control.
     *
     * This member variable holds a pointer to a `HISSTools_Text_Helper_Block` object, which is used to manage
     * and display the label text associated with the control. It handles the formatting, positioning, and rendering
     * of the label, ensuring that the label is displayed correctly within the graphical interface.
     */
    
    HISSTools_Text_Helper_Block *mTextLabel;

    // Name

    /**
     * @brief Stores the display name of the control or parameter.
     *
     * This member variable holds a `WDL_String` object that represents the display name associated with the control
     * or parameter. The display name is typically used to label or identify the control within the user interface,
     * ensuring the user can easily recognize its purpose or function.
     */
    
    WDL_String mDisplayName;

    // Mousing

    /**
     * @brief Indicates whether the control is currently being dragged by the user.
     *
     * This member variable holds a boolean value that specifies whether the control is in a drag state.
     * When `true`, it means the user is actively dragging the control (e.g., adjusting a knob or slider).
     * When `false`, the control is not being dragged. This variable is used to manage drag-related behavior and interaction.
     */
    
    bool mDrag;

    // Stored Dimensions

    /**
     * @brief Represents the size of the area allocated for displaying text.
     *
     * This member variable stores the size (usually height or width) of the area reserved for rendering text
     * within the control. It determines how much space is available for the label, value, or other text-related
     * content, ensuring that the text fits properly within the graphical interface.
     */
    
    double mTextArea;
};

// HISSTools_Dial
// Multipurpose dial with auto value display
// FIX - do your own mousing later...

/**
 * @class HISSTools_Dial
 * @brief A control class for implementing a dial (rotary knob) in the HISSTools graphics system.
 *
 * This class represents a dial control, allowing users to adjust values through rotary input.
 * It inherits from `iplug::igraphics::IKnobControlBase` for handling knob-based interaction,
 * and from `HISSTools_Control_Layers` for managing layered graphical elements. The `HISSTools_Dial`
 * class provides the functionality to display and interact with a dial, allowing for value adjustments
 * in a graphical interface.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IKnobControlBase` to manage rotary (knob) input and interaction.
 * - Publicly inherits from `HISSTools_Control_Layers` to handle layered rendering and graphical effects.
 */

class HISSTools_Dial : public iplug::igraphics::IKnobControlBase, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    /**
     * @brief Constructs a `HISSTools_Dial` object with the specified parameters for position, type, and design scheme.
     *
     * This constructor initializes a `HISSTools_Dial` object, representing a dial control for value adjustment.
     * The control is placed at the specified x and y coordinates, and can be further customized with an optional
     * type, design scheme, and name. The constructor also associates the dial with a specific parameter index (`paramIdx`)
     * to handle value changes.
     *
     * @param paramIdx The index of the parameter associated with this dial, used to manage value changes.
     * @param x The x-coordinate of the top-left corner of the dial control.
     * @param y The y-coordinate of the top-left corner of the dial control.
     * @param type A C-string representing the type of dial, which may influence its behavior or appearance (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object that defines the control's design elements, such as colors and fonts (default is `&DefaultDesignScheme`).
     * @param name A C-string representing the name of the dial control (default is `nullptr`).
     */
    
    HISSTools_Dial(int paramIdx, double x, double y, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char* name = nullptr);
    
    /**
     * @brief Destructor for the `HISSTools_Dial` class.
     *
     * The destructor cleans up any resources or memory used by the `HISSTools_Dial` object.
     * It is called when the object is destroyed to ensure that all associated resources,
     * such as graphical elements or memory allocations, are properly released.
     */
    
    ~HISSTools_Dial();

    /**
     * @brief Initializes the `HISSTools_Dial` control after it is created.
     *
     * This method overrides the base `OnInit` function and is called when the `HISSTools_Dial` control
     * is first initialized. It performs any necessary setup, such as initializing the dial's graphical
     * elements, setting default values, or configuring the control based on the provided parameters
     * and design scheme.
     */
    
    void OnInit() override;

    // Mousing Functions

    /**
     * @brief Handles the mouse down event for the `HISSTools_Dial` control.
     *
     * This method overrides the base `OnMouseDown` function and is called when the user presses the mouse button
     * while interacting with the `HISSTools_Dial` control. It processes the mouse down event by capturing the x and y
     * coordinates of the click and any modifier keys or mouse buttons that were pressed at the time of the event.
     *
     * @param x The x-coordinate where the mouse button was pressed.
     * @param y The y-coordinate where the mouse button was pressed.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the click.
     */
    
    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the double-click event for the `HISSTools_Dial` control.
     *
     * This method overrides the base `OnMouseDblClick` function and is called when the user performs a double-click
     * while interacting with the `HISSTools_Dial` control. It processes the double-click event by capturing the x and y
     * coordinates where the click occurred and any modifier keys or mouse buttons that were active during the event.
     *
     * @param x The x-coordinate where the double-click occurred.
     * @param y The y-coordinate where the double-click occurred.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the double-click.
     */
    
    void OnMouseDblClick(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse over event for the `HISSTools_Dial` control.
     *
     * This method overrides the base `OnMouseOver` function and is called when the mouse cursor moves over
     * the `HISSTools_Dial` control. It processes the event by capturing the x and y coordinates of the mouse
     * cursor and any modifier keys or mouse buttons that were active at the time of the event.
     *
     * @param x The x-coordinate of the mouse cursor when it moves over the control.
     * @param y The y-coordinate of the mouse cursor when it moves over the control.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the mouse over event.
     */
    
    void OnMouseOver(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse out event for the `HISSTools_Dial` control.
     *
     * This method overrides the base `OnMouseOut` function and is called when the mouse cursor leaves
     * the `HISSTools_Dial` control area. It is used to process any actions required when the mouse exits
     * the control, such as resetting visual states (e.g., hover effects) or performing other cleanup tasks.
     */
    
    void OnMouseOut() override;
    
    /**
     * @brief Enables or disables the `HISSTools_Dial` control.
     *
     * This method overrides the base `SetDisabled` function and is used to enable or disable the `HISSTools_Dial` control.
     * When `disable` is set to `true`, the control becomes inactive, preventing user interaction and often changing
     * its visual appearance to reflect its disabled state. When `disable` is set to `false`, the control is enabled
     * and interactive again.
     *
     * @param disable A boolean value indicating whether the control should be disabled (`true`) or enabled (`false`).
     */
    
    void SetDisabled(bool disable) override;
    
    /**
     * @brief Updates the dial's value based on user input.
     *
     * This method overrides the base `SetValueFromUserInput` function and is called when the user provides input
     * to change the value of the `HISSTools_Dial` control. It updates the control's value based on the provided
     * input and handles which specific value to update if the control manages multiple values (indicated by `valIdx`).
     *
     * @param value The new value provided by the user input.
     * @param valIdx The index of the value to be updated, used when the control manages multiple values
     *               (default is typically 0 for single-value controls).
     */
    
    void SetValueFromUserInput(double value, int valIdx) override;

    // Draw

    /**
     * @brief Renders the `HISSTools_Dial` control using the specified graphics context.
     *
     * This method overrides the base `Draw` function and is responsible for rendering the visual elements
     * of the `HISSTools_Dial` control. It draws the dial, including the knob, markers, labels, and any other
     * graphical features, using the provided `IGraphics` object. The method applies the current design scheme
     * and value settings to create a visually appealing and functional dial interface.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g) override;

    /**
     * @brief Configures the dial's rotational throw range and reference value.
     *
     * This method sets the parameters for the rotational behavior of the `HISSTools_Dial`. It specifies
     * the reference value for the dial, the starting angle, and the total throw angle (the range of motion
     * the dial can rotate). These parameters determine how the dial responds to user interaction and how far
     * it can rotate to represent different values.
     *
     * @param refValue The reference value associated with the dial's starting position, typically representing
     *                 the value at the start of the dial's range.
     * @param startAngle The angle (in degrees or radians, depending on the implementation) at which the dial's
     *                   rotation starts.
     * @param throwAngle The total range of the dial's rotation, defining how far the dial can rotate from the
     *                   start angle.
     */
    
    void setThrow(double refValue, double startAngle, double throwAngle);

private:

    /**
     * @brief Configures the appearance of the dial's pointer.
     *
     * This method sets the visual characteristics of the pointer used in the `HISSTools_Dial`.
     * It allows customization of the pointer's shape and size by adjusting the ratio of the pointer's
     * circumference, the tip ratio, and the pointer's angle. These parameters help define how the pointer
     * appears on the dial, affecting its length, width, and orientation.
     *
     * @param pointerCircRatio The ratio of the pointer's length relative to the dial's circumference, controlling
     *                         how far the pointer extends from the center.
     * @param pointerTipRatio The ratio defining the width or size of the pointer's tip relative to its base, controlling
     *                        the pointer's overall shape.
     * @param pointerAngle The angle (in degrees or radians) of the pointer's orientation, defining its default pointing direction.
     */
    
    void SetPointerAppearance(double pointerCircRatio, double pointerTipRatio, double pointerAngle);

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate of the center of the dial control.
     *
     * This member variable stores the x-coordinate for the center point of the dial.
     * It is used to position the dial's rotation point and ensure that graphical elements,
     * such as the knob and pointer, rotate around the correct central location.
     */
    
    double mCx;
    
    /**
     * @brief The y-coordinate of the center of the dial control.
     *
     * This member variable stores the y-coordinate for the center point of the dial.
     * It is used to position the dial's rotation point and ensure that graphical elements,
     * such as the knob and pointer, rotate around the correct central location.
     */
    
    double mCy;
    
    /**
     * @brief The radius of the dial control.
     *
     * This member variable stores the radius of the dial, which is used to define the size of the dial's circular area.
     * It determines the distance from the center of the dial to its outer edge, affecting both the dial's visual appearance
     * and how its pointer or knob is rendered and interacts.
     */
    
    double mR;

    // Pointer Appearance

    /**
     * @brief The radius ratio that determines the length of the dial's pointer relative to the dial's circumference.
     *
     * This member variable stores the ratio that defines how far the dial's pointer extends from the center
     * toward the outer edge of the dial. It is used to control the length of the pointer, with larger values
     * resulting in a longer pointer and smaller values shortening the pointer.
     */
    
    double mPointerCircRadius;
    
    /**
     * @brief The ratio that defines the width or size of the pointer's tip relative to its base.
     *
     * This member variable stores the ratio used to control the shape of the dial's pointer tip.
     * It determines how wide or narrow the pointer tip appears, with larger values making the tip wider
     * and smaller values making it narrower, affecting the overall appearance of the pointer.
     */
    
    double mPointerTipRadius;
    
    /**
     * @brief The angle of the dial's pointer in its default position.
     *
     * This member variable stores the angle (in degrees or radians, depending on implementation) that defines
     * the default orientation of the dial's pointer. It determines the direction in which the pointer points
     * when the dial is at its reference position. The angle is used to control the visual orientation of the pointer.
     */
    
    double mPointerAngle;

    // Display Angles

    /**
     * @brief The reference value for the dial control.
     *
     * This member variable stores the reference value associated with the dial's default or starting position.
     * It represents the value that the dial corresponds to when the pointer is at its initial position, typically
     * at the start of its rotation range. This value serves as a baseline for calculating changes as the user interacts
     * with the dial.
     */
    
    double mRefValue;
    
    /**
     * @brief The starting angle of the dial's rotation range.
     *
     * This member variable stores the angle (in degrees or radians, depending on implementation) that defines
     * the starting point of the dial's rotation. It determines where the dial's pointer begins its rotation
     * relative to the dial's circular range and is used to set the initial position of the dial.
     */
    
    double mStartAngle;
    
    /**
     * @brief The total rotational range of the dial in degrees or radians.
     *
     * This member variable stores the angle that defines the total range of motion (or "throw") for the dial's rotation.
     * It specifies how far the dial's pointer can rotate from the starting angle to the maximum angle, determining
     * the extent of the dial's control and how much it can be turned by the user.
     */
    
    double mThrowAngle;

    // Text Area

    /**
     * @brief The area allocated for displaying text within the dial control.
     *
     * This member variable stores the size (typically the height or width) of the area reserved for rendering
     * text associated with the dial control, such as labels or value readouts. It determines how much space
     * is available for text display, ensuring that the text fits properly within the dial's graphical layout.
     */
    
    double mTextArea;

    // Text Prompt

    /**
     * @brief The half-height of the prompt area for user input.
     *
     * This member variable stores the half-height of the area used for displaying prompts or receiving user input.
     * It defines the vertical size of the input area, allowing for positioning and scaling of interactive elements
     * like value prompts or input fields. The use of half-height ensures symmetrical positioning around a central point.
     */
    
    double mPromptHalfHeight;

    // Line Thicknesses

    /**
     * @brief The thickness of the dial's outline.
     *
     * This member variable stores the thickness of the outline or border of the dial control.
     * It determines how thick the border surrounding the dial appears, influencing the overall
     * visual prominence of the dial's edges and enhancing its visual clarity in the user interface.
     */
    
    double mOutlineTK;
    
    /**
     * @brief The thickness of the dial's pointer.
     *
     * This member variable stores the thickness of the pointer used in the dial control.
     * It defines how wide or thick the pointer appears, affecting its visual prominence
     * and the overall look of the dial. A higher value results in a thicker pointer, while
     * a lower value makes the pointer thinner.
     */
    
    double mPointerTK;
    
    /**
     * @brief The thickness of the outline around the dial's pointer.
     *
     * This member variable stores the thickness of the outline surrounding the dial's pointer.
     * It defines how thick the border around the pointer appears, affecting its visual clarity and contrast
     * within the dial's graphical interface. A larger value results in a thicker outline, making the pointer more prominent.
     */
    
    double mPointerOutlineTK;

    // Shadow Specs

    /**
     * @brief Pointer to the `HISSTools_Shadow` object that defines the shadow effect for the dial's outline.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which specifies the shadow properties
     * applied to the dial's outline. The shadow enhances the visual depth and appearance of the dial, adding contrast
     * and a three-dimensional effect to its outline.
     */
    
    HISSTools_Shadow *mOutlineSD;
    
    /**
     * @brief Pointer to the `HISSTools_Shadow` object that defines the shadow effect for the dial's pointer.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which specifies the shadow properties
     * applied to the dial's pointer. The shadow adds depth and dimension to the pointer, enhancing its visual appearance
     * and providing a three-dimensional effect that makes the pointer stand out within the dial's interface.
     */
    
    HISSTools_Shadow *mPointerSD;

    // Color Specs

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color specification for the dial's indicator.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color of the dial's indicator.
     * The indicator typically highlights important visual elements of the dial, such as the pointer or a value marker,
     * ensuring that the user can easily distinguish the current value or position of the dial.
     */
    
    HISSTools_Color_Spec *mIndicatorCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the fill color of the dial's circle.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used to fill
     * the background circle of the dial. This color is applied to the interior of the dial's circular area,
     * helping define its visual appearance and providing contrast for other elements, such as the pointer and indicators.
     */
    
    HISSTools_Color_Spec *mCircleFillCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the fill color of the dial's pointer.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used to fill
     * the dial's pointer. This color is applied to the interior of the pointer, helping to define its appearance and
     * making it visually distinct from other elements in the dial.
     */
    
    HISSTools_Color_Spec *mPointerFillCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the dial's outline.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the outline
     * or border of the dial. The outline color helps visually define the edges of the dial, making it stand out from the background
     * and providing contrast to other graphical elements.
     */
    
    HISSTools_Color_Spec *mOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the dial's pointer outline.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the outline
     * of the dial's pointer. The pointer outline color helps visually define the edges of the pointer, making it stand out
     * and providing contrast against the dial's background and other graphical elements.
     */
    
    HISSTools_Color_Spec *mPointerOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the inactive overlay.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the overlay when the dial is in an inactive or disabled state. The inactive overlay color is typically
     * applied to visually indicate that the dial is not interactive, often dimming or changing the appearance
     * of the dial to reflect its inactive status.
     */
    
    HISSTools_Color_Spec *mInactiveOverlayCS;

    // Text Blocks

    /**
     * @brief Pointer to the `HISSTools_Text_Helper_Block` object that manages the text label for the dial.
     *
     * This member variable holds a pointer to a `HISSTools_Text_Helper_Block` object, which is responsible
     * for managing and displaying the text label associated with the dial control. The text label typically
     * represents the name or value of the dial and is handled by the text helper for formatting, positioning,
     * and rendering within the dial's graphical interface.
     */
    
    HISSTools_Text_Helper_Block *mTextLabel;
    
    /**
     * @brief Pointer to the `HISSTools_Text_Helper_Param` object that manages parameter-related text for the dial.
     *
     * This member variable holds a pointer to a `HISSTools_Text_Helper_Param` object, which is responsible
     * for managing and displaying text related to the dial's parameter. This could include displaying the
     * parameter's name, current value, or other information, and it helps ensure the text is formatted and
     * positioned correctly within the dial's interface.
     */
    
    HISSTools_Text_Helper_Param *mTextParam;

    // Values on when mouse is over

    /**
     * @brief Indicates whether the mouse cursor is currently hovering over the dial.
     *
     * This member variable holds a boolean value that specifies whether the mouse cursor is currently over the dial control.
     * When `true`, it indicates that the mouse is hovering over the dial, and this state can trigger hover-specific behaviors,
     * such as visual changes or tooltips. When `false`, the mouse is not over the dial.
     */
    
    bool mMouseOver;
    
    /**
     * @brief Indicates whether the dial's value should only be displayed when the mouse is hovering over it.
     *
     * This member variable holds a boolean value that specifies if the dial's value should only be drawn
     * or displayed when the mouse is hovering over the dial. When `true`, the value is shown only during
     * mouse-over events; when `false`, the value is always displayed, regardless of mouse position.
     */
    
    bool mDrawValOnlyOnMO;

    /**
     * @brief Stores the display name of the dial control.
     *
     * This member variable holds a `WDL_String` object that represents the display name associated with the dial control.
     * The display name is typically used to label or identify the control within the user interface, making it easier
     * for users to understand its purpose or functionality.
     */
    
    WDL_String mDisplayName;
};

// HISSTools_Switch
// Switch - multi state control with a number of vertical or horizontal positions

/**
 * @class HISSTools_Switch
 * @brief A control class for implementing a switch in the HISSTools graphics system.
 *
 * This class represents a switch control, allowing users to toggle between states (e.g., on/off, enabled/disabled)
 * within the HISSTools system. It inherits from `iplug::igraphics::IControl` to handle user interaction and control logic,
 * and from `HISSTools_Control_Layers` to manage layered graphical elements, providing a visually rich switch interface.
 *
 * The `HISSTools_Switch` class provides functionality for managing switch states and rendering the switch,
 * enabling state transitions based on user interaction.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IControl` to handle user input and control logic.
 * - Publicly inherits from `HISSTools_Control_Layers` to manage and render graphical layers for the switch's appearance.
 */

class HISSTools_Switch : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor

    /**
     * @brief Constructs a `HISSTools_Switch` object with the specified parameters for position, size, number of states, and design scheme.
     *
     * This constructor initializes a `HISSTools_Switch` object, representing a switch control that can toggle between
     * multiple states. The control is positioned at the specified coordinates with a given width and height.
     * It also allows for customization of the number of switch states, an optional type, and a design scheme to define its appearance.
     *
     * @param paramIdx The index of the parameter associated with this switch, used to manage state transitions.
     * @param x The x-coordinate of the top-left corner of the switch.
     * @param y The y-coordinate of the top-left corner of the switch.
     * @param w The width of the switch control.
     * @param h The height of the switch control.
     * @param nStates The number of states the switch can toggle between (default is 2, for a simple on/off switch).
     * @param type A C-string representing the type of switch, which may influence its appearance or behavior (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object that defines the control's design elements, such as colors and fonts (default is `&DefaultDesignScheme`).
     */
    
    HISSTools_Switch(int paramIdx, double x, double y, double w, double h, int nStates = 2, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme);

    // Mousing Functions

    /**
     * @brief Handles the mouse down event for the `HISSTools_Switch` control.
     *
     * This method overrides the base `OnMouseDown` function and is called when the user presses the mouse button
     * while interacting with the `HISSTools_Switch` control. It processes the mouse down event by capturing the x and y
     * coordinates of the click and any modifier keys or mouse buttons that were pressed at the time of the event.
     * This method typically toggles the state of the switch when the user clicks on it.
     *
     * @param x The x-coordinate where the mouse button was pressed.
     * @param y The y-coordinate where the mouse button was pressed.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the click.
     */
    
    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse drag event for the `HISSTools_Switch` control.
     *
     * This method overrides the base `OnMouseDrag` function and is called when the user drags the mouse while interacting
     * with the `HISSTools_Switch` control. It processes the drag event by capturing the current x and y coordinates of the mouse,
     * as well as the change in position (dX, dY) during the drag, along with any modifier keys or mouse buttons that are active.
     * This method can be used to adjust the state of the switch based on the drag interaction.
     *
     * @param x The current x-coordinate of the mouse during the drag.
     * @param y The current y-coordinate of the mouse during the drag.
     * @param dX The change in the x-coordinate (delta) during the drag.
     * @param dY The change in the y-coordinate (delta) during the drag.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the drag.
     */
    
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;

    // Draw

    /**
     * @brief Renders the `HISSTools_Switch` control using the specified graphics context.
     *
     * This method overrides the base `Draw` function and is responsible for rendering the visual elements of the
     * `HISSTools_Switch` control. It draws the switch's appearance, including its current state (e.g., on/off, enabled/disabled),
     * using the provided `IGraphics` object. The method applies the design scheme and switch states to create a visually
     * appealing and interactive switch control.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g) override;

private:

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate of the switches position.
     *
     * This member variable stores the x-coordinate of the top-left corner of the switch,
     * determining its horizontal position within the graphical interface.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate of the switches position.
     *
     * This member variable stores the y-coordinate of the top-left corner of the switch,
     * determining its vertical position within the graphical interface.
     */
    
    double mY;
    
    /**
     * @brief The width of the switch.
     *
     * This member variable stores the width of the switch, determining how wide the switch will
     * be rendered within the graphical context.
     */
    
    double mW;
    
    /**
     * @brief The height of the switch.
     *
     * This member variable stores the height of the switch, determining how tall the switch will
     * be rendered within the graphical context.
     */
    
    double mH;
    
    /**
     * @brief A scaling factor for the switch control.
     *
     * This member variable stores a scaling factor (`mS`) that can be used to adjust the size or other dimensions
     * of the `HISSTools_Switch` control. It typically affects how the switch is drawn or how user interaction is handled,
     * allowing for dynamic resizing or adjustments based on the control's context or layout.
     */
    
    double mS;
    
    /**
     * @brief The roundness of the switch control's corners.
     *
     * This member variable stores the value that defines the curvature or roundness of the corners of the `HISSTools_Switch` control.
     * A higher value results in more rounded corners, while a lower value produces sharper corners, affecting the overall shape and appearance of the switch.
     */
    
    double mRoundness;

    // Line Thicknesses

    /**
     * @brief The thickness of the handle on the switch control.
     *
     * This member variable stores the value that defines the thickness of the handle in the `HISSTools_Switch` control.
     * The handle thickness affects how prominently the handle is rendered within the switch, with larger values resulting in a thicker handle and smaller values producing a thinner handle.
     */
    
    double mHandleTK;
    
    /**
     * @brief The thickness of the outline surrounding the switch control's box.
     *
     * This member variable stores the value that defines the thickness of the outline or border around the box
     * of the `HISSTools_Switch` control. The box outline thickness affects how prominently the border is drawn,
     * with larger values resulting in a thicker outline and smaller values creating a thinner outline.
     */
    
    double mBoxOutlineTK;

    // Shadow Spec

    /**
     * @brief Pointer to the `HISSTools_Shadow` object that defines the shadow effect for the switch control.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which specifies the shadow properties applied
     * to the `HISSTools_Switch` control. The shadow adds depth and visual contrast to the switch, enhancing its three-dimensional
     * appearance and making it stand out within the graphical interface.
     */
    
    HISSTools_Shadow *mShadow;

    // Color Specs

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the fill color of the switch control's handle.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used to fill the handle
     * of the `HISSTools_Switch` control. The handle fill color determines the visual appearance of the handle, making it distinct
     * from other elements of the switch, such as the background or outline.
     */
    
    HISSTools_Color_Spec *mHandleFillCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the outline color of the switch control's handle.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the outline
     * of the handle in the `HISSTools_Switch` control. The handle outline color helps to visually define the edges of the handle,
     * making it stand out from the switch's background and other elements.
     */
    
    HISSTools_Color_Spec *mHandleOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the fill color of the switch control's box.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used to fill the box
     * of the `HISSTools_Switch` control. The box fill color determines the background color of the switch, providing contrast
     * with the handle and other elements.
     */
    
    HISSTools_Color_Spec *mBoxFillCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the outline color of the switch control's box.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the outline
     * of the box in the `HISSTools_Switch` control. The box outline color visually defines the edges of the switch's box,
     * enhancing its appearance and providing contrast with other elements like the handle and background.
     */
    
    HISSTools_Color_Spec *mBoxOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the inactive overlay.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the overlay when the `HISSTools_Switch` control is in an inactive or disabled state. The inactive overlay
     * visually indicates that the switch is not interactive by dimming or altering its appearance, reflecting its inactive status.
     */
    
    HISSTools_Color_Spec *mInactiveOverlayCS;

    // Number of States

    /**
     * @brief The number of states the switch control can toggle between.
     *
     * This member variable stores the number of states (`mNStates`) that the `HISSTools_Switch` control can have.
     * It defines how many distinct positions or values the switch can toggle through (e.g., 2 for a simple on/off switch).
     * The value of `mNStates` controls how the switch behaves and transitions between its states.
     */
    
    int mNStates;
};

// HISSTools_Matrix
//

/**
 * @class HISSTools_Matrix
 * @brief A control class for implementing a matrix grid in the HISSTools graphics system.
 *
 * This class represents a matrix control, allowing users to interact with a grid of buttons or cells
 * that can toggle between various states. It is useful for creating interfaces that involve selecting
 * or toggling multiple elements, such as a matrix mixer or step sequencer. The class inherits from
 * `iplug::igraphics::IControl` for handling user interaction and control logic, and from `HISSTools_Control_Layers`
 * for managing layered graphical elements to provide a rich visual interface.
 *
 * The `HISSTools_Matrix` class enables interaction with multiple grid elements and provides functionality
 * for rendering, updating, and managing the state of each cell within the matrix.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IControl` to handle user input and control logic.
 * - Publicly inherits from `HISSTools_Control_Layers` to manage and render graphical layers for the matrix grid.
 */

class HISSTools_Matrix : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    /**
     * @brief Constructs a `HISSTools_Matrix` object with specified parameters for position, grid dimensions, and design schemes.
     *
     * This constructor initializes a `HISSTools_Matrix` object, representing a matrix grid control with multiple
     * selectable cells. The control is placed at the specified x and y coordinates, with defined grid dimensions
     * (xDim and yDim) for the number of rows and columns in the matrix. The constructor also allows customization
     * of the matrix type, a general design scheme for its appearance, and an optional state design scheme for managing
     * the appearance of individual cell states.
     *
     * @param paramIdx The index of the parameter associated with this matrix control, used for managing its state.
     * @param x The x-coordinate of the top-left corner of the matrix control.
     * @param y The y-coordinate of the top-left corner of the matrix control.
     * @param xDim The number of columns in the matrix (the horizontal dimension).
     * @param yDim The number of rows in the matrix (the vertical dimension).
     * @param type A C-string representing the type of matrix, which may influence its behavior or appearance (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object that defines the overall design elements (e.g., colors, fonts) for the matrix (default is `&DefaultDesignScheme`).
     * @param stateScheme A pointer to the `HISSTools_Design_Scheme` object that defines the design elements for individual cell states in the matrix (default is `nullptr`).
     */
    
    HISSTools_Matrix(int paramIdx, double x, double y, int xDim, int yDim, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, HISSTools_Design_Scheme *stateScheme = 0);

    /**
     * @brief Destructor for the `HISSTools_Matrix` class.
     *
     * The destructor cleans up any resources or memory used by the `HISSTools_Matrix` object.
     * It is called when the object is destroyed to ensure that all associated resources,
     * such as dynamically allocated memory, are properly released.
     */
    
    ~HISSTools_Matrix();

    /**
     * @brief Retrieves the x-coordinate of the matrix control's position.
     *
     * This method returns the x-coordinate of the top-left corner of the `HISSTools_Matrix` control.
     * It provides the current horizontal position of the matrix within the graphical interface.
     *
     * @return The x-coordinate of the matrix control's position.
     */
    
    int GetXPos() const;
    
    /**
     * @brief Retrieves the y-coordinate of the matrix control's position.
     *
     * This method returns the y-coordinate of the top-left corner of the `HISSTools_Matrix` control.
     * It provides the current vertical position of the matrix within the graphical interface.
     *
     * @return The y-coordinate of the matrix control's position.
     */
    
    int GetYPos() const;

    // Mousing Functions

    /**
     * @brief Handles mouse interaction events for the matrix control.
     *
     * This method processes various mouse events for the `HISSTools_Matrix` control, including clicks, drags,
     * and mouse wheel movements. It captures the mouse coordinates (x and y), any active modifiers (such as shift or control),
     * the type of mousing action (e.g., click, drag, wheel), and the optional mouse wheel delta. The method responds to user
     * input by performing the corresponding actions within the matrix, such as selecting or modifying cells.
     *
     * @param x The x-coordinate of the mouse during the event.
     * @param y The y-coordinate of the mouse during the event.
     * @param mod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift).
     * @param action An enum value representing the type of mousing action (e.g., click, drag, double-click, wheel).
     * @param wheel The delta value for mouse wheel movement (default is 0 if no wheel action is involved).
     *
     * @return A boolean value indicating whether the event was successfully handled.
     */
    
    bool OnMousing(float x, float y, const IMouseMod& mod, MousingAction action, float wheel = 0.f);
    
    /**
     * @brief Handles the mouse down event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseDown` function and is called when the user presses the mouse button
     * while interacting with the `HISSTools_Matrix` control. It captures the x and y coordinates of the mouse click,
     * along with any active mouse modifiers (e.g., control, shift). This method typically handles the selection
     * or toggling of a matrix cell based on the user's interaction.
     *
     * @param x The x-coordinate where the mouse button was pressed.
     * @param y The y-coordinate where the mouse button was pressed.
     * @param mod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the event.
     */
    
    void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    
    /**
     * @brief Handles the mouse up event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseUp` function and is called when the user releases the mouse button
     * while interacting with the `HISSTools_Matrix` control. It captures the x and y coordinates where the mouse
     * button was released, along with any active mouse modifiers (e.g., control, shift). This method can be used
     * to finalize actions started during a mouse down or drag event, such as confirming a cell selection.
     *
     * @param x The x-coordinate where the mouse button was released.
     * @param y The y-coordinate where the mouse button was released.
     * @param mod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the event.
     */
    
    void OnMouseUp(float x, float y, const IMouseMod& mod) override;
    
    /**
     * @brief Handles the double-click event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseDblClick` function and is called when the user performs a double-click
     * while interacting with the `HISSTools_Matrix` control. It captures the x and y coordinates where the double-click
     * occurred, along with any active mouse modifiers (e.g., control, shift). Double-clicking can be used to trigger
     * specific actions within the matrix, such as resetting a cell or selecting multiple cells.
     *
     * @param x The x-coordinate where the double-click occurred.
     * @param y The y-coordinate where the double-click occurred.
     * @param mod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the event.
     */
    
    void OnMouseDblClick(float x, float y, const IMouseMod& mod) override;
    
    /**
     * @brief Handles the mouse drag event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseDrag` function and is called when the user drags the mouse while interacting
     * with the `HISSTools_Matrix` control. It captures the current x and y coordinates of the mouse, the change in position
     * (dX, dY), and any active mouse modifiers (e.g., control, shift). Mouse dragging can be used to interact with multiple
     * cells, allowing for selection or modification of multiple matrix elements during the drag action.
     *
     * @param x The current x-coordinate of the mouse during the drag.
     * @param y The current y-coordinate of the mouse during the drag.
     * @param dX The change in the x-coordinate (delta) during the drag.
     * @param dY The change in the y-coordinate (delta) during the drag.
     * @param mod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the drag.
     */
    
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
    
    /**
     * @brief Handles the mouse wheel event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseWheel` function and is called when the user scrolls the mouse wheel
     * while interacting with the `HISSTools_Matrix` control. It captures the x and y coordinates of the mouse,
     * any active mouse modifiers (e.g., control, shift), and the scroll delta (`d`). The mouse wheel can be used to
     * change matrix parameters or navigate through different parts of the matrix, depending on the control's design.
     *
     * @param x The x-coordinate of the mouse during the wheel event.
     * @param y The y-coordinate of the mouse during the wheel event.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift).
     * @param d The delta value of the mouse wheel, indicating the direction and amount of scrolling.
     */
    
    void OnMouseWheel(float x, float y, const IMouseMod& pMod, float d) override;
    
    /**
     * @brief Handles the mouse over event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseOver` function and is called when the mouse cursor moves over
     * the `HISSTools_Matrix` control. It captures the x and y coordinates of the mouse during the event, along with
     * any active mouse modifiers (e.g., control, shift). This method can trigger hover-specific behavior, such as
     * highlighting matrix cells or displaying tooltips.
     *
     * @param x The x-coordinate of the mouse when it moves over the matrix control.
     * @param y The y-coordinate of the mouse when it moves over the matrix control.
     * @param mod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift).
     */
    
    void OnMouseOver(float x, float y, const IMouseMod& mod) override;
    
    /**
     * @brief Handles the mouse out event for the `HISSTools_Matrix` control.
     *
     * This method overrides the base `OnMouseOut` function and is called when the mouse cursor leaves
     * the `HISSTools_Matrix` control area. It is typically used to reset any visual changes or hover-specific
     * behaviors, such as removing cell highlights or hiding tooltips, that were triggered when the mouse was over the control.
     */
    
    virtual void OnMouseOut() override;

    // Draw

    /**
     * @brief Renders the `HISSTools_Matrix` control using the specified graphics context.
     *
     * This method overrides the base `Draw` function and is responsible for rendering the visual elements of the
     * `HISSTools_Matrix` control. It draws the matrix grid, including individual cells, highlights, and other visual cues
     * based on the current state of the matrix. The rendering is performed using the provided `IGraphics` object,
     * ensuring that the control's design and interactivity are visually represented in the user interface.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g) override;

    /**
     * @brief Sets the state of a specific cell in the matrix control.
     *
     * This method updates the state of the matrix cell located at the specified (x, y) coordinates. The new state
     * is provided as a character value (`state`), which can represent different conditions, such as on/off, active/inactive,
     * or other user-defined states. This method allows the matrix to reflect changes based on user interaction or programmatic updates.
     *
     * @param x The x-coordinate (column) of the matrix cell to be updated.
     * @param y The y-coordinate (row) of the matrix cell to be updated.
     * @param state A character value representing the new state of the cell.
     */
    
    void SetState(int x, int y, char state);
    
    /**
     * @brief Retrieves the current state of a specific cell in the matrix control.
     *
     * This method returns the state of the matrix cell located at the specified (x, y) coordinates.
     * The state is returned as an unsigned character, which represents the current condition of the cell
     * (e.g., on/off, active/inactive, or other user-defined states). This method allows access to the matrix
     * state for querying or further processing.
     *
     * @param x The x-coordinate (column) of the matrix cell whose state is to be retrieved.
     * @param y The y-coordinate (row) of the matrix cell whose state is to be retrieved.
     * @return The current state of the cell as an unsigned character.
     */
    
    unsigned char GetState(int x, int y);

    /**
     * @brief Enables or disables the highlight effect for the matrix control.
     *
     * This method sets the highlight state of the matrix control. When `on` is set to `true`,
     * the matrix or certain elements within it are visually highlighted, typically indicating user focus or interaction.
     * When `on` is set to `false`, the highlight effect is disabled, returning the matrix to its normal state.
     *
     * @param on A boolean value indicating whether to enable (`true`) or disable (`false`) the highlight effect.
     */
    
    void SetHilite(bool on);

private:

    /**
     * @brief Converts screen coordinates to matrix indices.
     *
     * This method translates the given screen coordinates (x, y) into the corresponding row and column indices
     * in the matrix control. It allows the system to map user interaction points (e.g., mouse clicks) to specific
     * cells in the matrix. The resulting indices are stored in `xPos` and `yPos`. The method returns `true` if the
     * coordinates are successfully converted to valid matrix indices, and `false` if the coordinates are outside the matrix bounds.
     *
     * @param x The x-coordinate on the screen.
     * @param y The y-coordinate on the screen.
     * @param xPos A pointer to an integer that will store the column index of the matrix.
     * @param yPos A pointer to an integer that will store the row index of the matrix.
     * @return `true` if the coordinates are within the matrix bounds and successfully converted, `false` otherwise.
     */
    
    bool CoordsToIndices(double x, double y, int *xPos, int *yPos);
    
    /**
     * @brief Reports user interaction within the matrix control to the plugin.
     *
     * This virtual method is intended to report interactions in the matrix control (such as mouse clicks, drags, or wheel events)
     * to the plugin. It provides details about the interaction, including the cell coordinates (`xPos`, `yPos`),
     * any active mouse modifiers (`mod`), the type of mousing action (`action`), and an optional mouse wheel delta (`wheel`).
     * The method can be overridden in derived classes to implement specific behavior for how these interactions
     * are communicated to the plugin.
     *
     * @param xPos The column index of the matrix cell where the interaction occurred.
     * @param yPos The row index of the matrix cell where the interaction occurred.
     * @param mod A reference to an `IMouseMod` object containing information about mouse modifiers (e.g., control, shift).
     * @param action An enum value representing the type of mousing action (e.g., click, drag, wheel).
     * @param wheel The delta value for mouse wheel movement (default is 0 if no wheel action is involved).
     */
    
    virtual void ReportToPlug(int xPos, int yPos, const IMouseMod& mod, MousingAction action, float wheel = 0.f)
    {}

    // Size (Dimension)

    /**
     * @brief The number of columns (horizontal dimension) in the matrix control.
     *
     * This member variable stores the number of columns in the matrix control, defining the horizontal size of the matrix grid.
     * It determines how many individual cells are present in the x-dimension, affecting both the appearance and behavior
     * of the matrix control.
     */
    
    int mXDim;
    
    /**
     * @brief The number of rows (vertical dimension) in the matrix control.
     *
     * This member variable stores the number of rows in the matrix control, defining the vertical size of the matrix grid.
     * It determines how many individual cells are present in the y-dimension, affecting both the appearance and behavior
     * of the matrix control.
     */
    int mYDim;

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate position of the matrix control.
     *
     * This member variable stores the x-coordinate (horizontal position) of the matrix control's top-left corner.
     * It is used to position the matrix within the user interface, defining where the control will be displayed
     * relative to other elements on the screen.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate position of the matrix control.
     *
     * This member variable stores the y-coordinate (vertical position) of the matrix control's top-left corner.
     * It is used to position the matrix within the user interface, defining where the control will be displayed
     * relative to other elements on the screen.
     */
    
    double mY;
    
    /**
     * @brief The width of the matrix control.
     *
     * This member variable stores the width of the matrix control, defining how wide the control appears on the screen.
     * It influences the overall size of the matrix and, in combination with other layout parameters, determines the size
     * of individual cells within the matrix grid.
     */
    
    double mW;
    
    /**
     * @brief The height of the matrix control.
     *
     * This member variable stores the height of the matrix control, defining how tall the control appears on the screen.
     * It influences the overall size of the matrix and, in combination with other layout parameters, determines the size
     * of individual cells within the matrix grid.
     */
    
    double mH;
    
    /**
     * @brief The scaling factor for the matrix control.
     *
     * This member variable stores a scaling factor (`mS`) that is used to adjust the size or dimensions of the matrix control
     * and its individual cells. The scaling factor can affect how large or small the matrix appears, providing flexibility
     * in resizing the control dynamically to fit different layouts or user interface requirements.
     */
    
    double mS;
    
    /**
     * @brief The roundness of the matrix control's corners.
     *
     * This member variable stores the value that defines the curvature or roundness of the corners of the matrix control
     * and its cells. A higher value results in more rounded corners, while a lower value produces sharper corners,
     * affecting the overall shape and appearance of the matrix control.
     */
    
    double mRoundness;
    
    /**
     * @brief The spacing or gap between the cells in the matrix control.
     *
     * This member variable stores the value that defines the gap or spacing between individual cells in the matrix grid.
     * It determines how much space is left between each cell, affecting the visual layout of the matrix and providing
     * separation between the cells for a cleaner appearance.
     */
    
    double mGap;
    
    /**
     * @brief The base unit size for the matrix control.
     *
     * This member variable stores the base unit size (`mUnit`) that is used to define the size of individual cells
     * in the matrix control. It serves as a reference for determining the dimensions of each cell, and may also be used
     * to calculate other layout properties, such as the overall size of the matrix or the spacing between cells.
     */
    
    double mUnit;

    // Line Thicknesses

    /**
     * @brief The thickness of the outline for empty (unfilled) handles in the matrix control.
     *
     * This member variable stores the value that defines the thickness of the outline around empty or unfilled handles
     * in the matrix control. It affects the visual appearance of handles when they are in an unselected or inactive state,
     * providing a distinct border to enhance the visibility of empty handles.
     */
    
    double mHandleEmptyOutlineTK;
    
    /**
     * @brief The thickness of the outline for filled handles in the matrix control.
     *
     * This member variable stores the value that defines the thickness of the outline around filled or selected handles
     * in the matrix control. It determines how thick the border appears for handles that are in an active or filled state,
     * providing a visual distinction between selected and unselected handles.
     */
    
    double mHandleFilledOutlineTK;
    
    /**
     * @brief The thickness of the highlight outline in the matrix control.
     *
     * This member variable stores the value that defines the thickness of the highlight outline applied to matrix elements
     * when they are in a highlighted state. It determines how thick the border appears around highlighted cells or handles,
     * enhancing their visibility during user interaction, such as hovering or selecting.
     */
    
    double mHiliteTK;

    // Shadow Spec

    /**
     * @brief Pointer to the `HISSTools_Shadow` object that defines the shadow effect for the matrix control.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which specifies the shadow properties applied
     * to the matrix control. The shadow adds depth and visual contrast, creating a three-dimensional appearance
     * that enhances the visual clarity of the matrix and its elements.
     */
    
    HISSTools_Shadow *mShadow;

    // Color Specs

    /**
     * @brief Array of pointers to `HISSTools_Color_Spec` objects that define the color scheme for different states in the matrix control.
     *
     * This member variable is an array of 256 pointers, each pointing to a `HISSTools_Color_Spec` object, which defines
     * the color associated with a particular state in the matrix control. Each index in the array corresponds to a unique state,
     * and the color scheme is applied to the matrix cells based on their current state. This allows the matrix to visually represent
     * different states using different colors.
     */
    
    HISSTools_Color_Spec *mStateCS[256];
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the outline color of the matrix control.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the outline or border around the matrix control. The outline color helps visually define the edges
     * of the matrix and its cells, enhancing the clarity and contrast between the matrix and the surrounding interface.
     */
    
    HISSTools_Color_Spec *mOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the highlight color for the matrix control.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for highlighting elements within the matrix control. The highlight color is applied when matrix cells or handles
     * are in a highlighted state, such as during mouse hover or selection, providing visual feedback to the user.
     */
    
    HISSTools_Color_Spec *mHiliteCS;

    // States

    /**
     * @brief Pointer to an array that stores the states of the cells in the matrix control.
     *
     * This member variable holds a pointer to an array of unsigned char values, where each element in the array
     * represents the current state of a corresponding cell in the matrix control. Each state value (typically represented
     * as an unsigned char) can correspond to different conditions such as active/inactive, selected/deselected, or custom states
     * depending on the implementation of the matrix.
     */
    
    unsigned char *mStates;
    
    /**
     * @brief The number of possible states for each cell in the matrix control.
     *
     * This member variable stores the number of possible states (`mNStates`) that each cell in the matrix can have.
     * It defines the range of different conditions or states that a matrix cell can represent, such as on/off,
     * active/inactive, or other user-defined states. The value of `mNStates` limits the range of state values
     * stored in the `mStates` array.
     */
    
    unsigned char mNStates;

    // Hilite

    /**
     * @brief The x-coordinate index of the currently highlighted cell in the matrix control.
     *
     * This member variable stores the x-coordinate (column index) of the matrix cell that is currently highlighted.
     * It tracks which cell in the matrix's horizontal dimension is in a highlighted state, typically in response
     * to user interaction such as hovering or selection. A value of `-1` may indicate that no cell is currently highlighted.
     */
    
    int mXHilite;
    
    /**
     * @brief The y-coordinate index of the currently highlighted cell in the matrix control.
     *
     * This member variable stores the y-coordinate (row index) of the matrix cell that is currently highlighted.
     * It tracks which cell in the matrix's vertical dimension is in a highlighted state, typically in response
     * to user interaction such as hovering or selection. A value of `-1` may indicate that no cell is currently highlighted.
     */
    
    int mYHilite;

    // Mousing Info

    /**
     * @brief The current x-coordinate index of a selected or active cell in the matrix control.
     *
     * This member variable stores the x-coordinate (column index) of a cell that is currently selected or active
     * in the matrix control. It tracks the horizontal position of the cell that is being interacted with or modified.
     * The value of `mXPos` helps manage user input and interaction with specific cells in the matrix.
     */
    
    int mXPos;
    
    /**
     * @brief The current y-coordinate index of a selected or active cell in the matrix control.
     *
     * This member variable stores the y-coordinate (row index) of a cell that is currently selected or active
     * in the matrix control. It tracks the vertical position of the cell that is being interacted with or modified.
     * The value of `mXPos` helps manage user input and interaction with specific cells in the matrix.
     */
    
    int mYPos;
};

// HISSTools_Progress
//

/**
 * @class HISSTools_Progress
 * @brief A control class for displaying progress in the HISSTools graphics system.
 *
 * This class represents a progress bar or indicator, used to visually show the progression of a task, value,
 * or operation within the HISSTools system. It inherits from `iplug::igraphics::IControl` for handling user interaction
 * and control logic, and from `HISSTools_Control_Layers` for managing layered graphical elements that define the visual
 * appearance of the progress bar.
 *
 * The `HISSTools_Progress` class allows for flexible rendering of progress states, including setting, updating,
 * and displaying the current progress in a visually rich format.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IControl` to manage user input and control logic.
 * - Publicly inherits from `HISSTools_Control_Layers` to handle graphical layering for a customizable appearance.
 */

class HISSTools_Progress : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
public:

    // Constructor and Destructor

    /**
     * @brief Constructs a `HISSTools_Progress` object with the specified parameters for position, size, type, and design scheme.
     *
     * This constructor initializes a `HISSTools_Progress` object, which represents a progress bar or indicator. The control
     * is positioned at the specified x and y coordinates, with a defined width (`w`) and height (`h`). An optional type
     * and design scheme can be provided to customize the appearance and behavior of the progress indicator.
     *
     * @param x The x-coordinate of the top-left corner of the progress bar.
     * @param y The y-coordinate of the top-left corner of the progress bar.
     * @param w The width of the progress bar.
     * @param h The height of the progress bar.
     * @param type A C-string representing the type of progress indicator, which may influence its appearance or behavior (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object that defines the control's design elements, such as colors and fonts (default is `&DefaultDesignScheme`).
     */
    
    HISSTools_Progress(double x, double y, double w, double h, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme);

    /**
     * @brief Destructor for the `HISSTools_Progress` class.
     *
     * The destructor is responsible for cleaning up any resources or memory used by the `HISSTools_Progress` object.
     * Since this implementation is empty (`{}`), it implies that no special resource management is required, and the default
     * behavior for object destruction is sufficient.
     */
    
    ~HISSTools_Progress() {}

    // Draw

    /**
     * @brief Renders the `HISSTools_Progress` control using the specified graphics context.
     *
     * This method is responsible for rendering the visual elements of the `HISSTools_Progress` control.
     * It draws the progress bar, including any background, fill, and other graphical details,
     * based on the current progress value. The rendering is performed using the provided `IGraphics` object,
     * ensuring that the progress bar is visually updated in the user interface.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g);

private:

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate position of the progress bar control.
     *
     * This member variable stores the x-coordinate (horizontal position) of the top-left corner of the `HISSTools_Progress` control.
     * It is used to position the progress bar within the user interface, determining where the control will be displayed relative to other UI elements.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate position of the progress bar control.
     *
     * This member variable stores the y-coordinate (vertical position) of the top-left corner of the `HISSTools_Progress` control.
     * It is used to position the progress bar within the user interface, determining where the control will be displayed relative to other UI elements.
     */
    
    double mY;
    
    /**
     * @brief The width of the progress bar control.
     *
     * This member variable stores the width of the `HISSTools_Progress` control, defining how wide the progress bar appears on the screen.
     * It affects the overall size of the progress bar and how much space it occupies within the user interface.
     */
    
    double mW;
    
    /**
     * @brief The height of the progress bar control.
     *
     * This member variable stores the height of the `HISSTools_Progress` control, defining how tall the progress bar appears on the screen.
     * It affects the overall size of the progress bar and how much space it occupies within the user interface.
     */
    
    double mH;

    // Line Thicknesses

    /**
     * @brief The thickness of the outline for the progress bar control.
     *
     * This member variable stores the thickness of the outline (`mOutlineTK`) surrounding the `HISSTools_Progress` control.
     * It determines how thick the border around the progress bar appears, affecting its visual prominence and clarity within the user interface.
     */
    
    double mOutlineTK;

    // Shadow Spec

    /**
     * @brief Pointer to the `HISSTools_Shadow` object that defines the shadow effect for the progress bar control.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which specifies the shadow properties applied
     * to the `HISSTools_Progress` control. The shadow adds depth and visual contrast, giving the progress bar a three-dimensional effect
     * and enhancing its appearance within the user interface.
     */
    
    HISSTools_Shadow *mShadow;

    // Color Specs

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the background color of the progress bar control.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the background of the `HISSTools_Progress` control. The background color is applied behind the progress bar,
     * providing contrast with the progress fill and other graphical elements.
     */
    
    HISSTools_Color_Spec *mBackgroundCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the outline color of the progress bar control.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the outline of the `HISSTools_Progress` control. The outline color is applied around the progress bar,
     * providing a border that visually separates it from the surrounding interface and enhances its appearance.
     */
    
    HISSTools_Color_Spec *mOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the progress bar fill.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the progress fill in the `HISSTools_Progress` control. The progress color represents the portion of the bar
     * that visually shows how much progress has been made, typically growing in length as the progress increases.
     */
    
    HISSTools_Color_Spec *mProgressCS;
};

// HISSTools_VUMeter
// A VU meter display. Ballistics should be provided on the DSP side
// The meter supports two on meter levels (drawn in order) and a side value, typically intended for peak hold

/**
 * @class HISSTools_VUMeter
 * @brief A control class for implementing a VU (Volume Unit) meter in the HISSTools graphics system.
 *
 * This class represents a VU meter control, used to visually display audio signal levels. It inherits from
 * `iplug::igraphics::IControl` for handling user interaction and control logic, and from `HISSTools_Control_Layers`
 * for managing layered graphical elements that define the visual appearance of the VU meter.
 *
 * The `HISSTools_VUMeter` class allows for flexible rendering of VU meters, including the ability to display
 * real-time audio levels, peak indicators, and dynamic visual feedback based on input signal strength.
 *
 * Inheritance:
 * - Publicly inherits from `iplug::igraphics::IControl` to handle control logic and input.
 * - Publicly inherits from `HISSTools_Control_Layers` to manage and render graphical layers for the VU meter's appearance.
 */

class HISSTools_VUMeter : public iplug::igraphics::IControl, public HISSTools_Control_Layers
{
    
    /**
     * @enum
     * @brief Defines an enumeration for update tags used in the `HISSTools_VUMeter` control.
     *
     * This enumeration defines constants that are used to identify different update events or tags in the `HISSTools_VUMeter` control.
     * The `kUpdateTag` value is typically used to signal or track specific updates, such as changes in the audio level,
     * or to trigger visual refreshes in the VU meter.
     *
     * @var kUpdateTag
     * A constant used to represent an update event or tag, typically associated with changes in the control's state or rendering.
     */
    
    enum { kUpdateTag = 0, };

    /**
     * @struct MeterValues
     * @brief A structure for holding the current meter values for the `HISSTools_VUMeter`.
     *
     * This structure is used to store the current values associated with the `HISSTools_VUMeter` control,
     * such as the audio signal level, peak level, or other related metrics. These values are updated in real time
     * and are used to display the dynamic behavior of the VU meter.
     *
     * The `MeterValues` struct helps encapsulate the data related to the VU meter's state, making it easier
     * to manage and process the meter's visual and functional updates.
     */
    
    struct MeterValues
    {
        
        /**
         * @brief The current value of the first VU meter level.
         *
         * This member variable stores the current value for the first VU (Volume Unit) meter in the `HISSTools_VUMeter` control.
         * It represents the real-time audio signal level or volume for the first channel or signal being monitored.
         * The value of `mVU1` is used to visually represent the signal level in the VU meter's graphical display.
         */
        
        double mVU1;
        
        /**
         * @brief The current value of the second VU meter level.
         *
         * This member variable stores the current value for the second VU (Volume Unit) meter in the `HISSTools_VUMeter` control.
         * It represents the real-time audio signal level or volume for the second channel or signal being monitored.
         * The value of `mVU2` is used to visually represent the signal level in the VU meter's graphical display.
         */
        
        double mVU2;
        
        /**
         * @brief The size or length of the side of the VU meter control.
         *
         * This member variable stores the value for the dimension of the side (`mSide`) of the `HISSTools_VUMeter` control.
         * It is used to determine the width or height of the VU meter, depending on the layout, and directly influences
         * the overall size and scale of the graphical display.
         */
        
        double mSide;
        
        /**
         * @brief A flag indicating whether the peak level is currently active in the VU meter.
         *
         * This member variable stores a boolean value that represents whether the peak level has been reached in the `HISSTools_VUMeter` control.
         * When `mPeak` is `true`, it indicates that the audio signal has reached or exceeded a defined peak threshold,
         * and this can trigger a visual indication (such as a peak hold or color change) in the VU meter display.
         * When `false`, the peak level is not active.
         */
        
        bool mPeak;
        
        /**
         * @brief A flag indicating whether the VU meter operates in linear mode.
         *
         * This member variable stores a boolean value that represents whether the `HISSTools_VUMeter` operates in linear mode.
         * When `mLinear` is `true`, the meter displays the audio signal on a linear scale, where equal increments represent equal changes in level.
         * When `false`, the meter may operate in a logarithmic or other scale, which is more typical for audio level representation.
         */
        
        bool mLinear;
    };

public:

    /**
     * @class Sender
     * @brief A utility class for sending data, such as VU meter values, from the control to other parts of the system.
     *
     * This class is responsible for transmitting or sending information, such as the current VU meter levels or other data,
     * from the `HISSTools_VUMeter` control to other components in the system. It could be used to send real-time data to
     * the audio engine, a plugin, or other graphical elements that need to receive updates based on the VU meter's state.
     *
     * The `Sender` class acts as an intermediary to facilitate communication between the control and other system components.
     */
    
    class Sender
    {
    public:

        /**
         * @brief Constructs a `Sender` object with the specified control tag.
         *
         * This constructor initializes a `Sender` object, which is responsible for sending data such as VU meter values.
         * It takes an integer `controlTag` as a parameter, which is used to identify the specific control or component
         * associated with this sender. The constructor also initializes a queue with a default size of 32 to handle
         * data transmission.
         *
         * @param controlTag An integer that uniquely identifies the control or component associated with this sender.
         */
        
        Sender(int controlTag) : mControlTag(controlTag), mQueue(32) {}

        /**
         * @brief Sets the values for the VU meter, including signal levels, size, and display mode.
         *
         * This method updates the state of the VU meter by setting the values for two VU levels, the size (side),
         * whether the peak indicator is active, and whether the VU meter operates in linear mode. These parameters
         * control how the VU meter displays audio levels and other visual aspects.
         *
         * @param VU1 The value for the first VU meter level.
         * @param VU2 The value for the second VU meter level (for stereo meters or multi-channel).
         * @param side The size or side dimension of the VU meter.
         * @param peak A boolean indicating whether the peak indicator is active (`true`) or not (`false`).
         * @param linear A boolean indicating whether the VU meter operates in linear mode (`true` by default) or logarithmic mode.
         */
        
        void Set(double VU1, double VU2, double side, bool peak, bool linear = true);
        
        /**
         * @brief Updates the control state and notifies the editor delegate of any changes.
         *
         * This method updates the state of the VU meter control and communicates any changes to the `IEditorDelegate`.
         * The editor delegate is responsible for handling updates in the user interface or other components based on the control's state.
         * This method ensures that any modifications to the VU meter, such as changes in levels or visual indicators,
         * are reflected and acted upon by the appropriate delegate.
         *
         * @param dlg A reference to the `IEditorDelegate` object that handles updates for the control.
         */
        
        void UpdateControl(IEditorDelegate& dlg);
        
        /**
         * @brief Resets the VU meter control to its initial state.
         *
         * This method resets the VU meter control, clearing any current values, such as signal levels or peak indicators,
         * and restoring the control to its default or initial state. It can be used to reinitialize the meter after
         * it has been in use or when starting a new measurement.
         */
        
        void Reset();

    private:

        /**
         * @brief A tag used to uniquely identify the control associated with this sender.
         *
         * This member variable stores the `mControlTag`, an integer value that uniquely identifies the control or component
         * associated with the sender. It is used to distinguish between different controls when communicating updates
         * or data, ensuring that the correct control receives the intended information.
         */
        
        int mControlTag;
        
        /**
         * @brief A queue for storing `MeterValues` to manage updates to the VU meter.
         *
         * This member variable holds an `IPlugQueue` of `MeterValues`, which is used to queue and manage updates to the VU meter control.
         * The queue ensures that changes in the VU meter's values, such as audio signal levels, are processed in order,
         * allowing for smooth and efficient real-time updates. It helps manage asynchronous updates or buffering of data to ensure
         * consistent behavior in the user interface.
         */
        
        IPlugQueue<MeterValues> mQueue;
    };

    /**
     * @brief Constructs a `HISSTools_VUMeter` object with specified dimensions, scaling, and design properties.
     *
     * This constructor initializes a `HISSTools_VUMeter` object, representing a visual volume unit meter. The VU meter
     * is positioned at the specified x and y coordinates with defined width (`w`) and height (`h`). The constructor allows
     * customization of the meter's orientation (via the `flip` flag), the minimum and maximum decibel range for the meter,
     * and the type and design scheme, which determine the meter's appearance and behavior.
     *
     * @param x The x-coordinate of the top-left corner of the VU meter.
     * @param y The y-coordinate of the top-left corner of the VU meter.
     * @param w The width of the VU meter.
     * @param h The height of the VU meter.
     * @param flip A boolean indicating whether to flip the VU meter's orientation (default is `false`).
     * @param minDB The minimum decibel level displayed by the VU meter (default is -60 dB).
     * @param maxDB The maximum decibel level displayed by the VU meter (default is 0 dB).
     * @param type A C-string representing the type of VU meter, which may influence its behavior or appearance (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object that defines the control's design elements, such as colors and fonts (default is `&DefaultDesignScheme`).
     */
    
    HISSTools_VUMeter(double x, double y, double w, double h, bool flip = false, double minDB = -60, double maxDB = 0, const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme);

    /**
     * @brief Destructor for the `HISSTools_VUMeter` class.
     *
     * The destructor is responsible for cleaning up any resources or memory used by the `HISSTools_VUMeter` object.
     * Since this implementation is empty (`{}`), it implies that no special resource management is required, and the default
     * behavior for object destruction is sufficient.
     */
    
    ~HISSTools_VUMeter() {}

    /**
     * @brief Handles messages sent from the delegate to the `HISSTools_VUMeter` control.
     *
     * This method overrides the base `OnMsgFromDelegate` function and is responsible for processing messages
     * that are sent from the delegate (such as the audio engine or other components) to the `HISSTools_VUMeter` control.
     * It captures the message tag (`messageTag`), the size of the data (`dataSize`), and a pointer to the message data (`pData`).
     * Based on the message, the method can update the VU meter's state, such as adjusting the meter levels or responding
     * to other control-related actions.
     *
     * @param messageTag An integer tag that identifies the type of message being sent.
     * @param dataSize The size of the data being transmitted with the message.
     * @param pData A pointer to the data being sent, which can include various control information.
     */
    
    void OnMsgFromDelegate(int messageTag, int dataSize, const void* pData) override;

    // Draw

    /**
     * @brief Renders the `HISSTools_VUMeter` control using the specified graphics context.
     *
     * This method overrides the base `Draw` function and is responsible for rendering the visual elements of the
     * `HISSTools_VUMeter` control. It draws the meter's background, level indicators, peak markers, and any other
     * graphical elements that represent the current state of the VU meter. The rendering is performed using the provided
     * `IGraphics` object, ensuring that the meter is visually updated to reflect the audio signal levels and other related information.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g) override;

private:

    // N.B. currently we linearly interpolate dB but we could do something nicer here later.....

    /**
     * @brief Calculates the size of the VU meter's display based on the input value and whether the scale is linear or logarithmic.
     *
     * This method computes the size or position of the VU meter's visual representation (such as the length of the meter bar)
     * based on the input `value` and the scaling mode. If `linear` is true, the calculation is performed using a linear scale;
     * otherwise, it uses a logarithmic scale, which is often used for audio level representation. The result determines
     * how much of the VU meter is filled, reflecting the current audio level.
     *
     * @param value The input value representing the current level of the VU meter.
     * @param linear A boolean indicating whether to apply linear (`true`) or logarithmic (`false`) scaling to the calculation.
     * @return The calculated size or position for the VU meter's visual display.
     */
    
    double GetSize(double value, bool linear);

    /**
     * @brief Draws a horizontal tick mark in the VU meter control at the specified position.
     *
     * This method is responsible for drawing a horizontal tick mark on the VU meter, which may represent
     * divisions, markers, or indicators along the scale of the meter. It uses the provided drawing context (`vecDraw`)
     * to render the tick between the coordinates `x1` and `x2` along the x-axis, at the y-coordinate `y`.
     * The tick is drawn with a height `h` and thickness `thickness`, with `normPosition` determining the normalized position
     * along the meter where the tick should be placed.
     *
     * @param vecDraw A reference to the `HISSTools_VecLib` object used for rendering the tick.
     * @param x1 The starting x-coordinate of the tick mark.
     * @param x2 The ending x-coordinate of the tick mark.
     * @param y The y-coordinate at which the tick mark is placed.
     * @param h The height of the tick mark.
     * @param normPosition The normalized position (e.g., 0.0 to 1.0) along the VU meter where the tick is placed.
     * @param thickness The thickness of the tick mark.
     */
    
    void HorzTick(HISSTools_VecLib& vecDraw, double x1, double x2, double y, double h, double normPosition, double thickness);

    /**
     * @brief Draws a vertical tick mark in the VU meter control at the specified position.
     *
     * This method is responsible for drawing a vertical tick mark on the VU meter, which can represent divisions, markers,
     * or indicators along the scale of the meter. It uses the provided drawing context (`vecDraw`) to render the tick between
     * the coordinates `y1` and `y2` along the y-axis, at the x-coordinate `x`. The tick is drawn with a width `w` and thickness `thickness`,
     * with `normPosition` determining the normalized position along the meter where the tick should be placed.
     *
     * @param vecDraw A reference to the `HISSTools_VecLib` object used for rendering the tick.
     * @param y1 The starting y-coordinate of the tick mark.
     * @param y2 The ending y-coordinate of the tick mark.
     * @param x The x-coordinate at which the tick mark is placed.
     * @param w The width of the tick mark.
     * @param normPosition The normalized position (e.g., 0.0 to 1.0) along the VU meter where the tick is placed.
     * @param thickness The thickness of the tick mark.
     */
    
    void VertTick(HISSTools_VecLib& vecDraw, double y1, double y2, double x, double w, double normPosition, double thickness);

    // Positioning / Dimensions

    /**
     * @brief The x-coordinate position of the VU meter control.
     *
     * This member variable stores the x-coordinate (horizontal position) of the top-left corner of the `HISSTools_VUMeter` control.
     * It is used to position the VU meter within the user interface, determining where the control will be displayed relative to other UI elements.
     */
    
    double mX;
    
    /**
     * @brief The y-coordinate position of the VU meter control.
     *
     * This member variable stores the y-coordinate (vertical position) of the top-left corner of the `HISSTools_VUMeter` control.
     * It is used to position the VU meter within the user interface, determining where the control will be displayed relative to other UI elements.
     */
    
    double mY;
    
    /**
     * @brief The width of the VU meter control.
     *
     * This member variable stores the width of the `HISSTools_VUMeter` control, defining how wide the VU meter appears on the screen.
     * It affects the overall size of the meter and how much space it occupies in the user interface, influencing the graphical representation of the meter's levels.
     */
    
    double mW;
    
    /**
     * @brief The height of the VU meter control.
     *
     * This member variable stores the height of the `HISSTools_VUMeter` control, defining how tall the VU meter appears on the screen.
     * It affects the overall size of the meter and how much space it occupies in the user interface, influencing the graphical representation of the meter's levels.
     */
    
    double mH;
    
    /**
     * @brief The position of the first tick mark on the VU meter.
     *
     * This member variable stores the position of the first tick mark (`mTick1`) on the VU meter control.
     * It defines where the first visual marker or division is placed along the scale of the meter, which helps
     * indicate specific levels or thresholds. The value of `mTick1` can correspond to a normalized position or
     * a specific decibel value, depending on the design of the meter.
     */
    
    double mTick1;
    
    /**
     * @brief The position of the second tick mark on the VU meter.
     *
     * This member variable stores the position of the second tick mark (`mTick2`) on the VU meter control.
     * It defines where the second visual marker or division is placed along the scale of the meter, which helps
     * indicate specific levels or thresholds. The value of `mTick2` can correspond to a normalized position or
     * a specific decibel value, depending on the design of the meter.
     */
    
    double mTick2;
    
    /**
     * @brief The position of the third tick mark on the VU meter.
     *
     * This member variable stores the position of the third tick mark (`mTick3`) on the VU meter control.
     * It defines where the third visual marker or division is placed along the scale of the meter, which helps
     * indicate specific levels or thresholds. The value of `mTick3` can correspond to a normalized position or
     * a specific decibel value, depending on the design of the meter.
     */
    
    double mTick3;
    
    /**
     * @brief The position of the fourth tick mark on the VU meter.
     *
     * This member variable stores the position of the fourth tick mark (`mTick4`) on the VU meter control.
     * It defines where the fourth visual marker or division is placed along the scale of the meter, which helps
     * indicate specific levels or thresholds. The value of `mTick4` can correspond to a normalized position or
     * a specific decibel value, depending on the design of the meter.
     */
    
    double mTick4;

    // Line Thicknesses

    /**
     * @brief The thickness of the outline for the VU meter control.
     *
     * This member variable stores the value that defines the thickness of the outline (`mOutlineTK`) around the VU meter.
     * The outline thickness affects how prominently the border around the VU meter is drawn, giving the control a defined edge
     * and enhancing its visual clarity in the user interface.
     */
    
    double mOutlineTK;
    
    /**
     * @brief The thickness of the tick marks in the VU meter control.
     *
     * This member variable stores the value that defines the thickness of the tick marks (`mTickTK`) displayed on the VU meter.
     * The tick thickness affects how prominently the marks, which indicate levels or divisions on the meter, are rendered,
     * helping to make them more visible to the user as a guide for audio signal levels.
     */
    
    double mTickTK;
    
    /**
     * @brief The thickness of the peak hold indicator in the VU meter control.
     *
     * This member variable stores the value that defines the thickness of the peak hold indicator (`mPeakHoldTK`)
     * in the VU meter. The peak hold indicator represents the maximum level reached by the audio signal, and its thickness
     * affects how prominently this indicator is displayed. A thicker peak hold line may make it more noticeable for tracking
     * peak levels in the meter.
     */
    
    double mPeakHoldTK;

    // Shadow Spec

    /**
     * @brief Pointer to the `HISSTools_Shadow` object that defines the shadow effect for the VU meter control.
     *
     * This member variable holds a pointer to a `HISSTools_Shadow` object, which specifies the shadow properties applied
     * to the VU meter control. The shadow effect adds depth and visual contrast, making the meter's display more visually
     * appealing and helping it stand out within the user interface.
     */
    
    HISSTools_Shadow *mShadow;

    // Drawing parameters

    /**
     * @brief The size of the first VU meter's visual representation.
     *
     * This member variable stores the value that represents the current size or length of the visual display for the first VU meter (`mVU1`).
     * It reflects the level of the audio signal being monitored, with the size increasing or decreasing based on the VU meter's input signal.
     * This value is used to dynamically update the graphical representation of the first channel or signal being monitored by the VU meter.
     */
    
    double mVU1Size;
    
    /**
     * @brief The size of the second VU meter's visual representation.
     *
     * This member variable stores the value that represents the current size or length of the visual display for the second VU meter (`mVU2`).
     * It reflects the level of the audio signal being monitored for the second channel (in the case of stereo or multi-channel meters).
     * The size changes dynamically based on the input signal, allowing the VU meter to visually represent the signal level for the second channel.
     */
    
    double mVU2Size;
    
    /**
     * @brief The size of the side or boundary of the VU meter control.
     *
     * This member variable stores the value that defines the size or length of the side boundary of the VU meter control.
     * It is used to adjust or manage the dimensions of the VU meter, particularly for defining the area in which the meter is displayed,
     * and can influence how the meter is visually structured in the user interface.
     */
    
    double mSideSize;

    /**
     * @brief A flag indicating whether the peak level is active in the VU meter control.
     *
     * This member variable stores a boolean value that represents whether the peak indicator is active in the `HISSTools_VUMeter` control.
     * When `mPeak` is set to `true`, it means that the audio signal has reached or exceeded a peak threshold,
     * and this can trigger a visual indicator (such as a peak hold marker) in the VU meter display.
     * When `false`, no peak has been detected or the peak indicator is disabled.
     */
    
    bool mPeak;

    // Values

    /**
     * @brief The minimum decibel level displayed by the VU meter control.
     *
     * This member variable stores the minimum decibel value (`mMinDB`) that the VU meter can display.
     * It defines the lower limit of the meter's range, typically representing the quietest signal level the meter can track.
     * This value is used to scale the meter's display, ensuring that the visual representation of audio levels
     * aligns with the appropriate decibel range.
     */
    
    double mMinDB;
    
    /**
     * @brief The maximum decibel level displayed by the VU meter control.
     *
     * This member variable stores the maximum decibel value (`mMaxDB`) that the VU meter can display.
     * It defines the upper limit of the meter's range, typically representing the loudest signal level the meter can track.
     * This value is used to scale the meter's display, ensuring that the visual representation of audio levels
     * aligns with the appropriate decibel range.
     */
    
    double mMaxDB;

    // Color Specs

    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the background color of the VU meter control.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * for the background of the `HISSTools_VUMeter` control. The background color provides contrast to the meter's foreground elements,
     * such as the signal level indicators and peak markers, enhancing the visual clarity of the meter.
     */
    
    HISSTools_Color_Spec *mBackgroundCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the outline color of the VU meter control.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the outline
     * or border around the `HISSTools_VUMeter` control. The outline color helps visually define the edges of the meter,
     * making it stand out within the user interface and enhancing the overall appearance of the VU meter.
     */
    
    HISSTools_Color_Spec *mOutlineCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the first VU meter's signal level.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used to display
     * the signal level of the first VU meter (`mVU1`). This color represents the visual indicator for the audio signal
     * in the first channel, providing a clear and distinct representation of the signal's strength in the meter.
     */
    
    HISSTools_Color_Spec *mVU1CS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the second VU meter's signal level.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used to display
     * the signal level of the second VU meter (`mVU2`). This color represents the visual indicator for the audio signal
     * in the second channel, providing a clear and distinct representation of the signal's strength in the meter.
     */
    
    HISSTools_Color_Spec *mVU2CS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the side or boundary of the VU meter.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the side
     * or boundary area of the VU meter (`mVUSideCS`). This color is applied to the edges or background surrounding the main
     * signal display area, helping to visually separate the meter from other interface elements and enhancing the overall appearance.
     */
    
    HISSTools_Color_Spec *mVUSideCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the peak indicator for the first VU meter.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * to display the peak indicator for the first VU meter (`mVU1`). The peak indicator represents the maximum level
     * the audio signal has reached, and this color visually highlights the peak to ensure it stands out from the rest
     * of the VU meter's display.
     */
    
    HISSTools_Color_Spec *mVU1PeakCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the peak indicator for the second VU meter.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used
     * to display the peak indicator for the second VU meter (`mVU2`). The peak indicator represents the maximum level
     * the audio signal has reached, and this color visually highlights the peak to ensure it stands out from the rest
     * of the VU meter's display.
     */
    
    HISSTools_Color_Spec *mVU2PeakCS;
    
    /**
     * @brief Pointer to the `HISSTools_Color_Spec` object that defines the color of the side peak indicator for the VU meter.
     *
     * This member variable holds a pointer to a `HISSTools_Color_Spec` object, which specifies the color used for the side
     * peak indicator of the VU meter (`mVUSidePeakCS`). The side peak indicator is used to display peak levels on the sides
     * or boundaries of the meter, and this color helps to distinguish it from the main peak indicator or other elements,
     * providing additional visual feedback on signal peaks.
     */
    
    HISSTools_Color_Spec *mVUSidePeakCS;

    /**
     * @brief A flag indicating whether a fixed gradient overlay is applied to the VU meter.
     *
     * This member variable stores a boolean value that determines whether a fixed gradient overlay is applied
     * to the VU meter display. When `mOverlayFixedGradientBox` is set to `true`, a gradient effect is applied to the meter,
     * potentially enhancing the visual appearance by providing a smooth transition of colors across the meter.
     * When set to `false`, the gradient overlay is disabled, and the VU meter is displayed without this effect.
     */
    
    bool mOverlayFixedGradientBox;
};

// HISSTools_FileSelector
// 

/**
 * @class HISSTools_FileSelector
 * @brief A control class for implementing a file selection button in the HISSTools system.
 *
 * This class represents a file selector button, allowing users to open a dialog for selecting files.
 * It inherits from the `HISSTools_Button` class, leveraging the button functionality while adding the capability
 * to trigger a file selection dialog. The `HISSTools_FileSelector` class is used in situations where file input
 * is required, such as loading or saving files within the application.
 *
 * Inheritance:
 * - Publicly inherits from `HISSTools_Button` to manage button-related behaviors, such as rendering and interaction.
 */

class HISSTools_FileSelector : public HISSTools_Button
{
public:

    /**
     * @enum EFileSelectorState
     * @brief Enumeration representing the different states of the file selector.
     *
     * This enumeration defines the possible states for the `HISSTools_FileSelector` control. The states are:
     * - `kFSNone`: Indicates that no file selection process is active.
     * - `kFSSelecting`: Indicates that the file selector dialog is currently open, and the user is in the process of selecting a file.
     * - `kFSDone`: Indicates that the file selection process has been completed, and a file has been selected.
     */
    
    enum EFileSelectorState { kFSNone, kFSSelecting, kFSDone };

    // FIX - turn automation off (also for matrix)

    // Constructor

    /**
     * @brief Constructs a `HISSTools_FileSelector` object with the specified parameters for position, size, file action, directory, and extensions.
     *
     * This constructor initializes a `HISSTools_FileSelector` object, which is used for selecting files in the user interface.
     * It positions the file selector button at the specified coordinates (x, y) with defined width (`w`) and height (`h`).
     * The constructor also sets the file action (`action`), default directory (`dir`), and allowed file extensions (`extensions`).
     * Additionally, it inherits properties from the `HISSTools_Button` class, including the parameter index (`paramIdx`),
     * button type (`type`), design scheme (`designScheme`), and label (`label`).
     *
     * @param paramIdx The index of the parameter associated with this file selector.
     * @param x The x-coordinate of the top-left corner of the file selector button.
     * @param y The y-coordinate of the top-left corner of the file selector button.
     * @param w The width of the file selector button.
     * @param h The height of the file selector button.
     * @param action The file action type, defining whether the selector is for opening or saving files (`EFileAction`).
     * @param dir A string representing the default directory for file selection (default is an empty string).
     * @param extensions A string representing allowed file extensions (default is an empty string).
     * @param type A C-string representing the button type, which may influence its appearance or behavior (default is `nullptr`).
     * @param designScheme A pointer to the `HISSTools_Design_Scheme` object that defines the button's design elements (default is `&DefaultDesignScheme`).
     * @param label A C-string representing the label displayed on the button (default is an empty string).
     */
    
    HISSTools_FileSelector(int paramIdx, double x, double y, double w, double h, EFileAction action, char* dir = "", char* extensions = "", const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme, const char *label = "")
        : HISSTools_Button(paramIdx, x, y, w, h, type, designScheme, label), mState(kFSNone), mFileAction(action), mDir(dir), mExtensions(extensions)
    {}

    // Mousing Functions

    /**
     * @brief Handles the mouse down event for the `HISSTools_FileSelector` control.
     *
     * This method overrides the base `OnMouseDown` function and is called when the user presses the mouse button
     * while interacting with the `HISSTools_FileSelector` control. It captures the x and y coordinates of the mouse click,
     * along with any active mouse modifiers (e.g., control, shift). When triggered, this method initiates the file selection process,
     * opening a file dialog where the user can select a file based on the predefined directory and file extension filters.
     *
     * @param x The x-coordinate where the mouse button was pressed.
     * @param y The y-coordinate where the mouse button was pressed.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the event.
     */
    
    void OnMouseDown(float x, float y, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse drag event for the `HISSTools_FileSelector` control.
     *
     * This method overrides the base `OnMouseDrag` function and is called when the user drags the mouse while interacting
     * with the `HISSTools_FileSelector` control. It captures the current x and y coordinates of the mouse, as well as the change
     * in position (dX, dY) during the drag, and any active mouse modifiers (e.g., control, shift). Typically, in a file selector,
     * this method might be used for additional drag-related interactions, but by default, it may not have a significant impact
     * on the file selection process.
     *
     * @param x The current x-coordinate of the mouse during the drag.
     * @param y The current y-coordinate of the mouse during the drag.
     * @param dX The change in the x-coordinate (delta) during the drag.
     * @param dY The change in the y-coordinate (delta) during the drag.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the event.
     */
    
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& pMod) override;
    
    /**
     * @brief Handles the mouse up event for the `HISSTools_FileSelector` control.
     *
     * This method overrides the base `OnMouseUp` function and is called when the user releases the mouse button
     * while interacting with the `HISSTools_FileSelector` control. It captures the x and y coordinates of the mouse release,
     * along with any active mouse modifiers (e.g., control, shift). In the context of a file selector, this method might be used
     * to finalize actions initiated during a mouse down event, such as confirming file selection or resetting the state after the user
     * has clicked and released the button.
     *
     * @param x The x-coordinate where the mouse button was released.
     * @param y The y-coordinate where the mouse button was released.
     * @param pMod A reference to an `IMouseMod` object that contains information about mouse modifiers (e.g., control, shift)
     *             and the state of the mouse buttons during the event.
     */
    
    void OnMouseUp(float x, float y, const IMouseMod& pMod) override;

    // Draw

    /**
     * @brief Renders the `HISSTools_FileSelector` control using the specified graphics context.
     *
     * This method overrides the base `Draw` function and is responsible for rendering the visual elements of the
     * `HISSTools_FileSelector` control. It draws the button, label, and any visual indicators that represent the current state
     * of the file selector, such as whether it is idle, in the process of file selection, or has completed file selection.
     * The rendering is performed using the provided `IGraphics` object, ensuring that the file selector is visually updated
     * within the user interface.
     *
     * @param g A reference to the `IGraphics` object used for rendering the control.
     */
    
    void Draw(IGraphics& g) override;

    // File Functions

    /**
     * @brief Retrieves the last file selected by the file selector for use within the plugin.
     *
     * This method returns a reference to a `WDL_String` object containing the path or name of the last file
     * that was selected using the `HISSTools_FileSelector` control. It allows the plugin to access the selected file
     * for processing, loading, or any other necessary operation. The returned string remains valid until a new file
     * is selected.
     *
     * @return A constant reference to a `WDL_String` containing the path or name of the last selected file.
     */
    
    const WDL_String& GetLastSelectedFileForPlug();
    
    /**
     * @brief Sets the last selected file in the file selector from the plugin.
     *
     * This method allows the plugin to set the last selected file in the `HISSTools_FileSelector` control.
     * The file path or name is passed as a string (`file`), which updates the file selector's internal state.
     * This can be used to prepopulate or restore the last selected file when the plugin initializes or needs to retain the previous selection.
     *
     * @param file A C-string representing the path or name of the file to set as the last selected file.
     */
    
    void SetLastSelectedFileFromPlug(const char* file);
    
    /**
     * @brief Sets the allowed file extensions for the file selector.
     *
     * This method updates the file extensions that the `HISSTools_FileSelector` control will accept during file selection.
     * The file extensions are passed as a string (`extensions`), where each extension is typically separated by a delimiter
     * (e.g., ".txt;.wav"). This limits the types of files the user can select, ensuring that only valid file types are chosen.
     *
     * @param extensions A C-string representing the allowed file extensions (e.g., ".txt;.wav").
     */
    
    void SetExtensions(char *extensions);

private:

    /**
     * @brief A virtual method to report the file selection or status back to the plugin.
     *
     * This method is intended to be overridden in derived classes to report information from the `HISSTools_FileSelector`
     * control back to the plugin. It allows the plugin to be informed of any changes or updates, such as a new file being selected.
     * In this base implementation, the method does nothing (`{}`), and should be implemented with specific behavior
     * in subclasses when needed.
     */
    
    virtual void ReportToPlug() {}

    // Strings

    /**
     * @brief Stores the directory path used by the file selector.
     *
     * This member variable holds a `WDL_String` that represents the directory path where the `HISSTools_FileSelector`
     * starts when prompting the user to select a file. It defines the default directory that is displayed in the file
     * selection dialog, and can be updated or accessed to control the file selector's behavior.
     */
    
    WDL_String mDir;
    
    /**
     * @brief Stores the file path or name selected by the file selector.
     *
     * This member variable holds a `WDL_String` that represents the file path or name of the file currently selected
     * by the `HISSTools_FileSelector` control. It is updated whenever the user selects a new file, and can be accessed
     * to retrieve the selected file for further processing by the plugin.
     */
    
    WDL_String mFile;
    
    /**
     * @brief Stores the allowed file extensions for the file selector.
     *
     * This member variable holds a `WDL_String` that represents the file extensions allowed by the `HISSTools_FileSelector` control.
     * The extensions define which types of files the user can select, typically formatted as a string with extensions
     * separated by semicolons (e.g., ".txt;.wav"). It ensures that only files matching the specified extensions are shown
     * or selectable in the file dialog.
     */
    
    WDL_String mExtensions;

    // States

    /**
     * @brief Stores the type of file action (e.g., open or save) for the file selector.
     *
     * This member variable holds a value of the `EFileAction` enum, which specifies the action type that the
     * `HISSTools_FileSelector` control will perform. The file action can be either to open a file or to save a file,
     * and it determines how the file dialog behaves when presented to the user.
     *
     * Example values for `EFileAction`:
     * - `kFileActionOpen`: The file selector is used to open an existing file.
     * - `kFileActionSave`: The file selector is used to save a file.
     */
    
    EFileAction mFileAction;
    
    /**
     * @brief Stores the current state of the file selector.
     *
     * This member variable holds a value of the `EFileSelectorState` enum, which represents the current state
     * of the `HISSTools_FileSelector` control. The state indicates whether the file selector is idle, actively selecting a file,
     * or has completed the file selection process.
     *
     * Possible values for `EFileSelectorState`:
     * - `kFSNone`: The file selector is idle, and no file selection is in progress.
     * - `kFSSelecting`: The file selector dialog is open, and the user is currently selecting a file.
     * - `kFSDone`: The file selection process has been completed, and a file has been selected.
     */
    
    EFileSelectorState mState;
};

#ifndef NO_HISSTOOLS_CONTROL_HELPERS_COMPILE
#include "HISSTools_Controls.cpp"
#endif

#endif /* __HISSTOOLS_CONTROLS__ */
