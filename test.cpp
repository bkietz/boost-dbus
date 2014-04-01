#include <dbus/connection.hpp>
#include <dbus/message.hpp>
#include <dbus/filter.hpp>
#include <dbus/match.hpp>
#include <iostream>

//TODO: use an actual test suite
int main()
{
  boost::asio::io_service io;
  dbus::connection system_bus(io, dbus::bus::system);

  dbus::message m = dbus::message::new_call(
      "org.freedesktop.Avahi",
      "/",
      "org.freedesktop.Avahi.Server",
      "GetHostName");

  dbus::message r;
  dbus::string hostname;
  using std::cout;
  using std::endl;

  r = system_bus.send(m);
  r.unpack(hostname);

  cout << hostname << endl;

  dbus::message m2 = dbus::message::new_call(
      "org.freedesktop.Avahi",
      "/",
      "org.freedesktop.Avahi.Server",
      "ServiceBrowserNew");

  dbus::string type_("_http._tcp"), domain_("local");

  m2.pack(-1).pack(-1).pack(type_).pack("local").pack<dbus::uint32>(0);

//  cout << (char)dbus::element< const char * >::code << endl;

  r = system_bus.send(m2, dbus::chrono::seconds(1));
  dbus::string browser_path;
  r.unpack(browser_path);

  cout << browser_path << endl;

  dbus::match ma(system_bus, "type='signal',path='" + browser_path + "'");

  dbus::filter f(system_bus, [](dbus::message m){ return true; });

  f.async_dispatch([](boost::system::error_code ec, dbus::message m){
    dbus::int32 i;
    m.unpack(i);
    cout << i << endl;
  });

  return 0;
}
