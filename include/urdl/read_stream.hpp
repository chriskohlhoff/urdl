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
#include "urdl/http.hpp"
#include "urdl/url.hpp"
#include "urdl/detail/coroutine.hpp"
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
    url tmp_url = u;
    for (;;)
    {
      if (tmp_url.protocol() == "file")
      {
        protocol_ = file;
        return file_.open(tmp_url, ec);
      }
      else if (tmp_url.protocol() == "http")
      {
        protocol_ = http;
        http_.open(tmp_url, ec);
        if (ec == http::errc::moved_permanently || ec == http::errc::found)
        {
          tmp_url = http_.location();
          http_.close(ec);
          continue;
        }
        return ec;
      }
#if !defined(URDL_DISABLE_SSL)
      else if (tmp_url.protocol() == "https")
      {
        protocol_ = https;
        https_.open(tmp_url, ec);
        if (ec == http::errc::moved_permanently || ec == http::errc::found)
        {
          tmp_url = https_.location();
          https_.close(ec);
          continue;
        }
        return ec;
      }
#endif // !defined(URDL_DISABLE_SSL)
      else
      {
        ec = boost::asio::error::operation_not_supported;
        return ec;
      }
    }
  }

  template <typename Handler>
  void async_open(const url& u, Handler handler)
  {
    open_coro<Handler>(this, u, handler)(boost::system::error_code());
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
  template <typename Handler>
  class open_coro : detail::coroutine
  {
  public:
    open_coro(read_stream* this_ptr, const url& u, Handler handler)
      : this_(this_ptr),
        url_(u),
        handler_(handler)
    {
    }

    void operator()(boost::system::error_code ec)
    {
      URDL_CORO_BEGIN;

      for (;;)
      {
        if (url_.protocol() == "file")
        {
          this_->protocol_ = file;
          URDL_CORO_YIELD(this_->file_.async_open(url_, *this));
          handler_(ec);
          return;
        }
        else if (url_.protocol() == "http")
        {
          this_->protocol_ = http;
          URDL_CORO_YIELD(this_->http_.async_open(url_, *this));
          if (ec == http::errc::moved_permanently || ec == http::errc::found)
          {
            url_ = this_->http_.location();
            this_->http_.close(ec);
            continue;
          }
          handler_(ec);
          return;
        }
#if !defined(URDL_DISABLE_SSL)
        else if (url_.protocol() == "https")
        {
          this_->protocol_ = https;
          URDL_CORO_YIELD(this_->https_.async_open(url_, *this));
          if (ec == http::errc::moved_permanently || ec == http::errc::found)
          {
            url_ = this_->https_.location();
            this_->https_.close(ec);
            continue;
          }
          handler_(ec);
          return;
        }
#endif // !defined(URDL_DISABLE_SSL)
        else
        {
          ec = boost::asio::error::operation_not_supported;
          this_->io_service_.post(
              boost::asio::detail::bind_handler(handler_, ec));
          return;
        }
      }

      URDL_CORO_END;
    }

    friend void* asio_handler_allocate(std::size_t size,
        open_coro<Handler>* this_handler)
    {
      using boost::asio::asio_handler_allocate;
      return asio_handler_allocate(size, &this_handler->handler_);
    }

    friend void asio_handler_deallocate(void* pointer, std::size_t size,
        open_coro<Handler>* this_handler)
    {
      using boost::asio::asio_handler_deallocate;
      asio_handler_deallocate(pointer, size, &this_handler->handler_);
    }

    template <typename Function>
    friend void asio_handler_invoke(const Function& function,
        open_coro<Handler>* this_handler)
    {
      using boost::asio::asio_handler_invoke;
      asio_handler_invoke(function, &this_handler->handler_);
    }

  private:
    read_stream* this_;
    url url_;
    Handler handler_;
  };

  template <typename Handler> friend class open_coro;

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
