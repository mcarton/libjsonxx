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

#ifndef JSON_READER_HPP
#define JSON_READER_HPP

#include <array>
#include <istream>
#include "json/reader.h"
#include "json/char_sequence.h"
#include "json/parsing.hpp"

namespace json
{

  [[noreturn]]
  void error_invalid_input_eof();

  [[noreturn]]
  void error_invalid_input_non_json();

  [[noreturn]]
  void error_invalid_input_unsupported_unicode();

  template < typename InputIterator >
  void skip_spaces(InputIterator &first, InputIterator &last)
  {
    while ((first != last) && std::isspace(*first))
      {
	++first;
      }
  }

  template < typename InputIterator >
  void next_char(InputIterator &first, InputIterator &last)
  {
    skip_spaces(first, last);
    if (first == last)
      {
	error_invalid_input_eof();
      }
  }

  template < typename InputIterator >
  void consume_char(InputIterator &first, InputIterator &last)
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
  void consume_char(InputIterator &first, InputIterator &last, const int c)
  {
    if ((first != last) && ((*first) != c))
      {
	error_invalid_input_non_json();
      }
    consume_char(first, last);
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_list(InputIterator &first,
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

  template < typename InputIterator >
  int read_unicode_digit(InputIterator& first, InputIterator& last)
  {
    if(first != last)
      {
        if(*first >= '0' && *first <= '9')
          {
            return *first-'0';
          }
        else if(*first >= 'a' && *first <= 'f')
          {
            return *first-'a'+10;
          }
      }

      error_invalid_input_non_json();
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  struct read_unicode_helper
  {
    static void read_unicode(InputIterator& first,
                         InputIterator &last,
                         std::basic_string<Char, Traits, Allocator> &str)
    {
      error_invalid_input_unsupported_unicode();
    }
  };

  template < typename InputIterator, typename Traits, typename Allocator >
  struct read_unicode_helper< InputIterator, char, Traits, Allocator >
  {
    static void read_unicode (InputIterator& first,
                         InputIterator &last,
                         std::basic_string<char, Traits, Allocator> &str)
    {
      consume_char(first, last); // consumes 'u'
      int code_point = 0;

      for (int i = 0; i < 3; ++i)
        {
          code_point += read_unicode_digit(first, last);
          code_point <<= 4;
          consume_char(first, last);
        }

      // last one should not be consumed
      code_point += read_unicode_digit(first, last);

      if (code_point < 0x80)
        {
          str.push_back(         code_point                  );
        }
      else if (code_point < 0x800)
        {
          str.push_back(0xC0 + ( code_point            >> 6 ));
          str.push_back(0x80 + ( code_point & 077           ));
        }
      else if (code_point <= 0xFFFF)
        {
          str.push_back(0xE0 + ( code_point            >> 12));
          str.push_back(0x80 + ((code_point & 07777)   >> 6 ));
          str.push_back(0x80 + ( code_point & 077           ));
        }
      else
        {
          str.push_back(0xF0 + ( code_point            >> 18));
          str.push_back(0x80 + ((code_point & 0777777) >> 12));
          str.push_back(0x80 + ((code_point & 07777)   >> 6 ));
          str.push_back(0x80 + ( code_point & 077           ));
        }
    }
  };

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_key(InputIterator &first,
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
	      case '/':  str.push_back('/');  break;
	      case 'b':  str.push_back('\b'); break;
	      case 'f':  str.push_back('\f'); break;
	      case 'n':  str.push_back('\n'); break;
              case 'r':  str.push_back('\r'); break;
              case 't':  str.push_back('\t'); break;
              case 'u':  read_unicode_helper<InputIterator, Char, Traits, Allocator>::read_unicode(first, last, str); break;
	      default:   error_invalid_input_non_json();
	      }
	  }

	consume_char(first, last);
      }

    consume_char(first, last); // consumes '"'
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_pair(InputIterator &first,
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
  void read_map(InputIterator &first,
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
  void read_equals(InputIterator &first,InputIterator &last, const char (&str)[N])
  {
    typedef typename std::size_t index;
    for (index i = 0; (first != last) && (i != (N - 1)); ++i)
      {
	consume_char(first, last, str[i]);
      }
  }

  template < typename InputIterator >
  void read_null(InputIterator &first, InputIterator &last)
  {
    read_equals(first, last, "null");
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_true(InputIterator &first,
                        InputIterator &last,
                        basic_object<Char, Traits, Allocator> &obj)
  {
    read_equals(first, last, "true");
    obj = true;
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_false(InputIterator &first,
                         InputIterator &last,
                         basic_object<Char, Traits, Allocator> &obj)
  {
    read_equals(first, last, "false");
    obj = false;
  }

  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_string(InputIterator &first,
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
  void read_object(InputIterator &first,
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
  void read_object(Iterable &iterable, basic_object<Char, Traits, Allocator> &obj)
  {
    auto b = begin(iterable);
    auto e = end(iterable);
    read_object<decltype(b), Char, Traits, Allocator>(b, e, obj);
  }

  template < typename Iterable, typename Object >
  Object read(Iterable &iterable, typename Object::allocator_type const &a)
  {
    typedef typename Object::char_type          char_type;
    typedef typename Object::traits_type        traits_type;
    typedef typename Object::allocator_type     allocator_type;
    Object obj ( a );
    read_object<Iterable, char_type, traits_type, allocator_type>(iterable, obj);
    return obj;
  }

  template < typename Iterable, typename Object >
  Object read(Iterable &iterable)
  {
    typedef typename Object::allocator_type allocator;
    return read<Iterable, Object>(iterable, allocator());
  }

  template < typename Object >
  Object read(const char *str, typename Object::allocator_type const &a)
  {
    const char_sequence cs ( str, std::strlen(str) );
    return read(cs, a);
  }

}

#endif // JSON_READER_HPP

