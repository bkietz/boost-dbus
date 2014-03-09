#include <boost/asio.hpp>

namespace dbus {

class connection;

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
      expression_(BOOST_ASIO_MOVE_CAST(e))
  {
    // dbus_bus_add_match
  }

  match() {}

public:
  friend class connection;

  ~match()
  {
    // dbus_bus_remove_match
  }

};

} // namespace dbus
