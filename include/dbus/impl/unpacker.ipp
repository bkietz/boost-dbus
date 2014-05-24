// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_UNPACKER_IPP
#define DBUS_UNPACKER_IPP

namespace dbus {

namespace impl {

template<typename Element> struct get_one
{
  //TODO: throw if invalid Element
  get_one(message::unpacker& u, Element& e)
  {
    dbus_message_iter_get_basic(&u.iter_, &e);
    dbus_message_iter_next(&u.iter_);
  }
};

template<> struct get_one<string>
{
  get_one(message::unpacker& u, string& e)
  {
    const char *c;
    dbus_message_iter_get_basic(&u.iter_, &c);
    e = c;
    dbus_message_iter_next(&u.iter_);
  }
};

} // namespace impl


template<typename Element>
message::unpacker& message::unpacker::unpack(Element& e)
{
  impl::get_one<Element>(*this, e);
  return *this;
}

int message::unpacker::code()
{
  return dbus_message_iter_get_arg_type(&iter_);
}


} // namespace dbus

#endif // DBUS_UNPACKER_IPP
