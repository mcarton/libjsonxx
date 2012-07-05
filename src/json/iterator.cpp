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

#include "json/error.h"
#include "json/object.hpp"

namespace json
{

  void error_null_iterator_cannot_be_incremented()
  {
    throw error("json::iterator::operator++: null iterator cannot be incremented");
  }

  template class iterator<object,
			  object::object_list::iterator,
			  object::object_map::iterator>;

  template class iterator<const object,
			  object::object_list::const_iterator,
			  object::object_map::const_iterator>;

}
