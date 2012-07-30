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

#include <iosfwd>
#include <iterator>
#include "json/def.h"

namespace json
{

  /**
   * @brief Reads JSON and loads the data directly into a pre-allocated object.
   *
   * @param first An iterator pointing to the begining of the data to be parsed.
   * @param last An iterator pointing to the end of the data to be parsed.
   * @param obj The destination object to build from the parsed data.
   *
   * @note The function will stop reading once it has loaded a valid JSON
   * object, which may be before reaching the 'last' iterator.
   */
  template < typename InputIterator, typename Char, typename Traits, typename Allocator >
  void read_object(InputIterator &first,
		   InputIterator &last,
		   basic_object<Char, Traits, Allocator> &obj);

  /**
   * @brief Reads JSON and loads the data directly into a pre-allocated object.
   *
   * @param iterable An 'Iterable' object that must feed the parser with valid
   * JSON content.
   * @param obj The destination object to build from the parsed data.
   *
   * @note The function will stop reading once it has loaded a valid JSON
   * object, which may be before consuming all data of the given 'Iterable'.
   */
  template < typename Iterable, typename Char, typename Traits, typename Allocator >
  void read_object(Iterable &iterable, basic_object<Char, Traits, Allocator> &obj);

  /**
   * @brief Reads a JSON object.
   *
   * @param iterable An 'Iterable' object that must feed the parser with a valid
   * JSON string.
   * @param a The allocator to use to create the object.
   * 
   * @return The function returns the newly created JSON object.
   */
  template < typename Iterable, typename Object = object >
  Object read(Iterable &iterable, typename Object::allocator_type const &a);

  /**
   * @brief Reads a JSON object.
   *
   * @param iterable An 'Iterable' object that must feed the parser with a valid
   * JSON string.
   * 
   * @return The function returns the newly created JSON object.
   */
  template < typename Iterable, typename Object = object >
  Object read(Iterable &iterable);

  /**
   * @brief Reads a JSON object.
   *
   * @param str The string to parse to create a JSON object.
   * @param a   The allocator that will be used to create the JSON object.
   *
   * @return The function returns the newly created JSON object.
   */
  template < typename Object >
  Object read(const char *str, typename Object::allocator_type const &a);

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
