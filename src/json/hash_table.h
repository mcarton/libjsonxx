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

#ifndef JSON_HASH_TABLE_H
#define JSON_HASH_TABLE_H

#include <functional>
#include "json/def.h"
#include "json/hash_vector.h"
#include "json/hash_table_iterator.h"

namespace json
{

  template < typename T,
	     typename Hash = std::hash<T>,
	     typename Equals = std::equal_to<T>,
	     typename Allocator = std::allocator<T> >
  class hash_table
  {

    typedef hash_vector<T, Allocator>			vector_type;
    typedef typename vector_type::iterator		vector_iterator;
    typedef typename vector_type::const_iterator	vector_const_iterator;
    typedef typename vector_type::slot_type		slot_type;

  public:

    typedef Allocator					allocator_type;
    typedef Hash					hash_type;
    typedef Equals					equal_type;
    typedef T						value_type;
    typedef value_type &				reference;
    typedef value_type *				pointer;
    typedef const value_type &				const_reference;
    typedef const value_type *				const_pointer;
    typedef typename vector_type::size_type		size_type;
    typedef hash_table_iterator<vector_iterator>	iterator;
    typedef hash_table_iterator<vector_const_iterator>	const_iterator;

    hash_table(size_type capacity, const allocator_type &a = allocator_type());

    hash_table(hash_table &&t);

    hash_table(const hash_table &t);

    hash_table &operator=(hash_table &&t);

    hash_table &operator=(const hash_table &t);

    void swap(hash_table &t);

    size_type size() const;

    size_type capacity() const;

    size_type load() const;

    allocator_type get_allocator() const;

    iterator begin();

    iterator end();

    iterator find(const_reference x);

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator find(const_reference x) const;

    iterator insert(const_reference x);

    iterator emplace(value_type &&x);

    void expand(size_type factor);

    void erase(const_reference x);

    iterator erase(iterator it);

    void clear();

  private:
    size_type	_size;
    vector_type	_vector;

    size_type best_index(const_reference x) const;
    size_type nice_index(const_reference x) const;
    size_type find_index(const_reference x) const;

    static size_type eof();
    static size_type hash(const_reference x);
    static bool equals(const_reference x1, const_reference x2);

  };

}

namespace std
{

  template < typename T, typename Hash, typename Equals, typename Allocator >
  inline void swap(json::hash_table<T, Hash, Equals, Allocator> &t1,
		   json::hash_table<T, Hash, Equals, Allocator> &t2)
  {
    t1.swap(t2);
  }

}

#endif // JSON_HASH_TABLE_H
