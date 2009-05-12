//
// read_stream.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_READ_STREAM_HPP
#define URDL_READ_STREAM_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/detail/bind_handler.hpp>
#include <boost/throw_exception.hpp>
#include "urdl/url.hpp"
#include "urdl/detail/file_read_stream.hpp"
#include "urdl/detail/http_read_stream.hpp"

#if !defined(URDL_DISABLE_SSL)
# include <boost/asio/ssl.hpp>
#endif // !defined(URDL_DISABLE_SSL)

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

class read_stream
{
public:
  explicit read_stream(boost::asio::io_service& io_service)
    : io_service_(io_service),
      file_(io_service),
      http_(io_service),
#if !defined(URDL_DISABLE_SSL)
      ssl_context_(io_service, boost::asio::ssl::context::sslv23),
      https_(io_service, ssl_context_),
#endif // !defined(URDL_DISABLE_SSL)
      protocol_(unknown)
  {
#if !defined(URDL_DISABLE_SSL)
    ssl_context_.set_verify_mode(boost::asio::ssl::context::verify_peer);
#endif // !defined(URDL_DISABLE_SSL)
  }

  boost::asio::io_service& get_io_service()
  {
    return io_service_;
  }

  void open(const url& u)
  {
    boost::system::error_code ec;
    if (open(u, ec))
    {
      boost::system::system_error ex(ec);
      boost::throw_exception(ex);
    }
  }

  boost::system::error_code open(const url& u, boost::system::error_code& ec)
  {
    if (u.protocol() == "file")
    {
      protocol_ = file;
      return file_.open(u, ec);
    }
    else if (u.protocol() == "http")
    {
      protocol_ = http;
      return http_.open(u, ec);
    }
#if !defined(URDL_DISABLE_SSL)
    else if (u.protocol() == "https")
    {
      protocol_ = https;
      return https_.open(u, ec);
    }
#endif // !defined(URDL_DISABLE_SSL)
    else
    {
      ec = boost::asio::error::operation_not_supported;
      return ec;
    }
  }

  template <typename Handler>
  void async_open(const url& u, Handler handler)
  {
    if (u.protocol() == "file")
    {
      protocol_ = file;
      file_.async_open(u, handler);
    }
    else if (u.protocol() == "http")
    {
      protocol_ = http;
      http_.async_open(u, handler);
    }
#if !defined(URDL_DISABLE_SSL)
    else if (u.protocol() == "https")
    {
      protocol_ = https;
      https_.async_open(u, handler);
    }
#endif // !defined(URDL_DISABLE_SSL)
    else
    {
      protocol_ = unknown;
      boost::system::error_code ec
        = boost::asio::error::operation_not_supported;
      io_service_.post(boost::asio::detail::bind_handler(handler, ec));
    }
  }

  void close()
  {
    boost::system::error_code ec;
    if (close(ec))
    {
      boost::system::system_error ex(ec);
      boost::throw_exception(ex);
    }
  }

  boost::system::error_code close(boost::system::error_code& ec)
  {
    switch (protocol_)
    {
    case file:
      return file_.close(ec);
    case http:
      return http_.close(ec);
#if !defined(URDL_DISABLE_SSL)
    case https:
      return https_.close(ec);
#endif // !defined(URDL_DISABLE_SSL)
    default:
      ec = boost::system::error_code();
      break;
    }

    return ec;
  }

  bool is_open() const
  {
    switch (protocol_)
    {
    case file:
      return file_.is_open();
    case http:
      return http_.is_open();
#if !defined(URDL_DISABLE_SSL)
    case https:
      return https_.is_open();
#endif // !defined(URDL_DISABLE_SSL)
    default:
      return false;
    }
  }

  std::string content_type() const
  {
    switch (protocol_)
    {
    case file:
      return std::string();
    case http:
      return http_.content_type();
#if !defined(URDL_DISABLE_SSL)
    case https:
      return https_.content_type();
#endif // !defined(URDL_DISABLE_SSL)
    default:
      return std::string();
    }
  }

  std::size_t content_length() const
  {
    switch (protocol_)
    {
    case file:
      return ~std::size_t(0);
    case http:
      return http_.content_length();
#if !defined(URDL_DISABLE_SSL)
    case https:
      return https_.content_length();
#endif // !defined(URDL_DISABLE_SSL)
    default:
      return ~std::size_t(0);
    }
  }

  std::string headers() const
  {
    switch (protocol_)
    {
    case file:
      return std::string();
    case http:
      return http_.headers();
#if !defined(URDL_DISABLE_SSL)
    case https:
      return https_.headers();
#endif // !defined(URDL_DISABLE_SSL)
    default:
      return std::string();
    }
  }

  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers)
  {
    boost::system::error_code ec;
    std::size_t bytes_transferred = read_some(buffers, ec);
    if (ec)
    {
      boost::system::system_error ex(ec);
      boost::throw_exception(ex);
    }
    return bytes_transferred;
  }

  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers,
      boost::system::error_code& ec)
  {
    switch (protocol_)
    {
    case file:
      return file_.read_some(buffers, ec);
    case http:
      return http_.read_some(buffers, ec);
#if !defined(URDL_DISABLE_SSL)
    case https:
      return https_.read_some(buffers, ec);
#endif // !defined(URDL_DISABLE_SSL)
    default:
      ec = boost::asio::error::operation_not_supported;
      return 0;
    }
  }

  template <typename MutableBufferSequence, typename Handler>
  void async_read_some(const MutableBufferSequence& buffers, Handler handler)
  {
    switch (protocol_)
    {
    case file:
      file_.async_read_some(buffers, handler);
      break;
    case http:
      http_.async_read_some(buffers, handler);
      break;
#if !defined(URDL_DISABLE_SSL)
    case https:
      https_.async_read_some(buffers, handler);
      break;
#endif // !defined(URDL_DISABLE_SSL)
    default:
      boost::system::error_code ec
        = boost::asio::error::operation_not_supported;
      io_service_.post(boost::asio::detail::bind_handler(handler, ec, 0));
      break;
    }
  }

private:
  boost::asio::io_service& io_service_;
  detail::file_read_stream file_;
  detail::http_read_stream<boost::asio::ip::tcp::socket> http_;
#if !defined(URDL_DISABLE_SSL)
  boost::asio::ssl::context ssl_context_;
  detail::http_read_stream<
      boost::asio::ssl::stream<
        boost::asio::ip::tcp::socket> > https_;
#endif // !defined(URDL_DISABLE_SSL)
  enum { unknown, file, http, https } protocol_;
};

} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#endif // URDL_READ_STREAM_HPP
