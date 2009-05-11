//
// connect.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_DETAIL_CONNECT_HPP
#define URDL_DETAIL_CONNECT_HPP

#include <boost/asio/ip/tcp.hpp>
#include <sstream>
#include "urdl/detail/coroutine.hpp"

namespace urdl {
namespace detail {

inline boost::system::error_code connect(
    boost::asio::ip::tcp::socket::lowest_layer_type& socket,
    boost::asio::ip::tcp::resolver& resolver,
    const url& u, boost::system::error_code& ec)
{
  // Create a query that corresponds to the url.
  std::ostringstream port_string;
  port_string << u.port();
  boost::asio::ip::tcp::resolver::query query(u.host(), port_string.str());

  // Get a list of endpoints corresponding to the query.
  boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query, ec);
  if (ec)
    return ec;

  // Try each endpoint until we successfully establish a connection.
  ec = boost::asio::error::host_not_found;
  while (ec && iter != boost::asio::ip::tcp::resolver::iterator())
  {
    socket.close(ec);
    socket.connect(*iter++, ec);
  }
  if (ec)
    return ec;

  // Disable the Nagle algorithm on all sockets.
  return socket.set_option(boost::asio::ip::tcp::no_delay(true), ec);
}

template <typename Handler>
class connect_coro : coroutine
{
public:
  connect_coro(Handler handler,
      boost::asio::ip::tcp::socket::lowest_layer_type& socket,
      boost::asio::ip::tcp::resolver& resolver)
    : handler_(handler),
      socket_(socket),
      resolver_(resolver)
  {
  }

  void operator()(boost::system::error_code ec,
      boost::asio::ip::tcp::resolver::iterator iter)
  {
    iter_ = iter;
    (*this)(ec);
  }

  void operator()(boost::system::error_code ec,
      const boost::asio::ip::tcp::resolver::query* query = 0)
  {
    URDL_CORO_BEGIN;

    // Open the socket to give the caller something to close to cancel the
    // asynchronous operation.
    socket_.open(boost::asio::ip::tcp::v4(), ec);
    if (ec)
    {
      URDL_CORO_YIELD(socket_.get_io_service().post(
            boost::asio::detail::bind_handler(*this, ec)));
      handler_(ec);
      return;
    }

    // Get a list of endpoints corresponding to the host name.
    URDL_CORO_YIELD(resolver_.async_resolve(*query, *this));
    if (ec)
    {
      handler_(ec);
      return;
    }

    // Try each endpoint until we successfully establish a connection.
    ec = boost::asio::error::host_not_found;
    while (ec && iter_ != boost::asio::ip::tcp::resolver::iterator())
    {
      // Check whether the operation has been cancelled.
      if (!socket_.is_open())
      {
        ec = boost::asio::error::operation_aborted;
        break;
      }

      // Try next endpoint.
      socket_.close(ec);
      endpoint_ = *iter_++;
      URDL_CORO_YIELD(socket_.async_connect(endpoint_, *this));
    }
    if (ec)
    {
      handler_(ec);
      return;
    }

    // Disable the Nagle algorithm on all sockets.
    socket_.set_option(boost::asio::ip::tcp::no_delay(true), ec);

    handler_(ec);

    URDL_CORO_END;
  }

private:
  Handler handler_;
  boost::asio::ip::tcp::socket::lowest_layer_type& socket_;
  boost::asio::ip::tcp::resolver& resolver_;
  boost::asio::ip::tcp::resolver::iterator iter_;
  boost::asio::ip::tcp::endpoint endpoint_;
};

template <typename Handler>
void async_connect(boost::asio::ip::tcp::socket::lowest_layer_type& socket,
    boost::asio::ip::tcp::resolver& resolver, const url& u, Handler handler)
{
  std::ostringstream port_string;
  port_string << u.port();
  boost::asio::ip::tcp::resolver::query query(u.host(), port_string.str());
  connect_coro<Handler>(handler, socket, resolver)(
      boost::system::error_code(), &query);
}

} // namespace detail
} // namespace urdl

#endif // URDL_DETAIL_CONNECT_HPP
