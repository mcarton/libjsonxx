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

#include <sstream>
#include "json/error.h"
#include "json/object.h"
#include "json/object.hpp"

namespace json
{

  template class basic_object<char>;

  const object null;

  void error_json_object_invalid_type(const void *const at,
				      const object_type expected,
				      const object_type found,
				      const char *function)
  {
    std::ostringstream s;
    s << function;
    s << ": expecting ";
    s << expected;
    s << " but ";
    s << found;
    s << " was found (at ";
    s << at;
    s << ")";
    throw error(s.str());
  }

  template bool operator==(const object &, const object &);
  template bool operator!=(const object &, const object &);
  template bool operator==(const object &, const char_sequence &);
  template bool operator!=(const object &, const char_sequence &);
  template bool operator==(const char_sequence &, const object &);
  template bool operator!=(const char_sequence &, const object &);
  template bool operator==(const object &, const char *);
  template bool operator!=(const object &, const char *);
  template bool operator==(const char *, const object &);
  template bool operator!=(const char *, const object &);
  template bool operator==(const object &, const std::string &);
  template bool operator!=(const object &, const std::string &);
  template bool operator==(const std::string &, const object &);
  template bool operator!=(const std::string &, const object &);

}
