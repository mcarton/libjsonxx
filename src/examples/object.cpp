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
  json::object obj1;
  json::object obj2;
  json::object obj3;
  json::object obj4;
  json::object obj5;
  json::object obj6;

  obj2 = "Hello World\n";
  obj3 = "-42.6E+10";
  obj4 = obj2;

  obj5[0] = 42;
  obj5[2] = obj3;

  obj6["Hello"] = "Achille";
  obj6["World"] = std::move(obj5);

  std::cout << obj1.type() << ' ' << obj1 << std::endl;
  std::cout << obj2.type() << ' ' << obj2 << std::endl;
  std::cout << obj3.type() << ' ' << obj3 << std::endl;
  std::cout << std::stod(obj3) << std::endl;
  std::cout << (obj1 != obj2) << std::endl;
  std::cout << (obj1 == obj1) << std::endl;
  std::cout << (obj2 == obj4) << std::endl;
  std::cout << obj5 << std::endl;
  std::cout << obj6 << std::endl;

  std::string s = obj2;
  std::cout << s << std::endl;
  return 0;
}
