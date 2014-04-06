#include <iostream>
using std::cout; using std::endl;
#include <dbus/connection.hpp>
#include <dbus/message.hpp>
#include <dbus/filter.hpp>
#include <dbus/match.hpp>
#include <dbus/functional.hpp>

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

  dbus::string hostname;
  {
  using dbus::message;
  using boost::system::error_code;
  system_bus.async_send(m,[&](error_code ec, message r){  
      r.unpack(hostname);
      cout << hostname << endl;
    });
  }

  dbus::message m2 = dbus::message::new_call(
      "org.freedesktop.Avahi",
      "/",
      "org.freedesktop.Avahi.Server",
      "ServiceBrowserNew");

  m2.pack<dbus::int32>(-1)
    .pack<dbus::int32>(-1)
    .pack<dbus::string>("_http._tcp")
    .pack<dbus::string>("local")
    .pack<dbus::uint32>(0);

  dbus::string browser_path;
  {
  dbus::message r;
  r = system_bus.send(m2);//, dbus::chrono::seconds(1));
  r.unpack(browser_path);
  cout << browser_path << endl;
  }

  dbus::match ma(system_bus, "type='signal',path='" + browser_path + "'");

  dbus::filter f(system_bus, [](dbus::message& m){
    return m.get_member() == "ItemNew";  });

  dbus::function<void(boost::system::error_code, dbus::message)> dispatch_handler;

  dispatch_handler = [&](boost::system::error_code ec, dbus::message m){
    dbus::int32 i;
    dbus::string name_, type_, domain_;
    dbus::uint32 u;
    m.unpack(i).unpack(i).unpack(name_).unpack(type_).unpack(domain_).unpack(u);
    cout << name_ <<'.'<< type_ <<'.'<< domain_ << endl;
    f.async_dispatch(dispatch_handler);
  };
  f.async_dispatch(dispatch_handler);

  io.run();
  return 0;
}
