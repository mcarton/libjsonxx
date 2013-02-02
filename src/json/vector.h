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

#ifndef JSON_VECTOR_H
#define JSON_VECTOR_H

#include <memory>
#include <vector>
#include "json/def.h"
#include "json/vector_iterator.h"

namespace json
{

  template < typename T, typename Allocator = std::allocator<T> >
  class vector
  {

    typedef typename std::vector<T, Allocator> vector_type;

  public:

    typedef Allocator			allocator_type;
    typedef T				value_type;
    typedef value_type &		reference;
    typedef value_type *		pointer;
    typedef const value_type &		const_reference;
    typedef const value_type *		const_pointer;
    typedef typename std::size_t	size_type;
    typedef vector_iterator<T>		iterator;
    typedef vector_iterator<const T>	const_iterator;

    vector(const allocator_type &a = allocator_type());

    vector(const vector &v);

    vector(vector &&v);

    ~vector();

    vector &operator=(const vector &v);

    vector &operator=(vector &&v);

    reference operator[](size_type index);

    const_reference operator[](size_type index) const;

    reference at(size_type index);

    const_reference at(size_type index) const;

    void swap(vector &v);

    void reserve(size_type n);

    void push_back(const_reference x);

    void push_back(value_type &&x);

    void pop_back();

    reference front();

    reference back();

    const_reference front() const;

    const_reference back() const;

    iterator erase(const_iterator it);

    iterator erase(iterator it);

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    bool empty() const;

    size_type size() const;

  private:

    union body_type
    {

      bool        dummy;
      value_type  value;
      vector_type vector;

      body_type();

      ~body_type();

    };

    size_type		_size;
    body_type		_body;
    allocator_type	_allocator;

    static void swap_bodies(body_type &, size_type, body_type &, size_type);

    static void move_body(body_type &&, size_type, body_type &);

  };

}

namespace std
{

  template < typename T, typename Allocator >
  inline void swap(json::vector<T, Allocator> &v1, json::vector<T, Allocator> &v2)
  {
    v1.swap(v2);
  }

}

#endif // JSON_VECTOR_H
