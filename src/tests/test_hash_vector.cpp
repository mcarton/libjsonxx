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

#include <algorithm>
#include <unit/main>
#include <json/hash_vector.h>
#include <json/hash_vector.hpp>

namespace json
{
  template class hash_vector<int>;
}

struct hash_vector_fixture
{
  json::hash_vector<int> vector1;
  json::hash_vector<int> vector2;

  hash_vector_fixture():
    vector1(10),
    vector2(10)
  {
    int i = 0;
    std::for_each(vector1.begin(), vector1.end(), [&](json::hash_slot<int> &s) {
	s = ++i;
      });
    std::for_each(vector2.begin(), vector2.end(), [&](json::hash_slot<int> &s) {
	s = --i;
      });
  }
};

TEST(hash_vector, simple)
{
  json::hash_vector<int> v;

  assert_equal(v.size(), 0);
}

TEST_FIXTURE(hash_vector, check, hash_vector_fixture)
{
  int i = 0;
  std::for_each(vector1.begin(), vector1.end(), [&](json::hash_slot<int> &s) {
      assert_equal(s.status, json::hash_slot<int>::busy);
      assert_equal(s.value(), ++i);
    });
  std::for_each(vector2.begin(), vector2.end(), [&](json::hash_slot<int> &s) {
      assert_equal(s.status, json::hash_slot<int>::busy);
      assert_equal(s.value(), --i);
    });
}

TEST_FIXTURE(hash_vector, move, hash_vector_fixture)
{
  int i = vector2.size();

  vector1 = std::move(vector2);

  std::for_each(vector1.begin(), vector1.end(), [&](json::hash_slot<int> &s) {
      assert_equal(s.status, json::hash_slot<int>::busy);
      assert_equal(s.value(), --i);
    });
}

TEST_FIXTURE(hash_vector, copy, hash_vector_fixture)
{
  int i = vector2.size();

  vector1 = vector2;

  std::for_each(vector1.begin(), vector1.end(), [&](json::hash_slot<int> &s) {
      assert_equal(s.status, json::hash_slot<int>::busy);
      assert_equal(s.value(), --i);
    });
}

TEST_FIXTURE(hash_vector, swap, hash_vector_fixture)
{
  std::swap(vector1, vector2);

  int i = 0;
  std::for_each(vector2.begin(), vector2.end(), [&](json::hash_slot<int> &s) {
      assert_equal(s.status, json::hash_slot<int>::busy);
      assert_equal(s.value(), ++i);
    });
  std::for_each(vector1.begin(), vector1.end(), [&](json::hash_slot<int> &s) {
      assert_equal(s.status, json::hash_slot<int>::busy);
      assert_equal(s.value(), --i);
    });
}
