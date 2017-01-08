namespace boost {
namespace asio {

io_context::io_context()
{

}

io_context::io_context(int concurrency_hint)
  /*: impl_(add_impl(new impl_type(*this, concurrency_hint == 1
          ? ASIO_CONCURRENCY_HINT_1 : concurrency_hint))) */
{
}

}
}
