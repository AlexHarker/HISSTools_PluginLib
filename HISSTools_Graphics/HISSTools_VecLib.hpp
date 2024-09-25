
/**
 * @file HISSTools_VecLib.hpp
 * @brief Provides vectorized mathematical and graphical utilities for high-performance rendering and processing.
 *
 * This file contains the declaration of various classes and methods related to vector-based mathematical
 * operations and graphical rendering in the HISSTools library. It includes functionality for drawing
 * shapes, managing color gradients, handling shadows, and optimizing graphical performance using vectorization
 * techniques. These utilities are designed to support high-performance graphical applications, such as audio-visual
 * tools or real-time rendering engines.
 *
 * Main components include:
 * - **HISSTools_VecLib class**: Provides methods for drawing and rendering shapes (e.g., rectangles, circles, arcs)
 *   with various visual effects such as gradients and shadows.
 * - **Gradient and Color management**: Supports dynamic gradient creation, color handling, and orientation
 *   settings for custom graphical elements.
 * - **Multi-line shapes**: Includes tools for drawing complex multi-line shapes with adjustable thickness.
 * - **Shadows and effects**: Allows for the configuration and rendering of shadows to add depth and visual
 *   emphasis to graphical elements.
 *
 * This file is part of the HISSTools library and is intended to be used for graphical interface development
 * and high-performance visual rendering tasks.
 *
 * @note The file heavily relies on external libraries for rendering, such as iPlug's `IGraphics` framework,
 * and includes several utility classes for handling bounds, colors, and shadows.
 */

#pragma once

#include "IGraphics.h"
#include "HISSTools_VecLib_Structs.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

/**
 * @brief The default color specification for the HISSTools_VecLib class.
 *
 * This static member holds a default color specification that can be used
 * throughout the HISSTools_VecLib class or associated functions. The color
 * specification defines the default color attributes (such as RGB values)
 * for rendering or visual representation purposes, depending on how the class
 * utilizes color information.
 *
 * @note This is a static member, meaning it is shared across all instances of
 * the class. Modifying this value will affect all instances that refer to it.
 */

static HISSTools_Color_Spec defaultColor;

/**
 * @brief A class providing vectorized mathematical operations.
 *
 * The HISSTools_VecLib class contains a set of optimized functions
 * for performing various mathematical operations on vectors. It is
 * designed to leverage SIMD (Single Instruction, Multiple Data)
 * techniques to process multiple data points in parallel, improving
 * the performance of numerical computations.
 *
 * This class is typically used in high-performance scenarios such as
 * audio processing or other real-time applications requiring fast
 * vector-based calculations.
 *
 * @note This class is part of the HISSTools library and is optimized
 * for specific hardware capabilities.
 */

class HISSTools_VecLib
{
    
    /**
     * @brief Alias for the IGraphics class from the iPlug framework.
     *
     * This `using` declaration creates an alias for the `IGraphics` class
     * within the `iplug::igraphics` namespace. The `IGraphics` class is
     * typically responsible for handling graphical rendering, including drawing
     * user interfaces, managing graphical resources, and interacting with
     * display systems in the iPlug framework.
     *
     * By using this alias, the code can reference `IGraphics` directly instead
     * of the fully qualified `iplug::igraphics::IGraphics` name, improving code
     * readability and reducing verbosity.
     */
    
    using IGraphics = iplug::igraphics::IGraphics;
    
    /**
     * @brief Alias for the IRECT class from the iPlug framework.
     *
     * This `using` declaration creates an alias for the `IRECT` class
     * within the `iplug::igraphics` namespace. The `IRECT` class typically
     * represents a rectangular region, often used for defining boundaries
     * or areas in graphical interfaces, such as defining the dimensions and
     * position of UI elements in the iPlug framework.
     *
     * By using this alias, the code can reference `IRECT` directly instead of
     * the fully qualified `iplug::igraphics::IRECT`, enhancing code clarity
     * and reducing verbosity.
     */
    
    using IRECT = iplug::igraphics::IRECT;
    
    /**
     * @brief Alias for the IText class from the iPlug framework.
     *
     * This `using` declaration creates an alias for the `IText` class
     * within the `iplug::igraphics` namespace. The `IText` class is typically
     * used for handling text properties such as font, size, color, alignment,
     * and other text-related attributes in graphical user interfaces (GUIs).
     *
     * By using this alias, the code can refer to `IText` directly instead of
     * the fully qualified `iplug::igraphics::IText` name, improving readability
     * and reducing the length of type declarations.
     */
    
    using IText = iplug::igraphics::IText;
    
    /**
     * @brief Alias for the IShadow class from the iPlug framework.
     *
     * This `using` declaration creates an alias for the `IShadow` class
     * within the `iplug::igraphics` namespace. The `IShadow` class is generally
     * used for managing shadow properties in graphical elements, such as defining
     * the shadow's offset, blur, opacity, and color for rendering shadows behind
     * UI components in the iPlug framework.
     *
     * By using this alias, the code can refer to `IShadow` directly instead of
     * the fully qualified `iplug::igraphics::IShadow`, improving code readability
     * and reducing verbosity.
     */
    
    using IShadow = iplug::igraphics::IShadow;
    
    /**
     * @brief Alias for a pointer to an ILayer object from the iPlug framework.
     *
     * This `using` declaration creates an alias for the `ILayerPtr` type,
     * which is typically a pointer to an `ILayer` object within the
     * `iplug::igraphics` namespace. The `ILayer` class generally represents
     * a graphical layer, which can be used for organizing and managing
     * different elements of the user interface, such as background images
     * or overlays.
     *
     * By using this alias, the code can reference `ILayerPtr` directly
     * instead of the fully qualified `iplug::igraphics::ILayerPtr` name,
     * improving code readability and reducing verbosity.
     */
    
    using ILayerPtr = iplug::igraphics::ILayerPtr;
    
    /**
      * @brief Alias for the EAlign enumeration from the iPlug framework.
      *
      * This `using` declaration creates an alias for the `EAlign` enumeration
      * within the `iplug::igraphics` namespace. The `EAlign` enum is typically
      * used for defining alignment options in graphical user interfaces (GUIs),
      * such as horizontal and vertical alignment of text or other visual elements
      * (e.g., left, right, center alignment).
      *
      * By using this alias, the code can reference `EAlign` directly instead of
      * the fully qualified `iplug::igraphics::EAlign` name, making the code
      * cleaner and easier to read.
      */
    
    using EAlign = iplug::igraphics::EAlign;
    
    /**
     * @brief Alias for the EVAlign enumeration from the iPlug framework.
     *
     * This `using` declaration creates an alias for the `EVAlign` enumeration
     * within the `iplug::igraphics` namespace. The `EVAlign` enum is typically
     * used for defining vertical alignment options in graphical user interfaces
     * (GUIs), such as aligning visual elements or text at the top, middle,
     * or bottom of a given area.
     *
     * By using this alias, the code can reference `EVAlign` directly instead of
     * the fully qualified `iplug::igraphics::EVAlign` name, improving readability
     * and reducing code verbosity.
     */
    
    using EVAlign = iplug::igraphics::EVAlign;
    
    /**
     * @brief Constant for the mathematical value of Pi.
     *
     * This declaration creates a constant `PI`, initialized to the value of
     * `iplug::PI`, which represents the mathematical constant Pi (approximately
     * 3.14159). Pi is typically used in mathematical calculations involving
     * circles, trigonometry, and other geometric operations.
     *
     * By using this constant, the code can conveniently refer to Pi without
     * manually specifying the value, leveraging the value defined in the
     * iPlug framework.
     */
    
    const double PI = iplug::PI;
    
public:
    
    /**
     * @brief Constructs a HISSTools_VecLib object with a reference to the IGraphics instance.
     *
     * This constructor initializes a HISSTools_VecLib object, linking it to the
     * provided `IGraphics` instance, which is responsible for managing graphical
     * rendering. It also initializes several member variables:
     *
     * - `mGraphics`: A reference to the `IGraphics` instance used for drawing and
     *   rendering operations.
     * - `mShadow`: Initialized to `nullptr`, this is likely used for shadow rendering
     *   and can be assigned later.
     * - `mForceGradientBox`: A boolean flag initialized to `false`, indicating
     *   whether gradient boxes should be forced (likely used for specific rendering effects).
     * - `mColor`: Points to `defaultColor`, which defines the default color scheme
     *   for graphical elements.
     * - `mCSOrientation`: Initialized to `kCSOrientHorizontal`, this likely controls
     *   the orientation (horizontal or vertical) of certain graphical elements or
     *   layouts.
     *
     * @param graphics A reference to the `IGraphics` object that manages rendering.
     */
    
    HISSTools_VecLib(IGraphics& graphics) : mGraphics(graphics), mShadow(nullptr), mForceGradientBox(false), mColor(&defaultColor), mCSOrientation(kCSOrientHorizontal)
    {}
    
    /**
     * @brief Sets the clip mode for the current operation.
     *
     * This method configures the object to use a clipping mode,
     * which limits or constrains values during processing.
     * The exact behavior and bounds of the clipping can be
     * implementation-specific and should be documented in further detail.
     *
     * @note This method does not take any parameters and returns no value.
     */
    
    void SetClip()
    {
        mGraphics.PathClipRegion();
    }
    
    /**
     * @brief Sets the clipping bounds for rendering operations.
     *
     * This method configures a clipping region for graphical operations by
     * setting the boundaries within which rendering is allowed. The clipping
     * ensures that any drawing or rendering outside of these bounds is not
     * displayed, effectively limiting the drawing area.
     *
     * @param clip An instance of `HISSTools_Bounds` that defines the rectangular
     * clipping region. This typically includes coordinates and dimensions (e.g.,
     * width and height) to specify the region where rendering is allowed.
     *
     * @note Setting a clip region can improve rendering performance and enforce
     * layout constraints within specific areas of the UI.
     */
    
    void SetClip(HISSTools_Bounds clip)
    {
        mGraphics.PathClipRegion(clip);
    }
    
    /**
     * @brief Sets the clipping region using specific coordinate values.
     *
     * This method defines a rectangular clipping region for graphical operations
     * by specifying the lower and upper bounds for both the x and y axes.
     * Any rendering outside of this defined region will be clipped (i.e., not displayed).
     *
     * @param xLo The lower bound on the x-axis (left side of the clip region).
     * @param yLo The lower bound on the y-axis (bottom side of the clip region).
     * @param xHi The upper bound on the x-axis (right side of the clip region).
     * @param yHi The upper bound on the y-axis (top side of the clip region).
     *
     * @note This method is useful when manually defining the clip region with
     * precise coordinates, enabling fine control over the rendering area.
     */
    
    void SetClip(double xLo, double yLo, double xHi, double yHi)
    {
        SetClip(HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo));
    }
    
    /**
     * @brief Sets the clipping region using an IRECT object.
     *
     * This method defines a rectangular clipping area for graphical rendering
     * by using an `IRECT` object. The clipping region restricts drawing to the
     * bounds specified by the `IRECT`, ensuring that any rendering outside of
     * this region will be clipped (i.e., not shown).
     *
     * @param rect An `IRECT` object that defines the clipping boundaries,
     * including the position and size of the rectangular region.
     *
     * @note This method is useful for setting the clipping region based on
     * existing rectangles, often used in UI layout calculations or other
     * graphical operations that need to limit the drawing area.
     */
    
    void SetClip(const IRECT& rect)
    {
        SetClip(rect.L, rect.T, rect.R, rect.B);
    }
    
    /**
     * @brief Begins a group of graphical operations within a defined rectangular area.
     *
     * This method starts a grouping of graphical operations that will be confined
     * to the region defined by the provided `IRECT` object. Grouping operations
     * can allow for batch processing, layering, or deferred rendering of graphical
     * elements within the specified area.
     *
     * @param r An `IRECT` object defining the rectangular region within which
     * the group of operations will occur. This region may serve as a boundary
     * for clipping or for limiting the effects of the operations in the group.
     *
     * @note Grouping operations can improve rendering efficiency or control
     * over complex UI elements that require multiple rendering steps within a specific region.
     */
    
    void StartGroup(const IRECT& r)
    {
        mGraphics.StartLayer(nullptr, r);
    }
    
    /**
     * @brief Ends the current group of graphical operations and returns a pointer to the created layer.
     *
     * This method finalizes the grouping of graphical operations that was initiated by `StartGroup()`.
     * Once the group is ended, the result of the grouped operations is encapsulated in an `ILayer` object,
     * and a pointer to this layer is returned. The layer can be used for further manipulation, such as
     * rendering, caching, or applying transformations independently from other graphical elements.
     *
     * @return An `ILayerPtr`, which is a pointer to the `ILayer` object containing the results of the
     * grouped graphical operations. This layer can be reused or modified as needed.
     *
     * @note Grouping operations and working with layers can improve rendering performance, especially
     * when dealing with complex or repetitive UI elements.
     */
    
    ILayerPtr EndGroup()
    {
        return mGraphics.EndLayer();
    }
    
    /**
     * @brief Renders a previously created layer of graphical operations.
     *
     * This method renders the content stored in the provided `ILayerPtr`,
     * which represents a layer of grouped graphical operations. The layer is
     * typically created using `StartGroup()` and `EndGroup()`, and this method
     * is used to draw or display the contents of that layer on the screen or
     * within the current graphical context.
     *
     * @param layer A pointer to an `ILayer` object that contains the graphical
     * operations to be rendered. This layer can have been cached or transformed
     * prior to rendering.
     *
     * @note Rendering from a layer allows for reusing previously computed
     * graphical content, which can improve performance by avoiding redundant
     * drawing operations.
     */
    
    void RenderGroup(const ILayerPtr& layer)
    {
        mGraphics.DrawLayer(layer);
    }
    
    /**
     * @brief Checks the validity or state of a previously created layer.
     *
     * This method verifies the state or validity of the provided `ILayerPtr` object,
     * which represents a layer of graphical operations. The check might involve
     * determining if the layer is properly initialized, contains valid content,
     * or is ready for rendering.
     *
     * @param layer A pointer to an `ILayer` object that represents the layer
     * to be checked.
     *
     * @return A boolean value indicating the result of the check.
     * - `true` if the layer is valid and can be rendered or used.
     * - `false` if the layer is invalid or not in a usable state.
     *
     * @note This method can be useful for ensuring that layers are properly
     * managed before performing operations such as rendering or transforming them.
     */
    
    bool CheckGroup(const ILayerPtr& layer)
    {
        return mGraphics.CheckLayer(layer);
    }
    
    /**
     * @brief Sets the color specification for graphical operations.
     *
     * This method sets the current color used for rendering operations by
     * assigning a new `HISSTools_Color_Spec` object. The color specification
     * includes attributes such as RGB values, alpha transparency, and other
     * color-related properties that will be applied to future graphical operations.
     *
     * @param color A pointer to a `HISSTools_Color_Spec` object that defines
     * the color attributes to be used. If `nullptr` is passed, default color
     * values may be used depending on the implementation.
     *
     * @note Changing the color specification affects subsequent drawing
     * operations and can be used to customize the appearance of rendered elements.
     */
    
    void SetColor(HISSTools_Color_Spec *color)
    {
        mColor = color;
    }
    
    // Orientation allows gradient rotation ONLY for relevant Color Specs
    
    /**
     * @brief Sets the orientation for color transitions or gradients.
     *
     * This method sets the orientation for how colors are applied, such as
     * in gradients or color transitions. The `ColorOrientation` parameter
     * specifies whether the color orientation should be horizontal, vertical,
     * or another predefined direction.
     *
     * @param CSOrientation An enumeration value of type `ColorOrientation`
     * that defines the direction in which the color changes or transitions
     * should occur (e.g., horizontal or vertical).
     *
     * @note This method is particularly useful when rendering gradients or
     * applying directional color effects to graphical elements.
     */
    
    void SetColorOrientation(ColorOrientation CSOrientation)
    {
        mCSOrientation = CSOrientation;
    }
    
    /**
     * @brief Disables the forced gradient box rendering.
     *
     * This method disables the use of a forced gradient box by setting
     * `mForceGradientBox` to `false`. A gradient box might be used to apply
     * a gradient effect to a rectangular area, and this method ensures
     * that this effect is no longer applied.
     *
     * @note After calling this method, no gradient box will be forced
     * during rendering operations. This might be useful when switching
     * between different rendering modes or visual effects.
     */
    
    void ForceGradientBox()     { mForceGradientBox = false; }
    
    /**
     * @brief Forces the use of a gradient box with specified boundaries.
     *
     * This method forces the rendering of a gradient box within the defined
     * rectangular area, specified by the lower and upper bounds on both
     * the x and y axes. The gradient box applies a gradient effect within
     * this region, likely affecting the visual appearance of graphical
     * elements within these coordinates.
     *
     * @param xLo The lower bound on the x-axis (left side of the gradient box).
     * @param yLo The lower bound on the y-axis (bottom side of the gradient box).
     * @param xHi The upper bound on the x-axis (right side of the gradient box).
     * @param yHi The upper bound on the y-axis (top side of the gradient box).
     *
     * @note This method ensures that a gradient box will be rendered within
     * the specified coordinates, and can be used to apply directional color
     * transitions or shading effects in the defined area.
     */
    
    void ForceGradientBox(double xLo, double yLo, double xHi, double yHi)
    {
        mGradientArea = HISSTools_Bounds(xLo, yLo, xHi - xLo, yHi - yLo);
        mForceGradientBox = true;
    }
    
    /**
     * @brief Begins the process of drawing a multi-line shape with specified starting point and thickness.
     *
     * This method initiates the drawing of a multi-line shape, starting at the specified
     * coordinates `(x, y)` and using the given line thickness. It prepares the rendering context
     * for a sequence of lines that will be drawn subsequently, allowing for the creation of complex
     * shapes or paths.
     *
     * @param x The x-coordinate of the starting point for the multi-line shape.
     * @param y The y-coordinate of the starting point for the multi-line shape.
     * @param thickness The thickness of the lines to be drawn in the multi-line shape.
     *
     * @note This method should be followed by additional drawing operations to complete
     * the multi-line shape. It is typically used in graphical applications that require
     * drawing paths or complex shapes made up of multiple lines.
     */
    
    void StartMultiLine(double x, double y, double thickness)
    {
        mMultiLineThickness = thickness;
        mGraphics.PathMoveTo(x, y);
    }
    
    /**
     * @brief Continues drawing a multi-line shape to the specified coordinates.
     *
     * This method extends the current multi-line shape by drawing a line from
     * the last point to the new point specified by the coordinates `(x, y)`.
     * It is typically called after `StartMultiLine()` to create additional
     * segments of the multi-line shape.
     *
     * @param x The x-coordinate of the next point in the multi-line shape.
     * @param y The y-coordinate of the next point in the multi-line shape.
     *
     * @note This method allows for the creation of complex shapes or paths
     * by continuing to add line segments. The line thickness and other
     * rendering properties should remain consistent with the initial setup
     * from `StartMultiLine()`.
     */
    
    void ContinueMultiLine(double x, double y)
    {
        mGraphics.PathLineTo(x, y);
    }
    
    /**
     * @brief Completes the drawing of the current multi-line shape.
     *
     * This method finalizes the multi-line shape that was initiated with `StartMultiLine()`
     * and extended using `ContinueMultiLine()`. After calling this method, the shape is
     * considered complete, and no further line segments will be added.
     *
     * @note This method should be called once all segments of the multi-line shape have
     * been drawn. It ensures that the drawing operation is properly concluded, allowing
     * for any necessary final rendering operations to take place.
     */
    
    void FinishMultiLine()
    {
        Stroke(mMultiLineThickness);
    }
    
    /**
     * @brief Calculates the intersection point on the circumference of a circle given an angle.
     *
     * This method computes the intersection point on the perimeter of a circle
     * defined by its center `(cx, cy)` and radius `r` at a specified angle `ang`.
     * The result is returned through the pointers `retX` and `retY`, which will
     * hold the x and y coordinates of the intersection point on the circle's edge.
     *
     * @param cx The x-coordinate of the circle's center.
     * @param cy The y-coordinate of the circle's center.
     * @param ang The angle (in radians or degrees, depending on the implementation)
     *            at which the intersection point should be calculated.
     * @param r The radius of the circle.
     * @param retX A pointer to a `double` where the x-coordinate of the intersection
     *             point will be stored.
     * @param retY A pointer to a `double` where the y-coordinate of the intersection
     *             point will be stored.
     *
     * @note This method is useful for graphical or geometrical applications that
     * require determining points on a circle's circumference based on a given angle,
     * such as in circular layouts or animations.
     */
    
    void CircleIntersection(double cx, double cy, double ang, double r, double *retX, double *retY)
    {
        *retX = cos(2.0 * ang * PI) * r + cx;
        *retY = sin(2.0 * ang * PI) * r + cy;
    }
    
    /**
     * @brief Draws an arc outline with specified thickness.
     *
     * This method draws an arc, defined by its center `(cx, cy)`, radius `r`,
     * starting angle `begAng`, and angular span `arcAng`. The arc is outlined
     * with the given `thickness`, creating a frame-like appearance.
     *
     * @param cx The x-coordinate of the arc's center.
     * @param cy The y-coordinate of the arc's center.
     * @param r The radius of the arc.
     * @param begAng The starting angle of the arc (in radians or degrees, depending on implementation).
     * @param arcAng The angular span of the arc (in radians or degrees, depending on implementation).
     * @param thickness The thickness of the arc's frame or outline.
     *
     * @note This method is useful for drawing partial circles or curved lines with a
     * specified outline thickness, such as in gauges, progress indicators, or decorative UI elements.
     */
    
    void FrameArc(double cx, double cy, double r, double begAng, double arcAng, double thickness)
    {
        Arc(cx, cy, r, begAng, arcAng);
        Stroke(thickness);
    }
    
    /**
     * @brief Fills an arc shape with the specified parameters.
     *
     * This method draws and fills an arc, defined by its center `(cx, cy)`, radius `r`,
     * starting angle `begAng`, and angular span `arcAng`. The arc is rendered as a
     * solid shape, with no outline, covering the area between the specified angles.
     *
     * @param cx The x-coordinate of the arc's center.
     * @param cy The y-coordinate of the arc's center.
     * @param r The radius of the arc.
     * @param begAng The starting angle of the arc (in radians or degrees, depending on implementation).
     * @param arcAng The angular span of the arc (in radians or degrees, depending on implementation).
     *
     * @note This method is useful for drawing filled arcs or segments of circles,
     * often used in visual elements like pie charts, progress indicators, or
     * decorative elements in user interfaces.
     */
    
    void FillArc(double cx, double cy, double r, double begAng, double arcAng)
    {
        mGraphics.PathClear();
        Arc(cx, cy, r, begAng, arcAng);
        mGraphics.PathLineTo(cx, cy);
        mGraphics.PathClose();
        Fill();
    }
    
    /**
     * @brief Fills a circle with the specified center and radius.
     *
     * This method draws and fills a circle, using the provided center coordinates `(cx, cy)`
     * and radius `r`. The entire area within the circle's boundary is filled with a solid color
     * or pattern, depending on the current rendering settings.
     *
     * @param cx The x-coordinate of the circle's center.
     * @param cy The y-coordinate of the circle's center.
     * @param r The radius of the circle.
     *
     * @note This method is typically used to render solid circular shapes, which can be used
     * for various graphical elements such as buttons, markers, or background decorations.
     */
    
    void FillCircle(double cx, double cy, double r)
    {
        mGraphics.PathCircle(cx, cy, r);
        SetShapeGradient(HISSTools_Bounds(cx - r, cy - r, 2.0 * r, 2.0 * r));
        Fill();
    }
    
    /**
     * @brief Draws an outlined circle with a specified thickness.
     *
     * This method renders a circular outline with the given center coordinates `(cx, cy)`,
     * radius `r`, and outline thickness. The circle is not filled, and only the border
     * is drawn with the specified thickness.
     *
     * @param cx The x-coordinate of the circle's center.
     * @param cy The y-coordinate of the circle's center.
     * @param r The radius of the circle.
     * @param thickness The thickness of the circle's outline.
     *
     * @note This method is useful for creating hollow circular shapes, which can be
     * used as frames, borders, or other decorative elements in graphical applications.
     */
    
    void FrameCircle(double cx, double cy, double r, double thickness)
    {
        FrameArc(cx, cy, r, 0.0, 1.0, thickness);
    }
    
    /**
     * @brief Draws an outlined triangle with specified vertex coordinates and thickness.
     *
     * This method renders a triangle by drawing its three sides, defined by the vertex
     * coordinates `(x1, y1)`, `(x2, y2)`, and `(x3, y3)`. The triangle is outlined
     * with the given thickness, leaving the interior of the triangle unfilled.
     *
     * @param x1 The x-coordinate of the first vertex.
     * @param y1 The y-coordinate of the first vertex.
     * @param x2 The x-coordinate of the second vertex.
     * @param y2 The y-coordinate of the second vertex.
     * @param x3 The x-coordinate of the third vertex.
     * @param y3 The y-coordinate of the third vertex.
     * @param thickness The thickness of the triangle's outline.
     *
     * @note This method is useful for creating outlined triangular shapes,
     * which can be used for decorative elements, markers, or frames in graphical applications.
     */
    
    void FrameTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double thickness)
    {
        Triangle(x1, y1, x2, y2, x3, y3);
        Stroke(thickness);
    }
    
    /**
     * @brief Draws and fills a triangle with specified vertex coordinates.
     *
     * This method renders a solid triangle defined by its three vertices
     * located at `(x1, y1)`, `(x2, y2)`, and `(x3, y3)`. The entire area
     * within the triangle is filled with a solid color or pattern, depending
     * on the current rendering settings.
     *
     * @param x1 The x-coordinate of the first vertex.
     * @param y1 The y-coordinate of the first vertex.
     * @param x2 The x-coordinate of the second vertex.
     * @param y2 The y-coordinate of the second vertex.
     * @param x3 The x-coordinate of the third vertex.
     * @param y3 The y-coordinate of the third vertex.
     *
     * @note This method is typically used to render filled triangular shapes,
     * which can be used for graphical elements such as buttons, icons, or
     * decorative UI elements.
     */
    
    void FillTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        Triangle(x1, y1, x2, y2, x3, y3);
        Fill();
    }
    
    /**
     * @brief Draws and fills a rectangle with specified position and dimensions.
     *
     * This method renders a filled rectangle using the top-left corner coordinates `(x, y)`
     * and the specified width `w` and height `h`. The entire area of the rectangle
     * is filled with a solid color or pattern, based on the current rendering settings.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     *
     * @note This method is commonly used for rendering solid rectangular shapes in graphical
     * applications, such as backgrounds, buttons, or containers.
     */
    
    void FillRect(double x, double y, double w, double h)
    {
        Rectangle(x, y, w, h);
        Fill();
    }
    
    /**
     * @brief Draws an outlined rectangle with specified dimensions and thickness.
     *
     * This method renders a rectangle outline (frame) using the top-left corner coordinates `(x, y)`,
     * and the specified width `w`, height `h`, and outline thickness `thickness`. The interior of the rectangle
     * is left unfilled, and only the border is drawn with the specified thickness.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param thickness The thickness of the rectangle's outline.
     *
     * @note This method is useful for creating framed or bordered rectangular shapes,
     * which can be used for graphical elements such as frames, borders, or containers
     * in user interfaces.
     */
    
    void FrameRect(double x, double y, double w, double h, double thickness)
    {
        Rectangle(x, y, w, h);
        Stroke(thickness);
    }
    
    /**
     * @brief Draws and fills a rounded rectangle with specified corner radii.
     *
     * This method renders a filled rectangle with rounded corners using the top-left corner
     * coordinates `(x, y)`, width `w`, height `h`, and the specified radii for each corner.
     * The rectangle is filled with a solid color or pattern, based on the current rendering settings.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param rtl The radius of the top-left corner.
     * @param rtr The radius of the top-right corner.
     * @param rbl The radius of the bottom-left corner.
     * @param rbr The radius of the bottom-right corner.
     *
     * @note This method is useful for rendering rounded rectangular shapes,
     * which are commonly used in modern user interfaces for buttons, panels, or containers.
     */
    
    void FillRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        RoundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        Fill();
    }
    
    /**
     * @brief Draws an outlined rounded rectangle with specified corner radii and outline thickness.
     *
     * This method renders a rectangle with rounded corners and an outline (frame) using
     * the top-left corner coordinates `(x, y)`, width `w`, height `h`, the radii for each corner,
     * and the thickness of the outline. The rectangle's interior is left unfilled, and only the
     * border is drawn with the specified thickness.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param rtl The radius of the top-left corner.
     * @param rtr The radius of the top-right corner.
     * @param rbl The radius of the bottom-left corner.
     * @param rbr The radius of the bottom-right corner.
     * @param thickness The thickness of the rectangle's outline.
     *
     * @note This method is useful for creating framed rectangular shapes with rounded corners,
     * which are often used for modern UI components like buttons, panels, or containers.
     */
    
    void FrameRoundRect(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr, double thickness)
    {
        RoundedRectangle(x, y, w, h, rtl, rtr, rbl, rbr);
        Stroke(thickness);
    }
    
    /**
     * @brief Draws and fills a rounded rectangle with uniform corner radius.
     *
     * This method renders a filled rectangle with rounded corners using the top-left corner
     * coordinates `(x, y)`, width `w`, height `h`, and a uniform radius `r` applied to all four corners.
     * The rectangle is filled with a solid color or pattern based on the current rendering settings.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param r The radius applied uniformly to all four corners of the rectangle.
     *
     * @note This method is useful for rendering rounded rectangular shapes commonly used
     * in user interfaces, such as buttons, panels, or containers with rounded corners.
     */
    
    void FillRoundRect(double x, double y, double w, double h, double r)
    {
        FillRoundRect(x, y, w, h, r, r, r, r);
    }
    
    /**
     * @brief Draws an outlined rounded rectangle with a uniform corner radius and specified outline thickness.
     *
     * This method renders a rectangle with rounded corners and an outline (frame) using the top-left corner
     * coordinates `(x, y)`, width `w`, height `h`, a uniform radius `r` for all corners, and the specified
     * outline thickness. The interior of the rectangle is left unfilled, and only the border is drawn with
     * the given thickness.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param r The radius applied uniformly to all four corners of the rectangle.
     * @param thickness The thickness of the rectangle's outline.
     *
     * @note This method is useful for creating framed rectangular shapes with rounded corners, which are
     * commonly used for modern UI elements such as buttons, panels, or containers.
     */
    
    void FrameRoundRect(double x, double y, double w, double h, double r, double thickness)
    {
        FrameRoundRect(x, y, w, h, r, r, r, r, thickness);
    }
    
    /**
     * @brief Draws and fills a circular pointer shape within a specified circular area.
     *
     * This method renders a filled circular pointer or indicator within a circle centered at `(cx, cy)`
     * with radius `r`. The pointer has a length defined by `pr`, an orientation defined by `ang`,
     * and an angular width or span defined by `pAng`. The pointer typically points in the direction
     * of the specified angle and is filled with a solid color or pattern.
     *
     * @param cx The x-coordinate of the center of the circle in which the pointer is drawn.
     * @param cy The y-coordinate of the center of the circle in which the pointer is drawn.
     * @param r The radius of the circle within which the pointer is drawn.
     * @param pr The radius or length of the pointer, extending from the center outward.
     * @param ang The angle (in radians or degrees, depending on implementation) indicating the direction
     *            the pointer is pointing.
     * @param pAng The angular width or span of the pointer, defining its spread.
     *
     * @note This method is typically used for rendering pointer indicators in circular gauges, dials,
     * or similar graphical user interface elements.
     */
    
    void FillCircularPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        CircularPointer(cx, cy, r, pr, ang, pAng);
        Fill();
    }
    
    /**
     * @brief Draws an outlined circular pointer within a specified circular area.
     *
     * This method renders an outlined circular pointer or indicator within a circle
     * centered at `(cx, cy)` with radius `r`. The pointer has a length defined by `pr`,
     * an orientation defined by `ang`, and an angular width or span defined by `pAng`.
     * The outline of the pointer is drawn with the specified `thickness`, leaving the interior unfilled.
     *
     * @param cx The x-coordinate of the center of the circle in which the pointer is drawn.
     * @param cy The y-coordinate of the center of the circle in which the pointer is drawn.
     * @param r The radius of the circle within which the pointer is drawn.
     * @param pr The radius or length of the pointer, extending from the center outward.
     * @param ang The angle (in radians or degrees, depending on implementation) indicating the direction
     *            the pointer is pointing.
     * @param pAng The angular width or span of the pointer, defining its spread.
     * @param thickness The thickness of the pointer's outline.
     *
     * @note This method is typically used for rendering pointer indicators in circular gauges, dials,
     * or similar graphical user interface elements, where a framed or outlined pointer is needed.
     */
    
    void FrameCircularPointer(double cx, double cy, double r, double pr, double ang, double pAng, double thickness)
    {
        CircularPointer(cx, cy, r, pr, ang, pAng);
        Stroke(thickness);
    }
    
    /**
     * @brief Draws a line between two points with specified thickness.
     *
     * This method renders a straight line between the two points `(x1, y1)` and `(x2, y2)`
     * with the given `thickness`. The line is drawn using the current rendering settings,
     * which may include color or pattern information.
     *
     * @param x1 The x-coordinate of the starting point of the line.
     * @param y1 The y-coordinate of the starting point of the line.
     * @param x2 The x-coordinate of the ending point of the line.
     * @param y2 The y-coordinate of the ending point of the line.
     * @param thickness The thickness of the line to be drawn.
     *
     * @note This method is useful for drawing lines in user interfaces,
     * graphs, or for connecting graphical elements. The line can be used for
     * decorative or functional purposes, depending on the context.
     */
    
    void Line(double x1, double y1, double x2, double y2, double thickness)
    {
        mGraphics.PathLine(x1, y1, x2, y2);
        SetShapeGradient(HISSTools_Bounds(std::min(x1, x2), std::min(y1, y2), std::abs(x1 - x2), std::abs(y1 - y2)));
        Stroke(thickness);
    }
    
    /**
     * @brief Renders text within a specified rectangular area with alignment options.
     *
     * This method draws the text specified by the string `str` using the provided `HISSTools_Text`
     * object `pTxt`, which defines the text properties (such as font and size). The text is rendered
     * within the rectangle defined by the top-left corner `(x, y)` and dimensions `w` (width) and `h` (height).
     * The text can be aligned horizontally and vertically within the rectangle using the optional parameters
     * `hAlign` and `vAlign`.
     *
     * @param pTxt A pointer to a `HISSTools_Text` object that defines the text properties (font, size, etc.).
     * @param str The string to be rendered as text.
     * @param x The x-coordinate of the top-left corner of the rectangle where the text will be rendered.
     * @param y The y-coordinate of the top-left corner of the rectangle where the text will be rendered.
     * @param w The width of the rectangle where the text will be rendered.
     * @param h The height of the rectangle where the text will be rendered.
     * @param hAlign The horizontal alignment of the text within the rectangle (default is centered).
     *               Options include `kHAlignLeft`, `kHAlignCenter`, and `kHAlignRight`.
     * @param vAlign The vertical alignment of the text within the rectangle (default is centered).
     *               Options include `kVAlignTop`, `kVAlignCenter`, and `kVAlignBottom`.
     *
     * @note This method is useful for rendering text within a UI element, allowing for flexible alignment
     * options. The text is drawn using the properties defined in the `HISSTools_Text` object, and the
     * alignment can be adjusted as needed to fit the design.
     */
    
    void Text(HISSTools_Text *pTxt, const char *str, double x, double y, double w, double h, HTextAlign hAlign = kHAlignCenter, VTextAlign vAlign = kVAlignCenter)
    {
        IText textSpec(pTxt->mSize, mColor->GetColor(), pTxt->mFont, (EAlign) hAlign, (EVAlign) vAlign, 0);
        HISSTools_Bounds rect(x, y, w, h);
        mGraphics.DrawText(textSpec, str, rect);
        
        SetShapeGradient(rect);
    }
    
    /**
     * @brief Retrieves the height of a single line of text based on the given text properties.
     *
     * This static method calculates and returns the height of a single line of text,
     * as defined by the properties in the provided `HISSTools_Text` object `pTxt`.
     * The height is typically determined by the font size and other text-related properties
     * such as line spacing.
     *
     * @param pTxt A pointer to a `HISSTools_Text` object that defines the text properties,
     *             including font size, typeface, and line spacing.
     *
     * @return The height of a single line of text as a `double`, based on the font and text properties.
     *
     * @note This method is useful for layout calculations, particularly when rendering multiple lines
     * of text or determining the space required for text elements in user interfaces.
     */
    
    static double GetTextLineHeight(HISSTools_Text *pTxt)
    {
        return pTxt->mSize;
    }
    
    /**
     * @brief Begins rendering a shadow effect for a specified rectangular area.
     *
     * This method initiates the rendering of a shadow effect using the provided `HISSTools_Shadow` object.
     * The shadow is applied to the rectangular region defined by the `IRECT` object `rect`. The `HISSTools_Shadow`
     * object contains the shadow's properties, such as offset, blur, opacity, and color.
     *
     * @param shadow A pointer to a `HISSTools_Shadow` object that defines the shadow properties, including
     *               its appearance and behavior.
     * @param rect An `IRECT` object that defines the rectangular area where the shadow will be applied.
     *
     * @note This method is typically used in graphical user interfaces to add depth or a visual highlight
     * to elements by rendering a shadow behind them. It should be followed by other rendering operations
     * to complete the shadow effect.
     */
    
    void StartShadow(HISSTools_Shadow *shadow, const IRECT& rect)
    {
        mShadow = shadow;
        StartGroup(rect);
    }
    
    /**
     * @brief Renders the previously configured shadow effect.
     *
     * This method finalizes and renders the shadow effect that was set up using `StartShadow()`.
     * The optional parameter `renderImage` determines whether the shadow itself should be rendered
     * as part of the process. If `renderImage` is set to `true`, the shadow is drawn; if set to `false`,
     * the shadow rendering is skipped, which may be useful in scenarios where only the shadow setup
     * is needed without immediate rendering.
     *
     * @param renderImage A boolean flag indicating whether the shadow should be rendered.
     *                    The default value is `true`, meaning the shadow will be rendered.
     *                    If `false`, the shadow is not rendered but may still be prepared
     *                    for future use.
     *
     * @note This method is typically called after `StartShadow()` to complete the shadow rendering
     * process. It allows for flexibility in deciding whether to immediately render the shadow
     * or defer it for later use.
     */
    
    void RenderShadow(bool renderImage = true)
    {
        ILayerPtr shadowLayer = EndGroup();
        
        // Check there is a shadow specified (otherwise only render original image)
        
        if (mShadow)
        {
            IShadow shadow(*mShadow);
            shadow.mDrawForeground = renderImage;
            mGraphics.ApplyLayerDropShadow(shadowLayer, shadow);
        }
        
        mGraphics.DrawLayer(shadowLayer);
    }
    
private:
    
    /**
     * @brief Fills the current graphical area with the active fill settings.
     *
     * This method fills the current drawing area or shape using the active fill settings, such as color,
     * gradient, or pattern, based on the current rendering context. It is typically used after defining a shape
     * (such as a rectangle, circle, or path) to apply the fill operation to that shape.
     *
     * @note The specific fill behavior depends on the current graphics context, including the active color, gradient,
     * or pattern settings. This method is useful for rendering solid or patterned shapes in graphical user interfaces.
     */
    
    void Fill()
    {
        mGraphics.PathFill(mColor->GetPattern());
    }
    
    /**
     * @brief Strokes the outline of the current shape with the specified thickness.
     *
     * This method draws the outline of the current shape (such as a path, rectangle, or circle)
     * using the specified line `thickness`. The stroke is applied according to the active rendering settings,
     * such as color or pattern, and it only affects the border of the shape, leaving the interior unfilled.
     *
     * @param thickness The thickness of the stroke to be applied to the shape's outline.
     *
     * @note This method is useful for drawing borders or outlines around shapes in graphical user interfaces.
     * It can be used in combination with `Fill()` for creating both filled and outlined shapes.
     */
    
    void Stroke(double thickness)
    {
        mGraphics.PathStroke(mColor->GetPattern(), thickness);
    }
    
    /**
     * @brief Ensures the radius value is valid and fits within the specified width and height constraints.
     *
     * This method adjusts and sanitizes the provided radius `r` to ensure that it fits appropriately
     * within the given width `w` and height `h`. It returns a corrected radius that is not larger
     * than half the width or height, preventing graphical distortions or out-of-bounds errors when rendering shapes.
     *
     * @param r The initial radius value to be sanitized.
     * @param w The width constraint for the shape or area where the radius is applied.
     * @param h The height constraint for the shape or area where the radius is applied.
     *
     * @return The sanitized radius value, ensuring that the radius fits within the dimensions of the given width and height.
     *
     * @note This method is commonly used when rendering rounded rectangles or circles to prevent the radius
     * from exceeding the bounds of the shape, ensuring proper visual appearance.
     */
    
    double SanitizeRadius(double r, double w, double h)
    {
        r = r < 0 ? 0 : r;
        r = ((r * 2.0) > w) ? w / 2.0: r;
        r = ((r * 2.0) > h) ? h / 2.0: r;
        
        return r;
    }
    
    /**
     * @brief Draws an arc with the specified center, radius, and angular span.
     *
     * This method renders an arc centered at `(cx, cy)` with a radius `r`,
     * starting at the angle `begAng` and spanning an angular distance of `arcAng`.
     * The arc is a portion of a circle, with its appearance and position defined by the
     * provided parameters.
     *
     * @param cx The x-coordinate of the center of the arc.
     * @param cy The y-coordinate of the center of the arc.
     * @param r The radius of the arc.
     * @param begAng The starting angle of the arc, in radians or degrees (depending on the implementation).
     * @param arcAng The angular span of the arc, in radians or degrees (depending on the implementation).
     *
     * @note This method is useful for rendering circular segments or partial circles,
     * such as in dials, gauges, or other circular UI elements. The angle measurements
     * may be in radians or degrees, depending on the specific implementation.
     */
    
    void Arc(double cx, double cy, double r, double begAng, double arcAng)
    {
        begAng = begAng * 360.0 + 90.f;
        arcAng = begAng + (arcAng * 360.0);
        
        mGraphics.PathArc(cx, cy, r, std::min(begAng, arcAng), std::max(arcAng, begAng));
        SetShapeGradient(HISSTools_Bounds(cx - r, cy - r, 2.0 * r, 2.0 * r));
    }
    
    /**
     * @brief Draws a rectangle with specified position and dimensions.
     *
     * This method renders a rectangle using the top-left corner coordinates `(x, y)`
     * and the specified width `w` and height `h`. The rectangle is outlined without
     * being filled, and its appearance (such as line thickness and color) is determined
     * by the current rendering settings.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     *
     * @note This method is typically used to draw simple rectangular shapes in user interfaces,
     * such as borders, frames, or layout guides. To fill the rectangle, the `Fill()` method
     * can be used in combination with this method.
     */
    
    void Rectangle(double x, double y, double w, double h)
    {
        HISSTools_Bounds r(x, y, w, h);
        mGraphics.PathRect(r);
        SetShapeGradient(r);
    }
    
    /**
     * @brief Draws a rectangle with specified corner radii for rounded corners.
     *
     * This method renders a rectangle with rounded corners using the top-left corner coordinates `(x, y)`,
     * width `w`, height `h`, and different radii for each corner. The corner radii are defined by
     * `rtl` (top-left corner), `rtr` (top-right corner), `rbl` (bottom-left corner), and `rbr` (bottom-right corner).
     * The rectangle is outlined without being filled, and the appearance is determined by the current rendering settings.
     *
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param rtl The radius of the top-left corner.
     * @param rtr The radius of the top-right corner.
     * @param rbl The radius of the bottom-left corner.
     * @param rbr The radius of the bottom-right corner.
     *
     * @note This method is useful for creating rounded rectangular shapes, such as buttons, panels, or other UI elements.
     * To fill the rectangle, the `Fill()` method can be used in combination with this method.
     */
    
    void RoundedRectangle(double x, double y, double w, double h, double rtl, double rtr, double rbl, double rbr)
    {
        rtl = SanitizeRadius(rtl, w, h);
        rtr = SanitizeRadius(rtr, w, h);
        rbl = SanitizeRadius(rbl, w, h);
        rbr = SanitizeRadius(rbr, w, h);
        
        HISSTools_Bounds r(x, y, w, h);
        mGraphics.PathRoundRect(r, rtl, rtr, rbl, rbr);
        SetShapeGradient(r);
    }
    
    /**
     * @brief Draws a circular pointer within a circular area, based on specified parameters.
     *
     * This method renders a circular pointer within a circle centered at `(cx, cy)` with radius `r`.
     * The pointer extends from the center of the circle and has a length defined by `pr`, a direction
     * indicated by the angle `ang`, and a width or spread determined by `pAng`.
     *
     * @param cx The x-coordinate of the center of the circle where the pointer is drawn.
     * @param cy The y-coordinate of the center of the circle where the pointer is drawn.
     * @param r The radius of the circle within which the pointer is drawn.
     * @param pr The radius or length of the pointer extending from the center outward.
     * @param ang The angle (in radians or degrees, depending on implementation) that defines
     *            the direction of the pointer.
     * @param pAng The angular width or span of the pointer, defining its spread.
     *
     * @note This method is typically used for rendering pointer indicators in circular UI elements
     * such as dials, gauges, or similar controls.
     */
    
    void CircularPointer(double cx, double cy, double r, double pr, double ang, double pAng)
    {
        double xx = cx + cos(2.0 * PI * ang) * pr;
        double yy = cy + sin(2.0 * PI * ang) * pr;
        
        double begAng = (ang - pAng) * 360.0 + 90.f;
        double arcAng = (pAng * 2.0 * 360.0) + begAng;
        
        mGraphics.PathClear();
        mGraphics.PathArc(cx, cy, r, arcAng, begAng);
        mGraphics.PathLineTo(xx, yy);
        mGraphics.PathClose();
        
        // FIX - revise...
        SetShapeGradient(HISSTools_Bounds(cx - pr, cy - pr, 2.0 * pr, 2.0 * pr));
    }
    
    /**
     * @brief Draws a triangle using the specified vertex coordinates.
     *
     * This method renders a triangle by connecting three points defined by their coordinates:
     * `(x1, y1)`, `(x2, y2)`, and `(x3, y3)`. The triangle is outlined without being filled,
     * and its appearance (such as line thickness and color) is determined by the current rendering settings.
     *
     * @param x1 The x-coordinate of the first vertex.
     * @param y1 The y-coordinate of the first vertex.
     * @param x2 The x-coordinate of the second vertex.
     * @param y2 The y-coordinate of the second vertex.
     * @param x3 The x-coordinate of the third vertex.
     * @param y3 The y-coordinate of the third vertex.
     *
     * @note This method is useful for drawing triangle shapes in user interfaces or graphical applications.
     * To fill the triangle, the `Fill()` method can be used in combination with this method.
     */
    
    void Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        mGraphics.PathTriangle(x1, y1, x2, y2, x3, y3);
        double l = std::min(x1, std::min(x2, x3));
        double r = std::max(x1, std::max(x2, x3));
        double t = std::min(y1, std::min(y2, y3));
        double b = std::max(y1, std::max(y2, y3));
        SetShapeGradient(HISSTools_Bounds(l, t, r - l, b - t));
    }
    
    /**
     * @brief Sets a gradient fill for the current shape based on the specified bounds.
     *
     * This method configures a gradient fill for the current shape using the boundaries defined
     * by the `HISSTools_Bounds` object `r`. The gradient is applied across the shape, with the
     * bounds determining the extent and orientation of the gradient.
     *
     * @param r A `HISSTools_Bounds` object that defines the rectangular area where the gradient
     *          will be applied. This typically includes the position and size of the bounds.
     *
     * @note This method is useful for adding gradient effects to shapes, improving their visual
     * appearance with smooth color transitions. The specific gradient type (e.g., linear, radial)
     * may be determined by the current rendering context or additional settings.
     */
    
    void SetShapeGradient(const HISSTools_Bounds& r)
    {
        mColor->SetRect(mForceGradientBox ? mGradientArea : r, mCSOrientation);
    }
    
    // IGraphics
    
    /**
     * @brief A reference to the graphics context used for rendering operations.
     *
     * This member holds a reference to an `IGraphics` object, which is responsible
     * for managing all graphical rendering operations. The `mGraphics` reference
     * allows the class to perform drawing, rendering shapes, managing colors, and
     * other graphical tasks using the provided graphics context.
     *
     * @note Since this is a reference, it is assumed that the `IGraphics` object
     * is provided and managed externally. This object is essential for rendering
     * visual elements in the user interface.
     */
    
    IGraphics& mGraphics;
    
    // Gradients
    
    /**
     * @brief Defines the area where a gradient will be applied.
     *
     * This member variable holds a `HISSTools_Bounds` object that specifies the
     * boundaries of the region where a gradient effect will be applied. The `mGradientArea`
     * is typically used to define the position and size of the gradient, controlling
     * how it is rendered across a shape or other graphical element.
     *
     * @note The `mGradientArea` is essential for managing the application of gradient
     * fills, ensuring that the gradient is correctly positioned and sized based on the
     * layout or design requirements.
     */
    
    HISSTools_Bounds mGradientArea;
    
    /**
     * @brief Indicates whether a gradient box is forcibly applied.
     *
     * This boolean member variable controls whether the gradient box is forced
     * to be applied to a shape or graphical element. When `mForceGradientBox` is set
     * to `true`, a gradient box will be rendered, regardless of other conditions.
     * If `false`, the gradient box may not be applied based on other logic or settings.
     *
     * @note This flag is typically used to enforce specific rendering behavior for
     * shapes that require a gradient, ensuring consistent visual appearance.
     */
    
    bool mForceGradientBox;
    
    /**
     * @brief Specifies the orientation of the color or gradient applied to the shape.
     *
     * This member variable holds a value of type `ColorOrientation`, which defines
     * the direction in which colors or gradients are applied to graphical elements.
     * The `mCSOrientation` can control whether the color transitions occur horizontally,
     * vertically, or in another direction, depending on the design of the graphical
     * element or shape.
     *
     * @note This variable is crucial for determining how color gradients or transitions
     * are rendered, allowing for more dynamic and visually appealing effects.
     */
    
    ColorOrientation mCSOrientation;
    
    // Other State (line thickness, color and shadow
    
    /**
     * @brief Defines the thickness of lines when drawing multiple lines.
     *
     * This member variable specifies the thickness of the lines used when drawing
     * multi-line shapes or paths. The `mMultiLineThickness` is initialized to `1.0`
     * by default, meaning the lines will have a thickness of 1 unit, but it can be
     * adjusted as needed for thicker or thinner lines.
     *
     * @note This variable is important for controlling the visual appearance of multi-line
     * drawings, ensuring consistency in line width across all segments of the shape or path.
     */
    
    double mMultiLineThickness = 1.0;
    
    /**
     * @brief A pointer to the current color specification used for rendering.
     *
     * This member variable points to a `HISSTools_Color_Spec` object, which defines
     * the color settings for rendering graphical elements. The `mColor` object can
     * include properties such as RGB values, alpha transparency, and other color-related
     * attributes, allowing for dynamic color management in the rendering process.
     *
     * @note The `mColor` pointer allows the flexibility to change or update the color
     * settings at runtime, enabling different graphical elements to be rendered with
     * varying colors based on the application's state or user interactions.
     */
    
    HISSTools_Color_Spec *mColor;
    
    /**
     * @brief A pointer to the shadow specification used for rendering shadows.
     *
     * This member variable points to a `HISSTools_Shadow` object, which defines the
     * properties of the shadow effect used in rendering. The `mShadow` object can include
     * attributes such as shadow color, blur radius, offset, and opacity, allowing for
     * dynamic shadow effects behind graphical elements.
     *
     * @note The `mShadow` pointer allows flexibility in applying or modifying shadows for
     * various graphical elements. If the pointer is `nullptr`, it indicates that no shadow
     * will be applied.
     */
    
    HISSTools_Shadow *mShadow;
};

