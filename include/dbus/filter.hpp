#ifndef DBUS_FILTER_HPP
#define DBUS_FILTER_HPP

#include <boost/asio.hpp>
#include <dbus/detail/queue.hpp>
#include <dbus/message.hpp>
#include <dbus/connection.hpp>
#include <dbus/functional.hpp>

namespace dbus {

/// Represents a filter of incoming messages.
/**
 * Filters examine incoming messages, demuxing them to multiple queues.
 */
class filter
{
  connection& connection_;
  function<bool(message)> predicate_;
  detail::queue<message> queue_;

public:
  // friend DBusHandlerResult connection_service::filter_callback(
  //     DBusConnection *, DBusMessage *, void *);

  bool offer(message m)
  { 
    bool passed = predicate_(m);
    queue_.push(m);
    return passed; 
  }

  template<typename MessagePredicate>
  filter(connection& c,
      BOOST_ASIO_MOVE_ARG(MessagePredicate) p)
    : connection_(c),
      predicate_(BOOST_ASIO_MOVE_CAST(MessagePredicate)(p)),
      queue_(connection_.get_io_service())
  {
    connection_.new_filter(*this);
  }

  ~filter()
  {
    connection_.delete_filter(*this);
  }
 
  // connection& get_connection() { return connection_; }

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

//TODO move this to dbus::impl stat
DBusHandlerResult connection_service::filter_callback(
    DBusConnection *c,
    DBusMessage *m,
    void *userdata)
{
  try
  {
    filter& f = *static_cast<filter *>(userdata);
    if(f.offer(message(m)))
    {
      return DBUS_HANDLER_RESULT_HANDLED;
    }
  } catch(...) {
    // do not throw in C callbacks. Just don't.
  }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

void connection_service::new_filter(implementation_type& impl,
    filter& f)
{
  dbus_connection_add_filter(impl,
      &filter_callback, &f, NULL);
}
  
void connection_service::delete_filter(implementation_type& impl,
    filter& f)
{
  dbus_connection_remove_filter(impl,
      &filter_callback, &f);
}


} // namespace dbus


#endif // DBUS_FILTER_HPP
