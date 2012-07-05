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

#ifndef JSON_CHAR_SEQUENCE_H
#define JSON_CHAR_SEQUENCE_H

#include <iosfwd>
#include <cctype>
#include <cstring>
#include "json/def.h"

namespace json
{

  template < typename Char, typename Traits, typename Allocator >
  inline std::size_t length(const std::basic_string<Char, Traits, Allocator> &s)
  {
    return s.length();
  }

  template < typename Char, int N >
  inline std::size_t length(const Char (&)[N])
  {
    return N - 1;
  }

  template < typename Char >
  inline std::size_t length(const Char *s)
  {
    std::size_t n = 0;
    while (*s)
      {
	++s;
	++n;
      }
    return n;
  }

  inline std::size_t length(const char *s)
  {
    return std::strlen(s);
  }
  
  template < typename Char, typename Traits >
  inline const Char *empty_string()
  {
    static const Char str[1] = { 0 };
    return str;
  }

  /**
   * @brief An object provided read-only operations on character strings.
   *
   * In a lot of cases, read-only string operations are required but the STL
   * would require us dynamically allocate memory for a <em>std::string</em>
   * object even if all operations are performed in the scope on constant
   * strings (for example the '[string]' operator on a map).
   * <br/>
   * This is a waste of both memory and CPU cycles.
   * To avoid this problem, libjson++ provides the <em>json::char_sequence</em>
   * type which can be constructed from c-style strings or STL strings and
   * implements read-only operations on those objects.
   * <br/>
   * <br/>
   * A common use of of <em>json::char_sequence</em> objects are to implement
   * comparisons using standard operators between objects that otherwise
   * couldn't be used this way.
   * <br/>
   * For exemple, if we wanted to compare two c-string, we'd have to use the
   * <em>std::strcmp</em> function, so when writing templates we'd need to
   * provide a specialization in case of c-string parameters. Using the
   * <em>json::char_sequence</em> class we could write something like this:
   * @code
   * template < typename T, typename U >
   * void f(const T &t, const U &u)
   * {
   *   // ...
   *
   *   // All we need to provide is a way to cast the T and U types to
   *   // json::char_sequence.
   *   const json::char_sequence s1 { t };
   *   const json::char_sequence s2 { u };
   *
   *   // Now we can treat the input parameters as strings without worrying to
   *   // provide specializations for objects that don't behave like STL strings.
   *   const bool equals { s1 == s2 };
   *
   *   // ...
   * }
   *
   * // We can now use the template function with any kind of input that could
   * // be converted to a json::char_sequence.
   * f(std::string("Hello"), std::string("World"));
   * f("Hello", "World");
   * @endcode
   *
   * @note This class is developped for internal purposes only and should not be
   * used outside of the libjson++ implementation.
   */
  template < typename Char, typename Traits = std::char_traits<Char> >
  class basic_char_sequence
  {

  public:

    typedef Char			char_type;
    typedef Traits			traits_type;
    typedef typename std::size_t	size_type;
    typedef typename std::ptrdiff_t	difference_type;

    typedef const char_type *		iterator;
    typedef const char_type *		const_iterator;

    basic_char_sequence():
      _str(empty_string<Char, Traits>()),
      _len(0)
    {
    }

    template < int N >
    basic_char_sequence(const char_type (&s)[N]):
      _str(s),
      _len(json::length(s))
    {
    }

    template < typename Allocator >
    basic_char_sequence(const std::basic_string<Char, Traits, Allocator> &s):
      _str(s.data()),
      _len(s.size())
    {
    }

    basic_char_sequence(const basic_char_sequence &s):
      _str(s._str),
      _len(s._len)
    {
    }

    basic_char_sequence(const char_type *str, const size_type len):
      _str(str),
      _len(len)
    {
    }

    basic_char_sequence &operator=(const basic_char_sequence &s)
    {
      _str = s._str;
      _len = s._len;
      return *this;
    }

    template < int N >
    basic_char_sequence &operator=(const char_type (&s)[N])
    {
      _str = s;
      _len = json::length(s);
      return *this;
    }

    template < typename Allocator >
    basic_char_sequence &operator=(const std::basic_string<Char, Traits, Allocator> &s)
    {
      _str = s.data();
      _len = s.size();
      return *this;
    }

    template < typename _Char, typename _Traits >
    bool equals(const basic_char_sequence<_Char, _Traits> &s) const
    {
      return (((void *)this) == ((void *)&s)) || ((size() == s.size()) && (compare(s) == 0));
    }

    int compare(const basic_char_sequence &s) const
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

    const_iterator begin() const
    {
      return _str;
    }

    const_iterator end() const
    {
      return _str + _len;
    }

    const char_type *data() const
    {
      return _str;
    }

    size_type size() const
    {
      return _len;
    }

    size_type length() const
    {
      return _len;
    }

  protected:
    const char_type *	_str;
    size_type		_len;

  };

  template < typename Char, typename Traits >
  inline bool operator==(const basic_char_sequence<Char, Traits> &s1,
			 const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.equals(s2);
  }

  template < typename Char, typename Traits >
  inline bool operator!=(const basic_char_sequence<Char, Traits> &s1,
			 const basic_char_sequence<Char, Traits> &s2)
  {
    return !s1.equals(s2);
  }

  template < typename Char, typename Traits >
  inline bool operator<(const basic_char_sequence<Char, Traits> &s1,
			const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) < 0;
  }

  template < typename Char, typename Traits >
  inline bool operator>(const basic_char_sequence<Char, Traits> &s1,
			const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) > 0;
  }

  template < typename Char, typename Traits >
  inline bool operator<=(const basic_char_sequence<Char, Traits> &s1,
			 const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) <= 0;
  }

  template < typename Char, typename Traits >
  inline bool operator>=(const basic_char_sequence<Char, Traits> &s1,
			 const basic_char_sequence<Char, Traits> &s2)
  {
    return s1.compare(s2) >= 0;
  }

  typedef basic_char_sequence<char> char_sequence;

  extern template class basic_char_sequence<char>;

  template < typename Char, typename Traits >
  inline
  std::basic_ostream<Char, Traits> &
  operator<<(std::basic_ostream<Char, Traits> &out,
	     const basic_char_sequence<Char, Traits> &s)
  {
    return out.write(s.data(), s.length());
  }

}

#endif // JSON_CHAR_SEQUENCE_H
