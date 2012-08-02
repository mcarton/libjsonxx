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

#ifndef JSON_HASH_VECTOR_H
#define JSON_HASH_VECTOR_H

#include <memory>
#include "json/def.h"
#include "json/hash_slot.h"
#include "json/hash_vector_iterator.h"

namespace json
{

  template < typename T, typename Allocator = std::allocator<T> >
  class hash_vector
  {

  public:

    typedef hash_slot<T>					  slot_type;
    typedef typename Allocator::template rebind<slot_type>::other slot_allocator;

    typedef Allocator				allocator_type;
    typedef slot_type				value_type;
    typedef value_type &			reference;
    typedef value_type *			pointer;
    typedef const value_type &			const_reference;
    typedef const value_type *			const_pointer;
    typedef typename std::size_t		size_type;
    typedef hash_vector_iterator<slot_type>	iterator;
    typedef hash_vector_iterator<const slot_type> const_iterator;

    hash_vector(const allocator_type &a = allocator_type());

    hash_vector(size_type size, const allocator_type &a = allocator_type());

    hash_vector(hash_vector &&v);

    hash_vector(const hash_vector &v);

    ~hash_vector();

    hash_vector &operator=(hash_vector &&v);

    hash_vector &operator=(const hash_vector &v);

    reference operator[](size_type index);

    const_reference operator[](size_type index) const;

    size_type size() const;

    void swap(hash_vector &v);

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    allocator_type get_allocator() const;

    iterator erase(iterator it);

    void clear();

  private:
    size_type		_size;
    slot_allocator	_allocator;
    slot_type *		_slots;

    void copy(const hash_vector &);
    void create(size_type);
    void destroy();

    static slot_type *allocate_slots(size_type, slot_allocator &);
    static void construct_slots(slot_type *, size_type, slot_allocator &, size_type &);
    static void destroy_slots(slot_type *, size_type, slot_allocator &);
    static void deallocate_slots(slot_type *, size_type, slot_allocator &);

  };

}

namespace std
{

  template < typename T, typename Allocator >
  inline void swap(json::hash_vector<T, Allocator> &v1,
		   json::hash_vector<T, Allocator> &v2)
  {
    v1.swap(v2);
  }

}

#endif // JSON_HASH_VECTOR_H
