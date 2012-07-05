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
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/char_sequence.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/def.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/error.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/error.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/for_each.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_key.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_key.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_map.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_map.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/hash_map.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/iterator.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/iterator.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/iterator.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/model.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/model.h)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object.cpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object.hpp)
list(APPEND JSON_SOURCES ${JSON_SOURCES_DIR}/object.h)
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
  message("Compiler: Unknown")
  set(COMPILER JSON_Unkown)

endif()

# ==============================================================================
# Set CMake compiler flags
# ==============================================================================

set(CMAKE_CXX_FLAGS_RELEASE ${FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_DEBUG ${FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})

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

add_executable(example-object ${JSON_EXAMPLES_DIR}/object.cpp)
target_link_libraries(example-object json++)

add_executable(example-reader ${JSON_EXAMPLES_DIR}/reader.cpp)
target_link_libraries(example-reader json++)

add_executable(example-iterator ${JSON_EXAMPLES_DIR}/iterator.cpp)
target_link_libraries(example-iterator json++)

add_executable(example-test ${JSON_EXAMPLES_DIR}/test.cpp)
target_link_libraries(example-test json++)

add_executable(example-model ${JSON_EXAMPLES_DIR}/model.cpp)
target_link_libraries(example-model json++)
