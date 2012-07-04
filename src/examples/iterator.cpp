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

#include <iostream>
#include "json/object.h"

int main()
{
  json::object obj;

  obj[0] = "Hello";
  obj[1] = "World";

  auto it = obj.begin();
  auto jt = obj.end();
  while (it != jt)
    {
      const json::object &x = *it++;
      std::cout << x << std::endl;
    }

  obj.make_map();
  obj["Hello"] = "World";
  json::for_each_pair(obj, [](const json::char_sequence &key, const json::object &obj) {
      std::cout << key << " = " << obj << std::endl;
    });

  return 0;
}
