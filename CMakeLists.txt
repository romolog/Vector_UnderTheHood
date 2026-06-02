cmake_minimum_required(VERSION 3.25)

project(myvec VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "Project: ${CMAKE_PROJECT_NAME}")
message(STATUS "Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Set build type to: ${default_build_type}")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Set build type" FORCE)
endif()

enable_testing()
find_package(GTest QUIET)
if(GTest_FOUND)
	message(STATUS "${CMAKE_PROJECT_NAME}: Using system GTest")
else()
	message(STATUS "${CMAKE_PROJECT_NAME}: GTest not found via find_package: fetching from GitHub")
	include(FetchContent)
	FetchContent_Declare(
		gtest_pack
		GIT_REPOSITORY	https://github.com/google/googletest.git
		GIT_TAG			v1.14.0
		GIT_SHALLOW		TRUE
	)	
	FetchContent_MakeAvailable(gtest_pack)
	if(NOT TARGET GTest::GTest)
		add_library(GTest::GTest ALIAS gtest)
	endif()
	if(NOT TARGET GTest::Main)
		add_library(GTest::Main ALIAS gtest_main)
	endif()
endif()

add_executable(myvec_tests MyVector.hpp MyVector_test.cpp)

target_include_directories(myvec_tests PUBLIC ${CMAKE_SOURCE_DIR})

target_compile_options(myvec_tests PRIVATE
	$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wdeprecated>
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_link_libraries(myvec_tests PRIVATE
    GTest::GTest
    GTest::Main
)

include(GoogleTest)
gtest_discover_tests(myvec_tests)