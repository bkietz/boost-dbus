#include <dbus/detail/watch_timeout.hpp>
#include <dbus/detail/queue.hpp>
#include <boost/asio.hpp>

namespace dbus {

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
	//TODO
  }

  void open(implementation_type& impl,
	  const string& address, bool shared=true)
  {
	io_service& io = this->get_io_service();

	using namespace detail;
	
    DBusError error;
    dbus_error_init(&error);
	impl = dbus_connection_open(address.c_str(), &error);
	//TODO actually deal with that error

    dbus_connection_set_watch_functions(impl,
      &add_watch, &remove_watch, &watch_toggled, &io, NULL);

    dbus_connection_set_timeout_functions(impl,
      &add_timeout, &remove_timeout, &timeout_toggled, &io, NULL);

    dbus_connection_set_dispatch_status_function(impl,
      &dispatch_status, &io, NULL);

    dispatch_status(impl,
      dbus_connection_get_dispatch_status(impl), &io);
  }

  void open(implementation_type& impl,
	  const unsigned bus_type = DBUS_BUS_SYSTEM,
	  bool shared=true)
  {
	io_service& io = this->get_io_service();

	using namespace detail;
	
    DBusError error;
    dbus_error_init(&error);
	impl = dbus_bus_get(bus_type, &error);
	//TODO actually deal with that error

    dbus_connection_set_watch_functions(impl,
      &add_watch, &remove_watch, &watch_toggled, &io, NULL);

    dbus_connection_set_timeout_functions(impl,
      &add_timeout, &remove_timeout, &timeout_toggled, &io, NULL);

    dbus_connection_set_dispatch_status_function(impl,
      &dispatch_status, &io, NULL);

    dispatch_status(impl,
      dbus_connection_get_dispatch_status(impl), &io);
  }

  template <typename Duration>
  message send(implementation_type& impl,
	  message& m,
      const Duration& timeout)
  {
	//TODO generically convert timeout to milliseconds
	if(timeout == 0) {
	  //TODO this can return false if it failed
	  dbus_connection_send(impl, m, &m.serial);
	  return message();
	} else {
	  DBusError error;
	  dbus_error_init(&error);
	  return dbus_connection_send_with_reply_and_block(impl, 
		m, timeout, &error);
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
	//TODO set up p with handler etc.
	dbus_connection_send_with_reply(impl,
		m, &p, -1);
  }
  
};

io_service::id connection_service::id;

} // namespace dbus
