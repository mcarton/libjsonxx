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

#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include <tuple>
#include <cmath>
#include <cctype>
#include <iterator>
#include "json/def.h"

namespace json
{

  void error_parsing_non_empty_end(const char *function);
  void error_parsing_empty_number(const char *function);
  void error_parsing_non_digit(const char *function);

  template < typename T >
  constexpr inline bool one_of(T &&)
  {
    return false;
  }

  template < typename T, typename U, typename... Args >
  constexpr inline bool one_of(T &&x, U &&y, Args&&... args)
  {
    return (x == y) ? true : one_of(x, std::forward<Args>(args)...);
  }

  template < typename InputIterator >
  inline void assert_empty(const InputIterator &first,
			   const InputIterator &last,
			   const char *function)
  {
    if (first != last)
      {
	error_parsing_non_empty_end(function);
      }
  }

  template < typename InputIterator >
  inline void assert_non_empty(const InputIterator &first,
			       const InputIterator &last,
			       const char *function)
  {
    if (first == last)
      {
	error_parsing_empty_number(function);
      }
  }

  template < typename InputIterator >
  inline void assert_has_digit(const InputIterator &first,
			       const InputIterator &,
			       const char *function)
  {
    if (!std::isdigit(*first))
      {
	error_parsing_non_digit(function);
      }
  }

  template < typename Number, typename InputIterator >
  inline std::tuple<Number, InputIterator> parse_sign(InputIterator first,
						      InputIterator last,
						      const char *function)
  {
    Number sign = 1;
    assert_non_empty(first, last, function);
    switch (*first)
      {
      case '-': sign = -1;
      case '+': ++first;
      }
    return std::make_tuple(sign, first);
  }

  template < typename Number, typename InputIterator >
  inline std::tuple<Number, InputIterator> parse_integer(InputIterator first,
							 InputIterator last,
							 const char *function)
  {
    Number value = 0;
    assert_non_empty(first, last, function);
    assert_has_digit(first, last, function);
    while ((first != last) && std::isdigit(*first))
      {
	value *= 10;
	value += (*first) - '0';
	++first;
      }
    return std::make_tuple(value, first);
  }

  template < typename Integer, typename InputIterator >
  inline Integer str_to_int(InputIterator first,
			    InputIterator last,
			    const char *function)
  {
    Integer sign  = 1;
    Integer value = 0;

    std::tie(sign, first) = parse_sign<Integer>(first, last, function);
    std::tie(value, first) = parse_integer<Integer>(first, last, function);
    assert_empty(first, last, function);
    return sign * value;
  }

  template < typename InputIterator >
  inline int stoi(const InputIterator &first, const InputIterator &last)
  {
    return str_to_int<int>(first, last, "json::stoi");
  }

  template < typename InputIterator >
  inline long stol(const InputIterator &first, const InputIterator &last)
  {
    return str_to_int<long>(first, last, "json::stol");
  }

  template < typename InputIterator >
  inline long long stoll(const InputIterator &first, const InputIterator &last)
  {
    return str_to_int<long long>(first, last, "json::stoll");
  }

  template < typename Iterable >
  inline int stoi(Iterable &object)
  {
    return stoi(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  inline long stol(Iterable &object)
  {
    return stol(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  inline long long stoll(Iterable &object)
  {
    return stoll(std::begin(object), std::end(object));
  }

  template < typename Float, typename InputIterator >
  inline std::tuple<Float, InputIterator> parse_decimal(InputIterator first,
							InputIterator last,
							const char *function)
  {
    Float value = 0;

    if ((first != last) && one_of(*first, '.'))
      {
	++first;
	assert_non_empty(first, last, function);
	assert_has_digit(first, last, function);
	while ((first != last) && std::isdigit(*first))
	  {
	    value /= 10;
	    value += (*first) - '0';
	    ++first;
	  }
      }
    return std::make_tuple(value / 10, first);
  }

  template < typename Float, typename InputIterator >
  inline std::tuple<Float, InputIterator> parse_exponent(InputIterator first,
							 InputIterator last,
							 const char *function)
  {
    Float sign  = 0;
    Float value = 0;

    if ((first != last) && one_of(*first, 'e', 'E'))
      {
	++first;
	std::tie(sign, first)  = parse_sign<Float>(first, last, function);
	std::tie(value, first) = parse_integer<Float>(first, last, function);
      }
    return std::make_tuple(sign * value, first);
  }

  template < typename Float, typename InputIterator >
  inline Float str_to_float(InputIterator first,
			    InputIterator last,
			    const char *function)
  {
    Float sign    = 0;
    Float i_value = 0;
    Float d_value = 0;
    Float e_value = 0;

    std::tie(sign, first)    = parse_sign<Float>(first, last, function);
    std::tie(i_value, first) = parse_integer<Float>(first, last, function);
    std::tie(d_value, first) = parse_decimal<Float>(first, last, function);
    std::tie(e_value, first) = parse_exponent<Float>(first, last, function);
    assert_empty(first, last, function);
    return sign * ((i_value + d_value) * std::pow(10, e_value));
  }

  template < typename InputIterator >
  inline float stof(const InputIterator &first, const InputIterator &last)
  {
    return str_to_float<double>(first, last, "json::stof");
  }

  template < typename InputIterator >
  inline double stod(const InputIterator &first, const InputIterator &last)
  {
    return str_to_float<double>(first, last, "json::stod");
  }

  template < typename InputIterator >
  inline long double stold(const InputIterator &first, const InputIterator &last)
  {
    return str_to_float<long double>(first, last, "json::stod");
  }

  template < typename Iterable >
  inline float stof(Iterable &object)
  {
    return stof(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  inline double stod(Iterable &object)
  {
    return stof(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  inline long double stold(Iterable &object)
  {
    return stof(std::begin(object), std::end(object));
  }

  template < typename InputIterator, typename OutputCallback >
  inline bool read_sign(InputIterator &first,
			InputIterator &last,
			const OutputCallback &callback)
  {
    if ((first != last) && one_of(*first, '+', '-'))
      {
	callback(*first);
	++first;
	return true;
      }
    return false;
  }

  template < typename InputIterator, typename OutputCallback >
  inline bool read_digits(InputIterator &first,
			  InputIterator &last,
			  const OutputCallback &callback)
  {
    bool ok = false;
    while ((first != last) && std::isdigit(*first))
      {
	callback(*first);
	++first;
	ok = true;
      }
    return ok;
  }

  template < typename InputIterator, typename OutputCallback >
  inline bool read_decimal_point(InputIterator &first,
				 InputIterator &last,
				 const OutputCallback &callback)
  {
    if ((first != last) && one_of(*first, '.'))
      {
	callback(*first);
	++first;
	return true;
      }
    return false;
  }

  template < typename InputIterator, typename OutputCallback >
  inline bool read_exponent_symbol(InputIterator &first,
				   InputIterator &last,
				   const OutputCallback &callback)
  {
    if ((first != last) && one_of(*first, 'e', 'E'))
      {
	callback(*first);
	++first;
	return true;
      }
    return false;
  }

  template < typename InputIterator, typename OutputCallback >
  inline bool read_number(InputIterator &first,
			  InputIterator &last,
			  const OutputCallback &callback)
  {
    read_sign(first, last, callback);
    if (!read_digits(first, last, callback))
      {
	return false;
      }

    if (read_decimal_point(first, last, callback))
      {
	if (!read_digits(first, last, callback))
	  {
	    return false;
	  }
      }

    if (read_exponent_symbol(first, last, callback))
      {
	read_sign(first, last, callback);
	if (!read_digits(first, last, callback))
	  {
	    return false;
	  }
      }

    return true;
  }

  template < typename InputIterator >
  inline bool is_json_number(InputIterator first, InputIterator last)
  {
    typedef typename std::iterator_traits<InputIterator>::reference reference;
    if (read_number(first, last, [](reference) {}))
      {
	return first == last;
      }
    return false;
  }

  template < typename Iterable >
  inline bool is_json_number(Iterable &&object)
  {
    return is_json_number(std::begin(object), std::end(object));
  }

  template < typename InputIterator, int N >
  inline bool equals(InputIterator &first, InputIterator &last, const char (&str)[N])
  {
    typedef typename std::size_t index;
    index i;
    for (i = 0; (first != last) && (i != (N - 1)); ++i, ++first)
      {
        if ((*first) != str[i])
          {
            return false;
          }
      }
    return i == (N - 1);
  }

  template < typename InputIterator >
  inline bool is_json_true(InputIterator first, InputIterator last)
  {
    return equals(first, last, "true"); 
  }

  template < typename Iterable >
  inline bool is_json_true(Iterable &&object)
  {
    return is_json_true(std::begin(object), std::end(object));
  }

  template < typename InputIterator >
  inline bool is_json_false(InputIterator first, InputIterator last)
  {
    return equals(first, last, "false"); 
  }

  template < typename Iterable >
  inline bool is_json_false(Iterable &&object)
  {
    return is_json_false(std::begin(object), std::end(object));
  }

  template < typename InputIterator >
  inline bool is_json_boolean(InputIterator first, InputIterator last)
  {
    return is_json_true(first, last) || is_json_false(first, last);
  }

  template < typename Iterable >
  inline bool is_json_boolean(Iterable &&object)
  {
    return is_json_boolean(std::begin(object), std::end(object));
  }

}

#endif // JSON_PARSING_H
