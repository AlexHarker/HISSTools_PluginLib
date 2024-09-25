
/**
 * @file HISSTools_Design_Scheme.hpp
 * @brief Defines the HISSTools_Design_Scheme class and associated components for managing design specifications.
 *
 * This file contains the declaration of the `HISSTools_Design_Scheme` class,
 * which is responsible for managing various design specifications, such as
 * dimensions, colors, text styles, and shadows, within the HISSTools framework.
 * It includes methods for adding, retrieving, and managing these specifications,
 * as well as support for default and customizable design schemes.
 *
 * The file also provides utility methods for handling labeled data (via the
 * `HISSTools_Label` class template) and supporting different design elements
 * (e.g., colors, text styles, shadows). A static default design scheme is also
 * declared, which can be used as a baseline configuration.
 *
 * @note The design scheme and associated components are intended to support
 * the customization of visual and interactive elements in applications or tools
 * using the HISSTools framework.
 */

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

/**
 * @brief A template class for labeling objects with associated data.
 *
 * The `HISSTools_Label` class is a generic container that associates a label
 * with a data value of type `T`. It allows for efficient storage and
 * retrieval of labeled values within various HISSTools components.
 *
 * @tparam T The type of the data value to be associated with the label.
 * This can be any data type, such as `int`, `float`, `bool`, etc.
 */

template <class T>
class HISSTools_Label
{
public:
    
    /**
     * @brief A string representing the type name.
     *
     * `TypeName` stores the name of the type as a `WDL_String`. It is used
     * to identify the type associated with a particular object or specification
     * within the HISSTools framework.
     */
    
    WDL_String TypeName;
    
    /**
     * @brief A string representing the subtype.
     *
     * `SubType` stores the name of the subtype as a `WDL_String`. It is used
     * to provide additional categorization or differentiation for an object
     * or specification within the HISSTools framework, complementing the main type name.
     */
    
    WDL_String SubType;
    
    /**
     * @brief The stored value of type `T`.
     *
     * `mValue` holds the actual data associated with this object. The type of
     * `mValue` is determined by the template parameter `T`, allowing for
     * flexibility in the type of data stored, such as `int`, `float`, `bool`, or
     * any other user-defined types.
     */
    
    T mValue;
    
    /**
     * @brief A flag indicating whether the associated object should be deleted.
     *
     * `mDelete` is a boolean flag that specifies whether the object associated
     * with this instance should be deleted when no longer needed. If set to `true`,
     * the object will be marked for deletion, otherwise it will not be deleted.
     */
    
    bool mDelete;

    /**
     * @brief Constructor for the HISSTools_Label class.
     *
     * This constructor initializes a `HISSTools_Label` object with the given value,
     * type name, subtype, and a flag indicating whether the object should be deleted.
     *
     * @param value The value of type `T` to be stored in the label.
     * @param name A C-style string representing the type name, which is stored in `TypeName`.
     * @param subType A C-style string representing the subtype, stored in `SubType`.
     * @param free A boolean flag (`mDelete`) that specifies whether the associated
     * object should be deleted (`true`) or not (`false`) when no longer needed.
     */
    
    HISSTools_Label(T value, const char *name, const char *subType, bool free)
        : TypeName(name), SubType(subType), mValue(value), mDelete(free)
    {}

    /**
     * @brief Constructor for the HISSTools_Label class without deletion control.
     *
     * This constructor initializes a `HISSTools_Label` object with the given value,
     * type name, and subtype, and sets the deletion flag (`mDelete`) to `false` by default.
     *
     * @param value The value of type `T` to be stored in the label.
     * @param name A C-style string representing the type name, stored in `TypeName`.
     * @param subType A C-style string representing the subtype, stored in `SubType`.
     *
     * @note The `mDelete` flag is set to `false`, meaning the object will not be
     * deleted when no longer needed.
     */
    
    HISSTools_Label(T value, const char *name, const char *subType)
        : TypeName(name), SubType(subType), mValue(value), mDelete(false)
    {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// Design Scheme Class ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class HISSTools_Design_Scheme
 * @brief A class representing the design scheme for HISSTools.
 *
 * The `HISSTools_Design_Scheme` class manages and stores design specifications,
 * such as dimensions, colors, and text styles, for various components in the
 * HISSTools framework. It allows customization and retrieval of design-related
 * parameters, and can be initialized with default values or custom configurations.
 */

class HISSTools_Design_Scheme
{
public:

    /**
     * @brief Constructor for the HISSTools_Design_Scheme class.
     *
     * This constructor initializes a design scheme for the HISSTools. If the
     * `defaultScheme` parameter is set to true, the scheme is initialized with
     * default values; otherwise, it is initialized as an empty scheme.
     *
     * @param defaultScheme A boolean flag indicating whether to initialize the
     * design scheme with default values. The default value is false.
     */
    
    HISSTools_Design_Scheme(bool defaultScheme = false)
    {
        if (defaultScheme)
            SetDefaults();
    }

    /**
     * @brief Destructor for the HISSTools_Design_Scheme class.
     *
     * This destructor is responsible for cleaning up resources used by the
     * `HISSTools_Design_Scheme` class. It ensures that any dynamically allocated
     * memory or objects associated with the design scheme are properly released.
     */
    
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

    /**
     * @brief A vector storing labeled text style specifications.
     *
     * `mTextStyles` is a collection of `HISSTools_Label` objects, where each label
     * contains a pointer to a `HISSTools_Text` object. It is used to store and manage
     * different text styles within the design scheme, allowing for the customization
     * and retrieval of various text style specifications.
     */
    
    std::vector<HISSTools_Label<HISSTools_Text*>>           mTextStyles;
    
    /**
     * @brief A vector storing labeled color specifications.
     *
     * `mColorSpecs` is a collection of `HISSTools_Label` objects, where each label
     * contains a pointer to a `HISSTools_Color_Spec` object. It is used to store and manage
     * various color specifications within the design scheme, allowing for the customization
     * and retrieval of color settings for different elements.
     */
    
    std::vector<HISSTools_Label<HISSTools_Color_Spec*>>     mColorSpecs;
    
    /**
     * @brief A vector storing labeled shadow specifications.
     *
     * `mShadowSpecs` is a collection of `HISSTools_Label` objects, where each label
     * contains a pointer to a `HISSTools_Shadow` object. It is used to store and manage
     * shadow specifications within the design scheme, allowing for customization and
     * retrieval of shadow properties for various design elements.
     */
    
    std::vector<HISSTools_Label<HISSTools_Shadow*>>         mShadowSpecs;
    
    /**
     * @brief A vector storing labeled dimensional values.
     *
     * `mDimensions` is a collection of `HISSTools_Label` objects, where each label
     * contains a `double` value representing a dimension. It is used to store and
     * manage various size or positional dimensions within the design scheme, allowing
     * for the customization and retrieval of numerical design parameters.
     */
    
    std::vector<HISSTools_Label<double>>                    mDimensions;
    
    /**
     * @brief A vector storing labeled boolean flags.
     *
     * `mFlags` is a collection of `HISSTools_Label` objects, where each label
     * contains a `bool` value representing a flag. These flags are used to store
     * and manage various boolean settings or options within the design scheme,
     * allowing for the customization and retrieval of on/off or true/false values.
     */
    
    std::vector<HISSTools_Label<bool>>                      mFlags;

    // Searching Template

    /**
     * @brief Finds a labeled value in a search space by its name and subtype.
     *
     * This template function searches through a vector of `HISSTools_Label<T>` objects
     * to find a value that matches the provided name and subtype. If no match is found,
     * the specified default value is returned.
     *
     * @tparam T The type of the value being searched for.
     *
     * @param searchSpace A reference to a vector of `HISSTools_Label<T>` objects
     * representing the search space.
     * @param searchName A C-style string representing the name of the label to search for.
     * @param searchSubTypes A C-style string representing the subtype of the label to search for.
     * @param defaultValue The value to return if no matching label is found.
     *
     * @return The value associated with the matching label, or the default value if
     * no match is found.
     */
    
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

    /**
     * @brief Adds a pointer to a labeled object in the specified search space.
     *
     * This template function inserts a new pointer to an object of type `T` into a
     * vector of `HISSTools_Label<T*>` objects. Each pointer is associated with a label
     * consisting of a name and an optional subtype.
     *
     * @tparam T The type of the object being pointed to.
     *
     * @param searchSpace A reference to a vector of `HISSTools_Label<T*>` objects
     * representing the search space where the new label and pointer will be added.
     * @param name A C-style string representing the name of the label to associate
     * with the pointer.
     * @param subType A C-style string representing the subtype of the label to
     * associate with the pointer.
     * @param newValue A pointer to the new object that will be added to the search space.
     */
    
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

    /**
     * @brief Deletes pointers in the specified search space.
     *
     * This template function iterates through a vector of `HISSTools_Label<T*>` objects
     * and deletes the pointers that were marked for deletion (if applicable). It ensures
     * that any dynamically allocated objects in the search space are properly cleaned up.
     *
     * @tparam T The type of the objects being pointed to and deleted.
     *
     * @param searchSpace A reference to a vector of `HISSTools_Label<T*>` objects
     * representing the search space from which pointers will be deleted.
     *
     * @note This function assumes that the objects pointed to need to be deleted
     * manually, and it should be used with care to avoid double deletions or dangling pointers.
     */
    
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

    /**
     * @brief Adds a color specification to the design scheme.
     *
     * This method inserts a new color specification into the design scheme by associating
     * a `HISSTools_Color_Spec` object with a specified name and subtype.
     *
     * @param name A C-style string representing the name of the color specification.
     * @param subType A C-style string representing the subtype of the color specification,
     * allowing for more specific categorization.
     * @param spec A pointer to a `HISSTools_Color_Spec` object representing the color
     * specification to be added.
     */
    
    void AddColorSpec(const char *name, const char *subType, HISSTools_Color_Spec *spec)
    {
        AddPointer(mColorSpecs, name, subType, spec);
    }

    /**
     * @brief Adds a color specification to the design scheme.
     *
     * This method inserts a new color specification into the design scheme by associating
     * a `HISSTools_Color_Spec` object with a specified name. Unlike the other overload,
     * this method does not require a subtype and is used for general color specifications.
     *
     * @param name A C-style string representing the name of the color specification.
     * @param spec A pointer to a `HISSTools_Color_Spec` object representing the color
     * specification to be added.
     */
    
    void AddColorSpec(const char *name, HISSTools_Color_Spec *spec)
    {
        AddColorSpec(name, nullptr, spec);
    }

    /**
     * @brief Retrieves a color specification by name and subtype.
     *
     * This method searches for and returns a pointer to a `HISSTools_Color_Spec` object
     * based on the specified name and optional subtype. If the subtype is not provided,
     * it defaults to `0`, and the method attempts to retrieve the color specification
     * based solely on the name.
     *
     * @param name A C-style string representing the name of the color specification.
     * @param subType A C-style string representing the subtype of the color specification.
     * If not provided, the default is `0` (null), meaning the search will be based only on the name.
     *
     * @return A pointer to the corresponding `HISSTools_Color_Spec` object if found,
     * or `nullptr` if no matching color specification is found.
     */
    
    HISSTools_Color_Spec *GetColorSpec(const char *name, const char *subType = 0)
    {
        return FindByName(mColorSpecs, name, subType, (HISSTools_Color_Spec *)NULL);
    }

    // Dimensions

    /**
     * @brief Adds a dimensional specification to the design scheme.
     *
     * This method inserts a new dimensional value into the design scheme by associating
     * a name and subtype with a given numerical dimension, such as thickness, width, or height.
     *
     * @param name A C-style string representing the name of the dimension.
     * @param subType A C-style string representing the subtype of the dimension,
     * allowing for more specific categorization of the dimension.
     * @param thickness A double value representing the dimension (e.g., thickness) to be added.
     */
    
    void AddDimension(const char *name, const char *subType, double thickness)
    {
        mDimensions.push_back(HISSTools_Label<double>(thickness, name, subType));
    }

    /**
     * @brief Adds a dimensional specification to the design scheme.
     *
     * This method inserts a new dimensional value into the design scheme by associating
     * a name with a given numerical dimension, such as thickness, width, or height.
     * Unlike the other overload, this method does not require a subtype and is used
     * for general dimensions.
     *
     * @param name A C-style string representing the name of the dimension.
     * @param thickness A double value representing the dimension (e.g., thickness) to be added.
     */
    
    void AddDimension(const char *name, double thickness)
    {
        AddDimension(name, nullptr, thickness);
    }

    /**
     * @brief Retrieves a dimensional value by name and optional subtype.
     *
     * This method searches for and returns a dimensional value (e.g., thickness, width, height)
     * based on the provided name and optional subtype. If the subtype is not specified,
     * it defaults to `0` and the method attempts to retrieve the dimension based solely on the name.
     *
     * @param name A C-style string representing the name of the dimension.
     * @param subType A C-style string representing the subtype of the dimension.
     * If not provided, the default is `0` (null), and the search will be based only on the name.
     *
     * @return A double value representing the corresponding dimension if found,
     * or a default value if no matching dimension is found.
     */
    
    double GetDimension(const char *name, const char *subType = 0)
    {
        return FindByName(mDimensions, name, subType, 0.0);
    }

    // Text

    /**
     * @brief Adds a text style specification to the design scheme.
     *
     * This method inserts a new text style into the design scheme by associating
     * a `HISSTools_Text` object with a specified name and subtype. It allows for
     * customization and categorization of different text styles used within the design.
     *
     * @param name A C-style string representing the name of the text style.
     * @param subType A C-style string representing the subtype of the text style,
     * allowing for more specific categorization.
     * @param spec A pointer to a `HISSTools_Text` object representing the text style specification to be added.
     */
    
    void AddTextStyle(const char *name, const char *subType, HISSTools_Text *spec)
    {
        AddPointer(mTextStyles, name, subType, spec);
    }

    /**
     * @brief Adds a text style specification to the design scheme.
     *
     * This method inserts a new text style into the design scheme by associating
     * a `HISSTools_Text` object with a specified name. Unlike the other overload,
     * this method does not require a subtype and is used for general text style specifications.
     *
     * @param name A C-style string representing the name of the text style.
     * @param spec A pointer to a `HISSTools_Text` object representing the text style specification to be added.
     */
    
    void AddTextStyle(const char *name, HISSTools_Text *spec)
    {
        AddTextStyle(name, nullptr, spec);
    }

    /**
     * @brief Retrieves a text style specification by name and optional subtype.
     *
     * This method searches for and returns a pointer to a `HISSTools_Text` object
     * representing a text style based on the specified name and optional subtype.
     * If the subtype is not provided, it defaults to `0`, and the method attempts to
     * retrieve the text style based solely on the name.
     *
     * @param name A C-style string representing the name of the text style.
     * @param subType A C-style string representing the subtype of the text style.
     * If not provided, the default is `0` (null), meaning the search will be based only on the name.
     *
     * @return A pointer to the corresponding `HISSTools_Text` object if found,
     * or `nullptr` if no matching text style is found.
     */
    
    HISSTools_Text *GetTextStyle(const char *name, const char *subType = 0)
    {
        return FindByName(mTextStyles, name, subType, (HISSTools_Text *)NULL);
    }

    // Shadows

    /**
     * @brief Adds a shadow specification to the design scheme.
     *
     * This method inserts a new shadow specification into the design scheme by associating
     * a `HISSTools_Shadow` object with a specified name and subtype. It allows for
     * customization and categorization of shadow properties used within the design elements.
     *
     * @param name A C-style string representing the name of the shadow specification.
     * @param subType A C-style string representing the subtype of the shadow specification,
     * allowing for more specific categorization.
     * @param spec A pointer to a `HISSTools_Shadow` object representing the shadow specification to be added.
     */
    
    void AddShadow(const char *name, const char *subType, HISSTools_Shadow *spec)
    {
        AddPointer(mShadowSpecs, name, subType, spec);
    }

    /**
     * @brief Adds a shadow specification to the design scheme.
     *
     * This method inserts a new shadow specification into the design scheme by associating
     * a `HISSTools_Shadow` object with a specified name. Unlike the other overload,
     * this method does not require a subtype and is used for general shadow specifications.
     *
     * @param name A C-style string representing the name of the shadow specification.
     * @param spec A pointer to a `HISSTools_Shadow` object representing the shadow specification to be added.
     */
    
    void AddShadow(const char *name, HISSTools_Shadow *spec)
    {
        AddShadow(name, nullptr, spec);
    }

    /**
     * @brief Retrieves a shadow specification by name and optional subtype.
     *
     * This method searches for and returns a pointer to a `HISSTools_Shadow` object
     * representing a shadow specification based on the provided name and optional subtype.
     * If the subtype is not provided, it defaults to `0`, and the method attempts to
     * retrieve the shadow specification based solely on the name.
     *
     * @param name A C-style string representing the name of the shadow specification.
     * @param subType A C-style string representing the subtype of the shadow specification.
     * If not provided, the default is `0` (null), meaning the search will be based only on the name.
     *
     * @return A pointer to the corresponding `HISSTools_Shadow` object if found,
     * or `nullptr` if no matching shadow specification is found.
     */
    
    HISSTools_Shadow *GetShadow(const char *name, const char *subType = 0)
    {
        return FindByName(mShadowSpecs, name, subType, (HISSTools_Shadow *)NULL);
    }

    // Flags

    /**
     * @brief Adds a boolean flag to the design scheme.
     *
     * This method inserts a new boolean flag into the design scheme by associating
     * the flag value with a specified name and subtype. It allows for the customization
     * of various on/off or true/false settings within the design.
     *
     * @param name A C-style string representing the name of the flag.
     * @param subType A C-style string representing the subtype of the flag,
     * allowing for more specific categorization.
     * @param flag A boolean value representing the flag, where `true` typically indicates
     * that the flag is active or enabled, and `false` indicates it is inactive or disabled.
     */
    
    void AddFlag(const char *name, const char *subType, bool flag)
    {
        mFlags.push_back(HISSTools_Label<bool>(flag, name, subType));
    }

    /**
     * @brief Adds a boolean flag to the design scheme.
     *
     * This method inserts a new boolean flag into the design scheme by associating
     * the flag value with a specified name. Unlike the other overload, this method
     * does not require a subtype and is used for general flags.
     *
     * @param name A C-style string representing the name of the flag.
     * @param flag A boolean value representing the flag, where `true` typically indicates
     * that the flag is active or enabled, and `false` indicates it is inactive or disabled.
     */
    
    void AddFlag(const char *name, bool flag)
    {
        AddFlag(name, nullptr, flag);
    }

    /**
     * @brief Retrieves a boolean flag by name and optional subtype.
     *
     * This method searches for and returns the value of a boolean flag based on the
     * provided name and optional subtype. If the subtype is not provided, it defaults
     * to `0`, and the method attempts to retrieve the flag based solely on the name.
     *
     * @param name A C-style string representing the name of the flag.
     * @param subType A C-style string representing the subtype of the flag.
     * If not provided, the default is `0` (null), meaning the search will be based only on the name.
     *
     * @return A boolean value representing the flag, where `true` typically indicates
     * that the flag is active or enabled, and `false` indicates it is inactive or disabled.
     * If no matching flag is found, a default value of `false` is returned.
     */
    
    bool GetFlag(const char *name, const char *subType = 0)
    {
        return FindByName(mFlags, name, subType, (bool)false);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////// Default Scheme Values ///////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////

private:

    /**
     * @brief Sets the default values for the design scheme.
     *
     * This method initializes the design scheme with a set of predefined default values
     * for various design parameters, such as dimensions, colors, and text styles. It ensures
     * that the design scheme has a consistent baseline configuration, which can be
     * customized further as needed.
     *
     * @note This method is typically called during the initialization of a design scheme
     * to ensure that all necessary parameters have appropriate default settings.
     */
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

        PanelFillCS->AddStop(HISSTools_Color(0.7, 0.7, 0.7, 0.4), 0.0);   // SUPER LUMINESCENT    (USED in shadow bitmap)
        PanelFillCS->AddStop(HISSTools_Color(0.2, 0.2, 0.2, 0.5), 0.9);
        PanelFillCS->AddStop(HISSTools_Color(0.075, 0.075, 0.075, 0.6), 1.0);

        PanelFillGreyCS->AddStop(HISSTools_Color(0.05, 0.05, 0.08, 0.3), 0.0);
        PanelFillGreyCS->AddStop(HISSTools_Color(0.05, 0.05, 0.08, 0.4), 1.0);

        matrixFillCS->AddStop(HISSTools_Color(0.3, 0.3, 0.3, 1.0), 0.8);
        matrixFillCS->AddStop(HISSTools_Color(0.1, 0.1, 0.1, 1.0), 1.0);

        matrixHalfFillCS->AddStop(HISSTools_Color(0.3, 0.3, 0.3, 0.7), 0.5);
        matrixHalfFillCS->AddStop(HISSTools_Color(0.1, 0.1, 0.1, 0.7), 1.0);

        handleFillCS->AddStop(HISSTools_Color(0.6, 0.6, 0.6, 1.0), 0.0);
        handleFillCS->AddStop(HISSTools_Color(0.15, 0.15, 0.15, 1.0), 0.9);

        valueFillCS->AddStop(HISSTools_Color(0.3, 0.3, 0.3, 0.75), 0.0);
        valueFillCS->AddStop(HISSTools_Color(0.15, 0.15, 0.15, 0.75), 0.9);

        offHandleFillCS->AddStop(HISSTools_Color(0.415, 0.415, 0.415, 1.0), 0.0);
        offHandleFillCS->AddStop(HISSTools_Color(0.169, 0.169, 0.169, 1.0), 1.0);

        //onHandleFillCS->addStop(HISSTools_Color(0.66, 0.00, 0.00, 1.0), 0);
        //onHandleFillCS->addStop(HISSTools_Color(0.33, 0.00, 0.00, 1.0), 1.);

        onHandleFillCS->AddStop(HISSTools_Color(0.66, 0.66, 0.66, 1.0), 0);
        onHandleFillCS->AddStop(HISSTools_Color(0.33, 0.33, 0.33, 1.0), 1.);

        dialCircleFillGradient->AddStop(HISSTools_Color(0.3, 0.3, 0.3, 0.5), 0.0);
        dialCircleFillGradient->AddStop(HISSTools_Color(0.86, 0.86, 0.86, 0.5), 0.6);   // SUPER LUMINESCENT    (USED in shadow bitmap)

        boxFill->AddStop(HISSTools_Color(0.76, 0.76, 0.76, 1.), 0);
        boxFill->AddStop(HISSTools_Color(0.90, 0.90, 0.90, 1.), 1.);

        dialIndicatorRed->AddStop(HISSTools_Color(0.54, 0, 0, 1), 0.);
        dialIndicatorRed->AddStop(HISSTools_Color(0.91, 0, 0, 1), 0.6);

        dialIndicatorGreen->AddStop(HISSTools_Color(0., 0.42, 0.0, 1.0), 0.);
        dialIndicatorGreen->AddStop(HISSTools_Color(0., 0.70, 0.0, 1.0), 0.6);

        VUOverlay->AddStop(HISSTools_Color(0.6, 0.6, 0.6, 0.4), 0.05);  // SL    (never used on shadow bitmap)
        VUOverlay->AddStop(HISSTools_Color(0.7, 0.7, 0.7, 0.4), 0.6);   // SL    (never used on shadow bitmap)
        VUOverlay->AddStop(HISSTools_Color(0.4, 0.4, 0.4, 0.4), 0.9);

        //VUOverlay->addStop(HISSTools_Color(0.075, 0.267, 0.000, 0.3), 0.000);
        //VUOverlay->addStop(HISSTools_Color(0.220, 0.769, 0.000, 0.3), 0.549);
        //VUOverlay->addStop(HISSTools_Color(1.000, 0.882, 0.169, 0.3), 0.730);
        //VUOverlay->addStop(HISSTools_Color(0.933, 0.800, 0.000, 0.3), 0.789);
        //VUOverlay->addStop(HISSTools_Color(0.769, 0.000, 0.000, 0.3), 0.996);

        VUGradient->AddStop(HISSTools_Color(0.075, 0.267, 0.000, 1.0), 0.000);
        VUGradient->AddStop(HISSTools_Color(0.220, 0.769, 0.000, 1.0), 0.549);
        VUGradient->AddStop(HISSTools_Color(1.000, 0.882, 0.169, 1.0), 0.730);
        VUGradient->AddStop(HISSTools_Color(0.933, 0.800, 0.000, 1.0), 0.789);
        VUGradient->AddStop(HISSTools_Color(0.769, 0.000, 0.000, 1.0), 0.996);

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

/**
 * @brief A static instance of the HISSTools_Design_Scheme initialized with default values.
 *
 * `DefaultDesignScheme` is a static instance of the `HISSTools_Design_Scheme` class
 * that is initialized with default values by passing `true` to the constructor.
 * This instance serves as the default design scheme and can be used as a baseline
 * configuration for other design schemes or components.
 *
 * @note As a static instance, `DefaultDesignScheme` is shared across all instances
 * of the class and persists for the duration of the program's execution.
 */

static HISSTools_Design_Scheme DefaultDesignScheme(true);

#endif /* __HISSTOOLS_DESIGN_SCHEME__ */
