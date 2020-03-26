cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Snake VERSION 1.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
set(SOURCE_LIST
  "src/Main.cpp"
  "src/Snake.cpp"
  "src/Snake.hpp"
  ".gitignore"
  "CMakeLists.txt"
  "LICENSE"
  "README.md"
  )

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
