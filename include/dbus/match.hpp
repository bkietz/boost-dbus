#ifndef DBUS_MATCH_HPP
#define DBUS_MATCH_HPP

#include <string>
#include <boost/asio.hpp>

namespace dbus {

class connection;

void delete_match(connection&, std::string);

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

  match(connection& c, 
      BOOST_ASIO_MOVE_ARG(std::string) e)
    : connection_(c),
      expression_(BOOST_ASIO_MOVE_CAST(std::string)(e))
  {}

public:
  friend class connection;

  const std::string& get_expression() const { return expression_; }
  connection& get_connection() { return connection_; }
  ~match();
};

} // namespace dbus

#endif // DBUS_MATCH_HPP
