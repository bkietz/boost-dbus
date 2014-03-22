#ifndef DBUS_CHRONO_HPP
#define DBUS_CHRONO_HPP

#ifdef BOOST_ASIO_HAS_STD_CHRONO
#include <chrono>
namespace dbus {
namespace chrono = std::chrono;
}
#else
#include <boost/chrono.hpp>
namespace dbus {
namespace chrono = boost::chrono;
}
#endif

#endif // DBUS_CHRONO_HPP
