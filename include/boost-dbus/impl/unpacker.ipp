/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_UNPACKER_IPP
#define DBUS_UNPACKER_IPP

#include <boost/utility/enable_if.hpp>

#include "../element.hpp"

namespace dbus {

inline
message::unpacker::unpacker(message& m)
{
  impl::message_iterator::init(m, iter_);
}

template<typename Element>
inline typename boost::enable_if<is_fixed_type<Element>, message::unpacker&>::type
operator>>(message::unpacker& u, Element& e)
{
  u.iter_.get_basic(&e);
  u.iter_.next();
  return u;
}

inline message::unpacker&
operator>>(message::unpacker& u, string& s)
{
  const char *c;
  u.iter_.get_basic(&c);
  s.assign(c);
  u.iter_.next();
  return u;
}

} // namespace dbus

#endif // DBUS_UNPACKER_IPP
