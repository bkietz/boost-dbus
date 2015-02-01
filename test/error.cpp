// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <dbus/connection.hpp>
#include <dbus/message.hpp>
#include <dbus/endpoint.hpp>
#include <dbus/filter.hpp>
#include <dbus/match.hpp>
#include <dbus/functional.hpp>

#include <gtest/gtest.h>
#include <unistd.h>
#include <utility/record_property.hpp>


using namespace boost::asio;
using namespace dbus;
using boost::system::error_code;


class ErrorTest
  : public testing::Test
{
protected:
  static void SetUpTestCase()
  {
  }
  static io_service io;
};
// It seems like these should be non-static,
// but I get a mysterious SEGFAULT for io
//   ¿related: http://stackoverflow.com/questions/18009156/boost-asio-segfault-no-idea-why
io_service ErrorTest::io;

TEST_F(ErrorTest, GetHostName)
{
  EXPECT_THROW(connection system_bus(io, "unix:path=/foo/bar/baz_socket"), boost::system::system_error);

  io.run();
}
