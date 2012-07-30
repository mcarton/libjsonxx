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
#include <unit/main>
#include <json/object.h>

static json::object from_string(const char *str)
{
  std::stringstream s;
  json::object obj;
  s.unsetf(std::ios::skipws);
  s << str;
  s >> obj;
  return obj;
}

TEST(read, null)
{
  assert_equal(from_string("null"), json::null);
}

TEST(read, string)
{
  assert_equal(from_string("\"Hello World\""), "Hello World");
}

TEST(read, boolean)
{
  assert_true(json::is_true(from_string("true")));
  assert_true(json::is_false(from_string("false")));
}

TEST(read, number)
{
  assert_equal(from_string("42"), "42");
}

TEST(read, list)
{
  json::object obj (from_string("[1, 2, 3]"));

  assert_equal(obj.size(), 3);
  assert_equal(obj[0], "1");
  assert_equal(obj[1], "2");
  assert_equal(obj[2], "3");
}

TEST(read, map)
{
  json::object obj (from_string("{\"Hello\": \"World\", \"Answer\": 42}"));

  assert_equal(obj.size(), 2);
  assert_equal(obj["Hello"], "World");
  assert_equal(obj["Answer"], "42");
}
