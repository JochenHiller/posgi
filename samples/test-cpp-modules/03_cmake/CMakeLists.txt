cmake_minimum_required(VERSION 3.26)

set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk")

project(MAIN CXX)
set(CMAKE_CXX_STANDARD 20)
if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fmodules-ts)
    add_link_options(-lstdc++)
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fmodules)
endif ()

include(modules-patched.cmake)

# TODO how to precompile system headers first?

add_module_library(math_interface math.cc)
add_library(math_implementation math_impl.cc)
# needed as "normal" library does not support modules
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(math_implementation PUBLIC
        -fmodule-file=${CMAKE_CURRENT_BINARY_DIR}/math.pcm
    )
endif ()

# client.cc will be compiled with modules and linked to math.pcm as i
# depends on math_interface and math_implementation
add_executable(main client.cc)
target_link_libraries(main math_interface math_implementation)
