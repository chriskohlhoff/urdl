//
// streambuf.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_ISTREAMBUF_HPP
#define URDL_ISTREAMBUF_HPP

#include <streambuf>
#include <boost/asio/deadline_timer.hpp>
#include <boost/throw_exception.hpp>
#include "urdl/read_stream.hpp"
#include "urdl/url.hpp"

namespace urdl {

class istreambuf : public std::streambuf
{
public:
  istreambuf()
    : read_stream_(io_service_),
      timer_(io_service_),
      read_timeout_(300 * 1000)
  {
    init_buffers();
  }

  istreambuf* open(const url& u)
  {
    init_buffers();
    read_stream_.close(error_);
    read_stream_.open(u, error_);
    return !error_ ? this : 0;
  }

  istreambuf* close()
  {
    read_stream_.close(error_);
    if (!error_)
      init_buffers();
    return !error_ ? this : 0;
  }

  bool is_open() const
  {
    return read_stream_.is_open();
  }

  const boost::system::error_code& puberror() const
  {
    return error();
  }

  std::size_t read_timeout() const
  {
    return read_timeout_;
  }

  void read_timeout(std::size_t milliseconds)
  {
    read_timeout_ = milliseconds;
  }

  std::string content_type() const
  {
    return read_stream_.content_type();
  }

  std::size_t content_length() const
  {
    return read_stream_.content_length();
  }

  std::string headers() const
  {
    return read_stream_.headers();
  }

protected:
  int_type underflow()
  {
    if (gptr() == egptr())
    {
      std::size_t bytes_transferred = 0;
      read_handler rh = { error_, bytes_transferred, timer_ };
      read_stream_.async_read_some(boost::asio::buffer(
            boost::asio::buffer(get_buffer_) + putback_max), rh);

      timeout_handler th = { read_stream_ };
      timer_.expires_from_now(boost::posix_time::milliseconds(read_timeout_));
      timer_.async_wait(th);

      io_service_.reset();
      io_service_.run();

      if (!read_stream_.is_open())
        error_ = boost::asio::error::timed_out;

      if (error_)
      {
        if (error_ == boost::asio::error::eof)
          return traits_type::eof();
        boost::throw_exception(boost::system::system_error(error_));
      }

      setg(get_buffer_.begin(), get_buffer_.begin() + putback_max,
          get_buffer_.begin() + putback_max + bytes_transferred);
      return traits_type::to_int_type(*gptr());
    }
    else
    {
      return traits_type::eof();
    }
  }

  virtual const boost::system::error_code& error() const
  {
    return error_;
  }

private:
  void init_buffers()
  {
    setg(get_buffer_.begin(),
        get_buffer_.begin() + putback_max,
        get_buffer_.begin() + putback_max);
  }

  struct read_handler
  {
    boost::system::error_code& error_;
    std::size_t& bytes_transferred_;
    boost::asio::deadline_timer& timer_;
    void operator()(boost::system::error_code ec, std::size_t bytes_transferred)
    {
      error_ = ec;
      bytes_transferred_ = bytes_transferred;
      timer_.cancel();
    }
  };

  struct timeout_handler
  {
    read_stream& read_stream_;
    void operator()(boost::system::error_code ec)
    {
      if (ec != boost::asio::error::operation_aborted)
        read_stream_.close(ec);
    }
  };

  enum { putback_max = 8 };
  enum { buffer_size = 512 };
  boost::array<char, buffer_size> get_buffer_;
  boost::asio::io_service io_service_;
  boost::system::error_code error_;
  read_stream read_stream_;
  boost::asio::deadline_timer timer_;
  std::size_t read_timeout_;
};

} // namespace urdl

#endif // URDL_ISTREAMBUF_HPP
