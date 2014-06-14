// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_UNPACKER_IPP
#define DBUS_UNPACKER_IPP

namespace dbus {

message::unpacker::unpacker(message& m)
{
  impl::message_iterator::init(m, iter_);
}

namespace impl {

template<typename Element> struct get_one
{
  //TODO: throw if invalid Element
  get_one(message::unpacker& u, Element& e)
  {
    u.iter_.get_basic(&e);
    u.iter_.next();
  }
};

template<> struct get_one<string>
{
  get_one(message::unpacker& u, string& e)
  {
    const char *c;
    u.iter_.get_basic(&c);
    e = c;
    u.iter_.next();
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
  return iter_.get_arg_type();
}


} // namespace dbus

#endif // DBUS_UNPACKER_IPP
