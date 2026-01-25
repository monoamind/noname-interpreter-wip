cmake_minimum_required(VERSION 3.31)

project(LibRegExp
    VERSION 0.1
    DESCRIPTION "RegExp engine library"
    LANGUAGES CXX
)

set(SOURCES
    NFA.cpp
    Scanner.cpp
)

add_library(LibRegExp)

target_include_directories(LibRegExp PUBLIC ${APP_INCLUDE_DIR}/regexp)

target_sources(LibRegExp PRIVATE ${SOURCES})