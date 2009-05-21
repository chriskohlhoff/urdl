//
// option_set.cpp
// ~~~~~~~~~~~~~~
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
#include "urdl/option_set.hpp"

#include "unit_test.hpp"

// Ensure all functions compile correctly.
void option_set_compile_test()
{
  // Constructors

  urdl::option_set option_set1;
  urdl::option_set option_set2(option_set1);

  // operator=

  option_set1 = option_set2;

  // set_option

  option_set1.set_option(0);
  option_set1.set_option<char>(0);

  // set_options

  const urdl::option_set& const_option_set1 = option_set1;
  option_set2.set_options(const_option_set1);

  // get_option()

  want<int>(const_option_set1.get_option<int>());
  want<char>(const_option_set1.get_option<char>());

  // clear_option()

  option_set1.clear_option<int>();
  option_set1.clear_option<char>();
}

test_suite* init_unit_test_suite(int, char*[])
{
  test_suite* test = BOOST_TEST_SUITE("option_set");
  test->add(BOOST_TEST_CASE(&option_set_compile_test));
  return test;
}
