#include <dbus/connection.hpp>
#include <dbus/message.hpp>
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

  dbus::int32  if_(-1), proto_(-1);
  dbus::string type_("_http._tcp"), domain_("local");
  dbus::uint32 flags_(0);
  dbus::string browser_path;

  m2.pack(if_).pack(proto_).pack(type_).pack(domain_).pack(flags_);

  r = system_bus.send(m2, dbus::chrono::seconds(1));
  r.unpack(browser_path);

  cout << browser_path << endl;

  dbus::match match = system_bus.new_match(
      "type='signal',path='" + browser_path + "'"
      );

  dbus::filter f = system_bus.new_filter([](dbus::message m){ return true; });

  f.async_pop([](boost::system::error_code ec, dbus::message m){
    dbus::int32 i;
    m.unpack(i);
    cout << i << endl;
  });

  return 0;
}
