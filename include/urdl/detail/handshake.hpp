//
// handshake.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_DETAIL_HANDSHAKE_HPP
#define URDL_DETAIL_HANDSHAKE_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/detail/bind_handler.hpp>

#if !defined(URDL_DISABLE_SSL)
# include <boost/asio/ssl.hpp>
#endif // !defined(URDL_DISABLE_SSL)

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {
namespace detail {

inline boost::system::error_code handshake(
    boost::asio::ip::tcp::socket& socket, boost::system::error_code& ec)
{
  ec = boost::system::error_code();
  return ec;
}

#if !defined(URDL_DISABLE_SSL)
inline boost::system::error_code handshake(
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket,
    boost::system::error_code& ec)
{
  return socket.handshake(boost::asio::ssl::stream_base::client, ec);
}
#endif // !defined(URDL_DISABLE_SSL)

template <typename Handler>
void async_handshake(boost::asio::ip::tcp::socket& socket, Handler handler)
{
  boost::system::error_code ec;
  socket.get_io_service().post(boost::asio::detail::bind_handler(handler, ec));
}

#if !defined(URDL_DISABLE_SSL)
template <typename Handler>
void async_handshake(
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket,
    Handler handler)
{
  socket.async_handshake(boost::asio::ssl::stream_base::client, handler);
}
#endif // !defined(URDL_DISABLE_SSL)

} // namespace detail
} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#endif // URDL_DETAIL_HANDSHAKE_HPP
