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
#include "json/char_sequence.hpp"

namespace json
{

  template class basic_char_sequence<char>;

  template bool operator==(const char_sequence &, const char_sequence &);
  template bool operator!=(const char_sequence &, const char_sequence &);
  template bool operator<(const char_sequence &, const char_sequence &);
  template bool operator>(const char_sequence &, const char_sequence &);
  template bool operator<=(const char_sequence &, const char_sequence &);
  template bool operator>=(const char_sequence &, const char_sequence &);

  template bool operator==(const char_sequence &, const char *);
  template bool operator!=(const char_sequence &, const char *);
  template bool operator<(const char_sequence &, const char *);
  template bool operator>(const char_sequence &, const char *);
  template bool operator<=(const char_sequence &, const char *);
  template bool operator>=(const char_sequence &, const char *);

  template bool operator==(const char *, const char_sequence &);
  template bool operator!=(const char *, const char_sequence &);
  template bool operator<(const char *, const char_sequence &);
  template bool operator>(const char *, const char_sequence &);
  template bool operator<=(const char *, const char_sequence &);
  template bool operator>=(const char *, const char_sequence &);

  template bool operator==(const char_sequence &, const std::string &);
  template bool operator!=(const char_sequence &, const std::string &);
  template bool operator<(const char_sequence &, const std::string &);
  template bool operator>(const char_sequence &, const std::string &);
  template bool operator<=(const char_sequence &, const std::string &);
  template bool operator>=(const char_sequence &, const std::string &);

  template bool operator==(const std::string &, const char_sequence &);
  template bool operator!=(const std::string &, const char_sequence &);
  template bool operator<(const std::string &, const char_sequence &);
  template bool operator>(const std::string &, const char_sequence &);
  template bool operator<=(const std::string &, const char_sequence &);
  template bool operator>=(const std::string &, const char_sequence &);

  template std::ostream &operator<<(std::ostream &, const char_sequence &);

}
