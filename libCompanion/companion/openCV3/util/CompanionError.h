#ifndef COMPANION_ERROR_H
#define COMPANION_ERROR_H

/**
 * Enumeration namespace for errors.
 *
 * @author Andreas Sekulski
 */
namespace CompanionError
{
    /// Enumeration class for error codes
    enum class error_code
    {
        image_not_found, ///< If image not found error.
        dimension_error, ///< If dimensions from given images inequal.
        template_dimension_error, ///< If dimensions from template is wrong.
        feature_detector_not_found, ///< If given feature detector not supported.
        descriptor_extractor_not_found,  ///< If given descriptor extractor not supported.
        descriptor_matcher_not_found ///< If given descriptor matcher not supported.
    };
}

#endif //COMPANION_ERROR_H