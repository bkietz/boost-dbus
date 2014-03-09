#include <dbus/connection.hpp>
#include <dbus/dbus.h>
#include <iostream>

int main()
{
  DBusError e;
  dbus_bus_get(DBUS_BUS_SYSTEM, &e);

  return 0;
}
