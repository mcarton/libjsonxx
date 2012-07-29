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

#include <cstring>
#include <sstream>
#include <iterator>
#include <unit/main>
#include <json/char_sequence.h>

TEST(char_sequence, compare)
{
  json::char_sequence s1 ("A");
  json::char_sequence s2 ("B");

  assert_lesser(s1.compare(s2), 0);
  assert_greater(s2.compare(s1), 0);
  assert_equal(s1.compare(s1), 0);
  assert_equal(s2.compare(s2), 0);
}

TEST(char_sequence, equals)
{
  json::char_sequence s1 ("Hello");
  json::char_sequence s2 ("Hello");

  assert_true(s1.equals(s2));
}

TEST(char_sequence, iterator)
{
  char str[] = "Hello World";
  json::char_sequence s (str);

  int i = 0;
  auto it = std::begin(s);
  auto jt = std::end(s);
  while ((str[i]) && (it != jt))
    {
      assert_equal(str[i], *it);
      ++it;
      ++i;
    }
  assert_equal(it, jt);
}

TEST(char_sequence, data)
{
  char str[] = "Hello World";
  json::char_sequence s (str);

  int i = 0;
  while (str[i])
    {
      assert_equal(str[i], s.data()[i]);
      ++i;
    }
  assert_equal(str[i], s.data()[i]);
}

TEST(char_sequence, size)
{
  char str[] = "Hello World";
  json::char_sequence s (str);
  std::size_t len = std::strlen(str);

  assert_equal(len, s.size());
  assert_equal(len, s.length());
}

TEST(char_sequence, length)
{
  char str[] = "Hello World";
  json::char_sequence s (str);
  std::size_t len = std::strlen(str);

  assert_equal(len, json::length(str));
  assert_equal(len, json::length(s));
}

TEST(char_sequence, ostream)
{
  unit::random_string str1;
  std::string str2;
  std::stringstream os;
  json::char_sequence s (str1);

  os << s;
  str2 = os.str();

  assert_equal(str1, str2);
}

