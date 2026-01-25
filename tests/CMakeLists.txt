cmake_minimum_required(VERSION 3.31)

project(pythonish-interpreter-test)

# ----------------------------------------------------------------------------------------------------------------------
# GOOGLE TEST INSTALLATION
# ----------------------------------------------------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(
    GoogleTest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(GoogleTest)

# ----------------------------------------------------------------------------------------------------------------------
# ENABLE GTEST
# ----------------------------------------------------------------------------------------------------------------------

enable_testing()

include(GoogleTest)

# ----------------------------------------------------------------------------------------------------------------------
# ALL TESTS
# ----------------------------------------------------------------------------------------------------------------------

file(GLOB REGEXP_TESTS ${APP_TEST_DIR}/regexp/*.cc)

add_executable(RegExpTests ${REGEXP_TESTS})

target_link_libraries(RegExpTests PRIVATE
    LibRegExp
    GTest::gtest_main
    GTest::gmock_main
)