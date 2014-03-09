#ifndef DBUS_MESSAGE_HPP
#define DBUS_MESSAGE_HPP

#include <dbus/dbus.h>

class message
{
  DBusMessage *message_;
public:
  unsigned serial;
  operator DBusMessage *()
  {
    return message_;
  }
};

#endif // DBUS_MESSAGE_HPP
