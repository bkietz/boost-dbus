#ifndef DBUS_MESSAGE_HPP
#define DBUS_MESSAGE_HPP

#include <dbus/dbus.h>
#include <dbus/element.hpp>


namespace dbus {

class message
{
  DBusMessage *message_;
public:
  uint32 serial;

  /// Create a method call message 
  static message new_call(
      const string& process_name,
      const string& object_path,
      const string& interface_name,
      const string& method_name);

  /// Create a method return message 
  static message new_return(); //TODO

  /// Create an error message 
  static message new_error(); //TODO

  /// Create a signal message
  static message new_signal(); //TODO

  message() {}

  /// Wrap an existing DBusMessage *
  explicit message(DBusMessage *m)
    : message_(m)
  {
  }

  operator DBusMessage *()
  {
    return message_;
  }

  struct packer
  {
    DBusMessageIter iter_;
    template<typename Element> packer& pack(Element&);
    template<typename Element> packer& pack_array(Element*, size_t);
    //TODO: add open/close container
  };
  struct unpacker
  {
    DBusMessageIter iter_;
    template<typename Element> unpacker& unpack(Element&);
    template<typename Element> unpacker& unpack_array(Element*&, size_t); //?
  };

  template<typename Element> packer pack(Element&);
  template<typename Element> unpacker unpack(Element&);

};

} // namespace dbus

#include <dbus/impl/packer.ipp>
#include <dbus/impl/unpacker.ipp>
#include <dbus/impl/message.ipp>

#endif // DBUS_MESSAGE_HPP
