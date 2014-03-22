#ifndef DBUS_FILTER_HPP
#define DBUS_FILTER_HPP

#include <functional>
#include <dbus/detail/queue.hpp>
#include <dbus/message.hpp>
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
  std::function<bool(message)> predicate_;
  detail::queue<message> queue_;

  template<typename MessagePredicate>
  filter(connection& c,
      BOOST_ASIO_MOVE_ARG(MessagePredicate) p)
    : connection_(c),
      predicate_(BOOST_ASIO_MOVE_CAST(MessagePredicate)(p))
  {}

public:
  friend class connection;

  filter(const filter& f)
    : connection_(f.connection_),
      predicate_(f.predicate_),
      queue_(f.queue_)
  {}

  bool offer(message m)
  { 
      bool passed = predicate_(m);
      queue_.push(m);
      return passed; 
  }
  connection& get_connection() { return connection_; }
  ~filter();

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


#endif // DBUS_FILTER_HPP
