cmake_minimum_required(VERSION 3.10)

project(Posgi VERSION 0.1)
configure_file(PosgiConfig.h.in PosgiConfig.h)

add_library(posgi STATIC
framework/impl/bundle_context_impl.cc
framework/impl/bundle_impl.cc
framework/impl/framework_impl.cc
framework/impl/manifest_parser.cc
framework/osgi_console.cc
framework/framework_factory.cc
)
target_compile_features(posgi PUBLIC cxx_std_17)
target_include_directories(posgi PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog-1.1.9/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-1.13.0/googletest/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_82_0>
)

add_executable(posgi_cli framework/main.cc)
target_link_libraries(posgi_cli PUBLIC posgi)
target_compile_features(posgi_cli PUBLIC cxx_std_17)
target_include_directories(posgi_cli PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
)

add_executable(posgi_sample03 samples/sample_simple_03/main.cc samples/sample_simple_03/some_bundle.cc)
target_link_libraries(posgi_sample03 PUBLIC posgi)
target_compile_features(posgi_sample03 PUBLIC cxx_std_17)
target_include_directories(posgi_sample03 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framework/include>
)
