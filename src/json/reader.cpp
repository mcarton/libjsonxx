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

#include <string>
#include <stdexcept>
#include "json/reader.h"
#include "json/object.h"

namespace json
{

  void error_invalid_input_eof()
  {
    throw std::runtime_error("json::read_object: unexpected EOF");
  }

  void error_invalid_input_non_json()
  {
    throw std::runtime_error("json::read_object: invalid non-JSON input");
  }

  void read_object(std::istream &in, object &obj)
  {
    typedef std::char_traits<char> char_traits;
    typedef std::allocator<char> char_allocator;
    std::istream_iterator<char> beg ( std::begin(in) );
    std::istream_iterator<char> end ( std::end(in) );
    read_object<std::istream_iterator<char>, char, char_traits, char_allocator>(beg, end, obj);
  }

  object read(const char *str)
  {
    return read<object>(str);
  }

  object read(const char_sequence &str)
  {
    return read<const char_sequence, object>(str);
  }

  object read(const std::string &str)
  {
    std::cout << "yay!" << std::endl;
    return read<const std::string, object>(str);
  }

  object read(std::string &str)
  {
    std::cout << "yay!" << std::endl;
    return read<const std::string, object>(str);
  }

}
