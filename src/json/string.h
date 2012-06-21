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

#include <cmath>
#include <string>
#include "json/def.h"

namespace json
{

  template < typename Number >
  inline Number __mul_10(const Number &x)
  {
    return x * 10;
  }

  template < typename Number >
  inline Number __div_10(const Number &x)
  {
    return x / 10;
  }

  template < typename Number >
  inline Number __mod_10(const Number &x)
  {
    return x % 10;
  }

  inline float __mod_10(const float &x)
  {
    return std::fmod(x, 10);
  }

  inline double __mod_10(const double &x)
  {
    return std::fmod(x, 10);
  }

  inline long double __mod_10(const long double &x)
  {
    return std::fmod(x, 10);
  }

  template < typename Number >
  inline bool __greater_or_equal_to_one(const Number &x)
  {
    return x >= 1;
  }

  template < typename Number >
  inline bool __is_neg(const Number &x)
  {
    return x < 0;
  }

  template < typename Number >
  inline bool __is_zero(const Number &x)
  {
    return x == 0;
  }

  template < typename Integer, typename Char, typename Traits, typename Allocator >
  inline void __int_to_str(const Integer x,
			   std::basic_string<Char, Traits, Allocator> &s)
  {
    const Integer div = __div_10(x);
    if (__greater_or_equal_to_one(div))
      {
	__int_to_str(div, s);
      }
    s.push_back(int(__mod_10(x)) + '0');
  }

  template < typename Integer, typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  __itos(Integer x, const Allocator &a)
  {
    std::basic_string<Char, Traits, Allocator> s ( a );
    s.reserve(30);
    if (__is_neg(x))
      {
	s.push_back('-');
	s.push_back(__mod_10(x) + '0');
	x = __div_10(x);
      }
    __int_to_str(x, s);
    return s;
  }

  template < typename Integer, typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  __utos(const Integer x, const Allocator &a)
  {
    std::basic_string<Char, Traits, Allocator> s ( a );
    s.reserve(30);
    __int_to_str(x, s);
    return s;
  }

  template < typename Float, typename Char, typename Traits, typename Allocator >
  inline void __float_decimal_part_to_str(Float x,
                                          std::basic_string<Char, Traits, Allocator> &s)
  {
    s.push_back('.');
    if (__is_zero(x))
      {
        s.push_back('0');
        return;
      }
    const std::size_t max_iterations = std::numeric_limits<Float>::digits10;
    for (std::size_t i = 0; (i != max_iterations) && !__is_zero(x); ++i)
      {
        Float digit;
        x = __mul_10(x);
        x = std::modf(x, &digit);
        s.push_back(int(digit) + '0');
      }
  }

  template < typename Float, typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  __ftos(const Float x, const Allocator &a)
  {
    Float integral = 0;
    Float decimal  = 0;

    decimal = std::modf(x, &integral);

    std::basic_string<Char, Traits, Allocator> s ( __itos<Float, Char, Traits, Allocator>(integral, a) );
    __float_decimal_part_to_str(std::abs(decimal), s);
    return s;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const short x, const Allocator &a = Allocator())
  {
    return __itos<short, Char, Traits, Allocator>(x, a);
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
  to_string(const unsigned short x, const Allocator &a = Allocator())
  {
    return __utos<unsigned short, Char, Traits, Allocator>(x, a);
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

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const float x, const Allocator &a = Allocator())
  {
    return __ftos<float, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const double x, const Allocator &a = Allocator())
  {
    return __ftos<double, Char, Traits, Allocator>(x, a);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline std::basic_string<Char, Traits, Allocator>
  to_string(const long double x, const Allocator &a = Allocator())
  {
    return __ftos<long double, Char, Traits, Allocator>(x, a);
  }

}

#endif // JSON_STRING_H
