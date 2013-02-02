#include <sstream>
#include <stdexcept>
#include "json/vector.hpp"

namespace json
{

  void error_index_out_of_bounds(size_t index, size_t size)
  {
    std::ostringstream buffer;
    buffer << "index out of bounds [" << index << " / " << size << "]";
    throw std::out_of_range(buffer.str());
  }

}
