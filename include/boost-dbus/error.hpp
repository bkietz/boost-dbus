// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_ERROR_HPP
#define DBUS_ERROR_HPP

#include <dbus/dbus.h>

#include "element.hpp"
#include "message.hpp"

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace dbus {

class error
  : public boost::system::error_category
{
  DBusError error_;

public:
  error()
  {
    dbus_error_init(&error_);
  }

  error(DBusError *src)
  {
    dbus_error_init(&error_);
    dbus_move_error(src, &error_);
  }

  error(dbus::message& m)
  {
    dbus_error_init(&error_);
    dbus_set_error_from_message(&error_, m);
  }

  ~error()
  {
    dbus_error_free(&error_);
  }

  virtual const char *name() const BOOST_SYSTEM_NOEXCEPT
  {
    return "DBus Error";
  }

  string message(int value) const
  {
    return "";
  }

  bool is_set() const
  {
    return dbus_error_is_set(&error_);
  }

  operator const DBusError *() const
  {
    return &error_;
  }

  operator DBusError *()
  {
    return &error_;
  }

  boost::system::error_code error_code() const;
  boost::system::system_error system_error() const;
  void throw_if_set() const;
};

inline const boost::system::error_category&
dbus_category() BOOST_SYSTEM_NOEXCEPT
{
  static error e;
  return e;
}

inline
boost::system::error_code error::error_code() const
{
  return boost::system::error_code(is_set(), dbus_category());
}

inline
boost::system::system_error error::system_error() const
{
  return boost::system::system_error(error_code(),
                                     std::string(error_.name) + ": " + error_.message);
}

inline
void error::throw_if_set() const
{
  if(is_set()) throw system_error();
}

} // namespace dbus

#endif // DBUS_ERROR_HPP
