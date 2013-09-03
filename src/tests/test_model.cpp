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
#include <string>
#include <vector>
#include <json/model>
#include <unit/main>

struct A
{
  int x;
  std::vector<std::string> y;
};

namespace models
{
  const json::model<A> A { make_model(
    json::field("x", &A::x),
    json::field("y", &A::y)
  )};
}

TEST(model, read)
{
  A a;

  std::stringstream ss;
  ss << "{\"x\":42, \"y\":[\"123\", \"456\"]}";
  ss >> models::A >> a;

  assert_equal(a.x, 42);
  assert_equal(a.y.size(), 2);
  assert_true((a.y[0] == "123" && a.y[1] == "456") ||
              (a.y[0] == "456" && a.y[1] == "123"));
}

TEST(model, write)
{
  A a { 42, {"123"} };

  std::stringstream ss;
  ss << models::A << a;

  std::string s;
  ss >> s;

  assert_one_of(s,
		"{\"x\":42,\"y\":[123]}",
		"{\"y\":[123],\"x\":42}");
}

