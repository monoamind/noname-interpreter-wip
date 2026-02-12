cmake_minimum_required(VERSION 3.31)

project(LibInterpret
    VERSION 0.1
    DESCRIPTION "Interpreter core"
    LANGUAGES CXX
)

set(SOURCES
    Environment.cpp
    Interpreter.cpp
    Object.cpp
    Resolver.cpp
    Parser.cpp
    ExpressionParser.cpp
    StatementParser.cpp
    Scanner.cpp
    Token.cpp
)

add_library(LibInterpret)

target_include_directories(LibInterpret PUBLIC ${APP_INCLUDE_DIR}/interpret)

target_sources(LibInterpret PRIVATE ${SOURCES})

target_link_libraries(LibInterpret PUBLIC LibRegExp)