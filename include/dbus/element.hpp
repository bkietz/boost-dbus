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
template<typename InvalidType = void>
struct element
{
  static const int invalid = DBUS_TYPE_INVALID;
  static const int string  = DBUS_TYPE_STRING;
  static const int int32   = DBUS_TYPE_INT32;
  static const int uint32  = DBUS_TYPE_UINT32;

  static const int code    = invalid;
};

template<> struct element< int32 >
{
  static const int code = element<>::int32;
};

template<> struct element< uint32 >
{
  static const int code = element<>::uint32;
};

template<> struct element< string >
{
  static const int code = element<>::string;
};

/// Convenience overload
template<> struct element< const char * >
{
  static const int code = element<>::string;
};

} // namespace dbus


#endif // DBUS_ELEMENT_HPP
