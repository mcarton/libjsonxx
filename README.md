libjson++
=========

C++11 JSON library.

libjson++ provides the implementation of classes and functions to deal read and write JSON objects in C++.
It's written in a STL-style and is thought to integrate to the standard library.

Compilation
-----------

libjson++ uses CMake to handle compilation, you can get CMake from this URL: http://www.cmake.org/
On UNIX systems the compilation will be something like:

    $ cmake .
    $ make

On Windows systems you'll have to generate the Visual Studio project using CMake and then compile libjson++ using MSVC.

Building JSON objects
---------------------

```c++
#include "json/object.h"

int main()
{
  json::object obj1;
  json::object obj2;
  json::object obj3;
  
  // String assignment sets the type of the object internally to be a JSON string.
  obj1 = "Hello World";
  
  // Accessing the object by index automatically defines it as a JSON list.
  obj2[0] = "Hello";
  obj2[1] = "World";
  
  // Accessing the object by key automatically defines it as a JSON map.
  obj3["Hello"] = "World";
  obj3["Answer"] = 42; // Numbers are parsed and stored as strings.
  
  return 0;
}
```

Reading JSON objects
--------------------

```c++
#include <iostream>
#include "json/object.h"

int main()
{
  json::object obj;
  
  // Reads stdin until a valid JSON object has been entirely read.
  std::cin >> obj;
  
  // Using the read function to parse a sequence of characters.
  obj = json::read("{\"Hello\":\"World\"}");
  
  return 0;
}
```

Writing JSON objects
--------------------

```c++
#include <iostream>
#include "json/object.h"

int main()
{
  json::object obj;
  
  obj[0]["Hello"] = "World";
  obj[0]["Answer"] = 42;
  
  // Writing the JSON object to stdout, prints [{"Hello":"World","Answer":42}]
  std::cout << obj << std::endl;
  
  return 0;
}
```

Using a custom memory allocator
-------------------------------

Sometimes one needs to provide one's own memory allocator, libjson++ uses the same design as the STL to solve this problem, the json::object class is actually a typedef to a template class which makes it easy to customize the memory allocator to be used by the object.
```c++
#include "json/object.h"

template < typename T >
class json_allocator
{
  // ...
};

int main()
{
  typedef json::basic_object< char, std::char_traits<char>, json_allocator<char> > json_object;
  
  json_allocator a;
  json_object obj { a };
  
  obj = "Storage for this string will be created by the custom allocator";
  
  return 0;
}
```