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
#include <json/hash_map.h>
#include <json/hash_map.hpp>

typedef json::hash_map<std::string>	hash_map;
typedef hash_map::key_type		key;

TEST(hash_map, empty)
{
  hash_map map;

  assert_true(map.empty());
  assert_equal(map.size(), 0);
  assert_equal(map.find(key()), map.end());
}

TEST(hash_map, emplace)
{
  hash_map map;

  map.emplace(key("Hello"), "World");

  assert_not_equal(map.find(key("Hello")), map.end());
  assert_equal(map.find(key("Hello"))->second, "World");
  assert_equal(map.size(), 1);
  assert_false(map.empty());
}

TEST(hash_map, bracket)
{
  hash_map map;

  map[key("Hello")] = "World";

  assert_not_equal(map.find(key("Hello")), map.end());
  assert_equal(map[key("Hello")], "World");
  assert_equal(map.size(), 1);
  assert_false(map.empty());
}

TEST(hash_map, erase)
{
  hash_map map;

  map[key("Hello")] = "World";
  map.erase(map.find(key("Hello")));

  assert_true(map.empty());
  assert_equal(map.size(), 0);
  assert_equal(map.find(key("Hello")), map.end());
}

TEST(hash_map, clear)
{
  hash_map map;

  map[key("A")] = "a";
  map[key("B")] = "b";
  map[key("C")] = "c";

  assert_equal(map.size(), 3);
  assert_false(map.empty());

  map.clear();

  assert_equal(map.size(), 0);
  assert_true(map.empty());
}

TEST(hash_map, iterator)
{
  hash_map map;

  map[key("A")] = "a";
  map[key("B")] = "b";
  map[key("C")] = "c";

  auto it = map.begin();
  auto jt = map.end();

  hash_map::size_type n = 0;

  while (it != jt)
    {
      assert_one_of(it->first, key("A"), key("B"), key("C"));
      ++it;
      ++n;
    }
  assert_equal(n, map.size());
}
