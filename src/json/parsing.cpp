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
#include "json/object.hpp"

namespace json
{

  void error_parsing_non_empty_end(const char *function)
  {
    std::ostringstream s;
    s << function;
    s << ": invalid extra data at end of parsed input";
    throw error(s.str());
  }

  void error_parsing_empty_number(const char *function)
  {
    std::ostringstream s;
    s << function;
    s << ": trying to parse number from empty input";
    throw error(s.str());
  }

  void error_parsing_non_digit(const char *function)
  {
    std::ostringstream s;
    s << function;
    s << ": trying to parse number from non-numeric input";
    throw error(s.str());
  }

  int stoi(const object &obj)
  {
    return stoi(obj.get_string());
  }

  long stol(const object &obj)
  {
    return stol(obj.get_string());
  }

  long long stoll(const object &obj)
  {
    return stoll(obj.get_string());
  }

  float stof(const object &obj)
  {
    return stof(obj.get_string());
  }

  double stod(const object &obj)
  {
    return stod(obj.get_string());
  }

  long double stold(const object &obj)
  {
    return stold(obj.get_string());
  }

  bool is_json_true(const object &obj)
  {
    return is_json_true(obj.get_string());
  }

  bool is_json_false(const object &obj)
  {
    return is_json_false(obj.get_string());
  }

}
