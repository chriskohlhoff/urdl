//
// istream.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_ISTREAM_HPP
#define URDL_ISTREAM_HPP

#include <istream>
#include <boost/utility/base_from_member.hpp>
#include <boost/system/error_code.hpp>
#include "urdl/istreambuf.hpp"

namespace urdl {

class istream
  : public boost::base_from_member<istreambuf>,
    public std::basic_istream<char>
{
public:
  istream()
    : std::basic_istream<char>(
        &this->boost::base_from_member<istreambuf>::member)
  {
  }

  istream(const url& u)
    : std::basic_istream<char>(
        &this->boost::base_from_member<istreambuf>::member)
  {
    if (rdbuf()->open(u) == 0)
      this->setstate(std::ios_base::failbit);
  }

  void open(const url& u)
  {
    if (rdbuf()->open(u) == 0)
      this->setstate(std::ios_base::failbit);
  }

  void close()
  {
    if (rdbuf()->close() == 0)
      this->setstate(std::ios_base::failbit);
  }

  istreambuf* rdbuf() const
  {
    return const_cast<istreambuf*>(
        &this->boost::base_from_member<istreambuf>::member);
  }

  bool is_open() const
  {
    return rdbuf()->is_open();
  }

  const boost::system::error_code& error() const
  {
    return rdbuf()->puberror();
  }

  std::size_t read_timeout() const
  {
    return rdbuf()->read_timeout();
  }

  void read_timeout(std::size_t milliseconds)
  {
    rdbuf()->read_timeout(milliseconds);
  }

  std::string content_type() const
  {
    return rdbuf()->content_type();
  }

  std::size_t content_length() const
  {
    return rdbuf()->content_length();
  }

  std::string headers() const
  {
    return rdbuf()->headers();
  }
};

} // namespace urdl

#endif // URDL_ISTREAM_HPP
