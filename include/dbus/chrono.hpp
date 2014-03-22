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

namespace dbus {

int timeout_default()
{
  return -1;
}

} // dbus

#endif // DBUS_CHRONO_HPP
