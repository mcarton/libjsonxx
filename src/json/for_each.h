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

#ifndef JSON_FOR_EACH_H
#define JSON_FOR_EACH_H

#include "json/def.h"

namespace json
{

  /**
   * @brief Iterates over a JSON list.
   *
   * @param obj      The object to iterate on.
   * @param function The function to be called on each objects of the list.
   */
  template < typename Object, typename Function >
  inline void for_each_object(Object &obj, const Function &function)
  {
    auto it = obj.begin();
    auto jt = obj.end();
    while (it != jt)
      {
        function(static_cast<Object&>(*it));
        ++it;
      }
  }

  /**
   * @brief Iterates over a JSON map.
   *
   * @param obj      The object to iterate on.
   * @param function The function called with the name and value of each
   * elements of the map - so it has to receive two parameters, the first
   * one being a <em>json::char_sequence</em> and the second a
   * <em>json::object</em>.
   */
  template < typename Object, typename Function >
  inline void for_each_pair(Object &obj, const Function &function)
  {
    auto it = obj.begin();
    auto jt = obj.end();
    while (it != jt)
      {
        function(it->first, it->second);
        ++it;
      }
  }

}

#endif // JSON_FOR_EACH_H
