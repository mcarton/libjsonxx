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

#ifndef JSON_HASH_TABLE_ITERATOR_H
#define JSON_HASH_TABLE_ITERATOR_H

#include <iterator>
#include <type_traits>
#include "json/def.h"

namespace json
{

  template < typename Iterator >
  class hash_table_iterator : public std::iterator
  < typename std::forward_iterator_tag,
    typename std::remove_reference<decltype(Iterator()->value())>::type,
    typename std::ptrdiff_t,
    typename std::remove_reference<decltype(Iterator()->value())>::type *,
    typename std::remove_reference<decltype(Iterator()->value())>::type & >
  {

  public:

    typedef typename std::remove_reference<decltype(Iterator()->value())>::type	value_type;
    typedef value_type &			reference;
    typedef value_type *			pointer;
    typedef typename Iterator::size_type	size_type;
    typedef typename Iterator::difference_type	difference_type;

    hash_table_iterator();

    hash_table_iterator(const Iterator &pos, const Iterator &end);

    bool operator==(const hash_table_iterator &it) const;

    bool operator!=(const hash_table_iterator &it) const;

    reference operator*() const;

    pointer operator->() const;

    hash_table_iterator &operator++();

    const hash_table_iterator operator++(int);

    Iterator slot_iterator() const;

  private:
    Iterator _pos;
    Iterator _end;

    void next();

  };

}

#endif // JSON_HASH_TABLE_ITERATOR_H
