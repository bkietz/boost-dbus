/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <iostream>

#include <boost/asio.hpp>

#include <boost-dbus/connection.hpp>
#include <boost-dbus/message.hpp>
#include <boost-dbus/endpoint.hpp>
#include <boost-dbus/filter.hpp>
#include <boost-dbus/match.hpp>
#include <boost-dbus/functional.hpp>

int
main()
{
  boost::asio::io_service io;
  dbus::connection bus(io, dbus::bus::system);

  dbus::message msg = dbus::message::new_call(
    dbus::endpoint("org.freedesktop.DBus",
                   "/org/freedesktop/DBus",
                   "org.freedesktop.DBus"),
    "Hello");
  bus.async_send(msg, [&] (const boost::system::error_code& ec, dbus::message& reply) {

      std::cout << reply << std::endl;
      std::string replyContent;
      reply.unpack(replyContent);

      std::cout << replyContent << std::endl;
      io.stop();
    });

  io.run();
  return 0;
}
