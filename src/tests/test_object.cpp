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

#include <unit/main>
#include <json/object.h>

TEST(object, create)
{
  json::object obj;

  assert_equal(obj, json::null);
  assert_true(json::is_null(obj));
}

TEST(object, copy)
{
  json::object obj1;
  json::object obj2 (obj1);

  assert_equal(obj1, obj2);
}

TEST(object, assign)
{
  json::object obj1;
  json::object obj2;

  obj2 = obj1;
  assert_equal(obj1, obj2);
}

TEST(object, string)
{
  json::object obj;

  obj = "Hello World";
  assert_equal(obj, "Hello World");
  assert_true(json::is_string(obj));
}

TEST(object, list)
{
  json::object obj;

  obj[0] = "Hello";
  obj[1] = "World";
  assert_equal(obj[0], "Hello");
  assert_equal(obj[1], "World");
  assert_true(json::is_list(obj));
}

TEST(object, map)
{
  json::object obj;

  obj["Hello"] = "World";
  assert_equal(obj["Hello"], "World");
  assert_true(json::is_map(obj));
}

TEST(obj, boolean)
{
  json::object obj1 (true);
  json::object obj2 (false);

  assert_true(json::is_true(obj1));
  assert_true(json::is_false(obj2));
}

TEST(obj, integer)
{
  json::object obj (42);

  assert_true(json::is_string(obj));
  assert_equal(obj, "42");
}

TEST(obj, float)
{
  json::object obj (42.42);

  assert_true(json::is_string(obj));
  assert_almost_equal(std::stod(obj.get_string()), 42.42);
}

TEST(obj, list_iterator)
{
  json::object obj;

  obj[0] = "Hello";
  obj[1] = "World";
  assert_equal(obj.size(), 2);

  json::object::size_type n = 0;

  auto it = obj.begin();
  auto jt = obj.end();
  while (it != jt)
    {
      json::object &o = *it;
      assert_one_of(o, "Hello", "World");
      ++it;
      ++n;
    }

  assert_equal(obj.size(), n);
}

TEST(obj, map_iterator)
{
  json::object obj;

  obj["Hello"] = "World";
  obj["Answer"] = 42;
  assert_equal(obj.size(), 2);

  json::object::size_type n = 0;

  auto it = obj.begin();
  auto jt = obj.end();
  while (it != jt)
    {
      assert_one_of(it->first, "Hello", "Answer");
      assert_one_of(it->second, "World", "42");
      ++it;
      ++n;
    }

  assert_equal(obj.size(), n);
}

TEST(object, compare)
{
  json::object obj ("Hello");

  assert_equal(obj, "Hello");
  assert_equal(obj, json::char_sequence("Hello"));
  assert_equal(obj, std::string("Hello"));
}
