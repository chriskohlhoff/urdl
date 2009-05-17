//
// file_read_stream.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_DETAIL_FILE_READ_STREAM_HPP
#define URDL_DETAIL_FILE_READ_STREAM_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/detail/bind_handler.hpp>
#include <fstream>
#include "urdl/url.hpp"

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {
namespace detail {

class file_read_stream
{
public:
  explicit file_read_stream(boost::asio::io_service& io_service)
    : io_service_(io_service)
  {
  }

  boost::system::error_code open(const url& u, boost::system::error_code& ec)
  {
    file_.clear();
    file_.open(u.path().c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file_)
    {
      ec = make_error_code(boost::system::errc::no_such_file_or_directory);
      return ec;
    }
    ec = boost::system::error_code();
    return ec;
  }

  template <typename Handler>
  void async_open(const url& u, Handler handler)
  {
    boost::system::error_code ec;
    open(u, ec);
    io_service_.post(boost::asio::detail::bind_handler(handler, ec));
  }

  boost::system::error_code close(boost::system::error_code& ec)
  {
    file_.close();
    file_.clear();
    ec = boost::system::error_code();
    return ec;
  }

  bool is_open() const
  {
    return file_.is_open();
  }

  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers,
      boost::system::error_code& ec)
  {
    if (!file_)
    {
      ec = boost::asio::error::eof;
      return 0;
    }

    typename MutableBufferSequence::const_iterator iter = buffers.begin();
    typename MutableBufferSequence::const_iterator end = buffers.end();
    for (; iter != end; ++iter)
    {
      boost::asio::mutable_buffer buffer(*iter);
      size_t length = boost::asio::buffer_size(buffer);
      if (length > 0)
      {
        file_.read(boost::asio::buffer_cast<char*>(buffer), length);
        length = file_.gcount();
        if (length == 0 && !file_)
          ec = boost::asio::error::eof;
        return length;
      }
    }

    ec = boost::system::error_code();
    return 0;
  }

  template <typename MutableBufferSequence, typename Handler>
  void async_read_some(const MutableBufferSequence& buffers, Handler handler)
  {
    boost::system::error_code ec;
    std::size_t bytes_transferred = read_some(buffers, ec);
    io_service_.post(boost::asio::detail::bind_handler(
          handler, ec, bytes_transferred));
  }

private:
  boost::asio::io_service& io_service_;
  std::ifstream file_;
};

} // namespace detail
} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#endif // URDL_DETAIL_FILE_READ_STREAM_HPP
