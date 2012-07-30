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
