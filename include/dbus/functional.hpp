// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_FUNCTIONAL_HPP
#define DBUS_FUNCTIONAL_HPP

/*
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
*/

// The above was not working because of
// reference_wrapper being declared in std::tr1.
// I suspect this is a bug in BOOST_HAS_TR1_REFERENCE_WRAPPER
#include <boost/function.hpp>
namespace dbus {
using boost::function;
}


#endif // DBUS_FUNCTIONAL_HPP
