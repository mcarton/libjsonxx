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

#include <string>
#include <sstream>
#include <unit/main>
#include <json/object.h>

static std::string to_string(const json::object &obj)
{
  std::stringstream s;
  s << obj;
  return s.str();
}

TEST(write, null)
{
  assert_equal(to_string(json::null), "null");
}

TEST(write, boolean)
{
  assert_equal(to_string("true"), "true");
  assert_equal(to_string("false"), "false");
}

TEST(write, number)
{
  assert_equal(to_string("42"), "42");
  assert_equal(to_string("-42"), "-42");
}

TEST(write, string)
{
  assert_equal(to_string("Hello World"), "\"Hello World\"");
}

TEST(write, list)
{
  json::object obj;

  obj[0] = 1;
  obj[1] = 2;
  obj[2] = 3;
  assert_equal(to_string(obj), "[1,2,3]");
}

TEST(write, map)
{
  json::object obj;

  obj["Hello"] = "World";
  obj["Answer"] = 42;

  assert_one_of(to_string(obj),
		"{\"Hello\":\"World\",\"Answer\":42}",
		"{\"Answer\":42,\"Hello\":\"World\"}");
}
