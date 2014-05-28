// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_PACKER_IPP
#define DBUS_PACKER_IPP

namespace dbus {

message::packer::packer(message& m)
{
  dbus_message_iter_init_append(m, &iter_);
}

namespace impl {

template<typename Element> struct append_one
{
  //TODO: throw if invalid Element
  append_one(message::packer& p, const Element& e)
  {
    dbus_message_iter_append_basic(&p.iter_, 
        element<Element>::code, &e);
  }
};

template<> struct append_one<string>
{
  append_one(message::packer& p, const string& e)
  {
    const char *c = e.c_str();
    dbus_message_iter_append_basic(&p.iter_, 
        element<string>::code, &c);
  }
};

/*
template<> struct append_one<const char *>
{
  append_one(message::packer& p, const char *e)
  {
    dbus_message_iter_append_basic(&p.iter_, 
        element<string>::code, &e);
  }
};
*/

} // namespace impl


#include <iostream>
template<typename Element>
message::packer& message::packer::pack(const Element& e)
{
  impl::append_one<Element>(*this, e);
  return *this;
}


} // namespace dbus

#endif // DBUS_PACKER_IPP
