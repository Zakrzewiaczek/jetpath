cmake_minimum_required(VERSION 3.14)
project(jetpath)

set(CMAKE_CXX_STANDARD 17)

# Pobierz Google Test automatycznie
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50f33f9142fd2b7c8b6a7a6da33e5f.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Główna aplikacja
add_executable(jetpath main.cpp)

# Testy
enable_testing()
add_executable(jetpath_tests tests/test_example.cpp)
target_link_libraries(jetpath_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(jetpath_tests)