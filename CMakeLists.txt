cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

# Enable coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# nlohmann/json setup
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

enable_testing()

add_executable(unit_tests tests/test_main.cpp ${SOURCES})
target_link_libraries(unit_tests GTest::gtest_main nlohmann_json::nlohmann_json)

add_executable(benchmarks tests/benchmark_main.cpp ${SOURCES})
target_link_libraries(benchmarks nlohmann_json::nlohmann_json)

add_executable(lab3 main.cpp ${SOURCES})
target_link_libraries(lab3 nlohmann_json::nlohmann_json)
