/*
 * Copyright 2012 Achille Roussel.
 *
 * This file is part of Libjson++.
 *
 * Libjson++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Libjson++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Libjson++.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSON_STRING_H
#define JSON_STRING_H

#include <string>
#include "json/def.h"

namespace json
{

  template < typename Integer, typename Char, typename Traits, typename Allocator >
  inline void __int_to_str(const Integer x,
			   std::basic_string<Char, Traits, Allocator> &s)
  {
    const Integer div = x / 10;
    if (div)
      {
	__int_to_str(div, s);
      }
    s.push_back((x % 10) + '0');
  }

  template < typename Integer, typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  __itos(Integer x, const Allocator &a)
  {
    std::basic_string<Char, Traits, Allocator> s (a);
    s.reserve(30);
    if (x < 0)
      {
	s.push_back('-');
	s.push_back((x % 10) + '0');
	x /= 10;
      }
    __int_to_str(x, s);
    return s;
  }

  template < typename Integer, typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  __utos(const Integer x, const Allocator &a)
  {
    std::basic_string<Char, Traits, Allocator> s (a);
    s.reserve(30);
    __int_to_str(x, s);
    return s;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const int x, const Allocator &a = Allocator())
  {
    return __itos<int, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const long x, const Allocator &a = Allocator())
  {
    return __itos<long, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const long long x, const Allocator &a = Allocator())
  {
    return __itos<long long, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const unsigned int x, const Allocator &a = Allocator())
  {
    return __utos<unsigned int, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const unsigned long x, const Allocator &a = Allocator())
  {
    return __utos<unsigned long, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const unsigned long long x, const Allocator &a = Allocator())
  {
    return __utos<unsigned long long, Char, Traits, Allocator>(x, a);
  }

}

#endif // JSON_STRING_H
