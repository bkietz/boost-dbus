#include <dbus/dbus.h>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace dbus {

class error
{
  DBusError *error_;

public:
  error()
  {
    dbus_error_init(error_);
  }

  bool is_set();
  error_code to_error_code();
  system_error to_system_error();
};

} // namespace dbus
