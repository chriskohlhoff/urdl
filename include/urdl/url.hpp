//
// url.hpp
// ~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// path LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_URL_HPP
#define URDL_URL_HPP

#include <string>
#include <boost/system/error_code.hpp>
#include "urdl/detail/config.hpp"

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

class URDL_DECL url
{
public:
  url()
  {
  }

  url(const char* s)
  {
    *this = from_string(s);
  }

  url(const std::string& s)
  {
    *this = from_string(s);
  }

  std::string protocol() const
  {
    return protocol_;
  }

  std::string user_info() const
  {
    return user_info_;
  }

  std::string host() const
  {
    return host_;
  }

  unsigned short port() const;

  std::string path() const;

  std::string query() const
  {
    return query_;
  }

  std::string fragment() const
  {
    return fragment_;
  }

  enum parts_type
  {
    protocol_part = 1,
    user_info_part = 2,
    host_part = 4,
    port_part = 8,
    path_part = 16,
    query_part = 32,
    fragment_part = 64,
    all_parts = protocol_part | user_info_part | host_part
      | port_part | path_part | query_part | fragment_part
  };

  std::string to_string(int parts = all_parts) const;

  static url from_string(const char* s);

  static url from_string(const char* s, boost::system::error_code& ec);

  static url from_string(const std::string& s);

  static url from_string(const std::string& s, boost::system::error_code& ec);

private:
  static bool unescape(const std::string& in, std::string& out);

  std::string protocol_;
  std::string user_info_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string query_;
  std::string fragment_;
};

} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#if defined(URDL_HEADER_ONLY)
# include "urdl/impl/url.ipp"
#endif

#endif // URDL_URL_HPP
