//
// option_set.ipp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_OPTION_SET_IPP
#define URDL_OPTION_SET_IPP

#if defined(URDL_HEADER_ONLY)
# define URDL_INLINE inline
#else
# define URDL_INLINE
#endif

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

URDL_INLINE
option_set::option_set(const option_set& other)
{
  std::auto_ptr<option_base>* prev_link = &head_;
  option_base* node = other.head_.get();
  while (node)
  {
    prev_link->reset(node->clone());
    prev_link = &prev_link->get()->next;
    node = node->next.get();
  }
}

URDL_INLINE
option_set& option_set::operator=(const option_set& other)
{
  option_set tmp(other);
  head_.reset(tmp.head_.release());
  return *this;
}

URDL_INLINE
void option_set::set_options(const option_set& other)
{
  option_base* node = other.head_.get();
  while (node)
  {
    set_option_base(node->clone());
    node = node->next.get();
  }
}

URDL_INLINE
void option_set::set_option_base(option_set::option_base* o)
{
  std::auto_ptr<option_base>* prev_link = &head_;
  option_base* node = head_.get();
  while (node)
  {
    if (o->type_info() == node->type_info())
    {
      o->next.reset(node->next.release());
      prev_link->reset(o);
      return;
    }
    prev_link = &node->next;
    node = node->next.get();
  }
  prev_link->reset(o);
}

URDL_INLINE
option_set::option_base* option_set::get_option_base(
    const std::type_info& ti) const
{
  option_base* node = head_.get();
  while (node)
  {
    if (ti == node->type_info())
      return node;
    node = node->next.get();
  }
  return 0;
}

URDL_INLINE
void option_set::clear_option_base(const std::type_info& ti)
{
  std::auto_ptr<option_base>* prev_link = &head_;
  option_base* node = head_.get();
  while (node)
  {
    if (ti == node->type_info())
    {
      prev_link->reset(node->next.release());
      return;
    }
    prev_link = &node->next;
    node = node->next.get();
  }
}

} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#undef URDL_INLINE

#endif // URDL_OPTION_SET_IPP
