cmake_minimum_required(VERSION 3.14)

project(ConcurrentBench 
    VERSION 1.0.0
    DESCRIPTION "Concurrent Data Structure Benchmark Framework"
    LANGUAGES CXX
)

# Force Unix Makefiles generator if not specified
if(NOT CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "Unix Makefiles" CACHE STRING "Build system generator" FORCE)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)
    
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    
    # Set default compiler if not specified
    if(NOT CMAKE_CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "g++" CACHE STRING "C++ compiler" FORCE)
    endif()
    if(NOT CMAKE_C_COMPILER)
        set(CMAKE_C_COMPILER "gcc" CACHE STRING "C compiler" FORCE)
    endif()
endif()

option(BUILD_BENCHMARKS "Build benchmarks" ON)
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Threads REQUIRED)

include(FetchContent)

# mimalloc
FetchContent_Declare(
    mimalloc
    GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
    GIT_TAG v2.1.2
)
set(MI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MI_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(MI_BUILD_OBJECT OFF CACHE BOOL "" FORCE)
set(MI_OVERRIDE OFF CACHE BOOL "" FORCE)


# Make dependencies available
FetchContent_MakeAvailable(mimalloc)

# Main library target (header-only)
add_library(conbench INTERFACE)
target_include_directories(conbench INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(conbench INTERFACE cxx_std_20)
target_link_libraries(conbench INTERFACE Threads::Threads)

# Apply compiler warnings
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake")
    include(cmake/CompilerWarnings.cmake)
endif()

# Apply sanitizers
if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(conbench INTERFACE
            -fsanitize=address
            -fsanitize=undefined
            -fno-omit-frame-pointer
        )
        target_link_options(conbench INTERFACE
            -fsanitize=address
            -fsanitize=undefined
        )
    endif()
endif()

add_subdirectory(src)

if(BUILD_BENCHMARKS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/CMakeLists.txt")
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()

