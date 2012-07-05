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

#include <iosfwd>
#include <vector>
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
    typedef const object_list						const_object_list;
    typedef const object_map						const_object_map;
    typedef const object_string						const_object_string;
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

    typedef typename object_list::iterator list_iterator;
    typedef typename object_map::iterator map_iterator;

    typedef typename object_list::const_iterator const_list_iterator;
    typedef typename object_map::const_iterator const_map_iterator;

  public:

    typedef typename json::iterator<basic_object,
				    list_iterator,
				    map_iterator> iterator;

    typedef typename json::iterator<const basic_object,
				    const_list_iterator,
				    const_map_iterator>	const_iterator;

    basic_object(const allocator_type &a = allocator_type());

    basic_object(const basic_object &obj);

    basic_object(basic_object &&obj);

    basic_object(bool x,
		 const allocator_type &a = allocator_type());

    basic_object(short x,
		 const allocator_type &a = allocator_type());

    basic_object(int x,
		 const allocator_type &a = allocator_type());

    basic_object(long x,
		 const allocator_type &a = allocator_type());

    basic_object(long long x,
		 const allocator_type &a = allocator_type());

    basic_object(unsigned short x,
		 const allocator_type &a = allocator_type());

    basic_object(unsigned int x,
		 const allocator_type &a = allocator_type());

    basic_object(unsigned long x,
		 const allocator_type &a = allocator_type());

    basic_object(unsigned long long x,
		 const allocator_type &a = allocator_type());

    basic_object(float x,
		 const allocator_type &a = allocator_type());

    basic_object(double x,
		 const allocator_type &a = allocator_type());

    basic_object(long double x,
		 const allocator_type &a = allocator_type());

    basic_object(const char_sequence_type &s,
		 const allocator_type &a = allocator_type());

    template < int N >
    basic_object(const char_type (&s)[N],
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = s;
    }

    template < typename _Alloc >
    basic_object(const std::basic_string<Char, Traits, _Alloc> &s,
		 const allocator_type &a = allocator_type()):
      _allocator(a),
      _type(type_null),
      _body()
    {
      (*this) = s;
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

    ~basic_object();

    basic_object &operator=(const basic_object &obj);

    basic_object &operator=(basic_object &&obj);

    basic_object &operator=(const char_sequence_type &s);

    basic_object &operator=(const_object_string &s);

    basic_object &operator=(object_string &&s);

    basic_object &operator=(bool x);

    basic_object &operator=(short x);

    basic_object &operator=(int x);

    basic_object &operator=(long x);

    basic_object &operator=(long long x);

    basic_object &operator=(unsigned short x);

    basic_object &operator=(unsigned int x);

    basic_object &operator=(unsigned long x);

    basic_object &operator=(unsigned long long x);

    basic_object &operator=(float x);

    basic_object &operator=(double x);

    basic_object &operator=(long double x);

    basic_object &operator[](size_type index);

    template < int N >
    basic_object &operator=(const char_type (&s)[N])
    {
      return (*this) = char_sequence_type(s);
    }

    template < typename _Alloc >
    basic_object &operator=(const std::basic_string<Char, Traits, _Alloc> &s)
    {
      return (*this) = char_sequence_type(s);
    }

    template < typename _Alloc >
    basic_object &operator=(const basic_object<_Alloc> &obj)
    {
      basic_object(obj).swap(*this);
      return *this;
    }

    const basic_object &operator[](size_type index) const;

    basic_object &operator[](const char_sequence_type &key);

    const basic_object &operator[](const char_sequence_type &key) const;

    operator object_string &()
    {
      return get_string();
    }

    operator const_object_string &() const
    {
      return get_string();
    }

    template < typename _Alloc >
    operator std::basic_string<char_type, traits_type, _Alloc>() const
    {
      return get_string();
    }

    void swap(basic_object &obj);

    allocator_type get_allocator() const;

    object_type type() const;

    void clear();

    void make_null();

    void make_string();

    void make_list();

    void make_map();

    object_string &get_string();

    const_object_string &get_string() const;

    object_list &get_list();

    const_object_list &get_list() const;

    object_map &get_map();

    const_object_map &get_map() const;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

  private:
    allocator_type	_allocator;
    object_type		_type;
    object_body		_body;

    template < typename Integer >
    void assign_number(const Integer &x)
    {
      object_string s ( to_string<char_type, traits_type>(x, _allocator) );
      _body.assign_string(_type, std::move(s), _allocator);
      _type = type_string;
    }

    void assert_type_is(object_type, const char *) const;

  };

  extern template class basic_object<char>;

  extern template class iterator<object,
				 object::object_list::iterator,
				 object::object_map::iterator>;

  extern template class iterator<const object,
				 object::object_list::const_iterator,
				 object::object_map::const_iterator>;

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
  bool operator==(const basic_object<Char, Traits, Allocator1> &obj1,
		  const basic_object<Char, Traits, Allocator2> &obj2);

  template < typename Char,
	     typename Traits,
	     typename Allocator1,
	     typename Allocator2 >
  bool operator!=(const basic_object<Char, Traits, Allocator1> &obj1,
		  const basic_object<Char, Traits, Allocator2> &obj2);

  template < typename Char,
	     typename Traits,
	     typename Allocator >
  bool operator==(const basic_object<Char, Traits, Allocator> &obj,
		  const basic_char_sequence<Char, Traits> &str);

  template < typename Char,
	     typename Traits,
	     typename Allocator >
  bool operator!=(const basic_object<Char, Traits, Allocator> &obj,
		  const basic_char_sequence<Char, Traits> &str);

  template < typename Char,
	     typename Traits,
	     typename Allocator >
  bool operator==(const basic_char_sequence<Char, Traits> &str,
		  const basic_object<Char, Traits, Allocator> &obj);

  template < typename Char,
	     typename Traits,
	     typename Allocator >
  bool operator!=(const basic_char_sequence<Char, Traits> &str,
		  const basic_object<Char, Traits, Allocator> &obj);

  inline std::istream &operator>>(std::istream &is, json::object &obj)
  {
    read_object(is, obj);
    return is;
  }

  inline std::ostream &operator<<(std::ostream &os, const json::object &obj)
  {
    write_object(os, obj);
    return os;
  }

}

namespace std
{

  template < typename Char, typename Traits, typename Allocator >
  inline void swap(json::basic_object<Char, Traits, Allocator> &obj1,
		   json::basic_object<Char, Traits, Allocator> &obj2)
  {
    obj1.swap(obj2);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline int stoi(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stoi(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline long stol(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stol(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline long long stoll(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stoll(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline float stof(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stof(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline double stod(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stod(obj);
  }

  template < typename Char, typename Traits, typename Allocator >
  inline long double stold(const json::basic_object<Char, Traits, Allocator> &obj)
  {
    return json::stold(obj);
  }

}

#endif // JSON_OBJECT_H

