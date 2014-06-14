// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_MESSAGE_HPP
#define DBUS_MESSAGE_HPP

#include <dbus/dbus.h>
#include <dbus/element.hpp>
#include <dbus/endpoint.hpp>
#include <boost/intrusive_ptr.hpp>
#include <dbus/impl/message_iterator.hpp>

void intrusive_ptr_add_ref(DBusMessage *m)
{
  dbus_message_ref(m);
}

void intrusive_ptr_release(DBusMessage *m)
{
  dbus_message_unref(m);
}

namespace dbus {

class message
{
  boost::intrusive_ptr<DBusMessage> message_;
public:

  /// Create a method call message 
  static message new_call(
      const endpoint& destination,
      const string& method_name);

  /// Create a method return message 
  static message new_return(
      message& call); 

  /// Create an error message 
  static message new_error(
      message& call,
      const string& error_name,
      const string& error_message);

  /// Create a signal message
  static message new_signal(
      const endpoint& origin,
      const string& signal_name);

  message() {}

  message(DBusMessage *m)
    : message_(dbus_message_ref(m))
  {
  }

  operator DBusMessage *()
  {
    return message_.get();
  }

  operator const DBusMessage *() const
  {
    return message_.get();
  }

  string get_path() const
  {
    return dbus_message_get_path(message_.get());
  }

  string get_interface() const
  {
    return dbus_message_get_interface(message_.get());
  }

  string get_member() const
  {
    return dbus_message_get_member(message_.get());
  }

  string get_sender() const
  {
    return dbus_message_get_sender(message_.get());
  }

  string get_destination() const
  {
    return dbus_message_get_destination(message_.get());
  }

  uint32 get_serial()
  {
    return dbus_message_get_serial(message_.get());
  }

  message& set_serial(uint32 serial)
  {
    dbus_message_set_serial(message_.get(), serial);
    return *this;
  }

  uint32 get_reply_serial()
  {
    return dbus_message_get_reply_serial(message_.get());
  }

  message& set_reply_serial(uint32 reply_serial)
  {
    dbus_message_set_reply_serial(message_.get(), reply_serial);
    return *this;
  }

  struct packer
  {
    packer(message&);
    impl::message_iterator iter_;
    template<typename Element> packer& pack(const Element&);
    template<typename Element> packer& pack_array(const Element*, size_t);
  };
  struct unpacker
  {
    unpacker(message&);
    impl::message_iterator iter_;
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
#include <dbus/impl/message_iterator.ipp>

#endif // DBUS_MESSAGE_HPP
