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

#ifndef JSON_HASH_KEY_H
#define JSON_HASH_KEY_H

#include "json/def.h"
#include "json/char_sequence.h"

namespace json
{

  enum
    {
      hash_init = 5381
    };

  template < typename Char >
  inline std::size_t hash_one(const Char c, const std::size_t h)
  {
    return ((h << 5) + h) ^ c;
  }

  template < typename Char, typename Traits >
  inline std::size_t hash(const basic_char_sequence<Char, Traits> &s)
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

  /**
   * @brief Instances of this class are used as keys for json::hash_map.
   *
   * Because of limitations of the STL we had to provide a custom implementation
   * of a hash map, this class represents the keys of the hash map.
   * <br/>
   * Keys are read-only parameters, there's no point allocating memory everytime
   * we want to look-up for an element (even if that's the design of STL maps
   * using STL strings as key type). The <em>json::hash_key</em> class inherits
   * from <em>json::char_sequence</em> and adds auto-computation and caching of
   * hash values on top of that.
   *
   * @note This class is developped for internal purposes only and should not be
   * used outside of the libjson++ implementation.
   */
  template < typename Char, typename Traits = char_traits >
  class hash_key : public basic_char_sequence<Char, Traits>
  {

  public:

    typedef basic_char_sequence<Char, Traits>		char_sequence_type;
    typedef typename char_sequence_type::traits_type	traits_type;
    typedef typename char_sequence_type::char_type	char_type;
    typedef typename char_sequence_type::size_type	size_type;

    hash_key():
      char_sequence_type(),
      _hash(hash_init)
    {
    }

    hash_key(const hash_key &k):
      char_sequence_type(k),
      _hash(k._hash)
    {
    }

    hash_key(const char_sequence_type &s):
      char_sequence_type(s),
      _hash(json::hash(s))
    {
    }

    hash_key &operator=(const hash_key &k)
    {
      char_sequence_type::operator=(k);
      _hash = k._hash;
      return *this;
    }

    hash_key &operator=(const char_sequence_type &s)
    {
      char_sequence_type::operator=(s);
      _hash = json::hash(s);
      return *this;
    }

    size_type hash() const
    {
      return _hash;
    }

    template < typename _Char, typename _Traits >
    bool equals(const hash_key<_Char, _Traits> &k) const
    {
      return (hash() == k.hash()) && char_sequence_type::equals(k);
    }

  protected:
    size_type _hash;

  };

  template < typename Char, typename Traits >
  inline bool operator==(const hash_key<Char, Traits> &k1,
			 const hash_key<Char, Traits> &k2)
  {
    return k1.equals(k2);
  }

  template < typename Char, typename Traits >
  inline bool operator!=(const hash_key<Char, Traits> &k1,
			 const hash_key<Char, Traits> &k2)
  {
    return !k1.equals(k2);
  }

}

#endif // JSON_HASH_KEY_H
