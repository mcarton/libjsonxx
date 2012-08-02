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

#ifndef JSON_ITERATOR_HPP
#define JSON_ITERATOR_HPP

#include "json/iterator.h"

namespace json
{

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>::
  iterator():
    _type(type_null),
    _body(),
    _cursor(),
    _index(0),
    _size(0)
  {
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>::
  iterator(const ListIterator &it, const size_type i, const size_type s):
    _type(type_list),
    _body(),
    _cursor(),
    _index(i),
    _size(s)
  {
    _body.create_list(it);
    set_cursor();
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>::
  iterator(const MapIterator &it, const size_type i, const size_type s):
    _type(type_map),
    _body(),
    _cursor(),
    _index(i),
    _size(s)
  {
    _body.create_map(it);
    set_cursor();
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>::
  iterator(const iterator &it):
    _type(it._type),
    _body(),
    _cursor(),
    _index(it._index),
    _size(it._size)
  {
    _body.create_copy(it._type, it._body);
    set_cursor();
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>::
  iterator(iterator &&it):
    _type(it._type),
    _body(),
    _cursor(),
    _index(it._index),
    _size(it._size)
  {
    _body.create_move(it._type, std::move(it._body));
    set_cursor();
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>::
  ~iterator()
  {
    _body.destroy(_type);
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator> &
  iterator<Object, ListIterator, MapIterator>::
  operator=(const iterator &it)
  {
    iterator(it).swap(*this);
    return *this;
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator> &
  iterator<Object, ListIterator, MapIterator>::
  operator=(iterator &&it)
  {
    it.swap(*this);
    return *this;
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator> &
  iterator<Object, ListIterator, MapIterator>::
  operator++()
  {
    ++_index;
    _body.increment(_type);
    set_cursor();
    return *this;
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  iterator<Object, ListIterator, MapIterator>
  iterator<Object, ListIterator, MapIterator>::
  operator++(int)
  {
    const iterator it ( *this );
    ++(*this);
    return it;
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  bool
  iterator<Object, ListIterator, MapIterator>::
  operator==(const iterator &it) const
  {
    return (_type == it._type) && _body.equals(_type, it._body);
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  bool
  iterator<Object, ListIterator, MapIterator>::
  operator!=(const iterator &it) const
  {
    return (_type != it._type) || _body.not_equals(_type, it._body);
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  typename iterator<Object, ListIterator, MapIterator>::value_type const &
  iterator<Object, ListIterator, MapIterator>::
  operator*() const
  {
    return _cursor;
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  typename iterator<Object, ListIterator, MapIterator>::value_type const *
  iterator<Object, ListIterator, MapIterator>::
  operator->() const
  {
    return &_cursor;
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  void
  iterator<Object, ListIterator, MapIterator>::
  swap(iterator &it)
  {
    body_type tmp;

    tmp.create_move(_type, std::move(_body));
      
    _body.destroy(_type);
    _body.create_move(it._type, std::move(it._body));
      
    it._body.destroy(it._type);
    it._body.create_move(_type, std::move(tmp));

    tmp.destroy(_type);

    std::swap(_type, it._type);
    std::swap(_index, it._index);
    std::swap(_size, it._size);

    set_cursor();
    it.set_cursor();
  }

  template < typename Object, typename ListIterator, typename MapIterator >
  void
  iterator<Object, ListIterator, MapIterator>::
  set_cursor()
  {
    if (_index != _size)
      {
	_cursor.~value_type();
	switch (_type)
	  {
	  case type_list:   new (&_cursor) value_type (*(_body.list)); break;
	  case type_map:    new (&_cursor) value_type ((*_body.map).first, (*_body.map).second); break;
	  case type_string:
	  case type_null:   new (&_cursor) value_type (); break;
	  }
      }
  }

}

#endif // JSON_ITERATOR_HPP
