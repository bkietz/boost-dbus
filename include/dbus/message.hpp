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
  static message new_return(); 

  /// Create an error message 
  static message new_error();

  /// Create a signal message
  static message new_signal();

  message() {}

  message(DBusMessage *m)
    : message_(dbus_message_ref(m))
  {
  }

  ~message()
  {
    dbus_message_unref(message_);
  }

  operator DBusMessage *()
  {
    return message_;
  }

  operator const DBusMessage *() const
  {
    return message_;
  }

  struct packer
  {
    DBusMessageIter iter_;
    template<typename Element> packer& pack(const Element&);
    template<typename Element> packer& pack_array(const Element*, size_t);
  };
  struct unpacker
  {
    DBusMessageIter iter_;
    /// return type code of the next element in line for unpacking
    int code();
    template<typename Element> unpacker& unpack(Element&);
    int array_code();
    template<typename Element> unpacker& unpack_array(Element*&, size_t);
  };

  template<typename Element> packer pack(const Element&);
  template<typename Element> unpacker unpack(Element&);

};

} // namespace dbus

#include <dbus/impl/packer.ipp>
#include <dbus/impl/unpacker.ipp>
#include <dbus/impl/message.ipp>

#endif // DBUS_MESSAGE_HPP
