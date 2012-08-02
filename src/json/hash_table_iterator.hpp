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

#ifndef JSON_HASH_TABLE_ITERATOR_HPP
#define JSON_HASH_TABLE_ITERATOR_HPP

#include "json/hash_table_iterator.h"

namespace json
{

  template < typename Iterator >
  hash_table_iterator<Iterator>::
  hash_table_iterator():
    _pos(),
    _end()
  {
  }

  template < typename Iterator >
  hash_table_iterator<Iterator>::
  hash_table_iterator(const Iterator &pos, const Iterator &end):
    _pos(pos),
    _end(end)
  {
    next();
  }

  template < typename Iterator >
  bool
  hash_table_iterator<Iterator>::
  operator==(const hash_table_iterator &it) const
  {
    return _pos == it._pos;
  }

  template < typename Iterator >
  bool
  hash_table_iterator<Iterator>::
  operator!=(const hash_table_iterator &it) const
  {
    return _pos != it._pos;
  }

  template < typename Iterator >
  typename hash_table_iterator<Iterator>::reference
  hash_table_iterator<Iterator>::
  operator*() const
  {
    return _pos->value();
  }

  template < typename Iterator >
  typename hash_table_iterator<Iterator>::pointer
  hash_table_iterator<Iterator>::
  operator->() const
  {
    return &operator*();
  }

  template < typename Iterator >
  hash_table_iterator<Iterator> &
  hash_table_iterator<Iterator>::
  operator++()
  {
    ++_pos;
    next();
    return *this;
  }

  template < typename Iterator >
  const hash_table_iterator<Iterator>
  hash_table_iterator<Iterator>::
  operator++(int)
  {
    const hash_table_iterator tmp (*this);
    ++(*this);
    return tmp;
  }

  template < typename Iterator >
  Iterator
  hash_table_iterator<Iterator>::
  slot_iterator() const
  {
    return _pos;
  }

  template < typename Iterator >
  void
  hash_table_iterator<Iterator>::
  next()
  {
    typedef typename Iterator::value_type slot_type;
    while ((_pos != _end) && (slot_iterator()->status != slot_type::busy))
      {
	++_pos;
      }
  }

}

#endif // JSON_HASH_VECTOR_ITERATOR_HPP
