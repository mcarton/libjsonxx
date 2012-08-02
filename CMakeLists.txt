#
# Copyright 2012 Achille Roussel.
#
# This file is part of Libjson++.
#
# Libjson++ is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Libjson++ is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Libjson++.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)
project(JSON)

# ==============================================================================
# Set general variables
# ==============================================================================

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")
set(JSON_SOURCES_DIR "${PROJECT_SOURCE_DIR}/json")
set(JSON_EXAMPLES_DIR "${PROJECT_SOURCE_DIR}/examples")
set(JSON_TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# ==============================================================================
# Sources files
# ==============================================================================

list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/char_sequence.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/char_sequence.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/char_sequence.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/def.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/error.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/error.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/for_each.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_slot.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_slot.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_vector_iterator.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_vector_iterator.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_vector.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_vector.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_table_iterator.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_table_iterator.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_table.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_table.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_key.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_key.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_key.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_map.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_map.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_slot.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_slot.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/iterator.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/iterator.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/iterator.h)
#list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/model.cpp)
#list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/model.h)
#list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/model)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/parsing.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/parsing.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/reader.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/reader.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/reader.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/string.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/types.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/writer.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/writer.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/writer.h)

# ==============================================================================
# Detect compiler
# ==============================================================================

if(${CMAKE_COMPILER_IS_GNUCXX})
  message("Compiler: GCC")
  set(COMPILER JSON_GCC)
  set(FLAGS "-std=c++0x -W -Wall -Wextra -Wno-uninitialized")
  set(FLAGS_RELEASE "${FLAGS} -DNDEBUG -O3")
  set(FLAGS_DEBUG "${FLAGS} -g3")

elseif(${MSVC})
  message("Compiler: Microsoft Visual C++")
  set(FLAGS "/EHsc")
  set(FLAGS_RELEASE ${FLAGS})
  set(FLAGS_DEBUG ${FLAGS})
  set(COMPILER JSON_MSCV)

else()
  message("Compiler: Clang (assumed)")
  set(COMPILER JSON_CLANG)
  set(FLAGS "-std=c++11 -stdlib=libc++ -W -Wall -Wextra -fPIC")
  set(FLAGS_RELEASE "${FLAGS} -DNDEBUG -O3")
  set(FLAGS_DEBUG "${FLAGS} -g3")

endif()

# ==============================================================================
# Set CMake compiler flags
# ==============================================================================

set(CMAKE_CXX_FLAGS_RELEASE ${FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_DEBUG ${FLAGS_DEBUG})
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})

# ==============================================================================
# Define config / includes / linkages
# ==============================================================================

configure_file(${JSON_SOURCES_DIR}/def.h.in ${JSON_SOURCES_DIR}/def.h)
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_BINARY_DIR})

# ==============================================================================
# Add targets
# ==============================================================================

add_library(json++ SHARED STATIC ${JSON_SOURCES})
file(GLOB JSON_INSTALL_HEADERS "${JSON_SOURCES_DIR}/*.h" "${JSON_SOURCES_DIR}/*.hpp")
list(APPEND JSON_INSTALL_HEADERS ${JSON_SOURCES_DIR}/object)
list(APPEND JSON_INSTALL_HEADERS ${JSON_SOURCES_DIR}/model)
install(FILES ${JSON_INSTALL_HEADERS} DESTINATION include/json)
install(TARGETS json++ ARCHIVE DESTINATION lib)

#add_executable(bin/example-object ${JSON_EXAMPLES_DIR}/object.cpp)
#target_link_libraries(bin/example-object json++)

#add_executable(bin/example-reader ${JSON_EXAMPLES_DIR}/reader.cpp)
#target_link_libraries(bin/example-reader json++)

#add_executable(bin/example-iterator ${JSON_EXAMPLES_DIR}/iterator.cpp)
#target_link_libraries(bin/example-iterator json++)

#add_executable(bin/example-test ${JSON_EXAMPLES_DIR}/test.cpp)
#target_link_libraries(bin/example-test json++)

#add_executable(bin/example-model ${JSON_EXAMPLES_DIR}/model.cpp)
#target_link_libraries(bin/example-model json++)

# ==============================================================================
# Add tests
# ==============================================================================

if(DEFINED COMPILE_TESTS)
else()
  set(COMPILE_TESTS 0)
endif()

if(${COMPILE_TESTS})
  message("-- Testing enabled")
  enable_testing()

  add_executable(bin/test-string ${JSON_TESTS_DIR}/test_string.cpp)
  target_link_libraries(bin/test-string json++ unit)

  add_executable(bin/test-char-sequence ${JSON_TESTS_DIR}/test_char_sequence.cpp)
  target_link_libraries(bin/test-char-sequence json++ unit)

  add_executable(bin/test-hash-slot ${JSON_TESTS_DIR}/test_hash_slot.cpp)
  target_link_libraries(bin/test-hash-slot json++ unit)

  add_executable(bin/test-hash-vector ${JSON_TESTS_DIR}/test_hash_vector.cpp)
  target_link_libraries(bin/test-hash-vector json++ unit)

  add_executable(bin/test-hash-table ${JSON_TESTS_DIR}/test_hash_table.cpp)
  target_link_libraries(bin/test-hash-table json++ unit)

  add_executable(bin/test-hash-key ${JSON_TESTS_DIR}/test_hash_key.cpp)
  target_link_libraries(bin/test-hash-key json++ unit)

  add_executable(bin/test-hash-map ${JSON_TESTS_DIR}/test_hash_map.cpp)
  target_link_libraries(bin/test-hash-map json++ unit)

  add_executable(bin/test-object ${JSON_TESTS_DIR}/test_object.cpp)
  target_link_libraries(bin/test-object json++ unit)

  add_executable(bin/test-parsing ${JSON_TESTS_DIR}/test_parsing.cpp)
  target_link_libraries(bin/test-parsing json++ unit)

  add_executable(bin/test-read ${JSON_TESTS_DIR}/test_read.cpp)
  target_link_libraries(bin/test-read json++ unit)

  add_executable(bin/test-write ${JSON_TESTS_DIR}/test_write.cpp)
  target_link_libraries(bin/test-write json++ unit)

  add_test(json-string bin/test-string)
  add_test(json-char-sequence bin/test-char-sequence)
  add_test(json-hash-slot bin/test-hash-slot)
  add_test(json-hash-vector bin/test-hash-vector)
  add_test(json-hash-table bin/test-hash-table)
  add_test(json-hash-key bin/test-hash-key)
  add_test(json-hash-map bin/test-hash-map)
  add_test(json-object bin/test-object)
  add_test(json-parsing bin/test-parsing)
  add_test(json-read bin/test-parsing)
  add_test(json-read bin/test-read)
  add_test(json-write bin/test-write)
endif()
