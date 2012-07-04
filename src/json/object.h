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

#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <vector>
#include <string>
#include <memory>
#include "json/def.h"
#include "json/types.h"
#include "json/for_each.h"
#include "json/hash_map.h"
#include "json/parsing.h"
#include "json/string.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/iterator.h"

namespace json
{

  void error_json_object_invalid_type(const void *at,
				      object_type expected,
				      object_type found,
				      const char *function);

  /**
   * @brief Implementation of a JSON object.
   *
   * <p>
   * This template class follows the STL design an provides the implementation
   * of a JSON object.
   * <br/>
   * JSON objects may be of 4 different types which are 'null', 'string', 'list'
   * and 'map'.
   * <br/>
   * The JSON standard also specify a 'number' type but to keep the code simple
   * and avoid useless work those types are stored under the 'string' type in
   * the object, they can be converted to integers or floating point numbers
   * using the <em>std::stol</em> and <em>std::stod</em> functions.
   * </p>
   */
  template < typename Char,
	     typename Traits = std::char_traits<Char>,
	     typename Allocator = std::allocator<Char> >
  class basic_object
  {

  public:

    typedef typename Allocator::template rebind<basic_object>::other	object_allocator;
    typedef typename std::vector<basic_object, object_allocator>	object_list;
    typedef hash_map<basic_object, Char, Traits, Allocator>		object_map;
    typedef typename std::basic_string<Char, Traits, Allocator>		object_string;
    typedef Allocator							allocator_type;
    typedef Traits							traits_type;
    typedef Char							char_type;
    typedef basic_char_sequence<Char, Traits>				char_sequence_type;
    typedef typename object_string::size_type				size_type;

  private:

    // The implementation uses a union to store the body of the JSON objects
    // and avoid wasting unused memory space.
    // This is possible thanks to the new C++11 standard which allows unions
    // of complex types by overloading the default constructor and the
    // destructor.

    union object_body
    {

      object_string string;
      object_list   list;
      object_map    map;

      object_body()
      {
      }

      ~object_body()
      {
      }

      template < typename... Args >
      void create_string(Args&&... args)
      {
	new (&string) object_string ( std::forward<Args>(args)... );
      }

      template < typename... Args >
      void create_list(Args&&... args)
      {
	new (&list) object_list ( std::forward<Args>(args)... );
	list.reserve(32);
      }

      template < typename... Args >
      void create_map(Args&&... args)
      {
	new (&map) object_map ( std::forward<Args>(args)... );
      }

      void create_copy(const object_type type, const object_body &body)
      {
	switch (type)
	  {
	  case type_string: create_string(body.string); break;
	  case type_list:   create_list(body.list);     break;
	  case type_map:    create_map(body.map);       break;
	  case type_null:                               break;
	  }
      }

      void create_move(const object_type type, object_body &&body)
      {
	switch (type)
	  {
	  case type_string: create_string(std::move(body.string)); break;
	  case type_list:   create_list(std::move(body.list));     break;
	  case type_map:    create_map(std::move(body.map));       break;
	  case type_null:                                          break;
	  }	
      }

      void destroy_string()
      {
	string.~object_string();
      }

      void destroy_list()
      {
	list.~object_list();
      }

      void destroy_map()
      {
	map.~object_map();
      }

      void destroy(const object_type type)
      {
	switch (type)
	  {
	  case type_string: destroy_string(); break;
	  case type_list:   destroy_list();   break;
	  case type_map:    destroy_map();    break;
	  case type_null:                     break;
	  }
      }

      void assign_string(const object_type type,
			 const char_sequence_type &s,
			 const allocator_type &a)
      {
	if (type != type_string)
	  {
	    destroy(type);
	    create_string(a);
	  }
	string.assign(s.data(), s.size());
      }

      void assign_string(const object_type type,
			 const object_string &s,
			 const allocator_type &a)
      {
	if (type != type_string)
	  {
	    destroy(type);
	    create_string(a);
	  }
	string.assign(s);
      }

      void assign_string(const object_type type,
			 object_string &&s,
			 const allocator_type &a)
      {
	if (type != type_string)
	  {
	    destroy(type);
	    create_string(a);
	  }
	string.assign(std::forward<object_string>(s));
      }

    };

    typedef typename object_list::iterator		list_iterator;
    typedef typename object_map::iterator		map_iterator;

    typedef typename object_list::const_iterator	const_list_iterator;
    typedef typename object_map::const_iterator		const_map_iterator;

  public:

    typedef typename json::iterator<basic_object,
				    list_iterator,
				    map_iterator>	iterator;

    typedef typename json::iterator<const basic_object,
				    const_list_iterator,
				    const_map_iterator>	const_iterator;

    basic_object(const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
    }

    basic_object(const basic_object &obj):
      _allocator(obj._allocator),
      _type(obj._type),
      _body()
    {
      _body.create_copy(obj._type, obj._body);
    }

    template < typename _Alloc >
    basic_object(const basic_object<Char, Traits, _Alloc> &obj,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(obj.type()),
      _body()
    {
      switch (obj.type())
	{
	case type_string: _body.create_string(obj.get_string()); break;
	case type_list:   _body.create_list(obj.get_list());     break;
	case type_map:    _body.create_map(obj.get_map());       break;
	case type_null:                                          break;
	}
    }

    basic_object(basic_object &&obj):
      _allocator(),
      _type(type_null),
      _body()
    {
      obj.swap(*this);
    }

    template < int N >
    basic_object(const char (&s)[N],
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = s;
    }

    basic_object(const char_sequence_type &s,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = s;
    }

    basic_object(const object_string &s,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = s;
    }

    basic_object(object_string &&s,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = std::forward<object_string>(s);
    }

    basic_object(const long x,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = x;
    }

    basic_object(const double x,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = x;
    }

    ~basic_object()
    {
      clear();
    }

    basic_object &operator=(const basic_object &obj)
    {
      if (this != &obj)
	{
	  basic_object(obj).swap(*this);
	}
      return *this;
    }

    template < typename _Alloc >
    basic_object &operator=(const basic_object<_Alloc> &obj)
    {
      basic_object(obj).swap(*this);
      return *this;
    }

    basic_object &operator=(basic_object &&obj)
    {
      obj.swap(*this);
      return *this;
    }

    template < int N >
    basic_object &operator=(const char (&s)[N])
    {
      return ((*this) = char_sequence_type(s));
    }

    basic_object &operator=(const char_sequence_type &s)
    {
      _body.assign_string(_type, s, _allocator);
      _type = type_string;
      return *this;
    }

    basic_object &operator=(const object_string &s)
    {
      _body.assign_string(_type, s, _allocator);
      _type = type_string;
      return *this;
    }

    basic_object &operator=(object_string &&s)
    {
      _body.assign_string(_type, std::forward<object_string>(s), _allocator);
      _type = type_string;
      return *this;
    }

    basic_object &operator=(const bool x)
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

    basic_object &operator=(const short x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const int x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const long x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const long long x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const unsigned short x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const unsigned int x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const unsigned long x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const unsigned long long x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const float x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const double x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator=(const long double x)
    {
      assign_number(x);
      return *this;
    }

    basic_object &operator[](const size_type index)
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

    const basic_object &operator[](const size_type index) const
    {
      if (_type != type_list)
	{
	  error_json_object_invalid_type(this, type_list, _type,
					 "json::basic_object<?>::operator[index]");
	}
      return _body.list.at(index);
    }

    basic_object &operator[](const char_sequence_type &key)
    {
      if (_type == type_null)
	{
	  _body.create_map(_allocator);
	  _type = type_map;
	}
      return get_map()[key];
    }

    const basic_object &operator[](const char_sequence_type &key) const
    {
      return get_map()[key];
    }

    operator object_string &()
    {
      return get_string();
    }

    operator const object_string &() const
    {
      return get_string();
    }

    template < typename _Alloc >
    operator std::basic_string<char_type, traits_type, _Alloc>() const
    {
      return get_string();
    }

    void swap(basic_object &obj)
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

    allocator_type get_allocator() const
    {
      return _allocator;
    }

    object_type type() const
    {
      return _type;
    }

    void clear()
    {
      _body.destroy(_type);
      _type = type_null;
    }

    void make_null()
    {
      clear();
    }

    void make_string()
    {
      if (_type != type_string)
	{
	  clear();
	  _body.create_string(_allocator);
	  _type = type_string;
	}
    }

    void make_list()
    {
      if (_type != type_list)
	{
	  clear();
	  _body.create_list(_allocator);
	  _type = type_list;
	}
    }

    void make_map()
    {
      if (_type != type_map)
	{
	  clear();
	  _body.create_map(_allocator);
	  _type = type_map;
	}
    }

    object_string &get_string()
    {
      assert_type_is(type_string, "json::basic_object<?>::get_string");
      return _body.string;
    }

    const object_string &get_string() const
    {
      assert_type_is(type_string, "json::basic_object<?>::get_string");
      return _body.string;
    }

    object_list &get_list()
    {
      assert_type_is(type_list, "json::basic_object<?>::get_list");
      return _body.list;
    }

    const object_list &get_list() const
    {
      assert_type_is(type_list, "json::basic_object<?>::get_list");
      return _body.list;
    }

    object_map &get_map()
    {
      assert_type_is(type_map, "json::basic_object<?>::get_map");
      return _body.map;
    }

    const object_map &get_map() const
    {
      assert_type_is(type_map, "json::basic_object<?>::get_map");
      return _body.map;
    }

    iterator begin()
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

    iterator end()
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

    const_iterator begin() const
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

    const_iterator end() const
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

  private:
    allocator_type	_allocator;
    object_type		_type;
    object_body		_body;

    template < typename Integer >
    void assign_number(const Integer x)
    {
      object_string s ( to_string<char_type, traits_type>(x, _allocator) );
      _body.assign_string(_type, std::move(s), _allocator);
      _type = type_string;
    }

    void assert_type_is(const object_type type, const char *function) const
    {
      if (type != _type)
	{
	  error_json_object_invalid_type(this, type, _type, function);
	}
    }

  };

  typedef basic_object<char> object;

  extern template class basic_object<char>;

  extern const object null;

  template < typename Char, typename Traits, typename Allocator >
  inline bool is_string(const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj.type() == type_string;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline bool is_list(const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj.type() == type_list;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline bool is_map(const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj.type() == type_map;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline bool is_null(const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj.type() == type_null;
  }

  template < typename Char, typename Traits, typename Allocator >
  inline bool is_true(const basic_object<Char, Traits, Allocator> &obj)
  {
    return (obj.type() == type_string) && is_json_true(obj.get_string());
  }

  template < typename Char, typename Traits, typename Allocator >
  inline bool is_false(const basic_object<Char, Traits, Allocator> &obj)
  {
    return (obj.type() == type_string) && is_json_false(obj.get_string());
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  inline bool equals_address(const basic_object<Char, Traits, Allocator1> &obj1,
			     const basic_object<Char, Traits, Allocator2> &obj2)
  {
    return std::addressof(obj1) == std::addressof(obj2);
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  inline bool equals_string(const basic_object<Char, Traits, Allocator1> &obj1,
			    const basic_object<Char, Traits, Allocator2> &obj2)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;
    return char_sequence(obj1.get_string()) == char_sequence(obj2.get_string());
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  inline bool equals_list(const basic_object<Char, Traits, Allocator1> &obj1,
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
  inline bool equals_map(const basic_object<Char, Traits, Allocator1> &obj1,
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
  inline bool equals(const basic_object<Char, Traits, Allocator1> &obj1,
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
  inline bool operator==(const basic_object<Char, Traits, Allocator1> &obj1,
			 const basic_object<Char, Traits, Allocator2> &obj2)
  {
    return equals(obj1, obj2);
  }

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  inline bool operator!=(const basic_object<Char, Traits, Allocator1> &obj1,
			 const basic_object<Char, Traits, Allocator2> &obj2)
  {
    return !(obj1 == obj2);
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  inline bool operator==(const basic_object<Char, Traits, Allocator> &obj,
                         const basic_char_sequence<Char, Traits> &str)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;
    return (obj.type() == type_string) && (char_sequence(obj.get_string()) == str);
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  inline bool operator!=(const basic_object<Char, Traits, Allocator> &obj,
                         const basic_char_sequence<Char, Traits> &str)
  {
    typedef basic_char_sequence<Char, Traits> char_sequence;
    return (obj.type() != type_string) || (char_sequence(obj.get_string()) != str);
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  inline bool operator==(const basic_char_sequence<Char, Traits> &str,
                         const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj == str;
  }

  template < typename Char,
             typename Traits,
             typename Allocator >
  inline bool operator!=(const basic_char_sequence<Char, Traits> &str,
                         const basic_object<Char, Traits, Allocator> &obj)
  {
    return obj != str;
  }

}

namespace std
{

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

  template < typename Char, typename Traits, typename Allocator >
  inline void swap(json::basic_object<Char, Traits, Allocator> &obj1,
		   json::basic_object<Char, Traits, Allocator> &obj2)
  {
    obj1.swap(obj2);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline int stoi(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stoi(obj.get_string());
  }

  template < typename Char, typename Traits, typename Allocator >
  inline long stol(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stol(obj.get_string());
  }

  template < typename Char, typename Traits, typename Allocator >
  inline long long stoll(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stoll(obj.get_string());
  }

  template < typename Char, typename Traits, typename Allocator >
  inline float stof(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stof(obj.get_string());
  }

  template < typename Char, typename Traits, typename Allocator >
  inline double stod(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stod(obj.get_string());
  }

  template < typename Char, typename Traits, typename Allocator >
  inline long double stold(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stold(obj.get_string());
  }

}

#endif // JSON_OBJECT_H

