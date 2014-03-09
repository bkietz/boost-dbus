#include <chrono>

#include <dbus/dbus.h>
#include <boost/asio/generic/stream_protocol.hpp>
#include <boost/asio/steady_timer.hpp>

namespace dbus {
namespace detail {

static void watch_toggled(DBusWatch *dbus_watch, void *data);
struct watch_handler {
  DBusWatchFlags flags;
  DBusWatch* dbus_watch;
  watch_handler(DBusWatchFlags f, DBusWatch* w):
    flags(f), dbus_watch(w) {}
  
  void operator()(boost::system::error_code ec, size_t) {
    if(ec) return;
    dbus_watch_handle(dbus_watch, flags);

    using boost::asio::generic::stream_protocol;
    stream_protocol::socket& socket = 
      *static_cast<stream_protocol::socket *>(dbus_watch_get_data(dbus_watch));

    watch_toggled(dbus_watch, &socket.get_io_service());
  }
};
static void watch_toggled(DBusWatch *dbus_watch, void *data) {
  using boost::asio::generic::stream_protocol;
  stream_protocol::socket& socket = 
    *static_cast<stream_protocol::socket *>(dbus_watch_get_data(dbus_watch));

  if(dbus_watch_get_enabled(dbus_watch)) {
    using boost::asio::null_buffers;

    if(dbus_watch_get_flags(dbus_watch) & DBUS_WATCH_READABLE)
      socket.async_read_some(null_buffers(),
        watch_handler(DBUS_WATCH_READABLE, dbus_watch));
    
    if(dbus_watch_get_flags(dbus_watch) & DBUS_WATCH_WRITABLE)
      socket.async_write_some(null_buffers(), 
        watch_handler(DBUS_WATCH_WRITABLE, dbus_watch));
  } else {
    socket.cancel();
  }
}
static dbus_bool_t add_watch(DBusWatch *dbus_watch, void *data) {
  if(!dbus_watch_get_enabled(dbus_watch)) return TRUE;

  using boost::asio::io_service;
  io_service& io = *static_cast<io_service *>(data);

  using boost::asio::generic::stream_protocol;
  int fd = dbus_watch_get_unix_fd(dbus_watch);
  stream_protocol::socket& socket =
    *new stream_protocol::socket(io);

  socket.assign(stream_protocol(0,0), fd);

  dbus_watch_set_data(dbus_watch, &socket, NULL);
  
  watch_toggled(dbus_watch, &io);
  return TRUE;
}
static void remove_watch(DBusWatch *dbus_watch, void *data) {
  using boost::asio::generic::stream_protocol;
  delete static_cast<stream_protocol::socket *>(dbus_watch_get_data(dbus_watch));
}




struct timeout_handler {
  DBusTimeout* dbus_timeout;
  timeout_handler(DBusTimeout* t):
    dbus_timeout(t) {}
  
  void operator()(boost::system::error_code ec) {
    if(ec) return;
    dbus_timeout_handle(dbus_timeout);
  }
};
static void timeout_toggled(DBusTimeout *dbus_timeout, void *data) {
  using boost::asio::steady_timer;
  steady_timer& timer = 
    *static_cast<steady_timer *>(dbus_timeout_get_data(dbus_timeout));

  if(dbus_timeout_get_enabled(dbus_timeout)) {
    using namespace std::chrono;
    steady_timer::duration interval = milliseconds(dbus_timeout_get_interval(dbus_timeout));
    timer.expires_from_now(interval);
    timer.cancel();
    timer.async_wait(timeout_handler(dbus_timeout));
  } else {
    timer.cancel();
  }
}
static dbus_bool_t add_timeout(DBusTimeout *dbus_timeout, void *data) {
  if(!dbus_timeout_get_enabled(dbus_timeout)) return TRUE;

  using boost::asio::io_service;
  io_service& io = *static_cast<io_service *>(data);

  using boost::asio::steady_timer;
  steady_timer& timer = 
    *new steady_timer(io);

  dbus_timeout_set_data(dbus_timeout, &timer, NULL);
  
  timeout_toggled(dbus_timeout, &io);
  return TRUE;
}
static void remove_timeout(DBusTimeout *dbus_timeout, void *data) {
  using boost::asio::steady_timer;
  delete static_cast<steady_timer *>(dbus_timeout_get_data(dbus_timeout));
}


struct dispatch_handler {
  boost::asio::io_service& io;
  DBusConnection *conn;
  dispatch_handler(boost::asio::io_service& i, DBusConnection *c): io(i), conn(c) {}
  void operator()() {
    if(dbus_connection_dispatch(conn) == DBUS_DISPATCH_DATA_REMAINS)
      io.post(dispatch_handler(io, conn));
  }
};
static void dispatch_status(DBusConnection *conn, DBusDispatchStatus new_status, void *data) {
  using boost::asio::io_service;
  io_service& io = *static_cast<io_service *>(data);
  if(new_status == DBUS_DISPATCH_DATA_REMAINS)
    io.post(dispatch_handler(io, conn));
}

} // namespace detail
} // namespace dbus
