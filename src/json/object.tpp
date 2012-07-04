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

#include "json/object.h"

namespace json
{

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const basic_object &obj):
    _allocator(obj._allocator),
    _type(obj._type),
    _body()
  {
    _body.create_copy(obj._type, obj._body);
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(basic_object &&obj):
    _allocator(),
    _type(type_null),
    _body()
  {
    obj.swap(*this);
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const bool x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const short x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const int x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const long x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const long long x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const unsigned short x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const unsigned int x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const unsigned long x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const unsigned long long x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const float x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const double x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const long double x, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = x;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  basic_object(const char_sequence_type &s, const allocator_type &a):
    _allocator(a),
    _type(type_null),
    _body()
  {
    (*this) = s;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator>::
  ~basic_object()
  {
    clear();
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const basic_object &obj)
  {
    if (this != &obj)
      {
	basic_object(obj).swap(*this);
      }
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(basic_object &&obj)
  {
    obj.swap(*this);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const char_sequence_type &s)
  {
    _body.assign_string(_type, s, _allocator);
    _type = type_string;
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const object_string &s)
  {
    _body.assign_string(_type, s, _allocator);
    _type = type_string;
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(object_string &&s)
  {
    _body.assign_string(_type, std::forward<object_string>(s), _allocator);
    _type = type_string;
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const bool x)
  {
    if (x)
      {
	(*this) = "true";
      }
    else
      {
	(*this) = "false";
      }
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const short x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const int x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const long x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const long long x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const unsigned short x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const unsigned int x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const unsigned long x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const unsigned long long x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const float x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const double x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator=(const long double x)
  {
    assign_number(x);
    return *this;
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator[](const size_type index)
  {
    if (_type == type_null)
      {
	_body.create_list(_allocator);
	_type = type_list;
      }
    else if (_type != type_list)
      {
	error_json_object_invalid_type(this, type_list, _type,
				       "json::basic_object<?>::operator[index]");
      }
    if (_body.list.size() <= index)
      {
	_body.list.resize(index + 1);
      }
    return _body.list[index];
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> const &
  basic_object<Char, Traits, Allocator>::
  operator[](const size_type index) const
  {
    if (_type != type_list)
      {
	error_json_object_invalid_type(this, type_list, _type,
				       "json::basic_object<?>::operator[index]");
      }
    return _body.list.at(index);
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> &
  basic_object<Char, Traits, Allocator>::
  operator[](const char_sequence_type &key)
  {
    if (_type == type_null)
      {
	_body.create_map(_allocator);
	_type = type_map;
      }
    return get_map()[key];
  }

  template < typename Char, typename Traits, typename Allocator >
  basic_object<Char, Traits, Allocator> const &
  basic_object<Char, Traits, Allocator>::
  operator[](const char_sequence_type &key) const
  {
    return get_map()[key];
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::
  swap(basic_object<Char, Traits, Allocator> &obj)
  {
    object_body tmp;

    tmp.create_move(_type, std::move(_body));

    _body.destroy(_type);
    _body.create_move(obj._type, std::move(obj._body));

    obj._body.destroy(obj._type);
    obj._body.create_move(_type, std::move(tmp));

    tmp.destroy(_type);

    std::swap(_type, obj._type);
    std::swap(_allocator, obj._allocator);
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::allocator_type
  basic_object<Char, Traits, Allocator>::get_allocator() const
  {
    return _allocator;
  }

  template < typename Char, typename Traits, typename Allocator >
  object_type
  basic_object<Char, Traits, Allocator>::type() const
  {
    return _type;
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::clear()
  {
    _body.destroy(_type);
    _type = type_null;
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::make_null()
  {
    clear();
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::make_string()
  {
    if (_type != type_string)
      {
	clear();
	_body.create_string(_allocator);
	_type = type_string;
      }
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::make_list()
  {
    if (_type != type_list)
      {
	clear();
	_body.create_list(_allocator);
	_type = type_list;
      }
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::make_map()
  {
    if (_type != type_map)
      {
	clear();
	_body.create_map(_allocator);
	_type = type_map;
      }
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::object_string &
  basic_object<Char, Traits, Allocator>::get_string()
  {
    assert_type_is(type_string, "json::basic_object<?>::get_string");
    return _body.string;
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::const_object_string &
  basic_object<Char, Traits, Allocator>::get_string() const
  {
    assert_type_is(type_string, "json::basic_object<?>::get_string");
    return _body.string;
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::object_list &
  basic_object<Char, Traits, Allocator>::get_list()
  {
    assert_type_is(type_list, "json::basic_object<?>::get_list");
    return _body.list;
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::const_object_list &
  basic_object<Char, Traits, Allocator>::get_list() const
  {
    assert_type_is(type_list, "json::basic_object<?>::get_list");
    return _body.list;
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::object_map &
  basic_object<Char, Traits, Allocator>::get_map()
  {
    assert_type_is(type_map, "json::basic_object<?>::get_map");
    return _body.map;
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::const_object_map &
  basic_object<Char, Traits, Allocator>::get_map() const
  {
    assert_type_is(type_map, "json::basic_object<?>::get_map");
    return _body.map;
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::iterator
  basic_object<Char, Traits, Allocator>::begin()
  {
    switch (_type)
      {
      case type_list:   return iterator(_body.list.begin(), 0, _body.list.size());
      case type_map:    return iterator(_body.map.begin(), 0, _body.map.size());
      case type_string: break;
      case type_null:   break;
      }
    return iterator();
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::iterator
  basic_object<Char, Traits, Allocator>::end()
  {
    switch (_type)
      {
      case type_list:   return iterator(_body.list.end(), _body.list.size(), _body.list.size());
      case type_map:    return iterator(_body.map.end(), _body.map.size(), _body.map.size());
      case type_string: break;
      case type_null:   break;
      }
    return iterator();
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::const_iterator
  basic_object<Char, Traits, Allocator>::begin() const
  {
    switch (_type)
      {
      case type_list:   return const_iterator(_body.list.begin(), 0, _body.list.size());
      case type_map:    return const_iterator(_body.map.begin(), 0, _body.map.size());
      case type_string: break;
      case type_null:   break;
      }
    return const_iterator();
  }

  template < typename Char, typename Traits, typename Allocator >
  typename basic_object<Char, Traits, Allocator>::const_iterator
  basic_object<Char, Traits, Allocator>::end() const
  {
    switch (_type)
      {
      case type_list:   return const_iterator(_body.list.end(), _body.list.size(), _body.list.size());
      case type_map:    return const_iterator(_body.map.end(), _body.map.size(), _body.map.size());
      case type_string: break;
      case type_null:   break;
      }
    return const_iterator();
  }

  template < typename Char, typename Traits, typename Allocator >
  void
  basic_object<Char, Traits, Allocator>::
  assert_type_is(const object_type type, const char *function) const
  {
    if (type != _type)
      {
	error_json_object_invalid_type(this, type, _type, function);
      }
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool equals_address(const basic_object<Char, Traits, Allocator1> &obj1,
		      const basic_object<Char, Traits, Allocator2> &obj2)
  {
    return std::addressof(obj1) == std::addressof(obj2);
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool equals_string(const basic_object<Char, Traits, Allocator1> &obj1,
		     const basic_object<Char, Traits, Allocator2> &obj2)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;
    return char_sequence(obj1.get_string()) == char_sequence(obj2.get_string());
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool equals_list(const basic_object<Char, Traits, Allocator1> &obj1,
		   const basic_object<Char, Traits, Allocator2> &obj2)
  {
    auto &list1 = obj1.get_list();
    auto &list2 = obj2.get_list();
    if (list1.size() != list2.size())
      {
	return false;
      }
    auto it1 = list1.begin();
    auto it2 = list2.begin();
    auto jt1 = list1.end();
    while (it1 != jt1)
      {
	if (!equals(*it1, *it2))
	  {
	    return false;
	  }
	++it1;
	++it2;
      }
    return true;
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool equals_map(const basic_object<Char, Traits, Allocator1> &obj1,
		  const basic_object<Char, Traits, Allocator2> &obj2)
  {
    auto &map1 = obj1.get_map();
    auto &map2 = obj2.get_map();
    if (map1.size() != map2.size())
      {
	return false;
      }
    auto it1 = map1.begin();
    auto jt1 = map1.end();
    auto jt2 = map2.end();
    while (it1 != jt1)
      {
	auto it2 = map2.find(it1->first);
	if ((it2 == jt2) || !equals(it1->second, it2->second))
	  {
	    return false;
	  }
	++it1;
      }
    return true;
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool equals(const basic_object<Char, Traits, Allocator1> &obj1,
	      const basic_object<Char, Traits, Allocator2> &obj2)
  {
    if (!equals_address(obj1, obj2))
      {
	if (obj1.type() != obj2.type())
	  {
	    return false;
	  }
	switch (obj1.type())
	  {
	  case type_null:   return true;
	  case type_string: return equals_string(obj1, obj2);
	  case type_list:   return equals_list(obj1, obj2);
	  case type_map:    return equals_map(obj1, obj2);
	  }
      }
    return true;
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool operator==(const basic_object<Char, Traits, Allocator1> &obj1,
		  const basic_object<Char, Traits, Allocator2> &obj2)
  {
    return equals(obj1, obj2);
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool operator!=(const basic_object<Char, Traits, Allocator1> &obj1,
		  const basic_object<Char, Traits, Allocator2> &obj2)
  {
    return !(obj1 == obj2);
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  bool operator==(const basic_object<Char, Traits, Allocator> &obj,
		  const basic_char_sequence<Char, Traits> &str)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;
    return (obj.type() == type_string) && (char_sequence(obj.get_string()) == str);
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  bool operator!=(const basic_object<Char, Traits, Allocator> &obj,
		  const basic_char_sequence<Char, Traits> &str)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;
    return (obj.type() != type_string) || (char_sequence(obj.get_string()) != str);
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  bool operator==(const basic_char_sequence<Char, Traits> &str,
		  const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj == str;
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  bool operator!=(const basic_char_sequence<Char, Traits> &str,
		  const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj != str;
  }

}

namespace std
{

  // Some say this is not good practice to forward declare STL classes but if
  // we want to provide template '<<' and '>>' operators for any stream types
  // we have no other choice...

  template < typename, typename > class basic_ostream;
  template < typename, typename > class basic_istream;

  template < typename Char, typename Traits, typename Allocator >
  inline basic_ostream<Char, Traits> &
  operator<<(basic_ostream<Char, Traits> &out,
	     const json::basic_object<Char, Traits, Allocator> &obj)
  {
    json::write_object(out, obj);
    return out;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline basic_istream<Char, Traits> &
  operator>>(basic_istream<Char, Traits> &in,
	     json::basic_object<Char, Traits, Allocator> &obj)
  {
    json::read_object(in, obj);
    return in;
  }

}
