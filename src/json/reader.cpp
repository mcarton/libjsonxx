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
#include "json/error.h"
#include "json/reader.h"
#include "json/object.h"

namespace json
{

  void error_invalid_input_eof()
  {
    throw error("json::read_object: unexpected EOF");
  }

  void error_invalid_input_non_json()
  {
    throw error("json::read_object: invalid non-JSON input");
  }

  template void read_object(std::istream &, object &);

  object read(const char *str)
  {
    const char_sequence cs ( str, std::strlen(str) );
    return read(str);
  }

  object read(const char_sequence &str)
  {
    return read<const char_sequence, object>(str);
  }

  object read(const std::string &str)
  {
    return read<const std::string, object>(str);
  }

  object read(std::string &str)
  {
    return read<const std::string, object>(str);
  }

}
