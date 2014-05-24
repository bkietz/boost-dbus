// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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
