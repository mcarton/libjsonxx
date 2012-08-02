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

#ifndef JSON_HASH_VECTOR_ITERATOR_H
#define JSON_HASH_VECTOR_ITERATOR_H

#include <iterator>
#include <type_traits>
#include "json/def.h"
#include "json/hash_slot.h"

namespace json
{

  template < typename T >
  class hash_vector_iterator :
    public std::iterator< typename std::forward_iterator_tag,
			  T,
			  typename std::ptrdiff_t,
			  T *,
			  T & >
  {

  public:

    typedef T				value_type;
    typedef value_type &		reference;
    typedef value_type *		pointer;
    typedef typename std::size_t	size_type;
    typedef typename std::ptrdiff_t	difference_type;

    hash_vector_iterator();

    hash_vector_iterator(T *ptr);

    bool operator==(const hash_vector_iterator &it) const;

    bool operator!=(const hash_vector_iterator &it) const;

    reference operator*() const;

    pointer operator->() const;

    hash_vector_iterator &operator++();

    const hash_vector_iterator operator++(int);

    const hash_vector_iterator operator+(size_type offset) const;

  private:
    pointer _ptr;

  };

}

#endif // JSON_HASH_VECTOR_ITERATOR_H
