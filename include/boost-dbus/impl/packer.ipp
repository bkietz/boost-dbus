/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_PACKER_IPP
#define DBUS_PACKER_IPP

#include <boost/utility/enable_if.hpp>

#include "../element.hpp"

namespace dbus {

inline
message::packer::packer(message& m)
{
  impl::message_iterator::init_append(m, iter_);
}

template<typename Element>
inline typename boost::enable_if<is_fixed_type<Element>, message::packer&>::type
operator<<(message::packer& p, const Element& e)
{
  p.iter_.append_basic(element<Element>::code, &e);
  return p;
}

inline
message::packer& operator<<(message::packer& p, const char *c)
{
  p.iter_.append_basic(element<string>::code, &c);
  return p;
}

inline
message::packer& operator<<(message::packer& p, const string& e)
{
  const char *c = e.c_str();
  return p << c;
}

} // namespace dbus

#endif // DBUS_PACKER_IPP
