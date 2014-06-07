// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <dbus/connection.hpp>
#include <dbus/message.hpp>
#include <dbus/endpoint.hpp>
#include <dbus/filter.hpp>
#include <dbus/match.hpp>
#include <dbus/functional.hpp>

#include <gtest/gtest.h>
#include <unistd.h>
#include <utility/record_property.hpp>


using namespace boost::asio;
using namespace dbus;
using boost::system::error_code;


class AvahiTest
  : public testing::Test
{
protected:
  static void SetUpTestCase()
  {
  }
  static io_service io;
  static string browser_path;
  static endpoint avahi_daemon;
};
// It seems like these should be non-static,
// but I get a mysterious SEGFAULT for io
//   ¿related: http://stackoverflow.com/questions/18009156/boost-asio-segfault-no-idea-why
io_service AvahiTest::io;
string AvahiTest::browser_path;
endpoint AvahiTest::avahi_daemon(
  "org.freedesktop.Avahi",
  "/",
  "org.freedesktop.Avahi.Server");

struct compare_hostnames
{
  io_service& io;
  connection& system_bus;

  void operator()(error_code ec, message r)
  {
    string avahi_hostname;
    string unix_hostname;

    {
      // get hostname from a system call
      char c[1024];
      gethostname(c, 1024);
      unix_hostname = c;
    }

    r.unpack(avahi_hostname);

    // this is only usually accurate
    EXPECT_EQ(unix_hostname, avahi_hostname);

    // eventually, connection should stop itself
    io.stop();
  }
};

bool member_is_itemnew(message& m)
{
  return m.get_member() == "ItemNew";
}

struct handle_dispatch
{
  io_service& io;

  void operator()(error_code ec, message s)
  {
    record_property("firstSignal") << s.get_member();
    io.stop();
  }
};

TEST_F(AvahiTest, GetHostName)
{
  connection system_bus(io, "unix:path=/var/run/dbus/system_bus_socket");

  {
  message m = message::new_call(
    avahi_daemon,
    "GetHostName");

  system_bus.async_send(m,
      (compare_hostnames){ io, system_bus });
  }

  io.run();
/*
}


TEST_F(AvahiTest, ServiceBrowser)
{
  connection system_bus(io, bus::system);
  */

  {
  // create new service browser
  message m = message::new_call(
    avahi_daemon,
    "ServiceBrowserNew");

  m.pack<int32>(-1)
   .pack<int32>(-1)
   .pack<string>("_http._tcp")
   .pack<string>("local")
   .pack<uint32>(0);

  message r = system_bus.send(m);

  r.unpack(browser_path);
  }
  record_property("browserPath") << browser_path;
  // RegEx match browser_path
  // catch a possible exception
  /*
}



TEST_F(AvahiTest, BrowseForHttp)
{
  */
  io.reset();
  //connection system_bus1(io, bus::system);

  match m(system_bus, "type='signal',path='" + browser_path + "'");
  filter f(system_bus, member_is_itemnew);

  f.async_dispatch(
      (handle_dispatch){ io });

  io.run();
}
