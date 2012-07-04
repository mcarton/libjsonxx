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

#ifndef JSON_ITERATOR_H
#define JSON_ITERATOR_H

#include <iterator>
#include "json/def.h"
#include "json/types.h"

namespace json
{

  void error_null_iterator_cannot_be_incremented();

  /**
   * @brief This class provides the implementation of an object that is returned
   * by a call to <em>json::object::begin</em> and <em>json::object::end</em>.
   *
   * JSON objects can be iterated when they are lists or maps but the iterator is
   * intended to be used slightly differently in those two cases.
   * <br/>
   * <br/>
   * <strong>Structure of a JSON iterator:</strong>
   * JSON iterators point to objects that can be used as STL pairs (providing
   * <em>first</em> and <em>last</em> fields) which can also be implicitly cast
   * to a <em>json::object</em>. This is the most flexible way that was found to
   * represent the fact that we may be iterating over two different type of objects
   * in C++.
   * <br/>
   * Because of this design it requires some knowledge on how to use it.
   * <br/>
   * <br/>
   * <strong>Iterating over a JSON list:</strong>
   * <br/>
   * When iterating over a JSON list we can dereference the iterator to obtain a
   * reference to an iterator's internal object that can then be implicitly cast
   * to a JSON object.
   * @code
   * json::object obj;
   *
   * // ...
   *
   * auto it = obj.begin();
   * auto it = obj.end();
   *
   * while (it != jt)
   *   {
   *     json::object &x = *it; // Implicit cast of the deferenced iterator.
   *     
   *     // This is invalid, we can't access the JSON object directly from the
   *     // iterator using the '->' operator.
   *     //
   *     // int n = it->size();
   *     
   *     ++it;
   *   }
   * @endcode
   * <br/>
   * <br/>
   * <strong>Iterating over a JSON map:</strong>
   * <br/>
   * When iterating over a JSON map we can dereference the iterator to acess the
   * name and value of the current pair, just the way we'd do it with a STL map.
   * @code
   * json::object obj;
   *
   * // ...
   *
   * auto it = obj.begin();
   * auto it = obj.end();
   *
   * while (it != jt)
   *   {
   *     const json::char_sequence &key = it->first;
   *     const json::object &value = it->second;
   *     
   *     // ...
   *     
   *     ++it;
   *   }
   * @endcode
   */
  template < typename Object,
	     typename ListIterator,
	     typename MapIterator >
  class iterator
  {

    union iterator_body
    {

      ListIterator   list;
      MapIterator    map;

      iterator_body()
      {
      }

      ~iterator_body()
      {
      }

      template < typename... Args >
      void create_list(Args&&... args)
      {
	new (&list) ListIterator (std::forward<Args>(args)...);
      }

      template < typename... Args >
      void create_map(Args&&... args)
      {
	new (&map) MapIterator (std::forward<Args>(args)...);
      }

      void create_copy(const object_type type, const iterator_body &body)
      {
	switch (type)
	  {
	  case type_list:   create_list(body.list); break;
	  case type_map:    create_map(body.map);   break;
	  case type_string:                         break;
	  case type_null:                           break;
	  }
      }

      void create_move(const object_type type, iterator_body &&body)
      {
	switch (type)
	  {
	  case type_list:   create_list(std::move(body.list)); break;
	  case type_map:    create_map(std::move(body.map));   break;
	  case type_string:                                    break;
	  case type_null:                                      break;
	  }
      }

      void destroy_list()
      {
	list.~ListIterator();
      }

      void destroy_map()
      {
	map.~MapIterator();
      }

      void destroy(const object_type type)
      {
	switch (type)
	  {
	  case type_list:   destroy_list(); break;
	  case type_map:    destroy_map();  break;
	  case type_string:                 break;
	  case type_null:                   break;
	  }
      }

      void increment(const object_type type)
      {
	switch (type)
	  {
	  case type_list:   ++list;   break;
	  case type_map:    ++map;    break;
	  case type_string:
	  case type_null: error_null_iterator_cannot_be_incremented();
	  }	
      }

      bool equals(const object_type type, const iterator_body &body) const
      {
	switch (type)
	  {
	  case type_list:   return list == body.list;
	  case type_map:    return map == body.map;
	  case type_string: return true;
	  case type_null:   return true;
	  }
	return false;
      }

      bool not_equals(const object_type type, const iterator_body &body) const
      {
	switch (type)
	  {
	  case type_list:   return list != body.list;
	  case type_map:    return map != body.map;
	  case type_string: return false;
	  case type_null:   return false;
	  }
	return true;
      }

    };

  public:

    typedef typename std::size_t size_type;

    class value_type
    {

    public:

      typedef typename Object::object_map::key_type const       first_type;
      typedef Object                                            second_type;

      first_type  &first;
      second_type &second;

      value_type():
        first(*(first_type *)0),
        second(*(second_type *)0)
      {
      }

      value_type(Object &obj):
        first(*(first_type *)0),
        second(obj)
      {
      }

      value_type(first_type &x1, second_type &x2):
        first(x1),
        second(x2)
      {
      }

      operator Object &() const
      {
        return second;
      }

    };

    iterator():
      _type(type_null),
      _body(),
      _cursor(),
      _index(0),
      _size(0)
    {
    }

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

    ~iterator()
    {
      _body.destroy(_type);
    }

    iterator &operator=(const iterator &it)
    {
      iterator(it).swap(*this);
      return *this;
    }

    iterator &operator=(iterator &&it)
    {
      it.swap(*this);
      return *this;
    }

    iterator &operator++()
    {
      ++_index;
      _body.increment(_type);
      set_cursor();
      return *this;
    }

    iterator operator++(int)
    {
      const iterator it ( *this );
      ++(*this);
      return it;
    }

    bool operator==(const iterator &it) const
    {
      return (_type == it._type) && _body.equals(_type, it._body);
    }

    bool operator!=(const iterator &it) const
    {
      return (_type != it._type) || _body.not_equals(_type, it._body);
    }

    const value_type &operator*() const
    {
      return _cursor;
    }

    const value_type *operator->() const
    {
      return &_cursor;
    }

    void swap(iterator &it)
    {
      iterator_body tmp;

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

  private:
    object_type		_type;
    iterator_body	_body;
    value_type          _cursor;
    size_type           _index;
    size_type           _size;

    void set_cursor()
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

  };

}

namespace std
{

  template < typename Object,
	     typename ListIterator,
	     typename MapIterator >
  inline void swap(json::iterator<Object, ListIterator, MapIterator> &it1,
		   json::iterator<Object, ListIterator, MapIterator> &it2)
  {
    it1.swap(it2);
  }

}

#endif // JSON_ITERATOR_H
