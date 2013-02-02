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

#ifndef JSON_VECTOR_HPP
#define JSON_VECTOR_HPP

#include <algorithm>
#include "json/vector.h"
#include "json/vector_iterator.hpp"

namespace json
{

  void error_index_out_of_bounds(size_t index, size_t size);

  template < typename T, typename Allocator >
  vector<T, Allocator>::body_type::body_type():
    dummy(false)
  {
  }

  template < typename T, typename Allocator >
  vector<T, Allocator>::body_type::~body_type()
  {
  }

  template < typename T, typename Allocator >
  vector<T, Allocator>::
  vector(const allocator_type &a):
    _size(0),
    _body(),
    _allocator(a)
  {
  }

  template < typename T, typename Allocator >
  vector<T, Allocator>::
  vector(const vector &v):
    _size(v._size),
    _body(),
    _allocator(v._allocator)
  {
    if (v._size > 0)
      {
	if (v._size == 1)
	  {
	    new (&(_body.value)) value_type { v._body.value };
	  }
	else
	  {
	    new (&(_body.vector)) vector_type { v._body.vector };
	  }
      }
  }

  template < typename T, typename Allocator >
  vector<T, Allocator>::
  vector(vector &&v):
    _size(0),
    _body(),
    _allocator(v._allocator)
  {
    v.swap(*this);
  }

  template < typename T, typename Allocator >
  vector<T, Allocator>::
  ~vector()
  {
    if (_size > 0)
      {
	if (_size == 1)
	  {
	    _body.value.~value_type();
	  }
	else
	  {
	    _body.vector.~vector_type();
	  }
      }
  }

  template < typename T, typename Allocator >
  vector<T, Allocator> &
  vector<T, Allocator>::operator=(const vector &v)
  {
    vector(v).swap(*this);
    return *this;
  }

  template < typename T, typename Allocator >
  vector<T, Allocator> &
  vector<T, Allocator>::operator=(vector &&v)
  {
    v.swap(*this);
    return *this;
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::reference
  vector<T, Allocator>::operator[](const size_type index)
  {
    return at(index);
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::const_reference
  vector<T, Allocator>::operator[](const size_type index) const
  {
    return at(index);
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::reference
  vector<T, Allocator>::at(const size_type index)
  {
    if (index >= _size)
      {
	error_index_out_of_bounds(index, _size);
      }
    return _size == 1 ? _body.value : _body.vector[index];
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::const_reference
  vector<T, Allocator>::at(const size_type index) const
  {
    if (index >= _size)
      {
	error_index_out_of_bounds(index, _size);
      }
    return _size == 1 ? _body.value : _body.vector[index];
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::swap(vector &v)
  {
    swap_bodies(_body, _size, v._body, v._size);
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::reserve(size_type)
  {
    // This method is provided only for interface compatibility with the STL vector class.
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::push_back(const_reference x)
  {
    push_back(std::move(value_type{ x }));
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::push_back(value_type &&x)
  {
    if (_size == 0)
      {
	new (&(_body.value)) value_type { std::move(x) };
      }
    else if (_size == 1)
      {
	value_type y = std::move(_body.value);
	try
	  {
	    new (&(_body.vector)) vector_type { _allocator };
	    _body.vector.reserve(2);
	    _body.vector.push_back(std::move(y));
	    _body.vector.push_back(std::move(x));
	  }
	catch (...)
	  {
	    new (&(_body.value)) value_type { std::move(y) };
	    throw;
	  }
      }
    else
      {
	_body.vector.push_back(std::move(x));
      }
    ++_size;
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::pop_back()
  {
    if (_size != 0)
      {
	erase(begin() + (_size - 1));
      }
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::const_reference
  vector<T, Allocator>::front() const
  {
    return _size <= 1 ? _body.value : _body.vector.front();
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::const_reference
  vector<T, Allocator>::back() const
  {
    return _size <= 1 ? _body.value : _body.vector.back();
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::reference
  vector<T, Allocator>::front()
  {
    return _size <= 1 ? _body.value : _body.vector.front();
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::reference
  vector<T, Allocator>::back()
  {
    return _size <= 1 ? _body.value : _body.vector.back();
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::iterator
  vector<T, Allocator>::erase(const_iterator it)
  {
    iterator next = end();
    size_t offset = 0;

    if (it != next)
      {
	if (_size == 1)
	  {
	    _body.value.~value_type();
	  }
	else
	  {
	    offset = it - const_cast<const vector*>(this)->begin();
	    _body.vector.erase(_body.vector.begin() + offset);

	    if (_body.vector.size() == 1)
	      {
		value_type x = std::move(_body.vector.front());
		_body.vector.~vector_type();
		new (&(_body.value)) value_type { std::move(x) };
	      }
	  }
	--_size;
	next = begin() + offset;
      }
    return next;
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::iterator
  vector<T, Allocator>::erase(iterator it)
  {
    return erase(const_iterator{ it.position() });
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::iterator
  vector<T, Allocator>::begin()
  {
    return { _size <= 1 ? &(_body.value) : &(_body.vector[0]) };
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::iterator
  vector<T, Allocator>::end()
  {
    return begin() + _size;
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::const_iterator
  vector<T, Allocator>::begin() const
  {
    return { _size <= 1 ? &(_body.value) : &(_body.vector[0]) };
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::const_iterator
  vector<T, Allocator>::end() const
  {
    return begin() + _size;
  }

  template < typename T, typename Allocator >
  bool
  vector<T, Allocator>::empty() const
  {
    return _size == 0;
  }

  template < typename T, typename Allocator >
  typename vector<T, Allocator>::size_type
  vector<T, Allocator>::size() const
  {
    return _size;
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::swap_bodies(body_type &b1, size_type s1,
				    body_type &b2, size_type s2)
  {
    body_type tmp;

    move_body(std::move(b1), s1, tmp);
    move_body(std::move(b2), s2, b1);
    move_body(std::move(tmp), s1, b2);
  }

  template < typename T, typename Allocator >
  void
  vector<T, Allocator>::move_body(body_type &&from, size_type size, body_type &to)
  {
    if (size > 0)
      {
	if (size == 1)
	  {
	    new (&(to.value)) value_type { std::move(from.value) };
	  }
	else
	  {
	    new (&(to.vector)) vector_type { std::move(from.vector) };
	  }
      }
  }

}

#endif // JSON_VECTOR_HPP
