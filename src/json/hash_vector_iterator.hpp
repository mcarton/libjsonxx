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

#ifndef JSON_HASH_VECTOR_ITERATOR_HPP
#define JSON_HASH_VECTOR_ITERATOR_HPP

#include "json/hash_slot.hpp"
#include "json/hash_vector_iterator.h"

namespace json
{

  template < typename T >
  hash_vector_iterator<T>::
  hash_vector_iterator():
    _ptr(nullptr)
  {
  }

  template < typename T >
  hash_vector_iterator<T>::
  hash_vector_iterator(T *ptr):
    _ptr(ptr)
  {
  }

  template < typename T >
  bool
  hash_vector_iterator<T>::
  operator==(const hash_vector_iterator &it) const
  {
    return _ptr == it._ptr;
  }

  template < typename T >
  bool
  hash_vector_iterator<T>::
  operator!=(const hash_vector_iterator &it) const
  {
    return _ptr != it._ptr;
  }

  template < typename T >
  typename hash_vector_iterator<T>::reference
  hash_vector_iterator<T>::
  operator*() const
  {
    return *_ptr;
  }

  template < typename T >
  typename hash_vector_iterator<T>::pointer
  hash_vector_iterator<T>::
  operator->() const
  {
    return _ptr;
  }

  template < typename T >
  hash_vector_iterator<T> &
  hash_vector_iterator<T>::
  operator++()
  {
    ++_ptr;
    return *this;
  }

  template < typename T >
  const hash_vector_iterator<T>
  hash_vector_iterator<T>::
  operator++(int)
  {
    const hash_vector_iterator tmp (*this);
    ++(*this);
    return *this;
  }

  template < typename T >
  const hash_vector_iterator<T>
  hash_vector_iterator<T>::
  operator+(const size_type offset) const
  {
    return hash_vector_iterator(_ptr + offset);
  }

}

#endif // JSON_HASH_VECTOR_ITERATOR_HPP
