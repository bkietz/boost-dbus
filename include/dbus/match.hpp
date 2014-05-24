// Copyright (c) Benjamin Kietzman (github.com/bkietz)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef DBUS_MATCH_HPP
#define DBUS_MATCH_HPP

#include <string>
#include <boost/asio.hpp>

#include <dbus/error.hpp>
#include <dbus/connection.hpp>

namespace dbus {

/// Simple placeholder object for a match rule.
/**
 * A match rule determines what messages will be received by this application.
 *
 * Each rule will be represented by an instance of match. To remove that rule,
 * dispose of the object.
 */
//TODO use noncopyable
class match
{
  connection& connection_;
  std::string expression_;

public:
  match(connection& c, 
      BOOST_ASIO_MOVE_ARG(std::string) e)
    : connection_(c),
      expression_(BOOST_ASIO_MOVE_CAST(std::string)(e))
  {
    connection_.new_match(*this);
  }

  ~match()
  {
    connection_.delete_match(*this);
  }

  const std::string& get_expression() const { return expression_; }
};

//TODO move this to dbus::impl stat
void connection_service::new_match(implementation_type& impl,
    match& m)
{
  error e;
  dbus_bus_add_match(impl, m.get_expression().c_str(), e);
  e.throw_if_set();
  // eventually, for complete asynchronicity, this should connect to
  // org.freedesktop.DBus and call AddMatch
}

void connection_service::delete_match(implementation_type& impl,
    match& m)
{
  error e;
  dbus_bus_remove_match(impl, m.get_expression().c_str(), e);
  e.throw_if_set();
}

} // namespace dbus

#endif // DBUS_MATCH_HPP
