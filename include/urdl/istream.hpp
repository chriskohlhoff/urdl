//
// istream.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_ISTREAM_HPP
#define URDL_ISTREAM_HPP

#include <istream>
#include <boost/utility/base_from_member.hpp>
#include <boost/system/error_code.hpp>
#include "urdl/istreambuf.hpp"

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

class istream
  : private boost::base_from_member<istreambuf>,
    public std::basic_istream<char>
{
public:
  /// Constructs an object of type @c istream.
  /**
   * @note Initializes the base class with @c std::basic_istream<char>(sb),
   * where sb is an object of type @c istreambuf stored within the class.
   */
  istream()
    : std::basic_istream<char>(
        &this->boost::base_from_member<istreambuf>::member)
  {
  }

  /// Constructs an object of type @c istream.
  /**
   * @param u The URL to open.
   *
   * @note Initializes the base class with @c std::basic_istream<char>(sb),
   * where @c sb is an object of type @c istreambuf stored within the class. It
   * also opens @c sb by performing @c sb.open(u) and, if that fails (returns a
   * null pointer), calls @c setstate(failbit).
   */
  explicit istream(const url& u)
    : std::basic_istream<char>(
        &this->boost::base_from_member<istreambuf>::member)
  {
    if (rdbuf()->open(u) == 0)
      setstate(std::ios_base::failbit);
  }

  /// Determines whether the stream is open.
  /**
   * @returns @c true if the stream is open, @c false otherwise.
   *
   * @note Returns @c rdbuf()->is_open().
   */
  bool is_open() const
  {
    return rdbuf()->is_open();
  }

  /// Opens the specified URL.
  /**
   * @param u The URL to open.
   *
   * @note Calls @c rdbuf()->open(u). If that function does not return a null
   * pointer, calls @c clear(). Otherwise calls @c setstate(failbit) (which may
   * throw @c ios_base::failure).
   */
  void open(const url& u)
  {
    if (rdbuf()->open(u) == 0)
      setstate(std::ios_base::failbit);
    else
      clear();
  }

  /// Closes the stream.
  /**
   * @note Calls @c rdbuf()->close() and, if that function returns a null
   * pointer, calls @c setstate(failbit) (which may throw @c ios_base::failure).
   */
  void close()
  {
    if (rdbuf()->close() == 0)
      setstate(std::ios_base::failbit);
  }

  /// Gets the underlying stream buffer.
  /**
   * @returns A pointer to the stream buffer contained within the class.
   */
  istreambuf* rdbuf() const
  {
    return const_cast<istreambuf*>(
        &this->boost::base_from_member<istreambuf>::member);
  }

  /// Gets the last error associated with the stream.
  /**
   * @returns An @c error_code corresponding to the last error from the stream.
   *
   * @note Returns a reference to an @c error_code object representing the last
   * failure reported by an @c istreambuf function. The set of possible
   * @c error_code values and categories depends on the protocol of the URL
   * used to open the stream.
   */
  const boost::system::error_code& error() const
  {
    return rdbuf()->puberror();
  }

  /// Gets the read timeout of the stream.
  /**
   * @returns The timeout, in milliseconds, used for individual read operations
   * on the underlying transport.
   *
   * @note Returns @c rdbuf()->read_timeout().
   */
  std::size_t read_timeout() const
  {
    return rdbuf()->read_timeout();
  }

  /// Sets the read timeout of the stream.
  /**
   * @param milliseconds The timeout, in milliseconds, to be used for individual
   * read operations on the underlying transport.
   *
   * @note Performs @c rdbuf()->read_timeout(milliseconds).
   */
  void read_timeout(std::size_t milliseconds)
  {
    rdbuf()->read_timeout(milliseconds);
  }

  /// Gets the MIME type of the content obtained from the URL.
  /**
   * @returns A string specifying the MIME type. Examples of possible return
   * values include @c text/plain, @c text/html and @c image/png.
   *
   * @note Returns @c rdbuf()->content_type().
   *
   * Not all URL protocols support a content type. For these protocols, this
   * function returns an empty string.
   */
  std::string content_type() const
  {
    return rdbuf()->content_type();
  }

  /// Gets the length of the content obtained from the URL.
  /**
   * @returns The length, in bytes, of the content. If the content associated
   * with the URL does not specify a length,
   * @c std::numeric_limits<std::size_t>::max().
   *
   * @note Returns @c rdbuf()->content_length().
   */
  std::size_t content_length() const
  {
    return rdbuf()->content_length();
  }

  /// Gets the protocol-specific headers obtained from the URL.
  /**
   * @returns A string containing the headers returned with the content from the
   * URL. The format and interpretation of these headers is specific to the
   * protocol associated with the URL.
   *
   * @note Returns @c rdbuf()->headers().
   */
  std::string headers() const
  {
    return rdbuf()->headers();
  }
};

} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#endif // URDL_ISTREAM_HPP
