#ifndef DBUS_FUNCTIONAL_HPP
#define DBUS_FUNCTIONAL_HPP

#ifndef BOOST_NO_CXX11_HDR_FUNCTIONAL
#include <functional>
namespace dbus {
using std::function;
}
#else
#include <boost/tr1/functional.hpp>
namespace dbus {
using std::tr1::function;
}
#endif

#endif // DBUS_FUNCTIONAL_HPP
