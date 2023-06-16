cmake_minimum_required(VERSION 3.16)

# TODO(jhi): How can we change folder where .o files will be created?

# https://cmake.org/cmake/help/latest/policy/CMP0135.html needed to avoid warn
# when fetching googletest content
cmake_policy(SET CMP0135 NEW)

project(Posgi VERSION 0.1)

# add Git hash to config file
find_package(Git REQUIRED)
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(PosgiConfig.h.in PosgiConfig.h)

# enable for debugging of CMake: set(CMAKE_VERBOSE_MAKEFILE ON)

# Set C++17 as the default language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# cifuzz tests
find_package(cifuzz NO_SYSTEM_ENVIRONMENT_PATH)
enable_fuzz_testing()

# ClangFormat
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/cmake)
include(ClangFormat)

# core library
add_library(
  posgi STATIC
  framework/impl/utils/file_utils.cc
  framework/impl/utils/system_utils.cc
  framework/impl/bundle_context_impl.cc
  framework/impl/bundle_impl.cc
  framework/impl/framework_impl.cc
  framework/impl/manifest_parser.cc
  framework/framework_factory.cc
  framework/posgi.cc
  framework/posgi.h
  framework/include/org/osgi/framework/bundle_activator.h
  framework/include/org/osgi/framework/bundle_context.h
  framework/include/org/osgi/framework/bundle_revision.h
  framework/include/org/osgi/framework/bundle.h
  framework/include/org/osgi/framework/constants.h
  framework/include/org/osgi/framework/launch/framework_factory.h
  framework/include/org/osgi/framework/launch/framework.h
  framework/include/org/osgi/framework/bundle_activator.h
  framework/include/org/posgi/framework/impl/utils/file_utils.h
  framework/include/org/posgi/framework/impl/utils/system_utils.h
  framework/include/org/posgi/framework/impl/bundle_context_impl.h
  framework/include/org/posgi/framework/impl/bundle_impl.h
  framework/include/org/posgi/framework/impl/framework_impl.h
  framework/include/org/posgi/framework/impl/bundle_context_impl.h
  framework/include/org/posgi/framework/impl/manifest_parser.h
  framework/include/org/posgi/framework/impl/utils/printable_mutex.h
  bundles/console/include/org/osgi/service/console/command_processor.h
  bundles/console/include/org/osgi/service/console/command_session.h
  bundles/console/include/org/osgi/service/console/command.h
  bundles/console/include/org/osgi/service/console/console.h
  bundles/console/impl/command_lb.h
  bundles/console/impl/command_lb.cc
  bundles/console/impl/commands_framework.h
  bundles/console/impl/commands_framework.cc
  bundles/console/impl/osgi_console.h
  bundles/console/impl/osgi_console.cc)
target_include_directories(
  posgi
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bundles/console/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog-1.1.9/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-1.13.0/googletest/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_82_0>)
target_clangformat_setup(posgi)

# TODO(Googletest): bug in Googletest:
# https://github.com/google/googletest/issues/4206 will results in 2 warnings
# "implicit-int-float-conversion" when compiling gtest-all.cc

# workaround: disable warnings when compiling gtest
target_compile_options(gtest PRIVATE "-Wno-everything")

# core tests
add_executable(
  posgi_tests framework/posgi_test.cc framework/impl/manifest_parser_test.cc
              framework/impl/utils/PosgiTxtFormatter_test.cc)
target_link_libraries(posgi_tests posgi GTest::gtest_main)
target_include_directories(
  posgi_tests
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bundles/console/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog-1.1.9/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-1.13.0/googletest/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_82_0>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/cifuzz/include>)
include(GoogleTest)
gtest_discover_tests(posgi_tests)

# run one single test only, use for development, add this optio

# TEST_FILTER PosgiTxtFormatterTest.fixed_column

#
# cifuzz tests
#
add_fuzz_test(posgi_fuzz_tests_manifest
              framework/impl/manifest_parser_fuzz_test.cc)
target_link_libraries(posgi_fuzz_tests_manifest PRIVATE posgi)
target_include_directories(
  posgi_fuzz_tests_manifest
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bundles/console/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog-1.1.9/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-1.13.0/googletest/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_82_0>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/cifuzz/include>)

add_fuzz_test(posgi_fuzz_tests_txtformatter
              framework/impl/utils/PosgiTxtFormatter_fuzz_test.cc)
target_link_libraries(posgi_fuzz_tests_txtformatter PRIVATE posgi)
target_include_directories(
  posgi_fuzz_tests_txtformatter
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bundles/console/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog-1.1.9/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-1.13.0/googletest/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_82_0>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/cifuzz/include>)

#
# posgi cli
#
add_executable(posgi_cli framework/main.cc framework/posgi.cc)
target_link_libraries(posgi_cli PRIVATE posgi)
target_include_directories(
  posgi_cli
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>)
target_clangformat_setup(posgi_cli)

# samples
add_executable(
  posgi_sample03
  samples/sample_simple_03/main.cc samples/sample_simple_03/some_bundle.cc
  samples/sample_simple_03/some_bundle.h)
target_link_libraries(posgi_sample03 PUBLIC posgi)
target_include_directories(
  posgi_sample03
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>)
target_clangformat_setup(posgi_sample03)
