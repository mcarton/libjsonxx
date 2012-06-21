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

namespace json
{

  namespace models
  {

    const model<A> A ( json::make_model(json::field("x", &A::x),
                                        json::field("y", &A::y)) );
    
  }

}

int main()
{
  A a;

  std::cin >> json::models::A >> a;

  std::cout << json::models::A << a << std::endl;

  return 0;
}
