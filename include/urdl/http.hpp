//
// http.hpp
// ~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_HTTP_HPP
#define URDL_HTTP_HPP

#include <boost/system/error_code.hpp>
#include "urdl/detail/config.hpp"

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {
namespace http {

URDL_DECL const boost::system::error_category& error_category();

namespace errc {

enum errc_t
{
  // Client-generated errors.
  malformed_status_line = 1,
  malformed_response_headers = 2,

  // Server-generated errors.
  continue_request = 100,
  switching_protocols = 101,
  ok = 200,
  created = 201,
  accepted = 202,
  non_authoritative_information = 203,
  no_content = 204,
  reset_content = 205,
  partial_content = 206,
  multiple_choices = 300,
  moved_permanently = 301,
  found = 302,
  see_other = 303,
  not_modified = 304,
  use_proxy = 305,
  temporary_redirect = 307,
  bad_request = 400,
  unauthorized = 401,
  payment_required = 402,
  forbidden = 403,
  not_found = 404,
  method_not_allowed = 405,
  not_acceptable = 406,
  proxy_authentication_required = 407,
  request_timeout = 408,
  conflict = 409,
  gone = 410,
  length_required = 411,
  precondition_failed = 412,
  request_entity_too_large = 413,
  request_uri_too_large = 414,
  unsupported_media_type = 415,
  requested_range_not_satisfiable = 416,
  expectation_failed = 417,
  internal_server_error = 500,
  not_implemented = 501,
  bad_gateway = 502,
  service_unavailable = 503,
  gateway_timeout = 504,
  version_not_supported = 505
};

inline boost::system::error_code make_error_code(errc_t e)
{
  return boost::system::error_code(
      static_cast<int>(e), http::error_category());
}

} // namespace errc
} // namespace http
} // namespace urdl

namespace boost {
namespace system {

template <>
struct is_error_code_enum<urdl::http::errc::errc_t>
{
  static const bool value = true;
};

} // namespace system
} // namespace boost

#include "urdl/detail/abi_suffix.hpp"

#if defined(URDL_HEADER_ONLY)
# include "urdl/impl/http.ipp"
#endif

#endif // URDL_HTTP_HPP
