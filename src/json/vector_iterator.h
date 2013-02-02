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

#ifndef JSON_VECTOR_ITERATOR_H
#define JSON_VECTOR_ITERATOR_H

#include <iterator>
#include "json/def.h"

namespace json
{

  template < typename T >
  class vector_iterator :
    public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, T *, T &>
  {

  public:

    typedef typename std::random_access_iterator_tag	iterator_category;
    typedef typename std::ptrdiff_t			difference_type;
    typedef T						value_type;
    typedef T *						pointer;
    typedef T &						reference;

    vector_iterator();

    vector_iterator(pointer pos);

    pointer position() const;

    pointer operator->() const;

    reference operator*() const;

    vector_iterator &operator++();

    vector_iterator &operator--();

    vector_iterator operator++(int);

    vector_iterator operator--(int);

    vector_iterator &operator+=(long offset);

    vector_iterator &operator-=(long offset);

  private:
    pointer _pos;

  };

  template < typename T1, typename T2 >
  bool operator==(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2);

  template < typename T1, typename T2 >
  bool operator!=(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2);

  template < typename T1, typename T2 >
  bool operator<(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2);

  template < typename T1, typename T2 >
  bool operator>(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2);

  template < typename T1, typename T2 >
  bool operator<=(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2);

  template < typename T1, typename T2 >
  bool operator>=(const vector_iterator<T1> &it1, const vector_iterator<T2> &it2);

  template < typename T >
  vector_iterator<T> operator+(const vector_iterator<T> &it, long offset);

  template < typename T >
  vector_iterator<T> operator-(const vector_iterator<T> &it, long offset);

  template < typename T >
  vector_iterator<T> operator+(long offset, const vector_iterator<T> &it);

  template < typename T >
  vector_iterator<T> operator-(long offset, const vector_iterator<T> &it);

  template < typename T >
  typename vector_iterator<T>::difference_type
  operator-(const vector_iterator<T> &it1, const vector_iterator<T> &it2);

}

#endif // JSON_VECTOR_ITERATOR_H
