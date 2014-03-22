#include <dbus/connection.hpp>
#include <dbus/message.hpp>
#include <dbus/dbus.h>
#include <iostream>

int main()
{
  boost::asio::io_service io;
  dbus::connection system_bus(io);
  system_bus.open(DBUS_BUS_SYSTEM);

  dbus::message m = dbus::message::new_call(
      "org.freedesktop.Avahi",
      "/",
      "org.freedesktop.Avahi.Server",
      "GetHostName");

  dbus::message r = system_bus.send(m);
  dbus::string hostname;
  r.unpack(hostname);

  using std::cout; using std::endl;
  cout << hostname << endl;

  return 0;
}
