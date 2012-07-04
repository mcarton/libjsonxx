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

#include <ostream>
#include "json/writer.hpp"
#include "json/object.hpp"

namespace json
{

  template void write_null(std::ostream &);

  template void write_pair(std::ostream &, const object::object_map::pair &);

  template void write_map(std::ostream &, const object::object_map &);

  template void write_list(std::ostream &, const object::object_list &);

  template void write_string(std::ostream &, const object::object_string &);

  template void write_object(std::ostream &, const object &);

}
