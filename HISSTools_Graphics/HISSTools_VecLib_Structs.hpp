
/**
 * @file HISSTools_Text.hpp
 * @brief Header file for the HISSTools text handling and rendering components.
 *
 * This file defines structures and functions related to the management and rendering
 * of text within the HISSTools system. It includes definitions for text alignment, font settings,
 * and other text-related properties. The file also defines the HISSTools_Text struct, which encapsulates
 * text attributes such as font, size, and alignment, and provides constructors for initializing these properties.
 *
 * Key elements in this file include:
 * - Enumeration types for horizontal and vertical text alignment (HTextAlign and VTextAlign).
 * - The HISSTools_Text struct, which stores and manages font and size information for text rendering.
 * - Default values for fonts and text sizes, which vary depending on the target platform.
 *
 * @note The platform-specific font defaults are handled using preprocessor directives to ensure
 * compatibility across macOS and other platforms.
 *
 * @see HISSTools_Text
 * @see HTextAlign
 * @see VTextAlign
 */

#pragma once

#include <IGraphicsStructs.h>
#include <algorithm>

// Bounds

/**
 * @struct HISSTools_Bounds
 * @brief A struct that represents bounds and inherits from iplug::igraphics::IRECT.
 *
 * This struct is used to define a bounding rectangle with additional functionality
 * provided by the base class iplug::igraphics::IRECT. It can be used to handle
 * rectangular regions within the graphical interface of the HISSTools system.
 *
 * @extends iplug::igraphics::IRECT
 */

struct HISSTools_Bounds : public iplug::igraphics::IRECT
{
    
    /**
     * @typedef IRECT
     * @brief Alias for iplug::igraphics::IRECT.
     *
     * This typedef creates an alias for the iplug::igraphics::IRECT class, allowing
     * it to be referenced as IRECT within the code. The IRECT class is commonly used
     * to represent rectangular regions in the graphical interface.
     */
    
    using IRECT = iplug::igraphics::IRECT;
    
    /**
     * @brief Default constructor for the HISSTools_Bounds struct.
     *
     * Initializes a HISSTools_Bounds object with default values. This constructor
     * does not perform any specific initialization beyond the default behavior of the base class.
     */
    
    HISSTools_Bounds() {}
    
    /**
     * @brief Constructor that initializes HISSTools_Bounds with a specified IRECT.
     *
     * This constructor takes an IRECT object and initializes the HISSTools_Bounds
     * instance by passing the IRECT to the base class constructor.
     *
     * @param rect The IRECT object used to initialize the bounds.
     */
    
    HISSTools_Bounds(const IRECT rect) : IRECT(rect) {}
    
    /**
     * @brief Constructor that initializes HISSTools_Bounds with specific coordinates and dimensions.
     *
     * This constructor allows initializing a HISSTools_Bounds object with the given x and y coordinates
     * and width (w) and height (h). It automatically adjusts the position based on whether the width and height
     * are positive or negative.
     *
     * @param x The x-coordinate of the rectangle's origin.
     * @param y The y-coordinate of the rectangle's origin.
     * @param w The width of the rectangle. If negative, the rectangle's x-coordinate is adjusted accordingly.
     * @param h The height of the rectangle. If negative, the rectangle's y-coordinate is adjusted accordingly.
     *
     * The constructor uses `static_cast<float>` to convert the double values to float, ensuring compatibility
     * with the base class IRECT, which expects float values for coordinates and dimensions.
     */
    
    HISSTools_Bounds(double x, double y, double w, double h)
    : IRECT(static_cast<float>(w > 0 ? x : x - w),
            static_cast<float>(h > 0 ? y : y - h),
            static_cast<float>(w > 0 ? x + w : x),
            static_cast<float>(h > 0 ? y + h : y))
    {}
    
    /**
     * @brief Expands the current bounds to include the specified HISSTools_Bounds.
     *
     * This method updates the current HISSTools_Bounds by computing the union of
     * the current bounds and the given HISSTools_Bounds, ensuring that the current
     * bounds encompass both the original and the provided bounds.
     *
     * @param inc The HISSTools_Bounds object to include in the current bounds.
     *
     * The method uses the Union operation to merge the two bounding rectangles.
     */
    
    void Include(HISSTools_Bounds inc)      { *this = Union(inc); }
    
    /**
     * @brief Expands the current bounds by adding a specified thickness to all sides.
     *
     * This method increases the size of the current HISSTools_Bounds by padding all sides
     * with half of the given thickness. The thickness value is clamped to a minimum of 0
     * to avoid reducing the size of the bounds.
     *
     * @param thickness The thickness to add to the bounds. If the value is negative,
     * it is clamped to 0 to ensure no reduction in size.
     *
     * The method uses the Pad function to apply the calculated padding.
     */
    
    void AddThickness(double thickness)     { Pad(static_cast<float>(std::max(0.0, thickness) * 0.5)); }
};

// Colors and Color Specs

/**
 * @enum ColorOrientation
 * @brief Specifies the possible orientations for color representation.
 *
 * This enum defines two possible orientations for handling color:
 * horizontal or vertical.
 */

enum ColorOrientation { kCSOrientHorizontal, kCSOrientVertical };

/**
 * @struct HISSTools_Color
 * @brief A struct representing a color with multiple components.
 *
 * The HISSTools_Color struct is used to define a color, typically for graphical elements
 * within the HISSTools system. It likely contains data members for different color channels
 * such as red, green, blue, and alpha (opacity), though specifics should be provided based on the implementation.
 *
 * This struct can be used for color manipulations, blending, or representation in graphical interfaces.
 */

struct HISSTools_Color
{
    
    /**
     * @brief Default constructor for HISSTools_Color.
     *
     * Initializes a HISSTools_Color object with default values for the color components.
     * The red (r), green (g), blue (b), and alpha (a) channels are all set to 0,
     * representing a fully transparent black color.
     *
     * The default values ensure that a newly created HISSTools_Color object has no visible color or opacity.
     */
    
    HISSTools_Color() : r(0), g(0), b(0), a(0) {}
    
    /**
     * @brief Constructor that initializes a HISSTools_Color object with specified color components.
     *
     * This constructor allows initializing a HISSTools_Color object by specifying values
     * for the red (r), green (g), blue (b), and alpha (a) channels.
     *
     * @param R The red channel value (0 to 1), where 0 is no red and 1 is full red.
     * @param G The green channel value (0 to 1), where 0 is no green and 1 is full green.
     * @param B The blue channel value (0 to 1), where 0 is no blue and 1 is full blue.
     * @param A The alpha channel value (0 to 1), where 0 is fully transparent and 1 is fully opaque.
     *
     * This constructor assigns the provided values to the corresponding color components.
     */
    
    HISSTools_Color(double R, double G, double B, double A) : r(R), g(G), b(B), a(A)
    {
        // FIX - issues with super-luminescence - where does this happen?
        
        if (a)
        {
            r /= a;
            g /= a;
            b /= a;
            a *= a;
        }
        
        if (r > 1.0 || g > 1.0 || b > 1.0)
            r = g = b = 1.0;
    }
    
    /**
     * @brief Color components of the HISSTools_Color object.
     *
     * These member variables represent the individual color channels:
     *
     * - @c r: Red channel value (0 to 1), where 0 represents no red and 1 represents full red.
     * - @c g: Green channel value (0 to 1), where 0 represents no green and 1 represents full green.
     * - @c b: Blue channel value (0 to 1), where 0 represents no blue and 1 represents full blue.
     * - @c a: Alpha (opacity) channel value (0 to 1), where 0 represents fully transparent and 1 represents fully opaque.
     */
    
    double r, g, b, a;
};

/**
 * @class HISSTools_Color_Spec
 * @brief A class that defines a color specification for graphical elements.
 *
 * The HISSTools_Color_Spec class is used to manage and apply various color specifications
 * within the HISSTools system. This class likely handles color schemes, gradients, or other
 * complex color-related functionality for graphical objects.
 *
 * It may provide methods for manipulating, blending, or converting colors between different spaces
 * depending on the specific use case.
 */

class HISSTools_Color_Spec
{
protected:
    
    /**
     * @typedef IRECT
     * @brief Alias for iplug::igraphics::IRECT.
     *
     * This typedef creates an alias for the iplug::igraphics::IRECT class, allowing
     * it to be referenced as IRECT within the code. The IRECT class represents a
     * rectangular region typically used for graphical operations in the iPlug graphics system.
     */
    
    using IRECT = iplug::igraphics::IRECT;
    
    /**
     * @typedef IPattern
     * @brief Alias for iplug::igraphics::IPattern.
     *
     * This typedef creates an alias for the iplug::igraphics::IPattern class, allowing
     * it to be referenced as IPattern within the code. The IPattern class is typically used
     * to represent patterns for graphical fills, such as gradients or solid colors, in the iPlug
     * graphics system.
     */
    
    using IPattern = iplug::igraphics::IPattern;
    
    /**
     * @typedef IColor
     * @brief Alias for iplug::igraphics::IColor.
     *
     * This typedef creates an alias for the iplug::igraphics::IColor class, allowing
     * it to be referenced as IColor within the code. The IColor class is used to represent
     * colors in the iPlug graphics system, typically including values for red, green, blue,
     * and alpha (opacity) channels.
     */
    
    using IColor = iplug::igraphics::IColor;
    
    /**
     * @typedef IColorStop
     * @brief Alias for iplug::igraphics::IColorStop.
     *
     * This typedef creates an alias for the iplug::igraphics::IColorStop class, allowing
     * it to be referenced as IColorStop within the code. The IColorStop class is typically
     * used to represent a specific point in a gradient where a color transition occurs,
     * commonly used in gradient fills in the iPlug graphics system.
     */
    
    using IColorStop = iplug::igraphics::IColorStop;
    
    /**
     * @typedef EPatternType
     * @brief Alias for iplug::igraphics::EPatternType.
     *
     * This typedef creates an alias for the iplug::igraphics::EPatternType enum, allowing it to
     * be referenced as EPatternType within the code. The EPatternType enum defines the types of
     * patterns that can be used in the iPlug graphics system, such as solid fills, linear gradients,
     * radial gradients, and more.
     */
    
    using EPatternType = iplug::igraphics::EPatternType;
    
public:
    
    /**
     * @brief Default constructor for HISSTools_Color_Spec.
     *
     * Initializes a HISSTools_Color_Spec object with a default pattern. The pattern is initialized
     * to a solid black color (iplug::igraphics::COLOR_BLACK). This constructor sets up a default color specification
     * that can later be modified with specific colors or patterns.
     */
    
    HISSTools_Color_Spec()
    : mPattern(iplug::igraphics::COLOR_BLACK)
    {}
    
    /**
     * @brief Constructor that initializes HISSTools_Color_Spec with a specified color.
     *
     * This constructor initializes a HISSTools_Color_Spec object by setting its pattern to the given color.
     * The color is clamped to ensure valid color values are used. The @c ClampColor function ensures that
     * the red, green, blue, and alpha channels of the provided color are within the allowable range.
     *
     * @param color The HISSTools_Color object used to initialize the pattern. It represents the desired color specification.
     */
    
    HISSTools_Color_Spec(HISSTools_Color color)
    : mPattern(ClampColor(color))
    {}
    
    /**
     * @brief Constructor that initializes HISSTools_Color_Spec with individual color components.
     *
     * This constructor initializes a HISSTools_Color_Spec object by creating a HISSTools_Color object
     * using the specified red, green, blue, and alpha values. It then delegates to the constructor that
     * accepts a HISSTools_Color object. The values for each color channel are clamped as needed.
     *
     * @param r The red channel value (0 to 1), where 0 represents no red and 1 represents full red.
     * @param g The green channel value (0 to 1), where 0 represents no green and 1 represents full green.
     * @param b The blue channel value (0 to 1), where 0 represents no blue and 1 represents full blue.
     * @param a The alpha (opacity) channel value (0 to 1), where 0 represents fully transparent and 1 represents fully opaque.
     *
     * This constructor allows direct initialization using individual color component values.
     */
    
    HISSTools_Color_Spec(double r, double g, double b, double a)
    : HISSTools_Color_Spec(HISSTools_Color(r, g, b, a))
    {}
    
    /**
     * @brief Virtual destructor for HISSTools_Color_Spec.
     *
     * This virtual destructor ensures that any derived classes of HISSTools_Color_Spec can
     * properly clean up resources when an object is destroyed. It allows for safe and correct
     * polymorphic behavior when deleting instances of derived classes through a pointer to the base class.
     */
    
    virtual ~HISSTools_Color_Spec(){}
    
    /**
     * @brief Retrieves the primary color from the color pattern.
     *
     * This method returns a reference to the first color stop's color from the internal color pattern.
     * It is typically used to access the main color used in the HISSTools_Color_Spec object.
     *
     * @return A const reference to the IColor object representing the primary color.
     *
     * @note This method assumes that the pattern contains at least one color stop.
     */
    
    const IColor& GetColor() const { return mPattern.GetStop(0).mColor; }
    
    /**
     * @brief Retrieves the current color pattern.
     *
     * This virtual method returns a copy of an IPattern object that is constructed
     * using the current primary color from the color specification. The pattern returned
     * is typically a solid color pattern based on the primary color retrieved from @c GetColor().
     *
     * @return An IPattern object initialized with the current primary color.
     *
     * @note This method can be overridden in derived classes to return more complex patterns, such as gradients.
     */
    
    virtual IPattern GetPattern() const { return IPattern(GetColor()); }
    
    /**
     * @brief Sets the rectangular area and orientation for the color specification.
     *
     * This virtual method is used to define the rectangular region for which the color specification
     * will be applied, as well as the orientation of the color (either horizontal or vertical).
     *
     * @param r The IRECT object that defines the rectangular area.
     * @param CSOrientation The orientation of the color, defined by the ColorOrientation enum
     *                      (e.g., horizontal or vertical).
     *
     * @note This method is currently empty and is intended to be overridden by derived classes
     *       to implement specific behavior for setting the rectangle and color orientation.
     */
    
    virtual void SetRect(const IRECT r, ColorOrientation CSOrientation) {}
    
private:
    
    /**
     * @brief Clamps a double value to the nearest integer within a valid range.
     *
     * This function takes a double value @p x and clamps it to the nearest integer. The behavior of this
     * function typically ensures that the resulting integer is within a valid or desired range, although
     * the exact range depends on the implementation (e.g., within the limits of standard integer types).
     *
     * @param x The double value to be clamped to an integer.
     * @return The clamped integer value.
     *
     * @note This function is intended to prevent values from exceeding a certain range when converted to integers.
     */
    
    int ClampToInt(double x)
    {
        return std::min(255, std::max(0, static_cast<int>(x * 255.0)));
    }
    
    /**
     * @brief Clamps the color components of a HISSTools_Color object to valid ranges.
     *
     * This function takes a HISSTools_Color object @p c and ensures that each of its color components
     * (red, green, blue, and alpha) are clamped to a valid range, typically between 0 and 1.
     * It returns an IColor object that contains the clamped values.
     *
     * @param c The HISSTools_Color object whose components need to be clamped.
     * @return An IColor object with the clamped color values.
     *
     * @note This function prevents color values from going outside of valid ranges, ensuring that
     * all color components remain within acceptable limits for rendering.
     */
    
    IColor ClampColor(HISSTools_Color c)
    {
        return IColor(ClampToInt(c.a), ClampToInt(c.r), ClampToInt(c.g), ClampToInt(c.b));
    }
    
protected:
    
    /**
     * @brief The color pattern used in the HISSTools_Color_Spec class.
     *
     * This member variable stores an IPattern object, which defines the color pattern
     * applied in the HISSTools_Color_Spec class. The pattern can represent solid colors,
     * gradients, or other types of fill patterns used for graphical elements.
     *
     * @note The initial value of mPattern is typically set to a default pattern, such as a solid black color.
     */
    
    IPattern mPattern;
};

/**
 * @class HISSTools_LICE_HVGradient
 * @brief A class for handling horizontal and vertical gradients.
 *
 * This class, derived from HISSTools_Color_Spec, represents a color specification
 * that supports horizontal and vertical gradients. It extends the functionality of
 * HISSTools_Color_Spec by adding the ability to create and manipulate gradients based on
 * a specified orientation.
 *
 * @details The class is designed to work with gradient patterns, allowing for the creation
 * of linear gradients in either horizontal or vertical directions. It inherits the basic
 * color management features of HISSTools_Color_Spec, while adding gradient-specific functionality.
 *
 * @extends HISSTools_Color_Spec
 */

class HISSTools_LICE_HVGradient : public HISSTools_Color_Spec
{
protected:
    
    /**
     * @enum EMode
     * @brief Defines the modes for handling gradient orientation.
     *
     * This enum specifies the different modes available for the horizontal and vertical gradient behavior.
     * It is used to control the direction and behavior of the gradient in the HISSTools_LICE_HVGradient class.
     *
     * @var kModeHorzVert
     * Represents a mode where both horizontal and vertical gradients are applied.
     *
     * @var kModeHVFlip
     * Represents a mode where the gradient is flipped between horizontal and vertical.
     *
     * @var kModeHorz
     * Represents a mode where only the horizontal gradient is applied.
     *
     * @var kModeVert
     * Represents a mode where only the vertical gradient is applied.
     */
    
    enum EMode { kModeHorzVert, kModeHVFlip, kModeHorz, kModeVert };
    
    /**
     * @brief Sets the gradient mode for the HISSTools_LICE_HVGradient object.
     *
     * This method allows the user to specify the mode for handling the gradient orientation
     * by setting the @p mMode member variable. The mode determines whether the gradient is
     * horizontal, vertical, flipped, or both.
     *
     * @param mode The mode to set, chosen from the EMode enum (e.g., kModeHorzVert, kModeHVFlip, kModeHorz, kModeVert).
     *
     * @see EMode
     */
    
    void SetMode(EMode mode) { mMode = mode; }
    
public:
    
    /**
     * @brief Constructor for HISSTools_LICE_HVGradient that initializes the gradient mode.
     *
     * This constructor initializes a HISSTools_LICE_HVGradient object and sets the gradient mode
     * based on the provided @p flipVertical flag. If @p flipVertical is true, the gradient mode is
     * set to @c kModeHVFlip (horizontal-vertical flip); otherwise, it is set to @c kModeHorzVert (horizontal and vertical).
     *
     * @param flipVertical A boolean flag that determines the initial gradient mode.
     *                     If true, the gradient will alternate between horizontal and vertical.
     *                     If false, both horizontal and vertical gradients will be applied.
     *
     * @see EMode
     */
    
    HISSTools_LICE_HVGradient(bool flipVertical = true) : mMode(flipVertical ? kModeHVFlip : kModeHorzVert)
    {
        mPattern = IPattern(EPatternType::Linear);
    }
    
    /**
     * @brief Adds a color stop to the gradient at a specified offset.
     *
     * This method allows you to add a new color stop to the gradient with a specified color and offset.
     * The color stop defines a point in the gradient where a color transition occurs.
     *
     * @param color The HISSTools_Color object representing the color to be added at the stop.
     * @param offset A double representing the position of the color stop along the gradient,
     *               typically in the range [0.0, 1.0], where 0.0 is the start and 1.0 is the end of the gradient.
     *
     * @note The offset should be clamped between 0.0 and 1.0 to ensure it falls within the gradient's valid range.
     */
    
    void AddStop(HISSTools_Color color, double offset)
    {
        HISSTools_Color_Spec colorClamp = HISSTools_Color_Spec(color);
        mPattern.AddStop(colorClamp.GetColor(), static_cast<float>(offset));
    }
    
    /**
     * @brief Sets the rectangular area and orientation for the gradient.
     *
     * This method overrides the base class implementation to define the rectangular region
     * where the gradient will be applied, as well as the orientation of the gradient
     * (horizontal or vertical) based on the @p CSOrientation parameter.
     *
     * @param r The IRECT object that defines the bounds of the rectangular area.
     * @param CSOrientation The orientation of the gradient, specified by the ColorOrientation enum.
     *                      It determines whether the gradient is applied horizontally or vertically.
     *
     * @note This method is an override and provides specific functionality for setting the
     *       rectangular area and gradient orientation in the HISSTools_LICE_HVGradient class.
     */
    
    virtual void SetRect(const IRECT r, ColorOrientation CSOrientation) override
    {
        if ((CSOrientation == kCSOrientHorizontal && mMode != kModeVert) || mMode == kModeHorz)
            mBox = IRECT(r.L, r.T, r.R, r.T);
        else
        {
            if (mMode == kModeHVFlip)
                mBox = IRECT(r.L, r.B, r.L, r.T);
            else
                mBox = IRECT(r.L, r.T, r.L, r.B);
        }
    }
    
    /**
     * @brief Retrieves the color pattern for the gradient.
     *
     * This method overrides the base class implementation to return an IPattern object representing
     * the gradient pattern defined in the HISSTools_LICE_HVGradient class. The pattern is constructed
     * based on the current gradient stops, rectangle dimensions, and orientation (horizontal or vertical).
     *
     * @return An IPattern object that represents the current gradient pattern.
     *
     * @note This method is an override and returns a more complex pattern (typically a gradient)
     *       compared to the base class, which may return a solid color pattern.
     */
    
    IPattern GetPattern() const override
    {
        IPattern pattern = IPattern::CreateLinearGradient(mBox.L, mBox.T, mBox.R, mBox.B);
        
        for (int i = 0; i < mPattern.NStops(); i++)
        {
            const IColorStop& stop = mPattern.GetStop(i);
            pattern.AddStop(stop.mColor, stop.mOffset);
        }
        
        return pattern;
    }
    
private:
    
    /**
     * @brief The current mode for gradient orientation.
     *
     * This member variable stores the current gradient mode, represented by the EMode enum.
     * It determines whether the gradient is applied horizontally, vertically, or a combination
     * of both, depending on the selected mode.
     *
     * @see EMode
     *
     * @note The mode affects how the gradient is rendered in the HISSTools_LICE_HVGradient class.
     */
    
    EMode mMode;
    
    /**
     * @brief The rectangular area where the gradient is applied.
     *
     * This member variable stores an IRECT object representing the dimensions and position
     * of the rectangular region in which the gradient is rendered. The gradient will be
     * applied within the bounds defined by this rectangle.
     *
     * @note The size and position of @p mBox affect how the gradient is drawn,
     *       and it is typically set through the @c SetRect() method.
     */
    
    IRECT mBox;
};

/**
 * @struct HISSTools_LICE_HGradient
 * @brief A struct for handling horizontal gradients.
 *
 * This struct is derived from HISSTools_LICE_HVGradient and specializes in creating
 * and managing horizontal gradients. It inherits all the functionality of
 * HISSTools_LICE_HVGradient but specifically focuses on the horizontal orientation
 * for gradient rendering.
 *
 * @details The class simplifies the usage of horizontal gradients by automatically
 * setting the gradient mode to horizontal, allowing easy creation and management of
 * such gradients within the HISSTools system.
 *
 * @extends HISSTools_LICE_HVGradient
 */

struct HISSTools_LICE_HGradient : public HISSTools_LICE_HVGradient
{
    
    /**
     * @brief Default constructor for HISSTools_LICE_HGradient.
     *
     * This constructor initializes a HISSTools_LICE_HGradient object by calling the base class constructor
     * of HISSTools_LICE_HVGradient and then sets the gradient mode to horizontal (@c kModeHorz).
     * This ensures that the gradient created by this object is always applied in a horizontal direction.
     *
     * @note The gradient mode is automatically set to horizontal, simplifying the creation of horizontal gradients.
     */
    
    HISSTools_LICE_HGradient() : HISSTools_LICE_HVGradient() { SetMode(kModeHorz); }
};

/**
 * @struct HISSTools_LICE_VGradient
 * @brief A struct for handling vertical gradients.
 *
 * This struct is derived from HISSTools_LICE_HVGradient and specializes in creating
 * and managing vertical gradients. It inherits all the functionality of
 * HISSTools_LICE_HVGradient but specifically focuses on the vertical orientation
 * for gradient rendering.
 *
 * @details The class simplifies the usage of vertical gradients by automatically
 * setting the gradient mode to vertical, allowing easy creation and management of
 * such gradients within the HISSTools system.
 *
 * @extends HISSTools_LICE_HVGradient
 */

struct HISSTools_LICE_VGradient : public HISSTools_LICE_HVGradient
{
    
    /**
     * @brief Default constructor for HISSTools_LICE_VGradient.
     *
     * This constructor initializes a HISSTools_LICE_VGradient object by calling the base class
     * constructor of HISSTools_LICE_HVGradient with a parameter to disable vertical flipping.
     * It then sets the gradient mode to vertical (@c kModeVert), ensuring that the gradient
     * will be applied in a vertical direction.
     *
     * @note The gradient mode is automatically set to vertical, simplifying the creation of vertical gradients.
     */
    
    HISSTools_LICE_VGradient() : HISSTools_LICE_HVGradient(false) { SetMode(kModeVert); }
};

// Shadows

/**
 * @struct HISSTools_Shadow
 * @brief A struct for handling shadow effects in graphical elements.
 *
 * This struct is derived from the iplug::igraphics::IShadow class and is used to manage
 * shadow effects within the HISSTools system. It extends the functionality of IShadow,
 * potentially providing additional features or customization specific to HISSTools.
 *
 * @details The HISSTools_Shadow struct is designed to create and manipulate shadow effects
 * for graphical elements, controlling properties such as shadow offset, blur radius, and color.
 *
 * @extends iplug::igraphics::IShadow
 */

struct HISSTools_Shadow : public iplug::igraphics::IShadow
{
    
    /**
     * @typedef IRECT
     * @brief Alias for iplug::igraphics::IRECT.
     *
     * This typedef creates an alias for the iplug::igraphics::IRECT class, allowing it to be referenced
     * as IRECT within the code. The IRECT class represents a rectangle used for defining
     * a region in a graphical interface, typically with properties such as position and size.
     *
     * @see iplug::igraphics::IRECT
     */
    
    using IRECT = iplug::igraphics::IRECT;
    
    /**
     * @brief Constructor for HISSTools_Shadow that initializes a shadow with color, offset, and blur size.
     *
     * This constructor initializes a HISSTools_Shadow object by specifying the shadow color, the x and y offset values,
     * and the blur size. The constructor passes these values to the base class, IShadow, converting the blur size and offsets
     * to float as required. The shadow is initialized with the provided color pattern, a blur radius, and offsets to control
     * the shadow's appearance.
     *
     * @param shadowColor The HISSTools_Color_Spec object that defines the color of the shadow.
     * @param xOffset The horizontal offset of the shadow, where positive values move the shadow to the right.
     * @param yOffset The vertical offset of the shadow, where positive values move the shadow downwards.
     * @param blurSize The size of the blur applied to the shadow, controlling its softness.
     *
     * @note The shadow opacity is set to 1.0 (fully opaque) and the shadow is rendered as part of the background, as defined by the IShadow constructor.
     */
    
    HISSTools_Shadow(HISSTools_Color_Spec& shadowColor, double xOffset, double yOffset, double blurSize)
    : IShadow(shadowColor.GetPattern(), static_cast<float>(blurSize), static_cast<float>(xOffset), static_cast<float>(yOffset), 1.f, true)
    {}
    
    /**
     * @brief Calculates the bounds needed to accommodate the blur effect.
     *
     * This method takes the current bounds of an object and adjusts them to account for the size of the blur effect.
     * The returned HISSTools_Bounds object represents the expanded area required to fully display the blurred shadow.
     *
     * @param currentBounds The current HISSTools_Bounds object representing the unblurred region.
     * @return A HISSTools_Bounds object that includes the additional area required for the blur effect.
     *
     * @note This method ensures that the object with the blur effect does not get clipped by expanding its bounds.
     */
    
    HISSTools_Bounds GetBlurBounds(HISSTools_Bounds currentBounds)
    {
        IRECT rect = currentBounds.GetPadded(mBlurSize - 1);
        currentBounds.Include(rect.GetTranslated(mXOffset, mYOffset));
        
        return currentBounds;
    }
};

// Text

// Defaults
/**
 * @brief Defines default font and text size based on the operating system.
 *
 * This section conditionally sets the default font and text size depending on the target operating system.
 * On macOS systems (detected using @c __APPLE__), the default font is set to "Monaco" with a text size of 10.
 * On other platforms, the default font is set to "Verdana" with a text size of 12.
 *
 * @note These values are used as the default configuration for text rendering within the HISSTools system.
 */

/**
 * @def HT_DEFAULT_FONT
 * @brief The default font name used for text rendering.
 *
 * The default font is "Monaco" on macOS and "Verdana" on other platforms.
 */
#if defined __APPLE__
const char* const HT_DEFAULT_FONT = "Monaco";   /**< Default font on macOS. */
const int HT_DEFAULT_TEXT_SIZE = 10;            /**< Default text size on macOS. */
#else
const char* const HT_DEFAULT_FONT = "Verdana";  /**< Default font on non-macOS platforms. */
const int HT_DEFAULT_TEXT_SIZE = 12;            /**< Default text size on non-macOS platforms. */
#endif

/**
 * @brief Defines the maximum length of the font name.
 *
 * @c HT_FONT_LEN is set to 32 to ensure the font name buffer can hold the font string,
 * allowing enough space for font handling operations.
 */
const int HT_FONT_LEN = 32;

// Alignment

/**
 * @enum HTextAlign
 * @brief Defines horizontal text alignment options.
 *
 * This enum specifies the different horizontal alignment options for text. It can be used
 * to control how text is aligned within a given region (e.g., left-aligned, centered, or right-aligned).
 *
 * @var kHAlignLeft
 * Aligns text to the left.
 *
 * @var kHAlignCenter
 * Centers the text horizontally.
 *
 * @var kHAlignRight
 * Aligns text to the right.
 */

enum HTextAlign { kHAlignLeft, kHAlignCenter, kHAlignRight };

/**
 * @enum VTextAlign
 * @brief Defines vertical text alignment options.
 *
 * This enum specifies the different vertical alignment options for text. It can be used
 * to control how text is aligned within a given region vertically (e.g., top-aligned, centered, or bottom-aligned).
 *
 * @var kVAlignTop
 * Aligns text to the top.
 *
 * @var kVAlignCenter
 * Centers the text vertically.
 *
 * @var kVAlignBottom
 * Aligns text to the bottom.
 */

enum VTextAlign { kVAlignTop, kVAlignCenter, kVAlignBottom };

/**
 * @struct HISSTools_Text
 * @brief A struct for managing text properties and rendering within the HISSTools system.
 *
 * The HISSTools_Text struct is used to define and manipulate text attributes such as font,
 * size, color, alignment, and other properties relevant to rendering text in graphical interfaces.
 * It provides an easy way to handle text-related operations in HISSTools.
 *
 * @details This struct likely includes information such as text content, font settings,
 * horizontal and vertical alignment (e.g., using HTextAlign and VTextAlign enums), and other
 * text-related settings that influence the appearance and placement of text within a graphical context.
 */

struct HISSTools_Text
{
    
    /**
     * @brief The font used for rendering text.
     *
     * This character array stores the name of the font to be used for rendering text.
     * The length of the array is defined by @c HT_FONT_LEN to ensure sufficient space
     * for storing font names. The font is used within the HISSTools_Text struct to determine
     * the appearance of the rendered text.
     *
     * @note The font name must be null-terminated and should not exceed the length defined by @c HT_FONT_LEN.
     */
    
    char mFont[HT_FONT_LEN];
    
    /**
     * @brief The size of the text.
     *
     * This member variable stores the size of the text to be rendered. It defines
     * the font size in points, determining how large or small the text appears when displayed.
     *
     * @note The size is typically specified in points and should be set to a positive value
     * to ensure proper text rendering.
     */
    
    int mSize;
    
    /**
     * @brief Constructor for HISSTools_Text that initializes text properties with default or specified values.
     *
     * This constructor initializes a HISSTools_Text object by setting the text size and font.
     * The size is set to the provided value or defaults to @c HT_DEFAULT_TEXT_SIZE if no value is specified.
     * The font can also be set if provided, otherwise it will be initialized later.
     *
     * @param size The size of the text in points. Defaults to @c HT_DEFAULT_TEXT_SIZE if not specified.
     * @param font A pointer to a character array representing the font name. Defaults to 0 (null) if not specified.
     *
     * @note The constructor ensures that the text size is initialized, and the font can be set later.
     */
    
    HISSTools_Text(int size = HT_DEFAULT_TEXT_SIZE, char* font = 0)
    : mSize(size)
    {
        strcpy(mFont, (font ? font : HT_DEFAULT_FONT));
    }
    
    /**
     * @brief Default constructor for HISSTools_Text.
     *
     * This constructor initializes a HISSTools_Text object with the default text size
     * defined by @c HT_DEFAULT_TEXT_SIZE. No font is set during this initialization,
     * and it may be configured later as needed.
     *
     * @note The text size is set to the default value, ensuring that any created text
     * object has a reasonable default size if no specific size is provided.
     */
    
    HISSTools_Text() : mSize(HT_DEFAULT_TEXT_SIZE)
    {
        strcpy(mFont, HT_DEFAULT_FONT);
    }
};
