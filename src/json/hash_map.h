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

#ifndef JSON_HASH_MAP_H
#define JSON_HASH_MAP_H

#include "json/def.h"
#include "json/hash_key.h"
#include "json/hash_table.h"

namespace json
{

  /**
   * @brief A generic hash map optimized for string keys.
   *
   * C++ provides two mapping object in the standard library, <em>std::map</em>
   * and <em>std::unordered_map</em> but there is multiple issues with those
   * objects:
   * <ul>
   *  <li>When used with <em>std::string</em> as key the program performs a
   *  memory allocation everytime it uses the '[]' operator, this is kind of
   *  sad and not matching the <em>"you don't pay for what you don't use"</em>
   *  spirit of C++.</li>
   *  <li><em>std::unordered_map</em> would refuse to compile in the case of
   *  the implementation of a JSON object because of recursive types (the JSON
   *  object class containing a map of other JSON objects). I'm not sure if
   *  that's only failing with GCC but the code has to be portable for multiple
   *  compilers anyway.</li>
   * </ul>
   * <br/>
   * The intent of this class is not to provide a replacement for <em>std::map</em>
   * or <em>std::unordered_map</em>, it is to provide a hash map implementation
   * that fits the needs to build the <em>json::object</em> class. The class API
   * is then not exactly the same as the one of the STL maps, it's similar but
   * simplier.
   *
   * @note This class is developped for internal purposes only and should not be
   * used outside of the libjson++ implementation.
   */
  template < typename T,
	     typename Char = char,
	     typename Traits = std::char_traits<Char>,
	     typename Allocator = std::allocator<Char> >
  class hash_map
  {

    enum
      {
	default_capacity = 42
      };

  public:

    typedef Char						char_type;
    typedef Traits						traits_type;
    typedef hash_key<Char, Traits>				key_type;
    typedef T							mapped_type;
    typedef typename std::pair<key_type, mapped_type>		value_type;
    typedef value_type &					reference;
    typedef value_type *					pointer;
    typedef const value_type &					const_reference;
    typedef const value_type *					const_pointer;
    typedef typename std::size_t				size_type;
    typedef typename std::ptrdiff_t				difference_type;
    typedef Allocator						allocator_type;

  private:

    struct hash
    {

      typename std::hash<key_type> hash_function;

      size_type operator()(const_reference x) const
      {
	return hash_function(x.first);
      }

    };

    struct equal_to
    {

      typename std::equal_to<key_type> equal_function;

      bool operator()(const_reference x1, const_reference x2) const
      {
	return equal_function(x1.first, x2.first);
      }

    };

    typedef typename Allocator::template rebind<mapped_type>::other pair_allocator;
    typedef hash_table<value_type, hash, equal_to, pair_allocator> table_type;
    typedef typename key_type::char_sequence_type char_sequence_type;

  public:

    typedef typename table_type::iterator	iterator;
    typedef typename table_type::const_iterator	const_iterator;

    hash_map(const allocator_type &a = allocator_type());

    hash_map(const hash_map &map);

    hash_map(hash_map &&map);

    ~hash_map();

    hash_map &operator=(const hash_map &map);

    hash_map &operator=(hash_map &&map);

    allocator_type get_allocator() const;

    void swap(hash_map &map);

    size_type size() const;

    bool empty() const;

    void clear();

    iterator erase(iterator it);

    iterator insert(const char_sequence_type &key, const mapped_type &value);

    iterator emplace(const char_sequence_type &key, mapped_type &&value);

    iterator begin();

    iterator end();

    iterator find(const char_sequence_type &key);

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator find(const char_sequence_type &key) const;

  private:
    table_type _table;

  };

}

namespace std
{

  template < typename T,
	     typename Char,
	     typename Traits,
	     typename Allocator >
  inline void swap(json::hash_map<T, Char, Traits, Allocator> &h1,
		   json::hash_map<T, Char, Traits, Allocator> &h2)
  {
    h1.swap(h2);
  }

}

#endif // JSON_HASH_MAP_H
