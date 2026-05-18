cmake_minimum_required(VERSION 3.10)
project(MarkovTextGenerator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(markov_gen
  src/main.cpp
  src/textgen.cpp
)

enable_testing()
find_package(GTest REQUIRED)

add_executable(run_tests
  test/tests.cpp
  src/textgen.cpp
)

target_link_libraries(run_tests
  GTest::GTest
  GTest::Main
)

add_test(NAME MarkovTests COMMAND run_tests)
