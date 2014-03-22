#ifndef DBUS_PACKER_IPP
#define DBUS_PACKER_IPP

namespace dbus {

namespace impl {

template<typename Element> struct append_one
{
  //TODO: throw if invalid Element
  append_one(message::packer& p, Element& e)
  {
	dbus_message_iter_append_basic(&p.iter_, 
		element<Element>::code, &e);
  }
};

template<> struct append_one<string>
{
  append_one(message::packer& p, string& e)
  {
	const char *c = e.c_str();
	dbus_message_iter_append_basic(&p.iter_, 
		element<string>::code, &c);
  }
};

template<> struct append_one<const char *>
{
  append_one(message::packer& p, const char *e)
  {
	dbus_message_iter_append_basic(&p.iter_, 
		element<string>::code, &e);
  }
};

} // namespace impl


template<typename Element>
message::packer& message::packer::pack(Element& e)
{
  impl::append_one<Element>(*this, e);
  return *this;
}


} // namespace dbus

#endif // DBUS_PACKER_IPP
