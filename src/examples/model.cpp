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
#include "json/model.h"

struct A
{
  int x;
  int y;
};

struct A_json_model : public json::model<A>
{

  A_json_model():
    json::model<A>
    (
     json::field("x", &A::x),
     json::field("y", &A::y)
     )
  {
  }

};

int main()
{
  A_json_model model;
  A a;

  std::cin >> model >> a;

  std::cout << model << a << std::endl;

  return 0;
}
