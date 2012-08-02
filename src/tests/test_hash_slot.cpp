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
#include <json/hash_slot.h>
#include <json/hash_slot.hpp>

namespace json
{
  template class hash_slot<int>;
}

TEST(hash_slot, simple)
{
  json::hash_slot<int> slot;

  assert_equal(slot.status, json::hash_slot<int>::free);
}

TEST(hash_slot, construct)
{
  json::hash_slot<int> slot (10);

  assert_equal(slot.status, json::hash_slot<int>::busy);
  assert_equal(slot.value(), 10);
}

TEST(hash_slot, assign)
{
  json::hash_slot<int> slot1 (10);
  json::hash_slot<int> slot2;

  slot2 = slot1;

  assert_equal(slot1.status, json::hash_slot<int>::busy);
  assert_equal(slot2.status, json::hash_slot<int>::busy);
  assert_equal(slot1.value(), 10);
  assert_equal(slot2.value(), 10);
}

TEST(hash_slot, copy)
{
  json::hash_slot<int> slot1 (10);
  json::hash_slot<int> slot2 (slot1);

  assert_equal(slot1.status, json::hash_slot<int>::busy);
  assert_equal(slot2.status, json::hash_slot<int>::busy);
  assert_equal(slot1.value(), 10);
  assert_equal(slot2.value(), 10);
}

TEST(hash_slot, move)
{
  json::hash_slot<int> slot1 (10);
  json::hash_slot<int> slot2 (std::move(slot1));

  assert_equal(slot1.status, json::hash_slot<int>::free);
  assert_equal(slot2.status, json::hash_slot<int>::busy);
  assert_equal(slot2.value(), 10);
}

template < typename T >
struct A
{
  json::hash_slot< A<T> > * slot;
};

TEST(hash_slot, complex)
{
  const int n = 10;
  A<int> a;

  a.slot = new json::hash_slot< A<int> > [n];

  std::for_each(a.slot, a.slot + n, [](json::hash_slot< A<int> > &s) {
      assert_equal(s.status, json::hash_slot< A<int> >::free);
    });

  delete [] a.slot;
}

TEST(hash_slot, swap)
{
  json::hash_slot<int> slot1 (10);
  json::hash_slot<int> slot2 (20);

  assert_equal(slot1.status, json::hash_slot<int>::busy);
  assert_equal(slot2.status, json::hash_slot<int>::busy);
  assert_equal(slot1.value(), 10);
  assert_equal(slot2.value(), 20);

  std::swap(slot1, slot2);

  assert_equal(slot1.status, json::hash_slot<int>::busy);
  assert_equal(slot2.status, json::hash_slot<int>::busy);
  assert_equal(slot1.value(), 20);
  assert_equal(slot2.value(), 10);
}

TEST(hash_slot, erase)
{
  json::hash_slot<int> slot (10);

  slot.erase();
  assert_equal(slot.status, json::hash_slot<int>::erased);
}

TEST(hash_slot, clear)
{
  json::hash_slot<int> slot (10);

  slot.clear();
  assert_equal(slot.status, json::hash_slot<int>::free);
}
