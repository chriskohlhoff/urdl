//
// url.hpp
// ~~~~~~~
//
// Copyright (c) 2009 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// path LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef URDL_URL_HPP
#define URDL_URL_HPP

#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <boost/throw_exception.hpp>

#include "urdl/detail/abi_prefix.hpp"

namespace urdl {

class url
{
public:
  url(const char* url_string)
  {
    parse(url_string);
  }

  url(const std::string& url_string)
  {
    parse(url_string.c_str());
  }

  std::string protocol() const
  {
    return protocol_;
  }

  std::string user_info() const
  {
    return user_info_;
  }

  std::string host() const
  {
    return host_;
  }

  unsigned short port() const
  {
    return std::atoi(port_.c_str());
  }

  std::string path() const
  {
    std::string tmp_path;
    unescape(path_, tmp_path);
    return tmp_path;
  }

  std::string query() const
  {
    return query_;
  }

  std::string fragment() const
  {
    return fragment_;
  }

  enum
  {
    protocol_part = 1,
    user_info_part = 2,
    host_part = 4,
    port_part = 8,
    path_part = 16,
    query_part = 32,
    fragment_part = 64,
    all_parts = protocol_part | user_info_part | host_part
      | port_part | path_part | query_part | fragment_part
  };

  std::string to_string(int parts = all_parts) const
  {
    std::string s;

    if ((parts & protocol_part) != 0 && !protocol_.empty())
    {
      s = protocol_;
      s += "://";
    }

    if ((parts & user_info_part) != 0 && !user_info_.empty())
    {
      s += user_info_;
      s += "@";
    }

    if ((parts & host_part) != 0)
      s += host_;

    if ((parts & port_part) != 0 && !port_.empty())
    {
      s += ":";
      s += port_;
    }

    if ((parts & path_part) != 0 && !path_.empty())
    {
      s += path_;
    }

    if ((parts & query_part) != 0 && !query_.empty())
    {
      s += "?";
      s += query_;
    }

    if ((parts & fragment_part) != 0 && !fragment_.empty())
    {
      s += "#";
      s += fragment_;
    }

    return s;
  }

private:
  void parse(const char* p)
  {
    // Protocol.
    std::size_t length = std::strcspn(p, ":");
    protocol_.assign(p, p + length);
    for (std::size_t i = 0; i < protocol_.length(); ++i)
      protocol_[i] = std::tolower(protocol_[i]);
    p += length;

    // "://"
    check(*p++ == ':');
    check(*p++ == '/');
    check(*p++ == '/');

    // UserInfo.
    length = std::strcspn(p, "@:/?#");
    if (p[length] == '@')
    {
      user_info_.assign(p, p + length);
      p += length;
    }
    else if (p[length] == ':')
    {
      std::size_t length2 = std::strcspn(p + length, "@/?#");
      if (p[length + length2] == '@')
      {
        user_info_.assign(p, p + length + length2);
        p += length + length2;
      }
    }

    // Host.
    length = std::strcspn(p, ":/?#");
    host_.assign(p, p + length);
    p += length;

    // Port.
    if (*p == ':')
    {
      length = std::strcspn(++p, "/?#");
      check(length > 0);
      for (std::size_t i = 0; i < port_.length(); ++i)
        check(std::isdigit(port_[i]));
      p += length;
    }
    else
      port_ = default_port(protocol_);

    // Path.
    if (*p == '/')
    {
      length = std::strcspn(p, "?#");
      path_.assign(p, p + length);
      std::string tmp_path;
      if (!unescape(path_, tmp_path))
      {
        std::invalid_argument ex("Malformed URL");
        boost::throw_exception(ex);
      }
      p += length;
    }
    else
      path_ = "/";

    // Query.
    if (*p == '?')
    {
      length = std::strcspn(++p, "#");
      query_.assign(p, p + length);
      p += length;
    }

    // Fragment.
    if (*p == '#')
      fragment_.assign(++p);
  }

  static void check(bool condition)
  {
    if (!condition)
    {
      std::invalid_argument ex("Malformed URL");
      boost::throw_exception(ex);
    }
  }

  static std::string default_port(const std::string& protocol)
  {
    if (protocol == "http")
      return "80";
    if (protocol == "https")
      return "443";
    if (protocol == "ftp")
      return "21";
    return "0";
  }

  static bool unescape(const std::string& in, std::string& out)
  {
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i)
    {
      if (in[i] == '%')
      {
        if (i + 3 <= in.size())
        {
          unsigned int value = 0;
          for (std::size_t j = i + 1; j < i + 3; ++j)
          {
            switch (in[j])
            {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
              value += in[j] - '0';
              break;
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
              value += in[j] - 'a' + 10;
              break;
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
              value += in[j] - 'A' + 10;
              break;
            default:
              return false;
            }
            if (j == i + 1)
              value <<= 4;
          }
          out += static_cast<char>(value);
          i += 2;
        }
        else
          return false;
      }
      else
        out += in[i];
    }
    return true;
  }

  std::string protocol_;
  std::string user_info_;
  std::string host_;
  std::string port_;
  std::string path_;
  std::string query_;
  std::string fragment_;
};

} // namespace urdl

#include "urdl/detail/abi_suffix.hpp"

#endif // URDL_URL_HPP
