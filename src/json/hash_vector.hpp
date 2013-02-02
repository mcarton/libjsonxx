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

#ifndef JSON_HASH_VECTOR_HPP
#define JSON_HASH_VECTOR_HPP

#include <algorithm>
#include "json/hash_slot.hpp"
#include "json/hash_vector_iterator.hpp"
#include "json/hash_vector.h"

namespace json
{

  template < typename T, typename Allocator >
  hash_vector<T, Allocator>::
  hash_vector(const allocator_type &a):
    _size(0),
    _allocator(a),
    _slots(nullptr)
  {
  }

  template < typename T, typename Allocator >
  hash_vector<T, Allocator>::
  hash_vector(const size_type n, const allocator_type &a):
    _size(0),
    _allocator(a),
    _slots(nullptr)
  {
    create(n);
  }

  template < typename T, typename Allocator >
  hash_vector<T, Allocator>::
  hash_vector(hash_vector &&v):
    _size(0),
    _allocator(),
    _slots(nullptr)
  {
    v.swap(*this);
  }

  template < typename T, typename Allocator >
  hash_vector<T, Allocator>::
  hash_vector(const hash_vector &v):
    _size(0),
    _allocator(v._allocator),
    _slots(nullptr)
  {
    create(v._size);
    try
      {
	copy(v);
      }
    catch (...)
      {
	destroy();
	throw;
      }
  }

  template < typename T, typename Allocator >
  hash_vector<T, Allocator>::
  ~hash_vector()
  {
    destroy();
  }

  template < typename T, typename Allocator >
  hash_vector<T, Allocator> &
  hash_vector<T, Allocator>::
  operator=(hash_vector &&v)
  {
    v.swap(*this);
    return *this;
  }

  template < typename T, typename Allocator >
  hash_vector<T, Allocator> &
  hash_vector<T, Allocator>::
  operator=(const hash_vector &v)
  {
    if (size() == v.size())
      {
	copy(v);
      }
    else
      {
	hash_vector(v).swap(*this);
      }
    return *this;
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::reference
  hash_vector<T, Allocator>::
  operator[](const size_type index)
  {
    return _slots[index];
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::const_reference
  hash_vector<T, Allocator>::
  operator[](const size_type index) const
  {
    return _slots[index];
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::size_type
  hash_vector<T, Allocator>::
  size() const
  {
    return _size;
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  swap(hash_vector &v)
  {
    std::swap(_size, v._size);
    std::swap(_allocator, v._allocator);
    std::swap(_slots, v._slots);
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::iterator
  hash_vector<T, Allocator>::
  begin()
  {
    return iterator(_slots);
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::iterator
  hash_vector<T, Allocator>::
  end()
  {
    return iterator(_slots + _size);
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::const_iterator
  hash_vector<T, Allocator>::
  begin() const
  {
    return const_iterator(_slots);
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::const_iterator
  hash_vector<T, Allocator>::
  end() const
  {
    return const_iterator(_slots + _size);
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::allocator_type
  hash_vector<T, Allocator>::
  get_allocator() const
  {
    return _allocator;
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::iterator
  hash_vector<T, Allocator>::
  erase(iterator it)
  {
    it->erase();
    ++it;
    return it;
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  clear()
  {
    std::for_each(begin(), end(), [&](slot_type &slot) { slot.clear(); });
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  copy(const hash_vector &v)
  {
    std::copy_if(v.begin(), v.end(), begin(), [](const slot_type &slot) { return slot.status == slot_type::busy; });
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  create(const size_type n)
  {
    _slots = allocate_slots(n, _allocator);
    try
      {
	construct_slots(_slots, n, _allocator, _size);
      }
    catch (...)
      {
	destroy_slots(_slots, _size, _allocator);
	deallocate_slots(_slots, n, _allocator);
	_slots = nullptr;
	_size = 0;
	throw;
      }
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  destroy()
  {
    if (_slots)
      {
	destroy_slots(_slots, _size, _allocator);
	deallocate_slots(_slots, _size, _allocator);
	_slots = nullptr;
	_size = 0;
      }
  }

  template < typename T, typename Allocator >
  typename hash_vector<T, Allocator>::slot_type *
  hash_vector<T, Allocator>::
  allocate_slots(const size_type n, slot_allocator &a)
  {
    return a.allocate(n);
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  construct_slots(slot_type *slots, size_type n, slot_allocator &a, size_type &size)
  {
    slot_type *const end = slots + n;

    while (slots != end)
      {
	a.construct(slots);
	++slots;
	++size;
      }
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  destroy_slots(slot_type *slots, size_type n, slot_allocator &a)
  {
    slot_type *const end = slots + n;

    while (slots != end)
      {
	a.destroy(slots);
	++slots;
      }
  }

  template < typename T, typename Allocator >
  void
  hash_vector<T, Allocator>::
  deallocate_slots(slot_type *slots, size_type n, slot_allocator &a)
  {
    a.deallocate(slots, n);
  }

}

#endif // JSON_HASH_VECTOR_HPP
