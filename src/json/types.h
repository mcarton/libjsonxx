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

#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include "json/def.h"

namespace json
{

  enum object_type
    {
      type_null,
      type_string,
      type_list,
      type_map
    };

  template < typename, typename, typename > class basic_object;

}

namespace std
{

  template < typename, typename > class basic_ostream;

  template < typename Char, typename Traits >
  inline basic_ostream<Char, Traits> &
  operator<<(basic_ostream<Char, Traits> &out, const json::object_type type)
  {
    switch (type)
      {
      case json::type_string: return out << "<JSON string>";
      case json::type_list:   return out << "<JSON list>";
      case json::type_map:    return out << "<JSON map>";
      case json::type_null:   return out << "<JSON null>";
      }
    return out;
  }

}

#endif // JSON_TYPES_H
