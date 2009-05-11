//
// coroutine.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_DETAIL_COROUTINE_HPP
#define URDL_DETAIL_COROUTINE_HPP

namespace urdl {
namespace detail {

class coroutine
{
protected:
  coroutine() : coro_value_(0) {}
  int coro_value_;
};

#define URDL_CORO_BEGIN \
  switch (this->coroutine::coro_value_) \
  { \
  case 0:

#define URDL_CORO_YIELD(s) \
  do \
  { \
    this->coroutine::coro_value_ = __LINE__; \
    s; \
    return; \
  case __LINE__: \
    ; \
  } while (0)

#define URDL_CORO_END \
  }

} // namespace detail
} // namespace urdl

#endif // URDL_DETAIL_COROUTINE_HPP
