// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_ELEMENT_HPP
#define DBUS_ELEMENT_HPP

#include <dbus/dbus.h>
#include <string>

namespace dbus {

/// Message elements
/**
 * D-Bus Messages are composed of simple elements of one of these types
 */
typedef std::string   string;
typedef dbus_int32_t  int32;
typedef dbus_uint32_t uint32;
//TODO: add more types...

/// Traits template for message elements
/**
 * D-Bus Message elements are identified by unique integer type codes.
 */
template<typename InvalidType=void> struct element
{
  static const int code = DBUS_TYPE_INVALID;
};

template<> struct element< int32 >
{
  static const int code = DBUS_TYPE_INT32;
};

template<> struct element< uint32 >
{
  static const int code = DBUS_TYPE_UINT32;
};

template<> struct element< string >
{
  static const int code = DBUS_TYPE_STRING;
};

/// Convenience overloads
template<> struct element< char * >
{
  static const int code = DBUS_TYPE_STRING;
};
template<> struct element< const char * >
{
  static const int code = DBUS_TYPE_STRING;
};

} // namespace dbus


#endif // DBUS_ELEMENT_HPP
