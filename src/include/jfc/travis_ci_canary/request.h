// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_TRAVIS_CI_CANARY_REQUEST_H
#define JFC_TRAVIS_CI_CANARY_REQUEST_H

#include <jfc/travis_ci_canary/enums.h>

#include <string>
#include <functional>

/// \brief functions used to interact with the travis-ci api
namespace jfc::travis_ci_canary::request
{
    /// \brief functor type for handling successful requests
    using response_handler_type = std::function<void(std::vector<unsigned char>)>;
    
    /// \brief functor type for handling failed requests
    using failed_handler_type = std::function<void()>;

    /// \brief retrieves a list of recent builds from the travis-ci api
    void builds(std::string aTravisToken, response_handler_type aSucceededHandler,
        failed_handler_type aFailedHandler);
}

#endif

