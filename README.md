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
  
  // So we need to explicitly parse them as numeric values if we want to manipulate them.
  double answer = std::stod(obj3["Answer"]);
  
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

Using JSON models
-----------------

JSON is a string format, C++ has strings but also has integers and floats.
While libjson++ tries to provide as mainy functions as it can for parsing JSON objects it may be annoying and error prone to handle to do it by hand in the code. Plus, we may not want to deal with JSON objects all over the codes and instead want to use more expressive classes.
libjson++ provides the 'model' concept to solve this problem, a model is an object discribing how to transform a JSON object into a C++ object and vice-versa.
```c++
#include <iostream>
#include "json/model.h"

struct A
{
  int x;
  int y;
};

namespace json
{

  namespace models
  {
    
    // This is a descriptor of how to translate from and to JSON on instances of A.
    const model<A> A { make_model(field("x", &A::x),
                                  field("y", &A::y)) };
  
  }

}

int main()
{
  A a;
  
  // Inserts the model in the input pipeline to read a JSON object and flush it into
  // the instance of A.
  std::cin >> json::models::A >> a;
  
  // Inserts the model in the output pipeline to create a JSON representation of the
  // instance of A and write it to the stream.
  std::cout << json::models::A << a << std::endl;
  
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