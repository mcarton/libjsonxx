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

#ifndef JSON_READER_H
#define JSON_READER_H

#include <iterator>
#include "json/def.h"
#include "json/parsing.h"
#include "json/char_sequence.h"

namespace json
{

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_object(InputIterator &,
                   InputIterator &,
                   basic_object<Char, Traits, Allocator> &);

  void error_invalid_input_eof();
  void error_invalid_input_non_json();

  template < typename InputIterator >
  inline void skip_spaces(InputIterator &first, InputIterator &last)
  {
    while ((first != last) && std::isspace(*first))
      {
	++first;
      }
  }

  template < typename InputIterator >
  inline void next_char(InputIterator &first, InputIterator &last)
  {
    skip_spaces(first, last);
    if (first == last)
      {
	error_invalid_input_eof();
      }
  }

  template < typename InputIterator >
  inline void consume_char(InputIterator &first, InputIterator &last)
  {
    if (first == last)
      {
	error_invalid_input_eof();
      }
    if (std::isspace(*first))
      {
	skip_spaces(first, last);
      }
    else
      {
	++first;
      }
  }

  template < typename InputIterator >
  inline void consume_char(InputIterator &first, InputIterator &last, const int c)
  {
    if ((first != last) && ((*first) != c))
      {
	error_invalid_input_non_json();
      }
    consume_char(first, last);
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_list(InputIterator &first,
			InputIterator &last,
			basic_object<Char, Traits, Allocator> &obj)
  {
    typedef basic_object<Char, Traits, Allocator> object;
    typedef typename std::size_t                  index;

    obj.make_list();
    consume_char(first, last); // consumes '['

    for (index i = 0; (first != last) && ((*first) != ']'); ++i)
      {
	object obj2 (obj.get_allocator());
	read_object(first, last, obj2);
	next_char(first, last);
	obj[i] = std::move(obj2);
	switch (*first)
	  {
	  case ',': consume_char(first, last);
	  case ']': break;
	  default:  error_invalid_input_non_json();
	  }
      }
    consume_char(first, last); // consumes ']'
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_key(InputIterator &first,
		       InputIterator &last,
		       std::basic_string<Char, Traits, Allocator> &str)
  {
    if ((first == last) || ((*first) != '"'))
      {
	error_invalid_input_non_json();
      }

    consume_char(first, last); // consumes '"'
    str.reserve(30);
    while ((first != last) && ((*first) != '"'))
      {

	if ((*first) != '\\')
	  {
	    str.push_back(*first);
	  }

	else
	  {
	    consume_char(first, last); // consumes '\\'
	    switch (*first)
	      {
	      case '"':  str.push_back('"');  break;
	      case '\\': str.push_back('\\'); break;
	      case 'b':  str.push_back('\b'); break;
	      case 'f':  str.push_back('\f'); break;
	      case 'n':  str.push_back('\n'); break;
              case 'r':  str.push_back('\r'); break;
              case 't':  str.push_back('\t'); break;
	      default:   error_invalid_input_non_json();
	      }
	  }

	consume_char(first, last);
      }

    consume_char(first, last); // consumes '"'
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_pair(InputIterator &first,
			InputIterator &last,
			std::basic_string<Char, Traits, Allocator> &key,
			basic_object<Char, Traits, Allocator> &obj)
  {
    read_key(first, last, key);
    next_char(first, last);
    consume_char(first, last, ':');
    read_object(first, last, obj);
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_map(InputIterator &first,
		       InputIterator &last,
		       basic_object<Char, Traits, Allocator> &obj)
  {
    typedef basic_object<Char, Traits, Allocator> object;
    typedef typename object::object_string        string;

    obj.make_map();
    consume_char(first, last); // consumes '{'
    string key (obj.get_allocator());
    while ((first != last) && ((*first) != '}'))
      {
	object obj2 (obj.get_allocator());
	key.clear();
	next_char(first, last);
	read_pair(first, last, key, obj2);
	next_char(first, last);
	obj[key] = std::move(obj2);
	switch (*first)
	  {
	  case ',': consume_char(first, last);
	  case '}': break;
	  default : error_invalid_input_non_json();
	  }
      }
    consume_char(first, last); // consumes '}'
  }

  template < typename InputIterator, int N >
  inline void read_equals(InputIterator &first,InputIterator &last, const char (&str)[N])
  {
    typedef typename std::size_t index;
    for (index i = 0; (first != last) && (i != (N - 1)); ++i)
      {
	consume_char(first, last, str[i]);
      }
  }

  template < typename InputIterator >
  inline void read_null(InputIterator &first, InputIterator &last)
  {
    read_equals(first, last, "null");
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_true(InputIterator &first,
                        InputIterator &last,
                        basic_object<Char, Traits, Allocator> &obj)
  {
    read_equals(first, last, "true");
    obj = true;
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_false(InputIterator &first,
                         InputIterator &last,
                         basic_object<Char, Traits, Allocator> &obj)
  {
    read_equals(first, last, "false");
    obj = false;
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_string(InputIterator &first,
			  InputIterator &last,
			  basic_object<Char, Traits, Allocator> &obj)
  {
    obj.make_string();
    if ((*first) == '"')
      {
	read_key(first, last, obj.get_string());
      }
    else if (!read_number(first, last, [&](const Char &c) { obj.get_string().push_back(c); }))
      {
	error_invalid_input_non_json();
      }
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  inline void read_object(InputIterator &first,
			  InputIterator &last,
			  basic_object<Char, Traits, Allocator> &obj)
  {
    next_char(first, last);
    switch (*first)
      {
      case '[': read_list(first, last, obj);  break;
      case '{': read_map(first, last, obj);   break;
      case 't': read_true(first, last, obj);  break;
      case 'f': read_false(first, last, obj); break;
      case 'n': read_null(first, last);       break;
      default:
	if (one_of(*first, '"', '-', '+', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'))
	  {
	    read_string(first, last, obj);
	  }
      }
  }

  template < typename Iterable, typename Char, typename Traits, typename Allocator >
  inline void read_object(Iterable &iterable, basic_object<Char, Traits, Allocator> &obj)
  {
    auto b = begin(iterable);
    auto e = end(iterable);
    read_object<decltype(b), Char, Traits, Allocator>(b, e, obj);
  }

  /**
   * @brief Reads a JSON object.
   *
   * @param iterable An 'Iterable' object that must feed the parser with a valid
   * JSON string.
   * @param a The allocator to use to create the object.
   * 
   * @return The function returns the newly created JSON object.
   */
  template < typename Iterable,
             typename Object = basic_object< char, std::char_traits<char>, std::allocator<char> > >
  inline Object read(Iterable &iterable, typename Object::allocator_type const &a)
  {
    typedef typename Object::char_type          char_type;
    typedef typename Object::traits_type        traits_type;
    typedef typename Object::allocator_type     allocator_type;
    Object obj ( a );
    read_object<Iterable, char_type, traits_type, allocator_type>(iterable, obj);
    return obj;
  }

  /**
   * @brief Reads a JSON object.
   *
   * @param iterable An 'Iterable' object that must feed the parser with a valid
   * JSON string.
   * 
   * @return The function returns the newly created JSON object.
   */
  template < typename Iterable,
             typename Object = basic_object< char, std::char_traits<char>, std::allocator<char> > >
  inline Object read(Iterable &iterable)
  {
    typedef typename Object::allocator_type allocator;
    return read<Iterable, Object>(iterable, allocator());
  }

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to parse to create a JSON object.
   * @param a   The allocator that will be used to create the JSON object.
   *
   * @return The function returns the newly created JSON object.
   */
  template < typename Object >
  inline Object read(const char *str, typename Object::allocator_type const &a)
  {
    typedef typename Object::allocator_type allocator_type;
    const char_sequence cs ( str, std::strlen(str) );
    return read(cs, a);
  }

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to read the JSON object from.
   *
   * @return The function returns the newly created instance of <em>json::object</em>.
   */
  object read(const char *str);

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to read the JSON object from.
   *
   * @return The function returns the newly created instance of <em>json::object</em>.
   */
  object read(const char_sequence &str);

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to read the JSON object from.
   *
   * @return The function returns the newly created instance of <em>json::object</em>.
   */
  object read(const std::string &str);

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to read the JSON object from.
   *
   * @return The function returns the newly created instance of <em>json::object</em>.
   */
  object read(char_sequence &str);

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to read the JSON object from.
   *
   * @return The function returns the newly created instance of <em>json::object</em>.
   */
  object read(std::string &str);

}

#endif // JSON_READER_H
