#include <dbus/detail/queue.hpp>
#include <boost/asio.hpp>

namespace dbus {

class connection;

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

} // namespace dbus
