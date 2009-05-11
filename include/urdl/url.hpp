//
// url.hpp
// ~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_URL_HPP
#define URDL_URL_HPP

#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <boost/throw_exception.hpp>

namespace urdl {

class url
{
public:
  url(const char* url_string)
    : port_(0)
  {
    parse(url_string);
  }

  url(const std::string& url_string)
    : port_(0)
  {
    parse(url_string.c_str());
  }

  url(const std::string& protocol, const std::string& host,
      unsigned short port, const std::string& file)
    : protocol_(protocol), host_(host), port_(port), file_(file)
  {
  }

  std::string protocol() const
  {
    return protocol_;
  }

  std::string host() const
  {
    return host_;
  }

  unsigned short port() const
  {
    return port_;
  }

  std::string file() const
  {
    return file_;
  }

private:
  void parse(const char* p)
  {
    // Protocol.
    std::size_t length = std::strcspn(p, ":");
    protocol_.assign(p, p + length);
    p += length;

    // "://"
    check(*p++ == ':');
    check(*p++ == '/');
    check(*p++ == '/');

    // Host.
    length = std::strcspn(p, ":/");
    host_.assign(p, p + length);
    p += length;

    if (*p == ':')
    {
      // Port.
      length = std::strcspn(++p, "/");
      port_ = static_cast<unsigned short>(std::atoi(p));
      p += length;
    }
    else
      port_ = default_port(protocol_);

    // File.
    check(*p == '/' || *p == 0);
    file_ = p;
    if (file_.empty())
      file_ = "/";
  }

  void check(bool condition)
  {
    if (!condition)
    {
      std::invalid_argument ex("Malformed URL");
      boost::throw_exception(ex);
    }
  }

  unsigned short default_port(const std::string protocol)
  {
    if (protocol == "http")
      return 80;
    if (protocol == "https")
      return 443;
    if (protocol == "ftp")
      return 21;
    return 0;
  }

  std::string protocol_;
  std::string host_;
  unsigned short port_;
  std::string file_;
};

} // namespace urdl

#endif // URDL_URL_HPP
