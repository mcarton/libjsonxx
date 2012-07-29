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
#include <json/hash_key.h>

TEST(hash_key, equal)
{
  json::hash_key<char> key1 ("Hello");
  json::hash_key<char> key2 ("Hello");

  assert_equal(key1, key2);
}

TEST(hash_key, not_equal)
{
  json::hash_key<char> key1 ("Hello");
  json::hash_key<char> key2;

  assert_not_equal(key1, key2);
}

TEST(hash_key, copy)
{
  json::hash_key<char> key1 ("Hello");
  json::hash_key<char> key2 (key1);

  assert_equal(key1, key2);
}
