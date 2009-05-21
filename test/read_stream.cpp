//
// read_stream.cpp
// ~~~~~~~~~~~~~~~
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
#include "urdl/read_stream.hpp"

#include "unit_test.hpp"
#include "urdl/option_set.hpp"
#include <boost/asio/io_service.hpp>

void open_handler(const boost::system::error_code&) {}
void read_handler(const boost::system::error_code&, std::size_t) {}

// Ensure all functions compile correctly.
void read_stream_compile_test()
{
  try
  {
    boost::asio::io_service io_service;
    boost::system::error_code ec;
    char buffer[1024];

    // Constructors

    urdl::read_stream stream1(io_service);

    // get_io_service()

    want<boost::asio::io_service>(stream1.get_io_service());

    // set_option()

    stream1.set_option(0);
    stream1.set_option<char>(0);

    // set_options()

    stream1.set_options(urdl::option_set());

    // get_option()

    const urdl::read_stream& const_stream1 = stream1;
    want<int>(const_stream1.get_option<int>());
    want<char>(const_stream1.get_option<char>());

    // get_options()

    want<urdl::option_set>(const_stream1.get_options());

    // is_open()

    want<bool>(const_stream1.is_open());

    // open()

    stream1.open("file://xyz");
    stream1.open(urdl::url("file://xyz"));
    want<boost::system::error_code>(stream1.open("file://xyz", ec));
    want<boost::system::error_code>(stream1.open(urdl::url("file://xyz"), ec));

    // async_open()

    stream1.async_open("file://xyz", open_handler);
    stream1.async_open(urdl::url("file://xyz"), open_handler);

    // close()

    stream1.close();
    want<boost::system::error_code>(stream1.close(ec));

    // content_type()

    want<std::string>(const_stream1.content_type());

    // content_length()

    want<std::size_t>(const_stream1.content_length());

    // headers()

    want<std::string>(const_stream1.headers());

    // read_some()

    want<std::size_t>(stream1.read_some(boost::asio::buffer(buffer)));
    want<std::size_t>(stream1.read_some(boost::asio::buffer(buffer), ec));

    // async_read_some()

    stream1.async_read_some(boost::asio::buffer(buffer), read_handler);
  }
  catch (std::exception&)
  {
  }
}

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("read_stream");
  test->add(BOOST_TEST_CASE(&read_stream_compile_test));
  return test;
}
