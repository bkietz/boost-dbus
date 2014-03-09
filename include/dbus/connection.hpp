#include <string>
#include <boost/asio.hpp>
#include "connection_service.hpp"

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

  //TODO add error_code catchers to all these...

  /// Send a message.
  /**
   * @param m The message to send.
   *
   * @param timeout Time to wait for a reply. Passing 0 as the timeout means
   * that you wish to ignore the reply. (Or catch it later somehow...)
   *
   * @return The reply received.
   *
   * @throws boost::system::system_error When the response timed out, if
   * timeout was not 0.
   */
  template <typename Duration>
  message send(message& m,
	  const Duration& timeout = timeout_default())
  {
    return this->get_service().send(
      this->get_implementation(),
      m,
      timeout);
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

  //TODO figure out incomplete type knot without inlining class declaration.
  //TODO use noncopyable

  /// Simple placeholder object for a match rule.
  /**
   * A match rule determines what messages will be received by this application.
   *
   * Each rule will be represented by an instance of match. To remove that rule,
   * dispose of the object.
   */
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

  /// Create a new match.
  match new_match(
	  BOOST_ASIO_MOVE_ARG(string) expression)
  {
    return this->get_service().new_match(
      this->get_implementation(),
      BOOST_ASIO_MOVE_CAST(string)(expression));
  }

  /// Represents a filter of incoming messages.
  /**
   * Filters examine incoming messages, demuxing them to multiple queues.
   */
  class filter
  {
	connection& connection_;
	function<bool(message)> predicate_;
	detail::queue<message> queue_;

    template<typename MessagePredicate>
    filter(connection& c,
        BOOST_ASIO_MOVE_ARG(MessagePredicate) p)
      : connection_(c),
		predicate_(BOOST_ASIO_MOVE_CAST(MessagePredicate)(p))
    {
	  // dbus_connection_add_filter
    }

	filter() {}

  public:
	friend class connection;

	~filter()
	{
	  // dbus_connection_remove_filter
	}

    template<typename MessageHandler>
    inline BOOST_ASIO_INITFN_RESULT_TYPE(MessageHandler,
        void(boost::system::error_code, message))
    async_dispatch(
        BOOST_ASIO_MOVE_ARG(MessageHandler) handler)
    {
	  return queue_.async_pop(
		BOOST_ASIO_MOVE_CAST(MessageHandler)(handler));
	}

  };

  /// Create a new filter.
  template<typename MessagePredicate>
  filter new_filter(
	  BOOST_ASIO_MOVE_ARG(MessagePredicate) p)
  {
    return this->get_service().new_match(
      this->get_implementation(),
      BOOST_ASIO_MOVE_CAST(MessagePredicate)(p));
  }

};

} // namespace dbus
