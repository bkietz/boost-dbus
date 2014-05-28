// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <dbus/message.hpp>


class MessageTest
  : public testing::Test
{
};


TEST_F(MessageTest, ConstMessage)
{
  using namespace dbus;

  const message m = message::new_call(
    "org.freedesktop.Avahi",
    "/",
    "org.freedesktop.Avahi.Server",
    "GetHostName");

  ASSERT_EQ("org.freedesktop.Avahi", m.get_destination());
  ASSERT_EQ("/", m.get_path());
  ASSERT_EQ("org.freedesktop.Avahi.Server", m.get_interface());
  ASSERT_EQ("GetHostName", m.get_member());

  // m.get_sender();
}
