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

#ifndef JSON_VECTOR_ITERATOR_HPP
#define JSON_VECTOR_ITERATOR_HPP

#include "json/vector_iterator.h"

namespace json
{

  template < typename T >
  vector_iterator<T>::vector_iterator():
    _pos(nullptr)
  {
  }

  template < typename T >
  vector_iterator<T>::vector_iterator(pointer pos):
    _pos(pos)
  {
  }

  template < typename T >
  typename vector_iterator<T>::pointer
  vector_iterator<T>::position() const
  {
    return _pos;
  }

  template < typename T >
  typename vector_iterator<T>::pointer
  vector_iterator<T>::operator->() const
  {
    return _pos;
  }

  template < typename T >
  typename vector_iterator<T>::reference
  vector_iterator<T>::operator*() const
  {
    return *_pos;
  }

  template < typename T >
  vector_iterator<T> &
  vector_iterator<T>::operator++()
  {
    ++_pos;
    return *this;
  }

  template < typename T >
  vector_iterator<T> &
  vector_iterator<T>::operator--()
  {
    --_pos;
    return *this;
  }

  template < typename T >
  vector_iterator<T>
  vector_iterator<T>::operator++(int)
  {
    const vector_iterator tmp { *this };
    ++(*this);
    return tmp;
  }

  template < typename T >
  vector_iterator<T>
  vector_iterator<T>::operator--(int)
  {
    const vector_iterator tmp { *this };
    --(*this);
    return tmp;
  }

  template < typename T >
  vector_iterator<T> &
  vector_iterator<T>::operator+=(const long offset)
  {
    _pos += offset;
    return *this;
  }

  template < typename T >
  vector_iterator<T> &
  vector_iterator<T>::operator-=(const long offset)
  {
    _pos -= offset;
    return *this;
  }

  template < typename T1, typename T2 >
  bool operator==(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2)
  {
    return it1.position() == it2.position();
  }

  template < typename T1, typename T2 >
  bool operator!=(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2)
  {
    return it1.position() != it2.position();
  }

  template < typename T1, typename T2 >
  bool operator<(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2)
  {
    return it1.position() < it2.position();
  }

  template < typename T1, typename T2 >
  bool operator>(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2)
  {
    return it1.position() > it2.position();
  }

  template < typename T1, typename T2 >
  bool operator<=(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2)
  {
    return it1.position() <= it2.position();
  }

  template < typename T1, typename T2 >
  bool operator>=(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2)
  {
    return it1.position() >= it2.position();
  }

  template < typename T >
  vector_iterator<T> operator+(const vector_iterator<T> &it, const long offset)
  {
    return { it.position() + offset };
  }

  template < typename T >
  vector_iterator<T> operator-(const vector_iterator<T> &it, const long offset)
  {
    return { it.position() - offset };
  }

  template < typename T >
  vector_iterator<T> operator+(const long offset, const vector_iterator<T> &it)
  {
    return it + offset;
  }

  template < typename T >
  vector_iterator<T> operator-(const long offset, const vector_iterator<T> &it)
  {
    return it - offset;
  }

  template < typename T >
  typename vector_iterator<T>::difference_type
  operator-(const vector_iterator<T> &it1, const vector_iterator<T> &it2)
  {
    return it1.position() - it2.position();
  }

}

#endif // JSON_VECTOR_ITERATOR_HPP
