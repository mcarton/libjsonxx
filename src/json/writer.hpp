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

#ifndef JSON_WRITER_HPP
#define JSON_WRITER_HPP

#include <ostream>
#include "json/types.h"
#include "json/parsing.hpp"
#include "json/char_sequence.hpp"
#include "json/writer.h"

namespace json
{

  template < typename Char, typename Traits >
  void write_null(std::basic_ostream<Char, Traits> &out)
  {
    out << "null";
  }

  template < typename String, typename Char, typename Traits >
  void write_string(std::basic_ostream<Char, Traits> &out, const String &s)
  {
    if (is_json_boolean(s) || is_json_number(s))
      {
	out << s;
      }
    else
      {
	out << '"';

	auto it = s.begin();
	auto jt = s.end();

	while (it != jt)
	  {
	    switch (*it)
	      {
	      case '"':  out << '\\' << '\"'; break;
	      case '\\': out << '\\' << '\\'; break;
	      case '\b': out << '\\' << 'b';  break; 
	      case '\f': out << '\\' << 'f';  break; 
	      case '\n': out << '\\' << 'n';  break;
	      case '\r': out << '\\' << 'r';  break;
	      case '\t': out << '\\' << 't';  break;
	      default: out << *it;            break;
	      }
	    ++it;
	  }
	      
	out << '"';
      }
  }

  template < typename List, typename Char, typename Traits >
  void write_list(std::basic_ostream<Char, Traits> &out, const List &list)
  {
    out << '[';

    if (!list.empty())
      {
	auto it = list.begin();
	auto jt = list.end();

	write_object(out, *it);

	while ((++it) != jt)
	  {
	    out << ',';
	    write_object(out, *it);
	  }
      }

    out << ']';
  }

  template < typename Pair, typename Char, typename Traits >
  void write_pair(std::basic_ostream<Char, Traits> &out, const Pair &pair)
  {
    write_string(out, pair.first);
    out << ':';
    write_object(out, pair.second);
  }

  template < typename Map, typename Char, typename Traits >
  void write_map(std::basic_ostream<Char, Traits> &out, const Map &map)
  {
    out << '{';

    if (!map.empty())
      {
	auto it = map.begin();
	auto jt = map.end();

	write_pair(out, *it);

	while ((++it) != jt)
	  {
	    out << ',';
	    write_pair(out, *it);
	  }
      }

    out << '}';
  }

  template < typename Char, typename Traits, typename Allocator >
  void write_object(std::basic_ostream<Char, Traits> &out,
		    const basic_object<Char, Traits, Allocator> &obj)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;

    switch (obj.type())
      {

      case type_string:
	write_string(out, char_sequence(obj.get_string()));
	break;

      case type_list:
	write_list(out, obj.get_list());
	break;

      case type_map:
	write_map(out, obj.get_map());
	break;

      case type_null:
	write_null(out);
	break;

      }
  }

}

#endif // JSON_WRITER_HPP
