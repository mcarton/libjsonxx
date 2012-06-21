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
#include <stdexcept>
#include "json/hash_map.h"

namespace json
{

  void error_hash_map_key_not_found(const void *at,
				    const void *key,
				    const std::size_t len)
  {
    std::ostringstream s;
    s << "json::hash_map::operator[] const: '";
    s.write((const char *) key, len);
    s << "' key not found (at ";
    s << at;
    s << ")";
    throw std::runtime_error(s.str());
  }

}
