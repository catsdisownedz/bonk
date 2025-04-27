#ifndef WEBSOCKETPP_COMPAT_HPP
#define WEBSOCKETPP_COMPAT_HPP

// Compatibility header for modern Boost versions (1.70+)
#include <boost/version.hpp>
#include <boost/asio.hpp>

#if BOOST_VERSION >= 107000
    #include <boost/asio/strand.hpp>
    #include <boost/asio/post.hpp>
    #include <boost/asio/executor.hpp>
    
    // Create compatibility layer for modern Boost
    namespace websocketpp {
        namespace lib {
            namespace asio {
                template <typename Executor>
                using strand = boost::asio::strand<Executor>;
                
                using boost::asio::post;
            }
        }
    }
#endif

#endif // WEBSOCKETPP_COMPAT_HPP
