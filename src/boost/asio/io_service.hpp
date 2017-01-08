#pragma once

// boost SHIM lib, not full boost! experimental!
// large chunks copied straight out of non-boost ASIO lib
// https://github.com/chriskohlhoff/asio/blob/master/asio/include/asio/io_context.hpp

//#include <cstddef>
#include <fact/std/cstddef.hpp>

#define ASIO_DECL

namespace fstd = FactUtilEmbedded::std;

namespace boost {

namespace asio {

class io_context;

class noncopyable {};

class io_context : private noncopyable
{
public:
    /// The type used to count the number of handlers executed by the context.
    typedef std::size_t count_type;

    /// Constructor.
    ASIO_DECL io_context();

    /// Constructor.
    /**
     * Construct with a hint about the required level of concurrency.
     *
     * @param concurrency_hint A suggestion to the implementation on how many
     * threads it should allow to run simultaneously.
     */
    ASIO_DECL explicit io_context(int concurrency_hint);
};


// recently io_service was renamed to io_context
typedef io_context io_service;


}
}

#include "boost/asio/impl/io_context.hpp"
#include "boost/asio/impl/io_context.ipp"
