//
// option_set.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// path LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_OPTION_SET_HPP
#define URDL_OPTION_SET_HPP

#include <memory>
#include <typeinfo>
#include "urdl/detail/config.hpp"

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

class URDL_DECL option_set
{
public:
  option_set()
  {
  }

  option_set(const option_set& other);

  option_set& operator=(const option_set& other);

  template <typename Option>
  void set_option(const Option& o)
  {
    set_option_wrapper_base(new option_wrapper<Option>(o));
  }

  void set_options(const option_set& other);

  template <typename Option>
  Option get_option() const
  {
    if (option_wrapper_base* o
        = get_option_wrapper_base(typeid(option_wrapper<Option>)))
      return static_cast<option_wrapper<Option>*>(o)->value;
    return Option();
  }

  template <typename Option>
  void clear_option()
  {
    clear_option_wrapper_base(typeid(option_wrapper<Option>));
  }

private:
  struct option_wrapper_base
  {
    virtual ~option_wrapper_base() {}
    virtual const std::type_info& type_info() const = 0;
    virtual option_wrapper_base* clone() const = 0;
    std::auto_ptr<option_wrapper_base> next;
  };

  template <typename Option>
  struct option_wrapper : option_wrapper_base
  {
    option(const Option& o) : value(o) {}
    const std::type_info& type_info() const
    { return typeid(option_wrapper<Option>); }
    option_wrapper_base* clone() const
    { return new option_wrapper<Option>(value); }
    Option value;
  };

  void set_option_wrapper_base(option_wrapper_base* o);
  option_wrapper_base* get_option_wrapper_base(const std::type_info& ti) const;
  void clear_option_wrapper_base(const std::type_info& ti);

  std::auto_ptr<option_wrapper_base> head_;
};

} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#if defined(URDL_HEADER_ONLY)
# include "urdl/impl/option_set.ipp"
#endif

#endif // URDL_OPTION_SET_HPP
