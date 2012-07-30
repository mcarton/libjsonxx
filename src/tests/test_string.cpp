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
#include <json/string.h>

TEST(string, itos)
{
  auto f = &json::itos< int, char, std::char_traits<char>, std::allocator<char> >;
  std::allocator<char> a;
  assert_return("0", f, 0, a);
  assert_return("1", f, 1, a);
  assert_return("-1", f, -1, a);
  assert_return("42", f, 42, a);
  assert_return("-42", f, -42, a);
}

TEST(string, utos)
{
  auto f = &json::utos< unsigned int, char, std::char_traits<char>, std::allocator<char> >;
  std::allocator<char> a;
  assert_return("0", f, 0, a);
  assert_return("1", f, 1, a);
  assert_return("42", f, 42, a);
}

TEST(string, ftos)
{
  auto f = [](double x) {
    return std::stod(json::ftos< double, char, std::char_traits<char>, std::allocator<char> >(x, std::allocator<char>()));
  };
  assert_almost_equal(f(0), 0);
  assert_almost_equal(f(1), 1);
  assert_almost_equal(f(-1.9), -1.9);
  assert_almost_equal(f(42.55), 42.55);
  assert_almost_equal(f(-42.55), -42.55);
}

TEST(string, to_string)
{
  auto fi = [](int i) {
    return std::stoi(json::to_string< char, std::char_traits<char>, std::allocator<char> >(i));
  };
  auto fd = [](double d) {
    return std::stod(json::to_string< char, std::char_traits<char>, std::allocator<char> >(d));
  };
  assert_return(42, fi, 42);
  assert_return(-42, fi, -42);
  assert_almost_equal(1e-3, fd(1e-3));
  assert_almost_equal(-1e-3, fd(-1e-3));
}
