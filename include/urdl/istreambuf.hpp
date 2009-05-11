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
#include <boost/system/error_code.hpp>
#include "urdl/url.hpp"

namespace urdl {

class istreambuf : public std::streambuf
{
public:
  istreambuf();

  ~istreambuf();

  istreambuf* open(const url& u);

  istreambuf* close();

  bool is_open() const;

  const boost::system::error_code& puberror() const;

  std::size_t read_timeout() const;

  void read_timeout(std::size_t milliseconds);

  std::string content_type() const;

  std::size_t content_length() const;

  std::string headers() const;

protected:
  int_type underflow();

  virtual const boost::system::error_code& error() const;

private:
  void init_buffers();

  struct body;
  body* body_;
};

} // namespace urdl

#if defined(URDL_HEADER_ONLY)
# include "urdl/impl/istreambuf.hpp"
#endif

#endif // URDL_ISTREAMBUF_HPP
