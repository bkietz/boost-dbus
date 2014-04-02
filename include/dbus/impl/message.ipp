#ifndef DBUS_MESSAGE_IPP
#define DBUS_MESSAGE_IPP

namespace dbus {

message message::new_call(
    const string& process_name,
    const string& object_path,
    const string& interface_name,
    const string& method_name)
{
/*
  message m;
  m.message_ = dbus_message_new_method_call(
      process_name.c_str(),
      object_path.c_str(),
      interface_name.c_str(),
      method_name.c_str());
  return m;
  */
  return dbus_message_new_method_call(
      process_name.c_str(),
      object_path.c_str(),
      interface_name.c_str(),
      method_name.c_str());
}

template<typename Element>
message::packer message::pack(const Element& e)
{
  message::packer p;
  dbus_message_iter_init_append(*this, &p.iter_);
  return p.pack(e);
}

template<typename Element>
message::unpacker message::unpack(Element& e)
{
  message::unpacker u;
  dbus_message_iter_init(*this, &u.iter_);
  return u.unpack(e);
}

} // namespace dbus

#endif // DBUS_MESSAGE_IPP
