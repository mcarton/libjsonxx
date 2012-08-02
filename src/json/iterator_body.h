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

#ifndef JSON_ITERATOR_BODY_H
#define JSON_ITERATOR_BODY_H

#include "json/def.h"
#include "json/types.h"

namespace json
{

  void error_null_iterator_cannot_be_incremented();

  template < typename ListIterator, typename MapIterator >
  union iterator_body
  {

    bool           dummy;
    ListIterator   list;
    MapIterator    map;

    iterator_body()
    {
      // This is required to avoid GCC complaining about non-initialized
      // fields (see -Wuninitialized).
      new (&dummy) bool (false);
    }

    ~iterator_body()
    {
    }

    template < typename... Args >
    void create_list(Args&&... args)
    {
      new (&list) ListIterator (std::forward<Args>(args)...);
    }

    template < typename... Args >
    void create_map(Args&&... args)
    {
      new (&map) MapIterator (std::forward<Args>(args)...);
    }

    void create_copy(const object_type type, const iterator_body &body)
    {
      switch (type)
	{
	case type_list:   create_list(body.list); break;
	case type_map:    create_map(body.map);   break;
	case type_string:                         break;
	case type_null:                           break;
	}
    }

    void create_move(const object_type type, iterator_body &&body)
    {
      switch (type)
	{
	case type_list:   create_list(std::move(body.list)); break;
	case type_map:    create_map(std::move(body.map));   break;
	case type_string:                                    break;
	case type_null:                                      break;
	}
    }

    void destroy_list()
    {
      list.~ListIterator();
    }

    void destroy_map()
    {
      map.~MapIterator();
    }

    void destroy(const object_type type)
    {
      switch (type)
	{
	case type_list:   destroy_list(); break;
	case type_map:    destroy_map();  break;
	case type_string:                 break;
	case type_null:                   break;
	}
    }

    void increment(const object_type type)
    {
      switch (type)
	{
	case type_list:   ++list;   break;
	case type_map:    ++map;    break;
	case type_string:
	case type_null: error_null_iterator_cannot_be_incremented();
	}	
    }

    bool equals(const object_type type, const iterator_body &body) const
    {
      switch (type)
	{
	case type_list:   return list == body.list;
	case type_map:    return map == body.map;
	case type_string: return true;
	case type_null:   return true;
	}
      return false;
    }

    bool not_equals(const object_type type, const iterator_body &body) const
    {
      switch (type)
	{
	case type_list:   return list != body.list;
	case type_map:    return map != body.map;
	case type_string: return false;
	case type_null:   return false;
	}
      return true;
    }

  };

}

#endif // JSON_ITERATOR_BODY_H
