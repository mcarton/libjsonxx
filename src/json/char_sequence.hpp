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

#ifndef JSON_CHAR_SEQUENCE_HPP
#define JSON_CHAR_SEQUENCE_HPP

#include "json/char_sequence.h"

namespace json
{

  template < typename Char, typename Traits >
  basic_char_sequence<Char, Traits>::
  basic_char_sequence():
    _str(empty_string<Char, Traits>()),
    _len(0)
  {
  }

  template < typename Char, typename Traits >
  basic_char_sequence<Char, Traits>::
  basic_char_sequence(const char_type *str, const size_type len):
    _str(str),
    _len(len)
  {
  }

  template < typename Char, typename Traits >
  bool
  basic_char_sequence<Char, Traits>::
  equals(const basic_char_sequence &s) const
  {
    return (this == &s) || ((size() == s.size()) && (compare(s) == 0));
  }

  template < typename Char, typename Traits >
  int
  basic_char_sequence<Char, Traits>::
  compare(const basic_char_sequence &s) const
  {
    auto it1 = begin();
    auto it2 = s.begin();
    auto jt1 = end();
    auto jt2 = s.end();

    while ((it1 != jt1) && (it2 != jt2))
      {
	const auto x1 = jt1 - it1;
	const auto x2 = jt2 - it2;
	const auto x3 = (x1 < x2) ? x1 : x2;
	switch (x3 % 8)
	  {
	  case 0: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 7: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 6: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 5: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 4: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 3: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 2: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  case 1: if ((*it1) != (*it2)) goto end; ++it1, ++it2;
	  }
      }

  end:
    return (*it1) - (*it2);
  }

  template < typename Char, typename Traits >
  typename basic_char_sequence<Char, Traits>::const_iterator
  basic_char_sequence<Char, Traits>::
  begin() const
  {
    return _str;
  }

  template < typename Char, typename Traits >
  typename basic_char_sequence<Char, Traits>::const_iterator
  basic_char_sequence<Char, Traits>::
  end() const
  {
    return _str + _len;
  }

  template < typename Char, typename Traits >
  typename basic_char_sequence<Char, Traits>::char_type const *
  basic_char_sequence<Char, Traits>::
  data() const
  {
    return _str;
  }

  template < typename Char, typename Traits >
  typename basic_char_sequence<Char, Traits>::size_type
  basic_char_sequence<Char, Traits>::
  size() const
  {
    return _len;
  }

  template < typename Char, typename Traits >
  typename basic_char_sequence<Char, Traits>::size_type
  basic_char_sequence<Char, Traits>::
  length() const
  {
    return _len;
  }

  template < typename Char, typename Traits >
  bool operator==(const basic_char_sequence<Char, Traits> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.equals(s2);
  }

  template < typename Char, typename Traits >
  bool operator!=(const basic_char_sequence<Char, Traits> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return !s1.equals(s2);
  }

  template < typename Char, typename Traits >
  bool operator<(const basic_char_sequence<Char, Traits> &s1,
		 const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) < 0;
  }

  template < typename Char, typename Traits >
  bool operator>(const basic_char_sequence<Char, Traits> &s1,
		 const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) > 0;
  }

  template < typename Char, typename Traits >
  bool operator<=(const basic_char_sequence<Char, Traits> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) <= 0;
  }

  template < typename Char, typename Traits >
  bool operator>=(const basic_char_sequence<Char, Traits> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) >= 0;
  }

  template < typename Char, typename Traits >
  bool operator==(const basic_char_sequence<Char, Traits> &s1,
		  const Char *s2)
  {
    return s1 == make_char_sequence(s2);
  }

  template < typename Char, typename Traits >
  bool operator!=(const basic_char_sequence<Char, Traits> &s1,
		  const Char *s2)
  {
    return s1 != make_char_sequence(s2);
  }

  template < typename Char, typename Traits >
  bool operator<(const basic_char_sequence<Char, Traits> &s1,
		 const Char *s2)
  {
    return s1 < make_char_sequence(s2);
  }

  template < typename Char, typename Traits >
  bool operator>(const basic_char_sequence<Char, Traits> &s1,
		 const Char *s2)
  {
    return s1 > make_char_sequence(s2);
  }

  template < typename Char, typename Traits >
  bool operator<=(const basic_char_sequence<Char, Traits> &s1,
		  const Char *s2)
  {
    return s1 <= make_char_sequence(s2);
  }

  template < typename Char, typename Traits >
  bool operator>=(const basic_char_sequence<Char, Traits> &s1,
		  const Char *s2)
  {
    return s1 >= make_char_sequence(s2);
  }

  template < typename Char, typename Traits >
  bool operator==(const Char *s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) == s2;
  }

  template < typename Char, typename Traits >
  bool operator!=(const Char *s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) != s2;
  }

  template < typename Char, typename Traits >
  bool operator<(const Char *s1,
		 const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) < s2;
  }

  template < typename Char, typename Traits >
  bool operator>(const Char *s1,
		 const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) > s2;
  }

  template < typename Char, typename Traits >
  bool operator<=(const Char *s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) <= s2;
  }

  template < typename Char, typename Traits >
  bool operator>=(const Char *s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) >= s2;
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator==(const basic_char_sequence<Char, Traits> &s1,
		  const std::basic_string<Char, Traits, Allocator> &s2)
  {
    return s1 == make_char_sequence(s2);
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator!=(const basic_char_sequence<Char, Traits> &s1,
		  const std::basic_string<Char, Traits, Allocator> &s2)
  {
    return s1 != make_char_sequence(s2);
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator<(const basic_char_sequence<Char, Traits> &s1,
		 const std::basic_string<Char, Traits, Allocator> &s2)
  {
    return s1 < make_char_sequence(s2);
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator>(const basic_char_sequence<Char, Traits> &s1,
		 const std::basic_string<Char, Traits, Allocator> &s2)
  {
    return s1 > make_char_sequence(s2);
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator<=(const basic_char_sequence<Char, Traits> &s1,
		  const std::basic_string<Char, Traits, Allocator> &s2)
  {
    return s1 <= make_char_sequence(s2);
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator>=(const basic_char_sequence<Char, Traits> &s1,
		  const std::basic_string<Char, Traits, Allocator> &s2)
  {
    return s1 >= make_char_sequence(s2);
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator==(const std::basic_string<Char, Traits, Allocator> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) == s2;
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator!=(const std::basic_string<Char, Traits, Allocator> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) != s2;
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator<(const std::basic_string<Char, Traits, Allocator> &s1,
		 const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) < s2;
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator>(const std::basic_string<Char, Traits, Allocator> &s1,
		 const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) > s2;
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator<=(const std::basic_string<Char, Traits, Allocator> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) <= s2;
  }

  template < typename Char, typename Traits, typename Allocator >
  bool operator>=(const std::basic_string<Char, Traits, Allocator> &s1,
		  const basic_char_sequence<Char, Traits> &s2)
  {
    return make_char_sequence(s1) >= s2;
  }

  template < typename Char, typename Traits >
  std::basic_ostream<Char, Traits> &
  operator<<(std::basic_ostream<Char, Traits> &out,
	     const basic_char_sequence<Char, Traits> &s)
  {
    return out.write((const char *) s.data(), s.length() * sizeof(Char));
  }

  template < typename Char >
  struct char_sequence_builder<const Char *>
  {
    typedef Char				char_type;
    typedef typename std::char_traits<Char>	traits_type;

    basic_char_sequence<char_type, traits_type>
    operator()(const Char *str) const
    {
      return basic_char_sequence<char_type, traits_type>(str, length(str));
    }
  };

  template < typename Char,
	     typename Traits,
	     typename Allocator >
  struct char_sequence_builder
  < typename std::basic_string<Char, Traits, Allocator> >
  {
    typedef Char   char_type;
    typedef Traits traits_type;

    basic_char_sequence<char_type, traits_type>
    operator()(const std::basic_string<Char, Traits, Allocator> &str) const
    {
      return basic_char_sequence<char_type, traits_type>(str);
    }
  };

}

#endif // JSON_CHAR_SEQUENCE_HPP
