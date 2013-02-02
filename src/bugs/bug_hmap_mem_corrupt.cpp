#include <iostream>
#include <json/object>

int main()
{
  json::object a;
  a["x"] = 1;

  json::object b { a };
  std::cout
    << "Move of the hash map is ok,\n"
    << a << std::endl  // expected to be null
    << b << std::endl; // expected to be {"x": 1}

  {
    json::object c { a };
    std::cout
      << "but copy of the hash map does not work when destroying the objects...\n"
      << b << std::endl  // expected to be {"x": 1}
      << c << std::endl; // expected to be {"x": 1}

    std::cout << "new destroying c" << std::endl;
  }

  std::cout << "now destryoing b" << std::endl;
  return 0;
}
