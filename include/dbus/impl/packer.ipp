// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_PACKER_IPP
#define DBUS_PACKER_IPP

namespace dbus {

message::packer::packer(message& m)
{
  impl::message_iterator::init_append(m, iter_);
}

template<typename Element>
message::packer& operator<<(message::packer& p, Element e)
{
  p.iter_.append_basic(element<Element>::code, &e);
  return p;
}

template<>
message::packer& operator<<(message::packer& p, string e)
{
  const char *c = e.c_str();
  p.iter_.append_basic(element<string>::code, &c);
  return p;
}

template<>
message::packer& operator<<(message::packer& p, const char *c)
{
  p.iter_.append_basic(element<string>::code, &c);
  return p;
}

} // namespace dbus

#endif // DBUS_PACKER_IPP
