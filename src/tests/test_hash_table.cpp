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
#include <json/hash_table.h>
#include <json/hash_table.hpp>

namespace json
{
  template class hash_table<int>;
}

struct hash_table_fixture
{
  json::hash_table<int> table1;
  json::hash_table<int> table2;

  hash_table_fixture():
    table1(30),
    table2(30)
  {
    const int x[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::for_each(x + 0, x + 10, [&](int n) { table1.insert(n); });
    std::for_each(x + 0, x + 5, [&](int n) { table2.insert(n + 10); });
  }
};

TEST(hash_table, simple)
{
  json::hash_table<int> table (10);

  assert_equal(table.size(), 0);
  assert_equal(table.capacity(), 10);
}

TEST_FIXTURE(hash_table, check, hash_table_fixture)
{
  assert_equal(table1.size(), 10);
  assert_equal(table2.size(), 5);

  std::for_each(table1.begin(), table1.end(), [](int n) {
      assert_one_of(n, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    });

  std::for_each(table2.begin(), table2.end(), [](int n) {
      assert_one_of(n, 10, 11, 12, 13, 14);
    });
}

TEST_FIXTURE(hash_table, copy, hash_table_fixture)
{
  table1 = table2;

  assert_equal(table1.size(), table2.size());

  std::for_each(table2.begin(), table2.end(), [](int n) {
      assert_one_of(n, 10, 11, 12, 13, 14);
    });
}

TEST(hash_table, expand)
{
  json::hash_table<int> table (1);

  for (int i = 0; i != 20; ++i)
    {
      table.insert(1);
    }

  assert_equal(table.size(), 20);

  std::for_each(table.begin(), table.end(), [](int n) {
      assert_equal(n, 1);
    });
}

TEST(hash_table, insert)
{
  json::hash_table<int> table (30);

  int i;

  for (i = 0; i != 10; ++i)
    {
      table.insert(i);
    }

  assert_equal(table.size(), 10);
  i = 0;
  std::for_each(table.begin(), table.end(), [&](int n) {
      assert_equal(n, i++);
    });
}

TEST_FIXTURE(hash_table, erase, hash_table_fixture)
{
  const size_t n = table1.size();

  table1.erase(2);
  table1.erase(3);

  assert_equal(table1.size(), n - 2);

  std::for_each(table1.begin(), table1.end(), [](int n) {
      assert_not_one_of(n, 2, 3);
    });
}

TEST_FIXTURE(hash_table, clear, hash_table_fixture)
{
  table1.clear();

  assert_equal(table1.size(), 0);

  int n = 0;
  std::for_each(table1.begin(), table1.end(), [&](int) {
      ++n;
    });
  assert_equal(n, 0);
}

TEST_FIXTURE(hash_table, find, hash_table_fixture)
{
  assert_true(table1.find(0) != table1.end());
  assert_true(table1.find(1) != table1.end());
  assert_true(table1.find(2) != table1.end());
  assert_true(table1.find(3) != table1.end());
  assert_true(table1.find(4) != table1.end());
  assert_true(table1.find(5) != table1.end());
  assert_true(table1.find(6) != table1.end());
  assert_true(table1.find(7) != table1.end());
  assert_true(table1.find(8) != table1.end());
  assert_true(table1.find(9) != table1.end());

  assert_true(table2.find(10) != table2.end());
  assert_true(table2.find(11) != table2.end());
  assert_true(table2.find(12) != table2.end());
  assert_true(table2.find(13) != table2.end());
  assert_true(table2.find(14) != table2.end());
}
