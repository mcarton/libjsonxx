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

#ifndef JSON_PARSING_HPP
#define JSON_PARSING_HPP

#include <tuple>
#include <cmath>
#include <cctype>
#include <iosfwd>
#include <iterator>
#include "json/parsing.h"

namespace json
{

  void error_parsing_non_empty_end(const char *function);

  void error_parsing_empty_number(const char *function);

  void error_parsing_non_digit(const char *function);

  template < typename Iterable >
  auto begin(Iterable &&it) -> decltype(std::begin(*it))
  {
    return std::begin(*it);
  }

  template < typename Iterable >
  auto end(Iterable &&it) -> decltype(std::end(*it))
  {
    return std::end(*it);
  }

  template < typename Char, typename Traits >
  std::istream_iterator<Char> begin(std::basic_istream<Char, Traits> &is)
  {
    return std::istream_iterator<Char>(is);
  }

  template < typename Char, typename Traits >
  std::istream_iterator<Char> end(std::basic_istream<Char, Traits> &)
  {
    return std::istream_iterator<Char>();
  }

  template < typename T >
  constexpr bool one_of(T &&)
  {
    return false;
  }

  template < typename T, typename U, typename... Args >
  constexpr bool one_of(T &&x, U &&y, Args&&... args)
  {
    return (x == y) ? true : one_of(x, std::forward<Args>(args)...);
  }

  template < typename InputIterator >
  void assert_empty(const InputIterator &first,
			   const InputIterator &last,
			   const char *function)
  {
    if (first != last)
      {
	error_parsing_non_empty_end(function);
      }
  }

  template < typename InputIterator >
  void assert_non_empty(const InputIterator &first,
			       const InputIterator &last,
			       const char *function)
  {
    if (first == last)
      {
	error_parsing_empty_number(function);
      }
  }

  template < typename InputIterator >
  void assert_has_digit(const InputIterator &first,
			       const InputIterator &,
			       const char *function)
  {
    if (!std::isdigit(*first))
      {
	error_parsing_non_digit(function);
      }
  }

  template < typename Number, typename InputIterator >
  std::tuple<Number, InputIterator> parse_sign(InputIterator first,
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
  std::tuple<Number, InputIterator> parse_integer(InputIterator first,
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
  Integer str_to_int(InputIterator first,
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
  int stoi(const InputIterator &first, const InputIterator &last)
  {
    return str_to_int<int>(first, last, "json::stoi");
  }

  template < typename InputIterator >
  long stol(const InputIterator &first, const InputIterator &last)
  {
    return str_to_int<long>(first, last, "json::stol");
  }

  template < typename InputIterator >
  long long stoll(const InputIterator &first, const InputIterator &last)
  {
    return str_to_int<long long>(first, last, "json::stoll");
  }

  template < typename Iterable >
  int stoi(Iterable &object)
  {
    return stoi(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  long stol(Iterable &object)
  {
    return stol(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  long long stoll(Iterable &object)
  {
    return stoll(std::begin(object), std::end(object));
  }

  template < typename Float, typename InputIterator >
  std::tuple<Float, InputIterator> parse_decimal(InputIterator first,
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
  std::tuple<Float, InputIterator> parse_exponent(InputIterator first,
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
  Float str_to_float(InputIterator first,
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
  float stof(const InputIterator &first, const InputIterator &last)
  {
    return str_to_float<double>(first, last, "json::stof");
  }

  template < typename InputIterator >
  double stod(const InputIterator &first, const InputIterator &last)
  {
    return str_to_float<double>(first, last, "json::stod");
  }

  template < typename InputIterator >
  long double stold(const InputIterator &first, const InputIterator &last)
  {
    return str_to_float<long double>(first, last, "json::stod");
  }

  template < typename Iterable >
  float stof(Iterable &object)
  {
    return stof(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  double stod(Iterable &object)
  {
    return stof(std::begin(object), std::end(object));
  }

  template < typename Iterable >
  long double stold(Iterable &object)
  {
    return stof(std::begin(object), std::end(object));
  }

  template < typename InputIterator, typename OutputCallback >
  bool read_sign(InputIterator &first,
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
  bool read_digits(InputIterator &first,
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
  bool read_decimal_point(InputIterator &first,
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
  bool read_exponent_symbol(InputIterator &first,
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
  bool read_number(InputIterator &first,
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
  bool is_json_number(InputIterator first, InputIterator last)
  {
    typedef typename std::iterator_traits<InputIterator>::reference reference;
    if (read_number(first, last, [](reference) {}))
      {
	return first == last;
      }
    return false;
  }

  template < typename Iterable >
  bool is_json_number(Iterable &&object)
  {
    return is_json_number(std::begin(object), std::end(object));
  }

  template < typename InputIterator, int N >
  bool equals(InputIterator &first, InputIterator &last, const char (&str)[N])
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
  bool is_json_true(InputIterator first, InputIterator last)
  {
    return equals(first, last, "true"); 
  }

  template < typename Iterable >
  bool is_json_true(Iterable &&object)
  {
    return is_json_true(std::begin(object), std::end(object));
  }

  template < typename InputIterator >
  bool is_json_false(InputIterator first, InputIterator last)
  {
    return equals(first, last, "false"); 
  }

  template < typename Iterable >
  bool is_json_false(Iterable &&object)
  {
    return is_json_false(std::begin(object), std::end(object));
  }

  template < typename InputIterator >
  bool is_json_boolean(InputIterator first, InputIterator last)
  {
    return is_json_true(first, last) || is_json_false(first, last);
  }

  template < typename Iterable >
  bool is_json_boolean(Iterable &&object)
  {
    return is_json_boolean(std::begin(object), std::end(object));
  }

}

#endif // JSON_PARSING_HPP
