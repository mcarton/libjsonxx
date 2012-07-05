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

#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "json/def.h"

namespace json
{

  int stoi(const object &obj);

  long stol(const object &obj);

  long long stoll(const object &obj);

  float stof(const object &obj);

  double stod(const object &obj);

  long double stold(const object &obj);

  bool is_json_true(const object &obj);

  bool is_json_false(const object &obj);

}

#endif // JSON_PARSING_H
