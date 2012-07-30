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

#ifndef JSON_HASH_MAP_H
#define JSON_HASH_MAP_H

#include <vector>
#include <utility>
#include "json/def.h"
#include "json/hash_key.h"

#ifdef JSON_ENABLE_DEBUG
#include <iosfwd>
#endif // JSON_ENABLE_DEBUG

namespace json
{

  /**
   * @brief A generic hash map optimized for string keys.
   *
   * C++ provides two mapping object in the standard library, <em>std::map</em>
   * and <em>std::unordered_map</em> but there is multiple issues with those
   * objects:
   * <ul>
   *  <li>When used with <em>std::string</em> as key the program performs a
   *  memory allocation everytime it uses the '[]' operator, this is kind of
   *  sad and not matching the <em>"you don't pay for what you don't use"</em>
   *  spirit of C++.</li>
   *  <li><em>std::unordered_map</em> would refuse to compile in the case of
   *  the implementation of a JSON object because of recursive types (the JSON
   *  object class containing a map of other JSON objects). I'm not sure if
   *  that's only failing with GCC but the code has to be portable for multiple
   *  compilers anyway.</li>
   * </ul>
   * <br/>
   * The intent of this class is not to provide a replacement for <em>std::map</em>
   * or <em>std::unordered_map</em>, it is to provide a hash map implementation
   * that fits the needs to build the <em>json::object</em> class. The class API
   * is then not exactly the same as the one of the STL maps, it's similar but
   * simplier.
   *
   * @note This class is developped for internal purposes only and should not be
   * used outside of the libjson++ implementation.
   */
  template < typename T,
	     typename Char = char,
	     typename Traits = std::char_traits<Char>,
	     typename Allocator = std::allocator<Char> >
  class hash_map
  {

  public:

    typedef Char			char_type;
    typedef Traits			traits_type;
    typedef T				value_type;
    typedef T *				pointer;
    typedef T &				reference;
    typedef const T *			const_pointer;
    typedef const T &			const_reference;
    typedef typename std::size_t	size_type;
    typedef typename std::ptrdiff_t	difference_type;
    typedef Allocator			allocator_type;
    typedef hash_key<Char, Traits>	key_type;

    class pair
    {

    public:

      key_type   first;
      value_type second;

      template < typename... Args >
      pair(const key_type &k, Args&&... args):
	first(k),
	second(std::forward<Args>(args)...)
      {
      }

      pair(pair &&p):
	first(std::move(p.first)),
	second(std::move(p.second))
      {
      }

      ~pair()
      {
      }

      pair &operator=(pair &&p)
      {
	p.swap(*this);
	return *this;
      }

      void swap(pair &p)
      {
	std::swap(first, p.first);
	std::swap(second, p.second);
      }

    private:

      pair(const pair &);
      pair &operator=(const pair &);

    };

  private:

    typedef typename Allocator::template rebind<pair>::other	pair_allocator;
    typedef typename std::vector<pair, pair_allocator>		pair_list;
    typedef typename pair_list::iterator			pair_iterator;
    typedef typename pair_list::const_iterator			const_pair_iterator;

    enum slot_type
      {
	slot_unset,
	slot_item,
	slot_list
      };

    union slot_body
    {

      pair      item;
      pair_list list;	

      slot_body()
      {
      }

      ~slot_body()
      {
      }

      template < typename... Args >
      void create_item(Args&&... args)
      {
	new (&item) pair (std::forward<Args>(args)...);
      }

      template < typename... Args >
      void create_list(Args&&... args)
      {
	new (&list) pair_list (std::forward<Args>(args)...);
	list.reserve(4);
      }

      void destroy(const slot_type type)
      {
	destroy(type, [](pair &) {});
      }

      template < typename Function >
      void destroy(const slot_type type, const Function &function)
      {
	if (type == slot_item)
	  {
	    function(item);
	    item.~pair();
	  }
	else if (type == slot_list)
	  {
	    auto it = list.begin();
	    auto jt = list.end();
	    while (it != jt)
	      {
		function(*it);
		++it;
	      }
	    list.~pair_list();
	  }
      }

    };

    class slot
    {

    public:

      slot_type type;
      slot_body body;

      slot():
	type(slot_unset),
	body()
      {
      }

      ~slot()
      {
	body.destroy(type);
      }

      void make_unset()
      {
	body.destroy(type);
	type = slot_unset;
      }

      template < typename... Args >
      void make_item(Args&&... args)
      {
	make_unset();
	body.create_item(std::forward<Args>(args)...);
	type = slot_item;
      }

      template < typename... Args >
      void make_list(Args&&... args)
      {
	make_unset();
	body.create_list(std::forward<Args>(args)...);
	type = slot_list;
      }

      template < typename Function >
      void destroy(const Function &function)
      {
	body.destroy(type, function);
	type = slot_unset;
      }

    };

    typedef typename Allocator::template rebind<slot>::other slot_allocator;

    template < typename Map, typename Pair, typename PairIterator >
    class basic_iterator
    {

      friend class hash_map;

    public:

      basic_iterator():
	_map(0),
	_pos(0),
	_cur(),
	_end()
      {
      }

      basic_iterator(Map *map, const size_type pos):
	_map(map),
	_pos(pos),
	_cur(),
	_end()
      {
	while ((_pos != _map->_capacity) && (_map->_table[_pos].type == slot_unset))
	  {
	    ++_pos;
	  }

	if (_pos != _map->_capacity)
	  {
	    slot &s = _map->_table[_pos];
	    if (s.type == slot_list)
	      {
		_cur = s.body.list.begin();
		_end = s.body.list.end();
	      }
	  }
      }

      basic_iterator(Map *map,
		     const size_type pos,
		     const PairIterator &cur,
		     const PairIterator &end):
	_map(map),
	_pos(pos),
	_cur(cur),
	_end(end)
      {
      }

      basic_iterator(const basic_iterator &it):
	_map(it._map),
	_pos(it._pos),
	_cur(it._cur),
	_end(it._end)
      {
      }

      basic_iterator(basic_iterator &&it):
	_map(std::move(it._map)),
	_pos(std::move(it._pos)),
	_cur(std::move(it._cur)),
	_end(std::move(it._end))
      {
      }

      basic_iterator &operator=(const basic_iterator &it)
      {
	_map = it._map;
	_pos = it._pos;
	_cur = it._cur;
	_end = it._end;
	return *this;
      }

      basic_iterator &operator=(basic_iterator &&it)
      {
	_map = std::move(it._map);
	_pos = std::move(it._pos);
	_cur = std::move(it._cur);
	_end = std::move(it._end);
	return *this;
      }

      bool operator==(const basic_iterator &it) const
      {
	return (_map == it._map) && (_pos == it._pos) && (_cur == it._cur);
      }

      bool operator!=(const basic_iterator &it) const
      {
	return !((*this) == it);
      }

      basic_iterator &operator++()
      {
	const size_type n = _map->_capacity;
	slot *const table = _map->_table;

	while (_pos != n)
	  {

	    // Increases the bucket cursor position.
	    if (_cur != _end)
	      {
		if ((++_cur) != _end)
		  {
		    break;
		  }
		_cur = PairIterator();
		_end = PairIterator();
		++_pos;
	      }

	    // Move ahead in the map.
	    else if ((++_pos) != n)
	      {
		slot &s = table[_pos];
		switch (s.type)
		  {
		  case slot_unset: break;
		  case slot_list:
		    _cur = s.body.list.begin();
		    _end = s.body.list.end();
		  case slot_item: goto end;		    
		  }
	      }
	  }

      end:
	return *this;
      }

      Pair &operator*() const
      {
	slot &s = _map->_table[_pos];
	if (s.type == slot_list)
	  {
	    return *_cur;
	  }
	return s.body.item;
      }

      Pair *operator->() const
      {
	return &operator*();
      }

    private:

      Map *        _map;
      size_type    _pos;
      PairIterator _cur;
      PairIterator _end;

    };

  public:

    typedef typename key_type::char_sequence_type char_sequence_type;

    typedef basic_iterator<hash_map, pair, pair_iterator> iterator;

    typedef basic_iterator<const hash_map, const pair, const_pair_iterator> const_iterator;

    hash_map(const allocator_type &a = allocator_type());

    hash_map(const hash_map &map);

    hash_map(hash_map &&map);

    ~hash_map();

    hash_map &operator=(const hash_map &map);

    hash_map &operator=(hash_map &&map);

    value_type &operator[](const key_type &key);

    const value_type &operator[](const key_type &key) const;

    allocator_type get_allocator() const;

    void swap(hash_map &map);

    size_type size() const;

    bool empty() const;

    void clear();

    template < typename... Args >
    pair &emplace(const key_type &key, Args&&... args)
    {

      // In case the table hasn't been initialized yet.
      if (_capacity == 0)
	{
	  reserve(default_capacity);
	}

      // We make sure we're not overloading the table.
      if (load(1) >= 75)
	{
	  expand(2);
	}

      key_type new_key ( create_key(key) );
      try
	{
	  return emplace_at(index_of(key), new_key, std::forward<Args>(args)...);
	}
      catch (...)
	{
	  destroy_key(new_key);
	  throw;
	}
    }

    iterator erase(iterator it);

    size_type load(size_type extra = 0) const;

    iterator begin();

    iterator end();

    iterator find(const key_type &key);

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator find(const key_type &key) const;

    void reserve(size_type n);

    void expand(size_type n = 2);

#ifndef JSON_ENABLE_DEBUG

    // This method is implemented for debugging purposes only, it serializes the
    // internal hash map structure into the output stream given as argument.

    void dump(std::basic_ostream<char_type, traits_type> &out) const;

#endif // JSON_ENABLE_DEBUG

  private:
    mutable slot_allocator	_allocator;
    size_type			_size;
    size_type			_capacity;
    slot *			_table;

    enum
      {
	default_capacity = 42
      };

    template < typename... Args >
    pair &emplace_at(const size_type index, const key_type &key, Args&&... args)
    {
      slot &base = _table[index];

      switch (base.type)
	{

	  // Simply set the slot to the new value.
	case slot_unset:
	  base.make_item(key, std::forward<Args>(args)...);
	  ++_size;
	  return base.body.item;

	  // Transforms the item slot to a list slot and appends the new one.
	case slot_item:
	  {
	    pair p ( std::move(base.body.item) );
	    pair_list list ( _allocator );
	    try
	      {
		list.emplace_back(std::move(p));
		list.emplace_back(key, std::forward<Args>(args)...);
	      }
	    catch (...)
	      {
		base.body.item = std::move(p);
		throw;
	      }
	    base.make_list(std::move(list));
	    ++_size;
	    return base.body.list.back();
	  }

	  // Appends the slot to the list.
	case slot_list:
	  base.body.list.emplace_back(key, std::forward<Args>(args)...);
	  ++_size;
	  return base.body.list.back();

	}

      // Dead code, but otherwise the compiler complains...
      return base.body.item;
    }

    size_type index_of(const key_type &key) const;

    slot *create_table(size_type size) const;

    void destroy_table(slot *table, size_type size) const;

    key_type create_key(const key_type &k) const;

    void destroy_key(key_type &k) const;

    static void detach_key(key_type &k)
    {
      k = key_type();
    }

    template < typename Iterator, typename HashMap >
    static Iterator find(HashMap *self, const key_type &key)
    {
      if (self->_capacity != 0)
	{

	  const size_type index = self->index_of(key);
	  slot &s = self->_table[index];

	  if (s.type == slot_list)
	    {
	      auto it = s.body.list.begin();
	      auto jt = s.body.list.end();
	      while (it != jt)
		{
		  if (it->first == key)
		    {
		      return Iterator( self, index, it, jt );
		    }
		  ++it;
		}
	    }

	  else if ((s.type == slot_item) && (key == s.body.item.first))
	    {
	      return Iterator( self, index );
	    }

	}
      return self->end();
    }

  };

}

namespace std
{

  template < typename T,
	     typename Char,
	     typename Traits,
	     typename Allocator >
  inline void swap(json::hash_map<T, Char, Traits, Allocator> &h1,
		   json::hash_map<T, Char, Traits, Allocator> &h2)
  {
    h1.swap(h2);
  }

}

#endif // JSON_HASH_MAP_H
