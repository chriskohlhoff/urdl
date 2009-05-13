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
#include "urdl/detail/config.hpp"
#include "urdl/url.hpp"

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

class URDL_DECL istreambuf
  : public std::streambuf
{
public:
  istreambuf();

  ~istreambuf();

  bool is_open() const;

  istreambuf* open(const url& u);

  istreambuf* close();

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

#include "urdl/detail/abi_suffix.hpp"

#if defined(URDL_HEADER_ONLY)
# include "urdl/impl/istreambuf.ipp"
#endif

#endif // URDL_ISTREAMBUF_HPP
