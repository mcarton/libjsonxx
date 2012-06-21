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

#include <iostream>
#include <vector>
#include <utility>
#include "json/def.h"
#include "json/hash_key.h"

#ifndef NDEBUG
#include <iostream>
#endif // NDEBUG

namespace json
{

  void error_hash_map_key_not_found(const void *at,
				    const void *key,
				    std::size_t len);

  /**
   * @brief A generic hash map optimized for string keys.
   *
   * @note
   * <p>
   * C++ provides two mapping object in the standard library, <em>std::map</em>
   * and <em>std::unordered_map</em> but there is multiple issues with those
   * objects:
   * <ul>
   *  <li>When used with <em>std::string</em> as key the program performs a
   *  memory allocation everytime it uses the '[]' operator.</li>
   *  <li><em>std::unordered_map</em> would refuse to compile in the case of
   *  the implementation of a JSON object because of recursive types.</li>
   * </ul>
   * </p>
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

      basic_iterator(basic_iterator &it):
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

    hash_map(const allocator_type &a = allocator_type()):
      _allocator(a),
      _size(0),
      _capacity(0),
      _table(0)
    {
    }

    hash_map(const hash_map &map):
      _allocator(map._allocator),
      _size(0),
      _capacity(0),
      _table(0)
    {
      reserve(map._capacity);
      const_iterator it = map.begin();
      const_iterator jt = map.end();
      while (it != jt)
	{
	  emplace(it->first, it->second);
	  ++it;
	}
    }

    hash_map(hash_map &&map):
      _allocator(),
      _size(0),
      _capacity(0),
      _table(0)
    {
      map.swap(*this);
    }

    ~hash_map()
    {
      clear();
    }

    hash_map &operator=(const hash_map &map)
    {
      if (this != &map)
	{
	  hash_map(map).swap(*this);
	}
      return *this;
    }

    hash_map &operator=(hash_map &&map)
    {
      map.swap(*this);
      return *this;
    }

    value_type &operator[](const key_type &key)
    {
      iterator it = find(key);
      if (it == end())
	{
	  return emplace(key).second;
	}
      return it->second;
    }

    const value_type &operator[](const key_type &key) const
    {
      const_iterator it = find(key);
      if (it == end())
	{
	  error_hash_map_key_not_found(this, key.data(), key.size());
	}
      return it->second;
    }

    allocator_type get_allocator() const
    {
      return _allocator;
    }

    void swap(hash_map &map)
    {
      std::swap(_allocator, map._allocator);
      std::swap(_size, map._size);
      std::swap(_capacity, map._capacity);
      std::swap(_table, map._table);
    }

    size_type size() const
    {
      return _size;
    }

    bool empty() const
    {
      return _size == 0;
    }

    void clear()
    {
      destroy_table(_table, _capacity);
      _size = 0;
      _capacity = 0;
      _table = 0;
    }

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

    iterator erase(iterator it)
    {
      if (it != end())
	{
	  slot &s = _table[it._pos];

	  if (it._cur == it._end)
	    {
	      s.destroy([=](pair &p) { this->destroy_key(p.first); });
	      it._pos += 1;
	    }

	  else
	    {
	      pair_iterator &ct = it._cur;
	      pair_iterator &et = it._end;
	      destroy_key(ct->first);
	      ct = s.body.list.erase(ct);

	      if (s.body.list.empty())
		{
		  ct = pair_iterator();
		  et = pair_iterator();
		  it._pos += 1;
		  s.make_unset();
		}
	      else
		{
		  et = s.body.list.end();
		}
	    }

	  --_size;
	}
      return it;
    }

    size_type load(const size_type extra = 0) const
    {
      return ((_size + extra) * 100) / _capacity;
    }

    iterator begin()
    {
      return iterator( this, 0 );
    }

    iterator end()
    {
      return iterator( this, _capacity );
    }

    iterator find(const key_type &key)
    {
      return find<iterator>(this, key);
    }

    const_iterator begin() const
    {
      return const_iterator( this, 0 );
    }

    const_iterator end() const
    {
      return const_iterator( this, _capacity );
    }

    const_iterator find(const key_type &key) const
    {
      return find<const_iterator>(this, key);
    }

    void reserve(const size_type n)
    {
      if (n > _capacity)
	{
	  if (_capacity == 0)
	    {
	      _table = create_table(n);
	      _capacity = n;
	    }
	  else
	    {
	      expand((n / _capacity) + 1);
	    }
	}
    }

    void expand(const size_type n = 2)
    {
      if ((n > 1) && (_capacity != 0))
	{
	  hash_map m  ( get_allocator() );
	  iterator it ( begin() );
	  iterator jt ( end() );

	  m.reserve(n * _capacity);
	  try
	    {
	      while (it != jt)
		{
		  m.emplace_at(m.index_of(it->first), it->first, std::move(it->second));
		  detach_key(it->first);
		  ++it;
		}
	    }
	  catch (...)
	    {
	      // In case something went wrong we roll back the changes, this
	      // doesn't throw any exception because the internal buckets are
	      // already allocated.
	      it = m.begin();
	      jt = m.end();
	      while (it != jt)
		{
		  emplace_at(m.index_of(it->first), it->first, std::move(it->second));
		  detach_key(it->first);
		  ++it;
		}
	      throw;
	    }

	  m.swap(*this);
	}
    }

#ifndef NDEBUG

    // This method is implemented for debugging purposes only, it serializes the
    // internal hash map structure into the output stream given as argument.

    void dump(std::basic_ostream<char_type, traits_type> &out) const
    {
      slot *it = _table;
      slot *jt = _table + _capacity;

      out << "{";
      while (it != jt)
	{
	  slot &s = *it;

	  out << " ";
	  switch (s.type)
	    {
	    case slot_unset:
	      out << "(unset)";
	      break;

	    case slot_item:
	      out << s.body.item.first;
	      break;

	    case slot_list:
	      auto ct = s.body.list.begin();
	      auto et = s.body.list.end();
	      out << "[";
	      while (ct != et)
		{
		  out << " " << ct->first;
		  ++ct;
		}
	      out << " ]";
	      break;
	    }

	  ++it;
	}
      out << " }";
      out << std::endl;
    }
#endif // NDEBUG

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

    size_type index_of(const key_type &key) const
    {
      return key.hash() % _capacity;
    }

    slot *create_table(const size_type size) const
    {
      slot *table = _allocator.allocate(size);
      size_type i = 0;
      size_type j;
      while (i != size)
	{
	  j = i++;
	  _allocator.construct(table + j);
	}
      return table;
    }

    void destroy_table(slot *table, const size_type size) const
    {
      slot *ptr = table;
      slot *end = table + size;
      while (ptr != end)
	{
	  ptr->destroy([=](pair &p) { this->destroy_key(p.first); });
	  _allocator.destroy(ptr++);
	}
      _allocator.deallocate(table, size);
    }

    key_type create_key(const key_type &k) const
    {
      if (k.size() == 0)
	{
	  return k;
	}
      char_type *s = get_allocator().allocate(k.size() + 1);
      std::copy(k.begin(), k.end(), s);
      s[k.size()] = 0;
      return key_type( char_sequence_type(s, k.size()) );
    }

    void destroy_key(key_type &k) const
    {
      if (k.size() != 0)
	{
	  get_allocator().deallocate(const_cast<char_type*>(k.data()), k.size());
	}
    }

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
