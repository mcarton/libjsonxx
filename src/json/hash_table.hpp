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

#ifndef JSON_HASH_TABLE_HPP
#define JSON_HASH_TABLE_HPP

#include <iostream>
#include <limits>
#include <algorithm>
#include "json/hash_vector.hpp"
#include "json/hash_table_iterator.hpp"
#include "json/hash_table.h"

namespace json
{

  template < typename T, typename Hash, typename Equals, typename Allocator >
  hash_table<T, Hash, Equals, Allocator>::
  hash_table(const size_type n, const allocator_type &a):
    _size(0),
    _vector(n, a)
  {
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  hash_table<T, Hash, Equals, Allocator>::
  hash_table(hash_table &&t):
    _size(std::move(t._size)),
    _vector(std::move(t._vector))
  {
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  hash_table<T, Hash, Equals, Allocator>::
  hash_table(const hash_table &t):
    _size(t._size),
    _vector(t._vector)
  {
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  hash_table<T, Hash, Equals, Allocator> &
  hash_table<T, Hash, Equals, Allocator>::
  operator=(hash_table &&t)
  {
    _size = std::move(t._size);
    _vector = std::move(t._vector);
    return *this;
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  hash_table<T, Hash, Equals, Allocator> &
  hash_table<T, Hash, Equals, Allocator>::
  operator=(const hash_table &t)
  {
    _size = t._size;
    _vector = t._vector;
    return *this;
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  void
  hash_table<T, Hash, Equals, Allocator>::
  swap(hash_table &t)
  {
    std::swap(_size, t._size);
    std::swap(_vector, t._vector);
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  size() const
  {
    return _size;
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  capacity() const
  {
    return _vector.size();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  load() const
  {
    return (size() * 100) / capacity();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::allocator_type
  hash_table<T, Hash, Equals, Allocator>::
  get_allocator() const
  {
    return _vector.get_allocator();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::iterator
  hash_table<T, Hash, Equals, Allocator>::
  begin()
  {
    return iterator(_vector.begin(), _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::iterator
  hash_table<T, Hash, Equals, Allocator>::
  end()
  {
    return iterator(_vector.end(), _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::iterator
  hash_table<T, Hash, Equals, Allocator>::
  find(const_reference x)
  {
    size_type i;

    if ((i = find_index(x)) == eof())
      {
	return end();
      }
    return iterator(_vector.begin() + i, _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::const_iterator
  hash_table<T, Hash, Equals, Allocator>::
  begin() const
  {
    return const_iterator(_vector.begin(), _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::const_iterator
  hash_table<T, Hash, Equals, Allocator>::
  end() const
  {
    return const_iterator(_vector.end(), _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::const_iterator
  hash_table<T, Hash, Equals, Allocator>::
  find(const_reference x) const
  {
    size_type i;

    if ((i = find_index(x)) == eof())
      {
	return end();
      }
    return const_iterator(_vector.begin() + i, _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::iterator
  hash_table<T, Hash, Equals, Allocator>::
  insert(const_reference x)
  {
    return emplace(std::move(value_type(x)));
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::iterator
  hash_table<T, Hash, Equals, Allocator>::
  emplace(value_type &&x)
  {
    if (load() > 70)
      {
	expand(2);
      }

    const size_type i = nice_index(x);
    _vector[i] = std::move(x);
    ++_size;
    return iterator(_vector.begin() + i, _vector.end());
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  void
  hash_table<T, Hash, Equals, Allocator>::
  expand(const size_type factor)
  {
    hash_table t (factor * capacity(), get_allocator());

    std::for_each(begin(), end(), [&](reference x) {
	t.emplace(std::move(x));
      });

    t.swap(*this);
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  void
  hash_table<T, Hash, Equals, Allocator>::
  erase(const_reference x)
  {
    erase(find(x));
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::iterator
  hash_table<T, Hash, Equals, Allocator>::
  erase(iterator it)
  {
    if (it != end())
      {
	_vector.erase(it.slot_iterator());
	--_size;
	++it;
      }
    return it;
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  void
  hash_table<T, Hash, Equals, Allocator>::
  clear()
  {
    _vector.clear();
    _size = 0;
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  best_index(const_reference x) const
  {
    return hash(x) % capacity();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  nice_index(const_reference x) const
  {
    size_type n = capacity();
    size_type i = best_index(x);
    size_type j = i;

    do
      {
	const auto &slot = _vector[j];
	switch (slot.status)
	  {
	  case slot_type::free:
	  case slot_type::erased: return j;
	  case slot_type::busy:   j = ((j + 1) % n);
	  }
      }
    while (i != j);
    return eof();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  find_index(const_reference x) const
  {
    size_type n = capacity();
    size_type i = best_index(x);
    size_type j = i;

    do
      {
	const auto &slot = _vector[j];
	if ((slot.status == slot_type::free) ||
	    (slot.status == slot_type::erased))
	  {
	    break;
	  }
	if (equals(slot.value(), x))
	  {
	    return j;
	  }
	j = (j + 1) % n;
      }
    while (i != j);
    return eof();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  eof()
  {
    return std::numeric_limits<size_type>::max();
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  typename hash_table<T, Hash, Equals, Allocator>::size_type
  hash_table<T, Hash, Equals, Allocator>::
  hash(const_reference x)
  {
    return hash_type()(x);
  }

  template < typename T, typename Hash, typename Equals, typename Allocator >
  bool
  hash_table<T, Hash, Equals, Allocator>::
  equals(const_reference x1, const_reference x2)
  {
    return equal_type()(x1, x2);
  }

}

#endif // JSON_HASH_TABLE_HPP
