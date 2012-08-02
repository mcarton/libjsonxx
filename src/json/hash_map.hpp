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

#ifndef JSON_HASH_MAP_HPP
#define JSON_HASH_MAP_HPP

#include <algorithm>
#include <utility>
#include "json/hash_table.hpp"
#include "json/hash_key.hpp"
#include "json/hash_map.h"

namespace json
{

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  hash_map(const allocator_type &a):
    _table(default_capacity, a)
  {
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  hash_map(const hash_map &map):
    _table(map._table)
  {
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  hash_map(hash_map &&map):
    _table(std::move(map._table))
  {
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  ~hash_map()
  {
    clear();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator> &
  hash_map<T, Char, Traits, Allocator>::
  operator=(const hash_map &map)
  {
    _table = map._table;
    return *this;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator> &
  hash_map<T, Char, Traits, Allocator>::
  operator=(hash_map &&map)
  {
    _table = std::move(map._table);
    return *this;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::allocator_type
  hash_map<T, Char, Traits, Allocator>::
  get_allocator() const
  {
    return _table.get_allocator();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  swap(hash_map &map)
  {
    std::swap(_table, map._table);
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::size_type
  hash_map<T, Char, Traits, Allocator>::
  size() const
  {
    return _table.size();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  bool
  hash_map<T, Char, Traits, Allocator>::
  empty() const
  {
    return size() == 0;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  clear()
  {
    allocator_type a = get_allocator();

    std::for_each(begin(), end(), [&](reference x) {
	const key_type &k = x.first;
	a.deallocate(const_cast<char_type*>(k.data()), k.size());
      });

    _table.clear();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  erase(iterator it)
  {
    allocator_type a = get_allocator();
    key_type k = it->first;
    iterator next = _table.erase(it);
    a.deallocate(const_cast<char_type*>(k.data()), k.size());
    return next;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  insert(const char_sequence_type &key, const mapped_type &value)
  {
    return emplace(key, std::move(mapped_type(value)));
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  emplace(const char_sequence_type &key, mapped_type &&value)
  {
    allocator_type a = get_allocator();
    size_type  n = key.size();
    char_type *s = a.allocate(n + 1);

    std::copy(key.begin(), key.end(), s);
    s[n] = 0;
    try
      {
	key_type   k (char_sequence_type(s, n));
	value_type x (k, std::move(value));
	return _table.insert(std::move(x));
      }
    catch (...)
      {
	a.deallocate(s, n);
	throw;
      }
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  begin()
  {
    return _table.begin();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  end()
  {
    return _table.end();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  find(const char_sequence_type &key)
  {
    return _table.find(std::make_pair(key, mapped_type()));
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::const_iterator
  hash_map<T, Char, Traits, Allocator>::
  begin() const
  {
    return _table.begin();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::const_iterator
  hash_map<T, Char, Traits, Allocator>::
  end() const
  {
    return _table.end();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::const_iterator
  hash_map<T, Char, Traits, Allocator>::
  find(const char_sequence_type &key) const
  {
    return _table.find(std::make_pair(key, mapped_type()));
  }

}

#endif // JSON_HASH_MAP_HPP

