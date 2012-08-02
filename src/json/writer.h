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

#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <iosfwd>
#include "json/def.h"

namespace json
{

  template < typename Char, typename Traits, typename Allocator >
  void write_object(std::basic_ostream<Char, Traits> &out,
		    const basic_object<Char, Traits, Allocator> &obj);

  extern template void write_object(std::ostream &, const object &);

}

#endif // JSON_WRITER_H
