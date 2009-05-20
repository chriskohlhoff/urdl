//
// url.cpp
// ~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include "urdl/url.hpp"

#include "unit_test.hpp"

// Ensure all functions compile correctly.
void url_compile_test()
{
  // Constructors

  urdl::url url1;
  urdl::url url2("http://foo/bar");
  urdl::url url3(std::string("http://foo/bar"));
  urdl::url url4(url1);
  urdl::url url5 = url1;

  // operator=

  url5 = url2;

  // protocol()

  const urdl::url& const_url1 = url1;
  want<std::string>(const_url1.protocol());

  // user_info()

  want<std::string>(const_url1.user_info());

  // host()

  want<std::string>(const_url1.host());

  // port()

  want<unsigned short>(const_url1.port());

  // path()

  want<std::string>(const_url1.path());

  // query()

  want<std::string>(const_url1.query());

  // fragment()

  want<std::string>(const_url1.fragment());

  // to_string()

  want<std::string>(const_url1.to_string());
  want<std::string>(const_url1.to_string(urdl::url::protocol_component));
  want<std::string>(const_url1.to_string(urdl::url::user_info_component));
  want<std::string>(const_url1.to_string(urdl::url::host_component));
  want<std::string>(const_url1.to_string(urdl::url::port_component));
  want<std::string>(const_url1.to_string(urdl::url::path_component));
  want<std::string>(const_url1.to_string(urdl::url::query_component));
  want<std::string>(const_url1.to_string(urdl::url::fragment_component));
  want<std::string>(const_url1.to_string(urdl::url::all_components));

  // from_string()

  boost::system::error_code ec;
  want<urdl::url>(urdl::url::from_string("http://foo/bar"));
  want<urdl::url>(urdl::url::from_string("http://foo/bar", ec));
  want<urdl::url>(urdl::url::from_string(std::string("http://foo/bar")));
  want<urdl::url>(urdl::url::from_string(std::string("http://foo/bar"), ec));

  // operator==

  const urdl::url& const_url2 = url2;
  want<bool>(const_url1 == const_url2);

  // operator!=

  want<bool>(const_url1 != const_url2);

  // operator<

  want<bool>(const_url1 < const_url2);
}

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("basic_socket_acceptor");
  test->add(BOOST_TEST_CASE(&url_compile_test));
  return test;
}
