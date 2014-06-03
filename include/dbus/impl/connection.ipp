// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_CONNECTION_IPP
#define DBUS_CONNECTION_IPP

#include <dbus/dbus.h>
#include <dbus/detail/watch_timeout.hpp>

#include <boost/intrusive_ptr.hpp>

void intrusive_ptr_add_ref(DBusConnection *c)
{
  dbus_connection_ref(c);
}

void intrusive_ptr_release(DBusConnection *c)
{
  dbus_connection_unref(c);
}

namespace dbus {
namespace impl {

class connection
{
public:
  boost::intrusive_ptr<DBusConnection> conn;
  bool is_shared;
  bool is_paused;

  connection()
  {
  }

  connection(boost::asio::io_service& io, int bus, bool shared)
    : is_shared(shared), 
      is_paused(true)
  {
    error e;
    if(is_shared)
    {
      conn = dbus_bus_get((DBusBusType)bus, e);
    }
    else
    {
      conn = dbus_bus_get_private((DBusBusType)bus, e);
    }
    e.throw_if_set();

    dbus_connection_set_exit_on_disconnect(conn.get(), false);

    detail::set_watch_timeout_dispatch_functions(conn.get(), io);
  }

  connection(boost::asio::io_service& io, const string& address, bool shared)
    : is_shared(shared), 
      is_paused(true)
  {
    error e;
    if(shared)
    {
      conn = dbus_connection_open(address.c_str(), e);
    }
    else
    {
      conn = dbus_connection_open_private(address.c_str(), e);
    }
    e.throw_if_set();

    dbus_bus_register(conn.get(), e);
    e.throw_if_set();

    dbus_connection_set_exit_on_disconnect(conn.get(), false);

    detail::set_watch_timeout_dispatch_functions(conn.get(), io);
  }

  ~connection()
  {
    if(!is_shared) dbus_connection_close(conn.get());
  }

  operator DBusConnection *() 
  { 
    return conn.get();
  }
  operator const DBusConnection *() const 
  { 
    return conn.get();
  }

  // begin asynchronous operation
  //FIXME should not get io from an argument
  void start(boost::asio::io_service& io)
  {
    if(is_paused)
    {
      is_paused = false;
      io.post(detail::dispatch_handler(io, conn.get()));
    }
  }
};

} // namespace impl
} // namespace dbus

#endif // DBUS_CONNECTION_IPP
