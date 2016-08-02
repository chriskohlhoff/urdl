//
// ssl.hpp
// ~~~~~~~
//
// Copyright (c) 2016 Yahoo Japan Corporation
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_SSL_HPP
#define URDL_SSL_HPP

#include <string>

namespace urdl {
namespace ssl {

/// Option to specify whether to verify peer.
/**
 * @par Remarks
 * The default value is true.
 * Set the option to false to disable verification.
 *
 * @par Example
 * To disable peer verification for an object of class @c urdl::istream:
 * @code
 * urdl::istream is;
 * is.set_option(urdl::ssl::verify_peer(false));
 * is.open("https://localhost");
 * @endcode
 *
 * To disable peer verification for an object of class
 * @c urdl::read_stream:
 * @code
 * urdl::read_stream stream;
 * stream.set_option(urdl::ssl::verify_peer(false));
 * stream.open("https://localhost/");
 * @endcode
 *
 * @par Requirements
 * @e Header: @c <urdl/ssl.hpp> @n
 * @e Namespace: @c urdl::ssl
 */
class verify_peer {
public:
  /// Constructs an object of class @c verify_peer.
  /**
   * @par Remarks
   * Postcondition: <tt>value() == true</tt>.
   */
  verify_peer()
    : value_(true)
  {
  }

  /// Constructs an object of class @c verify_peer.
  /**
   * @param v The desired value for the option.
   *
   * @par Remarks
   * Postcondition: <tt>value() == v</tt>
   */
  explicit verify_peer(const bool& v)
    : value_(v)
  {
  }

  /// Gets the value of the option.
  /**
   * @returns The value of the option.
   */
  bool value() const
  {
    return value_;
  }

  /// Sets the value of the option.
  /**
   * @param v The desired value for the option.
   *
   * @par Remarks
   * Postcondition: <tt>value() == v</tt>
   */
  void value(const bool& v)
  {
    value_ = v;
  }

private:
  bool value_;
};

/// Option to specify a path to CA's certificate.
/**
 * @par Remarks
 * The default is to not specify CA's certificate,
 * and the system default certificates are used.
 *
 * @par Example
 * To set CA's certificate for an object of class @c urdl::istream:
 * @code
 * urdl::istream is;
 * is.set_option(urdl::ssl::ca_cert("/path/to/cert.crt"));
 * is.open("https://localhost/");
 * @endcode
 *
 * To set CA's certificate for an object of class @c urdl::read_stream:
 * @code
 * urdl::read_stream stream;
 * stream.set_option(urdl::ssl::ca_cert("/path/to/cert.crt"));
 * stream.open("https://localhost/");
 * @endcode
 *
 * @par Requirements
 * @e Header: @c <urdl/ssl.hpp> @n
 * @e Namespace: @c urdl::ssl
 */
class ca_cert {
public:
  /// Constructs an object of class @c ca_cert.
  /**
   * @par Remarks
   * Postcondition: <tt>value() == ""</tt>.
   */
  ca_cert()
    : value_("")
  {
  }

  /// Constructs an object of class @c ca_cert.
  /**
   * @param v The desired value for the option.
   *
   * @par Remarks
   * Postcondition: <tt>value() == v</tt>
   */
  explicit ca_cert(const std::string& v)
    : value_(v)
  {
  }

  /// Gets the value of the option.
  /**
   * @returns The value of the option.
   */
  std::string value() const
  {
    return value_;
  }

  /// Sets the value of the option.
  /**
   * @param v The desired value for the option.
   *
   * @par Remarks
   * Postcondition: <tt>value() == v</tt>
   */
  void value(const std::string& v)
  {
    value_ = v;
  }

private:
  std::string value_;
};

/// Option to specify a path to client certificate.
/**
 * @par Remarks
 * The default is to not send client certificate.
 *
 * @par Example
 * To set client certificate for an object of class @c urdl::istream:
 * @code
 * urdl::istream is;
 * is.set_option(urdl::ssl::client_cert("/path/to/cert.crt"));
 * is.open("https://localhost/");
 * @endcode
 *
 * To set client certificate for an object of class @c urdl::read_stream:
 * @code
 * urdl::read_stream stream;
 * stream.set_option(urdl::ssl::client_cert("/path/to/cert.crt"));
 * stream.open("https://localhost/");
 * @endcode
 *
 * @par Requirements
 * @e Header: @c <urdl/ssl.hpp> @n
 * @e Namespace: @c urdl::ssl
 */
class client_cert {
public:
  client_cert()
    : certificate_path_(""), private_key_path_("")
  {
  }

  client_cert(std::string& cert_path, std::string& key_path)
    : certificate_path_(cert_path), private_key_path_(key_path)
  {
  }

  std::string certificate_path() const
  {
    return certificate_path_;
  }

  void certificate_path(const std::string& p)
  {
    certificate_path_ = p;
  }

  std::string private_key_path() const
  {
    return private_key_path_;
  }

  void private_key_path(const std::string& p)
  {
    private_key_path_ = p;
  }

private:
  std::string certificate_path_;
  std::string private_key_path_;
};

} // namespace ssl
} // namespace urdl

#endif // URDL_SSL_HPP
