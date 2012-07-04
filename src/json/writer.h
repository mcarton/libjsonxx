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

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <iosfwd>
#include "json/def.h"

namespace json
{

  template < typename Object, typename Char, typename Traits >
  void write_object(std::basic_ostream<Char, Traits> &out, const Object &obj);

  template < typename String, typename Char, typename Traits >
  void write_string(std::basic_ostream<Char, Traits> &out, const String &s);

  template < typename List, typename Char, typename Traits >
  void write_list(std::basic_ostream<Char, Traits> &out, const List &list);

  template < typename Map, typename Char, typename Traits >
  void write_map(std::basic_ostream<Char, Traits> &out, const Map &map);

  template < typename Pair, typename Char, typename Traits >
  void write_pair(std::basic_ostream<Char, Traits> &out, const Pair &pair);

  template < typename Char, typename Traits >
  void write_null(std::basic_ostream<Char, Traits> &out);

}

#endif // JSON_WRITER_H
