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

#include <functional>
#include "json/def.h"
#include "json/char_sequence.h"

namespace json
{

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
  template < typename Char, typename Traits = std::char_traits<Char> >
  class hash_key : public basic_char_sequence<Char, Traits>
  {

  public:

    typedef basic_char_sequence<Char, Traits>		char_sequence_type;
    typedef typename char_sequence_type::traits_type	traits_type;
    typedef typename char_sequence_type::char_type	char_type;
    typedef typename char_sequence_type::size_type	size_type;

    hash_key();

    hash_key(const char_sequence_type &s);

    hash_key &operator=(const char_sequence_type &s);

    size_type hash() const;

    size_type compute_hash() const;

    bool equals(const hash_key &k) const;

  protected:
    size_type _hash;

  };

  template < typename Char, typename Traits >
  bool operator==(const hash_key<Char, Traits> &k1,
		  const hash_key<Char, Traits> &k2);

  template < typename Char, typename Traits >
  bool operator!=(const hash_key<Char, Traits> &k1,
		  const hash_key<Char, Traits> &k2);

  extern template class hash_key<char>;

  extern template bool operator==(const hash_key<char> &,
				  const hash_key<char> &);

  extern template bool operator!=(const hash_key<char> &,
				  const hash_key<char> &);

}

namespace std
{

  template < typename Char, typename Traits >
  struct hash< typename json::hash_key<Char, Traits> >
  {

    typedef typename json::hash_key<Char, Traits>	argument_type;
    typedef typename std::size_t			return_type;

    return_type operator()(const argument_type &x) const
    {
      return x.hash();
    }

  };

}

#endif // JSON_HASH_KEY_H
