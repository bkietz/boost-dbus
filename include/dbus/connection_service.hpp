#ifndef DBUS_CONNECTION_SERVICE_HPP
#define DBUS_CONNECTION_SERVICE_HPP

#include <string>
#include <boost/asio.hpp>

#include <dbus/message.hpp>
#include <dbus/detail/watch_timeout.hpp>
#include <dbus/detail/queue.hpp>
#include <dbus/match.hpp>

namespace dbus {

namespace bus {
  static const int session = DBUS_BUS_SESSION;
  static const int system  = DBUS_BUS_SYSTEM;
  static const int starter = DBUS_BUS_STARTER;
} // namespace bus

using std::string;
using namespace boost::asio;

class connection_service
  : public io_service::service
{
public:
  static io_service::id id;

  typedef DBusConnection *implementation_type;

  explicit connection_service(io_service& io)
    :  service(io)
  {
  }

  void construct(implementation_type& impl)
  {
  }

  void destroy(implementation_type& impl)
  {
    dbus_connection_unref(impl);
  }

  void shutdown_service()
  {
    //TODO is there anything that needs shutting down?
  }

  void open(implementation_type& impl,
      const string& address, bool shared=true)
  {
    io_service& io = this->get_io_service();

    DBusError error;
    dbus_error_init(&error);
    impl = dbus_connection_open(address.c_str(), &error);
    //TODO actually deal with that error

    detail::set_watch_timeout_dispatch_functions(impl, io);
  }

  void open(implementation_type& impl,
      const int bus = (int) DBUS_BUS_SYSTEM,
      bool shared=true)
  {
    io_service& io = this->get_io_service();

    DBusError error;
    dbus_error_init(&error);
    impl = dbus_bus_get((DBusBusType)bus, &error);
    //TODO actually deal with that error

    detail::set_watch_timeout_dispatch_functions(impl, io);
  }

  message send(implementation_type& impl,
      message& m)
  {
    DBusError error;
    dbus_error_init(&error);
    return message(dbus_connection_send_with_reply_and_block(impl, 
        m, -1, &error));
        //TODO deal with that error
  }

  template <typename Duration>
  message send(implementation_type& impl,
      message& m,
      const Duration& timeout)
  {
    //TODO generically convert timeout to milliseconds
    if(timeout == Duration::zero()) {
      //TODO this can return false if it failed
      dbus_connection_send(impl, m, &m.serial);
      return message();
    } else {
      DBusError error;
      dbus_error_init(&error);
      return message(dbus_connection_send_with_reply_and_block(impl, 
          m, chrono::milliseconds(timeout).count(), &error));
          //TODO deal with that error
    }
  }

  template<typename MessageHandler>
  inline BOOST_ASIO_INITFN_RESULT_TYPE(MessageHandler,
      void(boost::system::error_code, message))
  async_send(implementation_type& impl,
      message& m,
      BOOST_ASIO_MOVE_ARG(MessageHandler) handler)
  {
    DBusPendingCall *p;
    dbus_connection_send_with_reply(impl,
        m, &p, -1);
    /*
    dbus_pending_call_set_notify(p, 
        &pending_call_notify, &get_io_service(), NULL);

    //FIXME Race condition: another thread might have 
    // processed the pending call's reply before a notify
    // function could be set. If so, the notify function
    // will never trigger, so it must be called manually:
    if(dbus_pending_call_get_completed(p))
    {
      //TODO: does this work, or might it call the notify 
      // function too many times? Might have to use steal_reply
      pending_call_notify(p, &get_io_service());
    }
    */
  }

  void new_match(implementation_type& impl,
      match& m)
  {
    DBusError err;
    dbus_error_init(&err);
    dbus_bus_add_match(impl, m.get_expression().c_str(), &err);
    //TODO deal with that error
    // eventually, for complete asynchronicity, this should connect to
    // org.freedesktop.DBus and call org.freedesktop.DBus.AddMatch
  }

  void delete_match(implementation_type& impl,
      match& m)
  {
    DBusError err;
    dbus_error_init(&err);
    dbus_bus_remove_match(impl, m.get_expression().c_str(), &err);
  }
  
  static DBusHandlerResult filter_wrap(
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

  void new_filter(implementation_type& impl,
      filter& f)
  {
    dbus_connection_add_filter(impl,
        &filter_wrap, &f, NULL);
  }
  
  void delete_filter(implementation_type& impl,
      filter& f)
  {
    dbus_connection_remove_filter(impl,
        &filter_wrap, &f);
  }
};

io_service::id connection_service::id;




} // namespace dbus

#endif // DBUS_CONNECTION_SERVICE_HPP
