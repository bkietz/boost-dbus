#ifndef DBUS_CONNECTION_HPP
#define DBUS_CONNECTION_HPP

#include <string>
#include <boost/asio.hpp>
//#include <dbus/match.hpp>
//#include <dbus/filter.hpp>
#include <dbus/chrono.hpp>
#include <dbus/message.hpp>
#include <dbus/connection_service.hpp>

namespace dbus {

using std::string;
using namespace boost::asio;

/// Root D-Bus IO object
/**
 * A connection to a bus, through which messages may be sent or received.
 */
class connection
  : public basic_io_object<connection_service>
{
public:

  /// Constructor.
  /**
   * @param io_service The io_service object that the connection will use to
   * wire D-Bus for asynchronous operation.
   */
  explicit connection(io_service& io)
    : basic_io_object<connection_service>(io)
  {
  }

  //TODO: should these be constructors...?

  /// Open a connection to a specified address.
  /**
   * @note This function or open(unsigned) should be called only once to initialize
   * a connection.
   *
   * @param address The address of the bus to connect to.
   *
   * @param shared Set false to open a private connection.
   *
   * @throws boost::system::system_error When opening the connection failed.
   */
  void open(const string& address, bool shared=true)
  {
    this->get_service().open(
      this->get_implementation(),
      address,
      shared);
  }

  /// Open a connection to a well-known bus.
  /**
   * D-Bus connections are usually opened to well-known buses like the
   * system or session bus.
   *
   * @note This function or open(string) should be called only once to initialize
   * a connection.
   *
   * @param bus_type The well-known bus to connect to.
   *
   * @param shared Set false to open a private connection.
   *
   * @throws boost::system::system_error When opening the connection failed.
   */
  // TODO: change this unsigned to an enumeration 
  void open(const unsigned bus_type, bool shared=true)
  {
    this->get_service().open(
      this->get_implementation(),
      bus_type,
      shared);
  }

  //TODO add error_code catchers to all these
  /// Send a message.
  /**
   * @param m The message to send.
   *
   * @return The reply received.
   *
   * @throws boost::system::system_error When the response timed out or
   * there was some other error.
   */
  message send(message& m)
  {
    return this->get_service().send(
      this->get_implementation(),
      m,
      timeout_default());
  }

  /// Send a message.
  /**
   * @param m The message to send.
   *
   * @param t Time to wait for a reply. Passing 0 as the timeout means
   * that you wish to ignore the reply. (Or catch it later somehow...)
   *
   * @return The reply received.
   *
   * @throws boost::system::system_error When the response timed out (if
   * timeout was not 0), or there was some other error.
   */
  template <typename Duration>
  message send(message& m,
      const Duration& t)
  {
    return this->get_service().send(
      this->get_implementation(),
      m,
      t);
  }

  /// Send a message asynchronously.
  /**
   * @param m The message to send.
   *
   * @param handler Handler for the reply.
   *
   * @return Asynchronous result
   */
  template<typename MessageHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(MessageHandler,
      void(boost::system::error_code, message))
  async_send(message& m,
      BOOST_ASIO_MOVE_ARG(MessageHandler) handler)
  {
    return this->get_service().async_send(
      this->get_implementation(),
      m,
      BOOST_ASIO_MOVE_CAST(MessageHandler)(handler));
  }

  /*

  /// Create a new match.
  match new_match(
      BOOST_ASIO_MOVE_ARG(string) expression)
  {
    return match(BOOST_ASIO_MOVE_CAST(string)(expression));
  }

  /// Create a new filter.
  template<typename MessagePredicate>
  filter new_filter(
      BOOST_ASIO_MOVE_ARG(MessagePredicate) p)
  {
    return filter(BOOST_ASIO_MOVE_CAST(MessagePredicate)(p));
  }

  */

};

} // namespace dbus


#endif // DBUS_CONNECTION_HPP
