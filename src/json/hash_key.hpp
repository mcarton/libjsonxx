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

#ifndef JSON_HASH_KEY_HPP
#define JSON_HASH_KEY_HPP

#include "json/char_sequence.hpp"
#include "json/hash_key.h"

namespace json
{

  enum
    {
      hash_init = 5381
    };

  template < typename Char >
  std::size_t hash_one(const Char c, const std::size_t h)
  {
    return ((h << 5) + h) ^ c;
  }

  template < typename Char, typename Traits >
  std::size_t hash(const basic_char_sequence<Char, Traits> &s)
  {
    std::size_t h = hash_init;
    auto it = s.begin();
    auto jt = s.end();
    while (it != jt)
      {
	h = hash_one(Traits::to_int_type(*it), h);
	++it;
      }
    return h;
  }

  template < typename Char, typename Traits >
  hash_key<Char, Traits>::
  hash_key():
    char_sequence_type(),
    _hash(hash_init)
  {
  }

  template < typename Char, typename Traits >
  hash_key<Char, Traits>::
  hash_key(const char_sequence_type &s):
    char_sequence_type(s),
    _hash(json::hash(s))
  {
  }

  template < typename Char, typename Traits >
  hash_key<Char, Traits> &
  hash_key<Char, Traits>::
  operator=(const char_sequence_type &s)
  {
    char_sequence_type::operator=(s);
    _hash = json::hash(s);
    return *this;
  }

  template < typename Char, typename Traits >
  typename hash_key<Char, Traits>::size_type
  hash_key<Char, Traits>::
  hash() const
  {
    return _hash;
  }

  template < typename Char, typename Traits >
  typename hash_key<Char, Traits>::size_type
  hash_key<Char, Traits>::
  compute_hash() const
  {
    return json::hash(*this);
  }

  template < typename Char, typename Traits >
  bool
  hash_key<Char, Traits>::
  equals(const hash_key &k) const
  {
    return (hash() == k.hash()) && char_sequence_type::equals(k);
  }

  template < typename Char, typename Traits >
  bool operator==(const hash_key<Char, Traits> &k1,
		  const hash_key<Char, Traits> &k2)
  {
    return k1.equals(k2);
  }

  template < typename Char, typename Traits >
  bool operator!=(const hash_key<Char, Traits> &k1,
		  const hash_key<Char, Traits> &k2)
  {
    return !k1.equals(k2);
  }

}

#endif // JSON_HASH_KEY_HPP
