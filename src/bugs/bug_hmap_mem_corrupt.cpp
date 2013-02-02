#include <iostream>
#include <json/object>

int main () {
  using namespace std;

  json::object a;
  a["x"] = 1;

  json::object b { std::move (a) };
  cout
    << "Move of the hash map is ok,\n"
    << a << endl  // expected to be null
    << b << endl; // expected to be {"x": 1}

  json::object c { b };
  cout
    << "but copy of the hash map does not work when destroying the objects...\n"
    << b << endl  // expected to be {"x": 1}
    << c << endl; // expected to be {"x": 1}

  return 0;
}
