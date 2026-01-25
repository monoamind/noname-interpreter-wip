cmake_minimum_required(VERSION 3.31)
project(pythonish-interpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(APP_INCLUDE_DIR ${APP_ROOT_DIR}/include)
set(APP_TEST_DIR ${APP_ROOT_DIR}/tests)
set(APP_CMAKE_DIR ${APP_ROOT_DIR}/cmake)

include_directories(${APP_INCLUDE_DIR})

add_subdirectory(libregexp)
add_subdirectory(tests)

add_executable(pythonish-interpreter)
target_sources(pythonish-interpreter PRIVATE app/Main.cpp)

target_link_libraries(pythonish-interpreter PRIVATE LibRegExp)