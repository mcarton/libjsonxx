#include <unit/main>
#include <json/vector.h>
#include <json/vector.hpp>

namespace json
{

  template class vector<int>;

}

TEST(vector, 01_create)
{
  json::vector<int> v;

  assert_true(v.empty());
  assert_equal(v.begin(), v.end());
  assert_equal(v.size(), 0);
}

TEST(vector, 02_push_back_one)
{
  json::vector<int> v;

  v.push_back(1);

  assert_false(v.empty());
  assert_not_equal(v.begin(), v.end());
  assert_equal(v.size(), 1);
  assert_equal(v.front(), 1);
  assert_equal(v.back(), 1);

  assert_equal(v[0], 1);

  auto it = v.begin();
  auto jt = v.end();

  assert_equal(*it++, 1);
  assert_equal(it, jt);
}

TEST(vector, 03_push_back_many)
{
  json::vector<int> v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  assert_false(v.empty());
  assert_not_equal(v.begin(), v.end());
  assert_equal(v.size(), 3);
  assert_equal(v.back(), 3);

  assert_equal(v[0], 1);
  assert_equal(v[1], 2);
  assert_equal(v[2], 3);

  auto it = v.begin();
  auto jt = v.end();

  assert_equal(*it++, 1);
  assert_equal(*it++, 2);
  assert_equal(*it++, 3);
  assert_equal(it, jt);
}

TEST(vector, 04_push_pop_one)
{
  json::vector<int> v;

  v.push_back(1);
  v.pop_back();

  assert_true(v.empty());
  assert_equal(v.size(), 0);
  assert_equal(v.begin(), v.end());
}

TEST(vector, 05_push_pop_many)
{
  json::vector<int> v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.pop_back();

  assert_false(v.empty());
  assert_equal(v.size(), 2);
}

TEST(vector, 06_push_erase_one)
{
  json::vector<int> v;

  v.push_back(1);

  assert_false(v.empty());
  assert_equal(v.size(), 1);
  assert_equal(v[0], 1);

  v.erase(v.begin());

  assert_true(v.empty());
  assert_equal(v.size(), 0);
  assert_equal(v.begin(), v.end());
}


TEST(vector, 07_push_erase_many)
{
  json::vector<int> v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);

  assert_equal(v.size(), 5);
  assert_equal(v[0], 1);

  v.erase(v.begin());

  assert_equal(v.size(), 4);
  assert_equal(v[0], 2);

  v.erase(v.begin() + 2);
  v.erase(v.begin() + 1);

  auto it = v.begin();
  auto jt = v.end();

  assert_equal(v.size(), 2);
  assert_equal(*it++, 2);
  assert_equal(*it++, 5);
  assert_equal(it, jt);
}
