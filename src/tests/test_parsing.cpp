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
#include <json/parsing.h>
#include <json/object.h>

TEST(parsing, stoi)
{
  assert_equal(0, json::stoi(json::object(0)));
  assert_equal(1, json::stoi(json::object(1)));
  assert_equal(-1, json::stoi(json::object(-1)));
  assert_equal(10, json::stoi(json::object(10)));
  assert_equal(-10, json::stoi(json::object(-10)));
  assert_equal(42, json::stoi(json::object(4.2e1)));
  assert_equal(-42, json::stoi(json::object(-4.2e1)));
}

TEST(parsing, stol)
{
  assert_equal(0, json::stol(json::object(0)));
  assert_equal(1, json::stol(json::object(1)));
  assert_equal(-1, json::stol(json::object(-1)));
  assert_equal(10, json::stol(json::object(10)));
  assert_equal(-10, json::stol(json::object(-10)));
  assert_equal(42, json::stol(json::object(4.2e1)));
  assert_equal(-42, json::stol(json::object(-4.2e1)));
}

TEST(parsing, stoll)
{
  assert_equal(0, json::stoll(json::object(0)));
  assert_equal(1, json::stoll(json::object(1)));
  assert_equal(-1, json::stoll(json::object(-1)));
  assert_equal(10, json::stoll(json::object(10)));
  assert_equal(-10, json::stoll(json::object(-10)));
  assert_equal(42, json::stoll(json::object(4.2e1)));
  assert_equal(-42, json::stoll(json::object(-4.2e1)));
}

TEST(parsing, stof)
{
  assert_almost_equal(0, json::stof(json::object(0)));
  assert_almost_equal(10.1, json::stof(json::object(10.1)));
  assert_almost_equal(-10.1, json::stof(json::object(-10.1)));
  assert_almost_equal(1e3, json::stof(json::object(1e+3)));
  assert_almost_equal(1e-3, json::stof(json::object(1e-3)));
  assert_almost_equal(-1e3, json::stof(json::object(-1e+3)));
  assert_almost_equal(-1e-3, json::stof(json::object(-1e-3)));
}

TEST(parsing, stod)
{
  assert_almost_equal(0, json::stod(json::object(0)));
  assert_almost_equal(10.1, json::stod(json::object(10.1)));
  assert_almost_equal(-10.1, json::stod(json::object(-10.1)));
  assert_almost_equal(1e3, json::stod(json::object(1e+3)));
  assert_almost_equal(1e-3, json::stod(json::object(1e-3)));
  assert_almost_equal(-1e3, json::stod(json::object(-1e+3)));
  assert_almost_equal(-1e-3, json::stod(json::object(-1e-3)));
}

TEST(parsing, stold)
{
  assert_almost_equal(0, json::stold(json::object(0)));
  assert_almost_equal(10.1, json::stold(json::object(10.1)));
  assert_almost_equal(-10.1, json::stold(json::object(-10.1)));
  assert_almost_equal(1e3, json::stold(json::object(1e+3)));
  assert_almost_equal(1e-3, json::stold(json::object(1e-3)));
  assert_almost_equal(-1e3, json::stold(json::object(-1e+3)));
  assert_almost_equal(-1e-3, json::stold(json::object(-1e-3)));
}

TEST(parsing, is_json_true)
{
  assert_true(json::is_json_true(json::object(true)));
}

TEST(parsing, is_json_false)
{
  assert_true(json::is_json_false(json::object(false)));
}
