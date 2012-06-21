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

#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include "json/model.h"

struct A
{
  int x;
  int y;
  std::string name;
  std::list<int> objects;
  std::map<std::string, A> dict;
};

namespace json
{

  namespace models
  {

    const model<A> A ( make_model(field("x", &A::x),
                                  field("y", &A::y),
                                  field("name", &A::name),
                                  field("objects", &A::objects),
                                  field("dict", &A::dict)) );
    
  }

  // If we have recursive structures (like A containing a map of other A objects)
  // we need to override this operator in order to make parsing possible.
  
  void operator<<(A &a, const object &obj)
  {
    models::A.load(a, obj);
  }

  void operator>>(const A &a, object &obj)
  {
    models::A.dump(a, obj);
  }

}

namespace std
{

  // We override the stream operators then anytime we read an object A from a
  // or write one to a stream se use the JSON model.

  istream &operator>>(istream &in, A &a)
  {
    return in >> json::models::A >> a;
  }

  ostream &operator<<(ostream &out, const A &a)
  {
    return out << json::models::A << a;
  }

}

int main()
{
  A a;

  std::stringstream s;
  s.str("{\"x\":42,\"y\":0,\"name\":\"Achille\",\"objects\":[1,2,3,4],\"dict\":{\"Hello\":{}}}");

  // Now this is using JSON parsing and formatting, how sexy?
  s >> a;
  std::cout << a << std::endl;

  return 0;
}
