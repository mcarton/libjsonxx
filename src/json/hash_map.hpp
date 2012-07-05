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

#ifndef JSON_HASH_MAP_HPP
#define JSON_HASH_MAP_HPP

#include "json/hash_map.h"

namespace json
{

  void error_hash_map_key_not_found(const void *at,
				    const void *key,
				    std::size_t len);

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  hash_map(const allocator_type &a):
    _allocator(a),
    _size(0),
    _capacity(0),
    _table(0)
  {
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  hash_map(hash_map &&map):
    _allocator(),
    _size(0),
    _capacity(0),
    _table(0)
  {
    map.swap(*this);
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator>::
  ~hash_map()
  {
    clear();
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator> &
  hash_map<T, Char, Traits, Allocator>::
  operator=(const hash_map &map)
  {
    if (this != &map)
      {
	hash_map(map).swap(*this);
      }
    return *this;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  hash_map<T, Char, Traits, Allocator> &
  hash_map<T, Char, Traits, Allocator>::
  operator=(hash_map &&map)
  {
    map.swap(*this);
    return *this;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::value_type &
  hash_map<T, Char, Traits, Allocator>::
  operator[](const key_type &key)
  {
    iterator it = find(key);
    if (it == end())
      {
	return emplace(key).second;
      }
    return it->second;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::value_type const &
  hash_map<T, Char, Traits, Allocator>::
  operator[](const key_type &key) const
  {
    const_iterator it = find(key);
    if (it == end())
      {
	error_hash_map_key_not_found(this, key.data(), key.size());
      }
    return it->second;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::allocator_type
  hash_map<T, Char, Traits, Allocator>::
  get_allocator() const
  {
    return _allocator;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  swap(hash_map &map)
  {
    std::swap(_allocator, map._allocator);
    std::swap(_size, map._size);
    std::swap(_capacity, map._capacity);
    std::swap(_table, map._table);
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::size_type
  hash_map<T, Char, Traits, Allocator>::
  size() const
  {
    return _size;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  bool
  hash_map<T, Char, Traits, Allocator>::
  empty() const
  {
    return _size == 0;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  clear()
  {
    destroy_table(_table, _capacity);
    _size = 0;
    _capacity = 0;
    _table = 0;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  erase(iterator it)
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::size_type
  hash_map<T, Char, Traits, Allocator>::
  load(const size_type extra) const
  {
    return ((_size + extra) * 100) / _capacity;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  begin()
  {
    return iterator( this, 0 );
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  end()
  {
    return iterator( this, _capacity );
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::iterator
  hash_map<T, Char, Traits, Allocator>::
  find(const key_type &key)
  {
    return find<iterator>(this, key);
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::const_iterator
  hash_map<T, Char, Traits, Allocator>::
  begin() const
  {
    return const_iterator( this, 0 );
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::const_iterator
  hash_map<T, Char, Traits, Allocator>::
  end() const
  {
    return const_iterator( this, _capacity );
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::const_iterator
  hash_map<T, Char, Traits, Allocator>::
  find(const key_type &key) const
  {
    return find<const_iterator>(this, key);
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  reserve(const size_type n)
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  expand(const size_type n)
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::size_type
  hash_map<T, Char, Traits, Allocator>::
  index_of(const key_type &key) const
  {
    return key.hash() % _capacity;
  }

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::slot *
  hash_map<T, Char, Traits, Allocator>::
  create_table(const size_type size) const
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  destroy_table(slot *table, const size_type size) const
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  typename hash_map<T, Char, Traits, Allocator>::key_type
  hash_map<T, Char, Traits, Allocator>::
  create_key(const key_type &k) const
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

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  destroy_key(key_type &k) const
  {
    if (k.size() != 0)
      {
	get_allocator().deallocate(const_cast<char_type*>(k.data()), k.size());
      }
  }

#ifndef JSON_ENABLE_DEBUG

  template < typename T, typename Char, typename Traits, typename Allocator >
  void
  hash_map<T, Char, Traits, Allocator>::
  dump(std::basic_ostream<char_type, traits_type> &out) const
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
    out << " }\n";
    out.flush();
  }

#endif // JSON_ENABLE_DEBUG

}

#endif // JSON_HASH_MAP_HPP

